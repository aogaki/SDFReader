TH3F *his;

Double_t maxVal;


Double_t my_transfer_function(const Double_t *x, const Double_t * /*param*/)
{

   if(fabs(*x) < maxVal * 0.1) return 0.;
   else if(fabs(*x) < maxVal * 0.2) return 0.;
   else if(fabs(*x) < maxVal * 0.3) return 0.0;
   else if(fabs(*x) < maxVal * 0.4) return 0.02;
   else if(fabs(*x) < maxVal * 0.5) return 0.05;
   
   return 0.5;
}

void Plot3D()
{
   TFile *file = new TFile("0027_MeshInfo.root", "READ");
   //his = (TH3F*)file->Get("Ey");
   his = (TH3F*)file->Get("Ey");

   cout << Int_t(his->GetEntries()) << endl;
   cout << his->GetNbinsX() <<"\t"
        << his->GetNbinsY() <<"\t"
        << his->GetNbinsZ() << endl;

   his->RebinX(4);
   his->RebinY(4);
   his->RebinZ(4);
   
   maxVal = his->GetMaximum();

   //Now, specify the transfer function.
   TList *lf = his->GetListOfFunctions();
   if(lf){
      TF1 *tf = new TF1("TransferFunction", my_transfer_function);
      lf->Add(tf);
   }

   //his->GetXaxis()->SetRange(1, 128);
   
   gStyle->SetCanvasPreferGL(kTRUE);
   gStyle->SetPalette(kRainBow);
   his->Draw("glcol");
}
