TH1D *hisIon;
TH1D *hisElectron;


void test()
{
   hisIon = new TH1D("hisIon", "test", 100, 0., 0.);
   hisElectron = new TH1D("hisElectron", "test", 100, 0., 0.);
   
   
   auto file = TFile::Open("0210.root");
   auto ex = (TH1D*)file->Get("Ex");
   auto maxBin = ex->GetMaximumBin();
   auto minBin = ex->GetMinimumBin();
   auto lowEdge = ex->GetBinLowEdge(maxBin);
   auto binWidth = ex->GetBinWidth(maxBin);
   auto upEdge = lowEdge + binWidth;

   cout << lowEdge <<"\t"<< upEdge << endl;

   auto tree = (TTree*)file->Get("particles");
   tree->SetBranchStatus("*", kFALSE);
   
   TString *Ptype{nullptr};
   tree->SetBranchStatus("Ptype", kTRUE);
   tree->SetBranchAddress("Ptype", &Ptype);
   
   Double_t x;
   tree->SetBranchStatus("x", kTRUE);
   tree->SetBranchAddress("x", &x);
   
   Double_t Px;
   tree->SetBranchStatus("Px", kTRUE);
   tree->SetBranchAddress("Px", &Px);
   
   const Long64_t nPar = tree->GetEntries();
   for(Long64_t i = 0; i < nPar; i++){
      tree->GetEntry(i);

      if(x >= lowEdge && x < upEdge){
         if(*Ptype == "ion") hisIon->Fill(Px);
         else if(*Ptype == "electron") hisElectron->Fill(Px);
      }
      
   }
}
