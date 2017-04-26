//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Apr 24 11:45:07 2017 by ROOT version 6.09/03
// from TChain electron/
//////////////////////////////////////////////////////////

#ifndef electron_h
#define electron_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TVector3.h>
#include <TH2.h>

// Headers needed by this particular selector

constexpr Double_t kgElectronMass = 9.10938291e-31;// These values are used in EPOCH. The unit is kg
constexpr Double_t kgLightSpeed = 2.99792458e8;

class electron : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   //TTreeReaderValue<Double_t> x = {fReader, "x"};
   //TTreeReaderValue<Double_t> y = {fReader, "y"};
   //TTreeReaderValue<Double_t> z = {fReader, "z"};
   TTreeReaderValue<Double_t> Px = {fReader, "Px"};
   TTreeReaderValue<Double_t> Py = {fReader, "Py"};
   TTreeReaderValue<Double_t> Pz = {fReader, "Pz"};
   //TTreeReaderValue<Double_t> Vx = {fReader, "Vx"};
   //TTreeReaderValue<Double_t> Vy = {fReader, "Vy"};
   //TTreeReaderValue<Double_t> Vz = {fReader, "Vz"};
   //TTreeReaderValue<Double_t> Ek = {fReader, "Ek"};
   TTreeReaderValue<Double_t> Weight = {fReader, "Weight"};

   TVector3 fP;
   Double_t GetEne() const;
   Double_t GetTheta() const;
   TH2D *fHisTest;
   
   electron(TTree * /*tree*/ =0) { }
   virtual ~electron() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(electron,0);

};

#endif

#ifdef electron_cxx
void electron::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t electron::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef electron_cxx
