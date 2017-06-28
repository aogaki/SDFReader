#include <iostream>

#include <TStyle.h>
#include <TFile.h>
#include <TH3.h>
#include <TH2.h>
#include <TCanvas.h>


void SaveSlices(TH3F *his, TString number)
{// Making slices at Y-Z plane and X-Y plane of 3D histogram
 // X-Z plane? suck my balls

   // Y-Z plane
   const Int_t kNoBinX = his->GetXaxis()->GetNbins();
   const Double_t kMinX = his->GetXaxis()->GetBinLowEdge(1);
   const Double_t kMaxX = his->GetXaxis()->GetBinUpEdge(his->GetXaxis()->GetNbins());
   const Int_t kNoBinY = his->GetYaxis()->GetNbins();
   const Double_t kMinY = his->GetYaxis()->GetBinLowEdge(1);
   const Double_t kMaxY = his->GetYaxis()->GetBinUpEdge(his->GetYaxis()->GetNbins());
   const Int_t kNoBinZ = his->GetZaxis()->GetNbins();
   const Double_t kMinZ = his->GetZaxis()->GetBinLowEdge(1);
   const Double_t kMaxZ = his->GetZaxis()->GetBinUpEdge(his->GetZaxis()->GetNbins());
   const Double_t kMaxCon = his->GetMaximum();
   const Double_t kMinCon = his->GetMinimum();

   number += his->GetName();
   
   TH2F *hisYZ[kNoBinX];
   for(Int_t i = 0; i < kNoBinX; i++){
      Double_t xPos = his->GetXaxis()->GetBinCenter(i + 1) * 1000000;
      hisYZ[i] = new TH2F(Form("HisYZ%03d", i), Form("x = %f #it{#mu}m", xPos),
                          kNoBinY, kMinY, kMaxY,  kNoBinZ, kMinZ, kMaxZ);
      hisYZ[i]->SetXTitle("Y [m]");
      hisYZ[i]->SetYTitle("Z [m]");
      hisYZ[i]->SetMaximum(kMaxCon * 1.1);
      hisYZ[i]->SetMinimum(kMinCon * 1.1);

      //for(Int_t x = 1; x <= kNoBinX; x++){
      Int_t x = i + 1;
      for(Int_t y = 1; y <= kNoBinY; y++){
         for(Int_t z = 1; z <= kNoBinZ; z++){
            Float_t val = his->GetBinContent(x, y, z);
            hisYZ[i]->SetBinContent(y, z, val);
         }
      }
      //}
   }

   TH2F *hisXY[kNoBinZ];
   for(Int_t i = 0; i < kNoBinZ; i++){
      Double_t zPos = his->GetZaxis()->GetBinCenter(i + 1) * 1000000;
      hisXY[i] = new TH2F(Form("HisXY%03d", i), Form("z = %f #it{#mu}m", zPos),
                          kNoBinX, kMinX, kMaxX,  kNoBinY, kMinY, kMaxY);
      hisXY[i]->SetXTitle("X [m]");
      hisXY[i]->SetYTitle("Y [m]");
      hisXY[i]->SetMaximum(kMaxCon * 1.1);
      hisXY[i]->SetMinimum(kMinCon * 1.1);

      //for(Int_t x = 1; x <= kNoBinX; x++){
      Int_t z = i + 1;
      for(Int_t x = 1; x <= kNoBinX; x++){
         for(Int_t y = 1; y <= kNoBinY; y++){
            Float_t val = his->GetBinContent(x, y, z);
            hisXY[i]->SetBinContent(x, y, val);
         }
      }
      //}
   }

   
   gStyle->SetOptStat(kFALSE);
   gStyle->SetPalette(kRainBow);

   TFile *fileYZ = new TFile(number + "SliceYZ.root", "RECREATE");
   TCanvas *canYZ = new TCanvas("canYZ", "", 900, 900);
   canYZ->Print(number + "SliceYZ.pdf[", "pdf");
   for(auto &result: hisYZ){
      result->Write();
      result->Draw("COLZ");
      canYZ->Print(number + "SliceYZ.pdf", "pdf");
   }
   canYZ->Print(number + "SliceYZ.pdf]", "pdf");
   fileYZ->Close();
   delete canYZ;

   TFile *fileXY = new TFile(number + "SliceXY.root", "RECREATE");
   TCanvas *canXY = new TCanvas("canXY", "", 900, 900);
   canXY->Print(number + "SliceXY.pdf[", "pdf");
   for(auto &result: hisXY){
      result->Write();
      result->Draw("COLZ");
      canXY->Print(number + "SliceXY.pdf", "pdf");
   }
   canXY->Print(number + "SliceXY.pdf]", "pdf");
   fileXY->Close();
   delete canXY;
}

void MakeSlice(TString number)
{
   TFile *file = new TFile("../" + number + "_MeshInfo.root", "READ");
   SaveSlices((TH3F*)file->Get("Ey"), number);
   SaveSlices((TH3F*)file->Get("number_density_electron"), number);
   SaveSlices((TH3F*)file->Get("number_density_proton"), number);
   
   //file->Close();
}
