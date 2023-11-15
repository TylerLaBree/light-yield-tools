// Tyler LaBree
// Northern Illinois University

/* Draw average and minimum light yield vs detector depth on a given photon
 * library and on the range defined.
 */
void DrawDepthPlot(PhotonLibrary* lib, Int_t normDir, Int_t minDepth, Int_t maxDepth) {
  Int_t dist = maxDepth-minDepth;
  Double_t depths[dist];
  Double_t averageLYs[dist];
  Double_t minimumLYs[dist];
  Double_t plotMax = 0;
  for (int i=0; i<dist; i++) {
    depths[i] = lib->GetPosInM(normDir,Double_t(i+minDepth)+0.5);

    Int_t slice[2] = {i+minDepth,i+minDepth+1};
    LightYieldMap* depthLYMap = new LightYieldMap(lib, normDir, slice);
    averageLYs[i] = depthLYMap->average;
    minimumLYs[i] = depthLYMap->minimum;

    if (averageLYs[i] > plotMax)
      plotMax = averageLYs[i];
  }
  plotMax *= 1.2;

  TCanvas* myC2 = new TCanvas("myC2", "Light Yield vs Depth", 20, 52, 1250, 650);
  dunestyle::SetDuneStyle();
  TGraph *lyAvgDepth = new TGraph(dist,depths, averageLYs);
  TGraph *lyMinDepth = new TGraph(dist,depths, minimumLYs);
  dunestyle::colors::NextColor();
  lyAvgDepth->SetMarkerStyle(20);
  lyAvgDepth->SetMarkerColor(dunestyle::colors::NextColor());
  lyMinDepth->SetMarkerStyle(20);
  lyMinDepth->SetMarkerColor(dunestyle::colors::NextColor());
  lyAvgDepth->SetTitle(Form("Light Yield vs Detector Depth;%c [m];Light Yield [PE/MeV]", dirNames[normDir]));
  lyAvgDepth->Draw("AP");
  lyAvgDepth->GetYaxis()->SetRangeUser(0.,plotMax);
  lyMinDepth->Draw("SAMEP");
  auto legend = new TLegend(0.725,0.775,0.825,0.875);
  legend->AddEntry(lyAvgDepth,"Average","p");
  legend->AddEntry(lyMinDepth,"Minimum","p");
  legend->Draw();
  dunestyle::Simulation();
}
