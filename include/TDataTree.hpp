#include <TChain.h>
#include <TString.h>
#include <TTree.h>
#include <TVector3.h>

// These values are used in EPOCH. The unit is kg
constexpr Double_t kgElectronMass = 9.10938291e-31;
constexpr Double_t kgLightSpeed = 2.99792458e8;

class TDataTree
{
 public:
  TDataTree();
  ~TDataTree();

  void SetParName(TString name = "");
  void SetParMass(Double_t mass = kgElectronMass);
  void SetPDGCode(Int_t code);
  void SetDirectory(TString dir = "./");

  void AddFiles();

  void GetOutput();

 private:
  // Parameter variables for finding trees and files
  TString fParName;
  Double_t fParMass;
  Int_t fPDGCode;
  TString fDirectory;

  // Pointers for input and output data
  TChain *fEjectedChain;
  TChain *fLastFile;
  TTree *fOutTree;

  void AddEjected();
  void AddLastFile();

  Double_t GetEne(TVector3 &P);
};
