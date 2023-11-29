// Tyler LaBree
// Northern Illinois University

#include "DUNEStyle.h"

// From a photon library, average voxel slices to get light yield map.
class LightYieldMap {
  public:
    PhotonLibrary* lib;       // The photon library used to make the map.
    Int_t normDir;            // Direction normal to the light yield map.
    Int_t normBoundsInVx[2];  // Bounds on which to average the normal direction
    Int_t mapDirs[2];         // The two dirs of the light yield map (0, 1, or 2)
    Double_t map[maxVxDim][maxVxDim]; // Contains light yield map to plot.
    Double_t average;         // Average light yield for the map.
    Double_t minimum;         // Minimum light yield for the map.
    TString title;

    LightYieldMap(PhotonLibrary* myLib, Int_t myNormDir, Int_t myNormBoundsInVx[2]) {
      lib = myLib;

      // Error handling. Are arguments in an acceptable range?
      if (!(0<= myNormDir && myNormDir<=2)) {
        cout << "ERROR: Normal direction not valid." << endl;
      } 
      if (!(0 <= myNormBoundsInVx[0] && myNormBoundsInVx[0] < lib->simulatedBoundsInVx[myNormDir][1])) {
        cout << "ERROR: Position is out of cryo range." << endl;
      } else if (!(lib->fiducialBoundsInVx[myNormDir][0] <= myNormBoundsInVx[0] && myNormBoundsInVx[0] < lib->fiducialBoundsInVx[myNormDir][1])) {
        cout << "WARNING: Position is out of fiducial range: " << dirNames[myNormDir] << " = " << myNormBoundsInVx[0]<< endl;
      }
      if (!(0 <= myNormBoundsInVx[1]-1 && myNormBoundsInVx[1]-1 < lib->simulatedBoundsInVx[myNormDir][1])) {
        cout << "ERROR: Position is out of cryo range." << endl;
      } else if (!(lib->fiducialBoundsInVx[myNormDir][0] <= myNormBoundsInVx[1]-1 && myNormBoundsInVx[1]-1 < lib->fiducialBoundsInVx[myNormDir][1])) {
        cout << "WARNING: Position is out of fiducial range: " << dirNames[myNormDir] << " = " << myNormBoundsInVx[1]-1<< endl;
      }

      normDir = myNormDir;
      for (int i=0; i<2; i++) normBoundsInVx[i] = myNormBoundsInVx[i];
      SetMapDirs();
      SetMap();
      SetAvg();
      SetMin();
      title = Form("Light Yield Average On {%c | %.2f < %c < %.2f m};%c [m];%c [m]; Light Yield [PE/MeV]"
          , dirNames[normDir], lib->GetPosInM(normDir,Double_t(normBoundsInVx[0])), dirNames[normDir]
          , lib->GetPosInM(normDir,Double_t(normBoundsInVx[1])), dirNames[mapDirs[1]], dirNames[mapDirs[0]]);
    }

    // Find and set the two directions of the light yield map.
    void SetMapDirs() {
      int i=0;
      for (int j=0; j<3; j++)
        if (j!=normDir)
          mapDirs[i++] = j;
    }

    // Find and set the light yield map, averaging over bounds in the direction
    // normal to the map.
    void SetMap() {
      Int_t indices[3] = {0,0,0};
      for (indices[mapDirs[0]]=lib->fiducialBoundsInVx[mapDirs[0]][0]; indices[mapDirs[0]]<lib->fiducialBoundsInVx[mapDirs[0]][1]; indices[mapDirs[0]]++) {
        for (indices[mapDirs[1]]=lib->fiducialBoundsInVx[mapDirs[1]][0]; indices[mapDirs[1]]<lib->fiducialBoundsInVx[mapDirs[1]][1]; indices[mapDirs[1]]++) {
          map[indices[mapDirs[0]]][indices[mapDirs[1]]] = 0;
          for (indices[normDir]=normBoundsInVx[0]; indices[normDir]<normBoundsInVx[1]; indices[normDir]++) {
            map[indices[mapDirs[0]]][indices[mapDirs[1]]] += lib->LYPerVx[indices[0]][indices[1]][indices[2]];
          }
          map[indices[mapDirs[0]]][indices[mapDirs[1]]] /= (normBoundsInVx[1] - normBoundsInVx[0]);
        }
      }
    }

