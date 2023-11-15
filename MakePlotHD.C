// Tyler LaBree
// Northern Illinois University

#include "PhotonLibrary.h"
#include "LightYieldMap.h"
#include "DepthPlot.h"

void MakePlotHD() {
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
    "lib_dunehd10kt_1x2x6_20230403.root"
    , dims
    , simulatedBoundsInM
    , fiducialBoundsInM);
  
  // Define depth ranges on which to average over to make light yield plots.
  Int_t fullAvg[2] = {1,193};
  Int_t centerSlice[2] = {(fullAvg[1] + fullAvg[0])/2-1, (fullAvg[1] + fullAvg[0])/2+1};
  Int_t centerAvg[2] = {81,113};
  Int_t badSlice[2] = {6,7};
  Int_t backSlice[2] = {192,193};

  /* Make a light yield plot perpendicular to the z-dir, averaged over a
   * range, using the photon library made above.
   */
  LightYieldMap* lymap = new LightYieldMap(lib, kZ, backSlice);
  lymap->Draw();
  /*
  Int_t xRange[2] = {1,74};
  Int_t yRange[2] = {1,180};
  LightYieldMap* lymap2 = new LightYieldMap(lib, kY, yRange);
  lymap2->Draw();
  */

  // Draw average and minimum light yield across detector depth.
  //DrawDepthPlot(lib, kZ, centerAvg[0], centerAvg[1]);
  //DrawDepthPlot(lib, kZ, centerAvg[0], centerAvg[1]);
}
