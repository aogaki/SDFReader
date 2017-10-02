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

TH2F *GetSlice(TH3F *hisSource)
{
   // make slice around center of Z
   
   const Int_t t = 10; // thickness of slice in percentage
   const Int_t xBin = hisSource->GetNbinsX();
   const Int_t yBin = hisSource->GetNbinsY();
   const Int_t zBin = hisSource->GetNbinsZ();

   TString name = TString(hisSource->GetName()) + "Slice";
   TH2F *his = new TH2F(name, hisSource->GetTitle(),
                        xBin, hisSource->GetXaxis()->GetBinLowEdge(1), hisSource->GetXaxis()->GetBinLowEdge(xBin),
                        yBin, hisSource->GetYaxis()->GetBinLowEdge(1), hisSource->GetYaxis()->GetBinLowEdge(yBin));

   const Int_t zMin = (zBin / 2) - (zBin / t / 2); 
   const Int_t zMax = (zBin / 2) + (zBin / t / 2); 
   
   for(Int_t x = 1; x <= xBin; x++){
      for(Int_t y = 1; y <= yBin; y++){
         for(Int_t z = zMin; z <= zMax; z++){
            Float_t val = his->GetBinContent(x, y) + hisSource->GetBinContent(x, y, z);
            his->SetBinContent(x, y, val);
         }
      }
   }

   return his;
}

void sliceEyEle()
{
   gStyle->SetPalette(kRainBow);

   TFile *file = new TFile("test.root", "READ");
   HisEy= (TH3F*)file->Get("Ey");
   HisEle = (TH3F*)file->Get("number_density_electron");

   //HisEyPro = (TH2F*)HisEy->Project3D("yx");
   //HisElePro = (TH2F*)HisEle->Project3D("yx");

   HisEyPro = GetSlice(HisEy);
   HisElePro = GetSlice(HisEle);

   NormHist(HisEyPro);
   NormHist(HisElePro);

   HisElePro->Draw("COL");
   HisEyPro->Draw("CONT0SAME");
   HisElePro->Draw("COLSAME");
   
   //gStyle->SetCanvasPreferGL(kTRUE);
}
