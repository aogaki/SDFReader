#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>


Double_t GetAngP(TTree *tree)
{
   Double_t retVal = 0.;
   
   tree->SetBranchStatus("*", kFALSE);

   Double_t y;
   tree->SetBranchStatus("y", kTRUE);
   tree->SetBranchAddress("y", &y);
   Double_t z;
   tree->SetBranchStatus("z", kTRUE);
   tree->SetBranchAddress("z", &z);
   
   Double_t Py;
   tree->SetBranchStatus("Py", kTRUE);
   tree->SetBranchAddress("Py", &Py);
   Double_t Pz;
   tree->SetBranchStatus("Pz", kTRUE);
   tree->SetBranchAddress("Pz", &Pz);

   Double_t Weight;
   tree->SetBranchStatus("Weight", kTRUE);
   tree->SetBranchAddress("Weight", &Weight);

   const Long64_t kMax = tree->GetEntries();
   for(Long64_t i = 0; i < kMax; i++){
      tree->GetEntry(i);

      retVal += Weight*(y*Pz - z*Py);
      //retVal += (y*Pz - z*Py);
      
   }

   return retVal;
}

void CalAngP(TString number)
{
   TFile *fileProton = new TFile(number + "_proton.root", "READ");
   TTree *treeProton = (TTree*)fileProton->Get("proton");

   Double_t val = GetAngP(treeProton);
   
   fileProton->Close();

   TFile *fileElectron = new TFile(number + "_electron.root", "READ");
   TTree *treeElectron = (TTree*)fileElectron->Get("electron");

   val += GetAngP(treeElectron);
   
   fileElectron->Close();

   cout << number <<"\t"<< val << endl;   
}

void angP()
{
   CalAngP("../0003");
   CalAngP("../0004");
   CalAngP("../0006");
   CalAngP("../0009");
   CalAngP("../0011");
   CalAngP("../0012");
   CalAngP("../0015");
   //CalAngP("0040");
}
