// Tyler LaBree
// Northern Illinois University

#include "PhotonLibrary.h"
#include "LightYieldMap.h"
#include "DepthPlot.h"
void MakePlotVD() {
  Double_t simulatedBoundsInM[3][2] = {
    {-4.2512,  4.252 },
    {-7.7412,  7.7412},
    {-1.0012, 21.9173}
  };
  Double_t fiducialBoundsInM[3][2] = {
    {-3.25  ,  3.25  },
    {-6.75  ,  6.75  },
    { 0.    , 21.    }
  };

  Int_t dims[3] = {50,93,137};
  PhotonLibrary* lib = new PhotonLibrary(
    "../photon-libraries/lib_dunevd10kt_1x8x14_3_20221025.root"
    , dims
    , simulatedBoundsInM
    , fiducialBoundsInM);
  
  // Define depth ranges on which to average over to make light yield plots.
  Int_t centerSlice[2] = {68,70};
  Int_t centerAvg[2] = {60,78};
  Int_t fullAvg[2] = {6,131};

  /* Make a light yield plot perpendicular to the z-dir, averaged over the
   * range 60-78 voxels, using the photon library made above.
   */
  LightYieldMap* lymap = new LightYieldMap(lib, kZ, centerAvg);
  lymap->Draw();

  // Draw average and minimum light yield across detector depth.
  DrawDepthPlot(lib, kZ, 7, 131);
  //DrawDepthPlot(lib, kY, 6, 85);
  //DrawDepthPlot(lib, kX, 6, 44);
}