    // Find and set the average value of the light yield map.
    void SetAvg() {
      Double_t numerator = 0;
      Double_t denominator =
        (lib->fiducialBoundsInVx[mapDirs[0]][1] - lib->fiducialBoundsInVx[mapDirs[0]][0])
        * (lib->fiducialBoundsInVx[mapDirs[1]][1] - lib->fiducialBoundsInVx[mapDirs[1]][0]);
      for (int i=lib->fiducialBoundsInVx[mapDirs[0]][0]; i<lib->fiducialBoundsInVx[mapDirs[0]][1]; i++) {
        for (int j=lib->fiducialBoundsInVx[mapDirs[1]][0]; j<lib->fiducialBoundsInVx[mapDirs[1]][1]; j++) {
          numerator += map[i][j];
        }
      }
      average = numerator/denominator;
    }

    // Find and set the minimum value of the light yield map.
    void SetMin() {
      Double_t minVal = 1.79769e+308;
      Int_t minPos[2] = {-1,-1};
      for (int i=lib->fiducialBoundsInVx[mapDirs[0]][0]; i<lib->fiducialBoundsInVx[mapDirs[0]][1]; i++) {
        for (int j=lib->fiducialBoundsInVx[mapDirs[1]][0]; j<lib->fiducialBoundsInVx[mapDirs[1]][1]; j++) {
          if (map[i][j] < minVal) {
            minVal = map[i][j];
            minPos[0] = i;
            minPos[1] = j;
          }
        }
      }
      minimum = minVal;
    }

    // Draw the light yield map.
    void Draw() {
      TH2D *mapHist = new TH2D("mapHist",title.Data(),
          (lib->fiducialBoundsInVx[mapDirs[1]][1] - lib->fiducialBoundsInVx[mapDirs[1]][0]),
          lib->GetPosInM(mapDirs[1], lib->fiducialBoundsInVx[mapDirs[1]][0]),
          lib->GetPosInM(mapDirs[1], lib->fiducialBoundsInVx[mapDirs[1]][1]),
          (lib->fiducialBoundsInVx[mapDirs[0]][1] - lib->fiducialBoundsInVx[mapDirs[0]][0]),
          lib->GetPosInM(mapDirs[0], lib->fiducialBoundsInVx[mapDirs[0]][0]),
          lib->GetPosInM(mapDirs[0], lib->fiducialBoundsInVx[mapDirs[0]][1]));
      for (int i=lib->fiducialBoundsInVx[mapDirs[0]][0]; i<lib->fiducialBoundsInVx[mapDirs[0]][1]; i++) {
        Double_t x = lib->GetPosInM(mapDirs[0],Double_t(i)+0.5);
        for (int j=lib->fiducialBoundsInVx[mapDirs[1]][0]; j<lib->fiducialBoundsInVx[mapDirs[1]][1]; j++) {
          Double_t y = lib->GetPosInM(mapDirs[1],Double_t(j)+0.5);
          Int_t bin = mapHist->Fill(y,x,map[i][j]);
        }
      }

      TCanvas* myC = new TCanvas("myC", "Light Yield", 20, 52, 1250,
          Double_t(lib->fiducialBoundsInVx[mapDirs[0]][1])
          /Double_t(lib->fiducialBoundsInVx[mapDirs[1]][1])*1250.);

      dunestyle::SetDuneStyle();
      //mapHist->GetZaxis()->SetRangeUser(0, 110);
      mapHist->Draw("colz");

      TPaveText *pt = new TPaveText(0.7,0.775,0.825,0.875,"NB NDC");
      pt->AddText(Form("#LTLY#GT = %.2f", average));
      pt->AddText(Form("LY_{min} = %.2f", minimum));
      pt->Draw();
      dunestyle::Simulation();
    }
};


