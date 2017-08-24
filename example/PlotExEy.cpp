#include <iostream>

#include <TPolyLine3D.h>
#include <TFile.h>
#include <TH3.h>
#include <TH2.h>
#include <TString.h>
#include <TCanvas.h>
#include <TView.h>
#include <TGraph2D.h>


TPolyLine3D *GetCurve(TH2 *his, TString axis = "y")
{
   Double_t max = his->GetMaximum();
   if(max < fabs(his->GetMinimum())) max = fabs(his->GetMinimum());
   const Int_t nBinsX = his->GetNbinsX();
   const Int_t nBinsY = his->GetNbinsY();
   TPolyLine3D *gr = new TPolyLine3D(nBinsX);

   for(Int_t i = 1; i <= nBinsX; i++){
      if(axis == "y")gr->SetPoint(i - 1, his->GetXaxis()->GetBinCenter(i),
                                  0., his->GetBinContent(i, nBinsY / 2) / max);
      else if(axis == "x")gr->SetPoint(i - 1, his->GetXaxis()->GetBinCenter(i),
                                       his->GetBinContent(i, (nBinsY / 2) + 50) / max, 0.);
      else cout << "set axis option x or y" << endl;
   }
   return gr;
}


void PlotExEy()
{
   TString fileName = "0002_MeshInfo.root";
   TFile *file = new TFile(fileName, "READ");

   TH2 *hisEy = (TH2 *)file->Get("Ey");
   TPolyLine3D *grEy = GetCurve(hisEy, "y");
   grEy->SetLineWidth(2);
   grEy->SetLineColor(kGreen);
   
   TH2 *hisEx = (TH2 *)file->Get("Ex");
   TPolyLine3D *grEx = GetCurve(hisEx, "x");
   grEx->SetLineWidth(2); 
   grEx->SetLineColor(kBlue);
  
   TCanvas *canv = new TCanvas();
   TView *view = TView::CreateView(1);
   view->SetRange(hisEx->GetXaxis()->GetBinLowEdge(1), -1.1, -1.1,
                  hisEx->GetXaxis()->GetBinLowEdge(hisEx->GetNbinsX() + 1), 1.1, 1.1);
   view->ShowAxis();
   
   grEy->Draw("LINE");
   grEx->Draw("LINE");
}
