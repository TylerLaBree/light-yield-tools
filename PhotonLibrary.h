// Tyler LaBree
// Northern Illinois University

// Note:
// "vx" means voxel or voxels, which is short for volume elements.
// The x, y, and z directions are 0, 1, and 2, respectively.

#include "TH2D.h"
#include "TH1.h"

// Extra constants
Char_t dirNames[3] = {'x', 'y', 'z'};
const int kX = 0;
const int kY = 1;
const int kZ = 2;
Double_t nPhotPerEDep = 25000.;     // photons per MeV
Double_t arapucaEfficiency = 0.03;
const int maxVxDim = 200;           // Reserves memory. Should be greater than 
                                    // the number of voxels in each direction.

// Import a photon library file and put it in a 3D array of light yields.
class PhotonLibrary {
  public:
    Double_t simulatedBoundsInM[3][2];
    Double_t fiducialBoundsInM[3][2];
    Int_t simulatedBoundsInVx[3][2]; // Include min, but exclude max
    Int_t fiducialBoundsInVx[3][2];  // Include min, but exclude max
    Double_t LYPerVx[maxVxDim][maxVxDim][maxVxDim]; // Light Yield per voxel. Only valid on detector bounds.

    PhotonLibrary(TString myFilename, Int_t myVxDims[3], Double_t mySimBoundsInM[3][2], Double_t myFidBoundsInM[3][2]) {
      // Set simulated and fiducial boundaries.
      for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
          simulatedBoundsInM[i][j] = mySimBoundsInM[i][j];
          fiducialBoundsInM[i][j] = myFidBoundsInM[i][j];
        }
        simulatedBoundsInVx[i][0] = 0;
        simulatedBoundsInVx[i][1] = myVxDims[i];
        fiducialBoundsInVx[i][0] = Int_t(GetPosInVx(i, fiducialBoundsInM[i][0])+1);
        fiducialBoundsInVx[i][1] = Int_t(GetPosInVx(i, fiducialBoundsInM[i][1]));
      }
      SetLYPerVx(myFilename);
    }

    void SetLYPerVx(TString filename) {
      Int_t nVx = 1;
      for (int i=0; i<3; i++)
        nVx *= (simulatedBoundsInVx[i][1] - simulatedBoundsInVx[i][0]);
      vector<Double_t> visPerVx(nVx,0);

      TFile *file = TFile::Open(filename);
      TTree *tree = (TTree*)file->Get("PhotonLibraryData");
      Int_t nEntries = tree->GetEntries();
      Int_t voxel, opChannel;
      Float_t visibility;
      tree->SetBranchAddress("Voxel",&voxel);
      tree->SetBranchAddress("OpChannel",&opChannel);
      tree->SetBranchAddress("Visibility",&visibility);
      for (Int_t i=0; i<nEntries; i++) {
        tree->GetEntry(i);
        //if (opChannel>=56) // cathode plane
        //if (opChannel<56) // field cage walls
        //if (opChannel<56 && opChannel%2==0) // right field cage wall
        //if (opChannel<56 && opChannel%2==1) // left field cage wall
        visPerVx[voxel] += visibility;
      }
      for (int i=0; i<simulatedBoundsInVx[0][1]; i++) {
        for (int j=0; j<simulatedBoundsInVx[1][1]; j++) {
          for (int k=0; k<simulatedBoundsInVx[2][1]; k++) {
            int vxIndex = i + j*simulatedBoundsInVx[0][1]
              + k*simulatedBoundsInVx[1][1]*simulatedBoundsInVx[0][1];
            LYPerVx[i][j][k] = GetLightYield(visPerVx[vxIndex]);
          }
        }
      }
    }

    /* Converts position in voxels to position in meters along a direction.
     * An integer voxel number will produce the leading edge position of 
     * each voxel.
     */
    Double_t GetPosInM(Int_t dir, Double_t posInVx) {
      return posInVx * (simulatedBoundsInM[dir][1] - simulatedBoundsInM[dir][0])
        / Double_t(simulatedBoundsInVx[dir][1] - simulatedBoundsInVx[dir][0]) 
        + simulatedBoundsInM[dir][0];
    }
    // Converts position in meters to position in voxels.
    Double_t GetPosInVx(Int_t dir, Double_t posInM) {
      return (posInM - simulatedBoundsInM[dir][0])
        * Double_t(simulatedBoundsInVx[dir][1] - simulatedBoundsInVx[dir][0])
        / (simulatedBoundsInM[dir][1] - simulatedBoundsInM[dir][0]);
    }
    Double_t GetLightYield(Double_t vis) {
      return vis * arapucaEfficiency * nPhotPerEDep; // PE/MeV
    }
};
