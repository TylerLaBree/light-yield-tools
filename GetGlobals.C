// Tyler LaBree
// Northern Illinois University

#include "PhotonLibrary.h"
#include "LightYieldMap.h"
#include "DepthPlot.h"

void GetGlobals() {
  /* Simulated Volume
   * XMin: 0 
   * XMax: 379
   * YMin: -650 
   * YMax: 650
   * ZMin: 0 
   * ZMax: 1440
   */
  Double_t simulatedBoundsInM[3][2] = { // For horizontal drift
    { 0.    ,  3.79  },
    {-6.50  ,  6.50  },
    { 0.    , 14.40  }
  };
  Double_t fiducialBoundsInM[3][2] = { // For horizontal drift
    { 0.00  ,  3.5   }, // 14 m / 4 = 3.5 m. By eye this appears to be 3.6 m.
    {-6.    ,  6.    }, // 12 m tall APAs. Checking by eye confirms this.
    { 0.    , 13.9   }  // Estimated by eye
  };
  Int_t dims[3] = {75,180,200};
  PhotonLibrary* lib = new PhotonLibrary(
    "/pnfs/dune/scratch/users/lpaulucc/dunehd1x2x6lib/lib_fd1hd_1x2x6.root"
    , dims
    , simulatedBoundsInM
    , fiducialBoundsInM);

  Double_t minAcc = 1.79769e+308;
  Int_t minLocation[3] = {-1, -1, -1};
  Double_t sumAcc = 0.;
  Int_t countAcc = 0;
  for (int i=lib->fiducialBoundsInVx[0][0]; i<lib->fiducialBoundsInVx[0][1]; i++) {
    for (int j=lib->fiducialBoundsInVx[1][0]; j<lib->fiducialBoundsInVx[1][1]; j++) {
      for (int k=lib->fiducialBoundsInVx[2][0]; k<lib->fiducialBoundsInVx[2][1]; k++) {
        if (lib->LYPerVx[i][j][k] < minAcc 
            && lib->LYPerVx[i][j][k] != 0 
            && 3. - 0.1 < lib->GetPosInM(0,i) && lib->GetPosInM(0,i) < 3. + 0.1) {
          minAcc = lib->LYPerVx[i][j][k];
          //minLocation = {i, j, k};
        }
        sumAcc += lib->LYPerVx[i][j][k];
        countAcc++;
      }
    }
  }
  Double_t minimum = minAcc;
  Double_t average = sumAcc / Double_t(countAcc);
  cout << minimum << endl;
  cout << average << endl;
    /*
    Double_t GetPosInM(Int_t dir, Double_t posInVx) {
      return posInVx * (simulatedBoundsInM[dir][1] - simulatedBoundsInM[dir][0])
        / Double_t(simulatedBoundsInVx[dir][1] - simulatedBoundsInVx[dir][0]) 
        + simulatedBoundsInM[dir][0];
    }
    */

    /*
    Int_t simulatedBoundsInVx[3][2]; // Include min, but exclude max
    Int_t fiducialBoundsInVx[3][2];  // Include min, but exclude max
    Double_t LYPerVx[maxVxDim][maxVxDim][maxVxDim]; // Light Yield per voxel. Only valid on detector bounds.
    */
}
