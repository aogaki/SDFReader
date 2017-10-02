TH3F *HisEy;
TH3F *HisEle;
TH2F *HisEyPro;
TH2F *HisElePro;

void NormHist(TH2F *his)
{
   Float_t max = his->GetMaximum();
   Float_t min = his->GetMinimum();
   if(fabs(min) > max) max = fabs(min);

   Int_t maxX = his->GetNbinsX();
   Int_t maxY = his->GetNbinsY();
   
   for(Int_t x = 1; x <= maxX; x++){
      for(Int_t y = 1; y <= maxY; y++){
         Float_t val = his->GetBinContent(x, y) / max;
         his->SetBinContent(x, y, val);
      }
   }
}

void proEyEle()
{
   gStyle->SetPalette(kRainBow);

   TFile *file = new TFile("test.root", "READ");
   HisEy= (TH3F*)file->Get("Ey");
   HisEle = (TH3F*)file->Get("number_density_electron");

   HisEyPro = (TH2F*)HisEy->Project3D("yx");
   HisElePro = (TH2F*)HisEle->Project3D("yx");

   NormHist(HisEyPro);
   NormHist(HisElePro);

   HisElePro->Draw("COL");
   HisEyPro->Draw("CONT0SAME");
   HisElePro->Draw("COLSAME");
   
   //gStyle->SetCanvasPreferGL(kTRUE);
}
