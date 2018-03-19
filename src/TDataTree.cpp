#include <iostream>

#include <TFile.h>
#include <TVector3.h>

#include "TDataTree.hpp"

template <class T>
void DelPointer(T *&pointer)
{
  delete pointer;
  pointer = nullptr;
}

TDataTree::TDataTree()
    : fEjectedChain(nullptr), fLastFile(nullptr), fOutTree(nullptr)
{
  fParName = "";
  fParMass = kgElectronMass;
  fPDGCode = 22;  // Gamma is 22
  fDirectory = "./";
}

TDataTree::~TDataTree()
{
  DelPointer(fEjectedChain);
  DelPointer(fLastFile);
  DelPointer(fOutTree);
}

void TDataTree::SetParName(TString name)
{
  fParName = name;
  std::cout << "Particle name: " << fParName << std::endl;
};

void TDataTree::SetParMass(Double_t mass)
{
  fParMass = mass;
  std::cout << "Particle mass: " << fParMass << " kg" << std::endl;
};

void TDataTree::SetPDGCode(Int_t code)
{
  fPDGCode = code;
  std::cout << "PDG code: " << fPDGCode << std::endl;
};

void TDataTree::SetDirectory(TString dir)
{
  fDirectory = dir;
  if (fDirectory[fDirectory.Sizeof() - 2] != TString("/")) fDirectory += "/";
  std::cout << "Directory: " << fDirectory << std::endl;
};

void TDataTree::AddFiles()
{
  if (fParName != "") {
    AddEjected();
    AddLastFile();
  }
}

void TDataTree::AddEjected()
{
  auto treeName = "ejected_" + fParName;
  fEjectedChain = new TChain(treeName);

  auto fileName = fDirectory + "*ejected_" + fParName + "*";
  fEjectedChain->Add(fileName);
}

void TDataTree::AddLastFile()
{
  // Now this function search from 0 to 9999.
  // If you want more.  Do the right thing!
  TString fileName = "";
  for (Int_t i = 0; i < 10000; i++) {
    fileName = fDirectory + Form("%04d_", i) + fParName + ".root";
    TFile file(fileName);

    if (file.IsZombie()) {
      file.Close();  // Need?
      fileName = fDirectory + Form("%04d_", i - 1) + fParName + ".root";
      break;
    } else {
      file.Close();
    }
  }

  fLastFile = new TChain(fParName);
  fLastFile->Add(fileName);
}

Double_t TDataTree::GetEne(TVector3 &P)
{
  Double_t magP = P.Mag2();  // magnitude * magnitude
  if (magP == 0.) return 0.;

  constexpr Double_t c2 = kgLightSpeed * kgLightSpeed;
  Double_t K =
      sqrt(fParMass * fParMass * c2 * c2 + magP * c2) - fParMass * c2;  // in J

  constexpr Double_t MeV = 6.241509e12;
  return K * MeV;
}

void TDataTree::GetOutput()
{
  // Defining output tree
  auto outTree = new TTree(fParName, "for A and D");
  Double_t ene;
  outTree->Branch("KE", &ene, "KE/D");
  Double_t theta;
  outTree->Branch("Divergence", &theta, "Divergence/D");
  Double_t weight;
  outTree->Branch("Weight", &weight, "Weight/D");
  // Re-use varibles defined other places.
  outTree->Branch("PDGCode", &fPDGCode, "PDGCode/I");

  // First, using ejected particles
  fEjectedChain->SetBranchStatus("*", kFALSE);

  // This is for 2D, TVector3 calculate theta
  // to the Z-axis.
  TVector3 P;
  fEjectedChain->SetBranchStatus("Px", kTRUE);
  fEjectedChain->SetBranchAddress("Px", &P[2]);
  fEjectedChain->SetBranchStatus("Py", kTRUE);
  fEjectedChain->SetBranchAddress("Py", &P[1]);
  fEjectedChain->SetBranchStatus("Pz", kTRUE);
  fEjectedChain->SetBranchAddress("Pz", &P[0]);

  fEjectedChain->SetBranchStatus("Weight", kTRUE);
  fEjectedChain->SetBranchAddress("Weight", &weight);

  const auto nEjected = fEjectedChain->GetEntries();
  for (auto i = 0; i < nEjected; i++) {
    fEjectedChain->GetEntry(i);
    ene = GetEne(P);
    theta = P.Theta() * 180. / TMath::Pi();
    outTree->Fill();
  }

  // Next, using not ejected
  fLastFile->SetBranchStatus("*", kFALSE);

  // This is for 2D, TVector3 calculate theta
  // to the Z-axis.
  // Re-use variables
  fLastFile->SetBranchStatus("Px", kTRUE);
  fLastFile->SetBranchAddress("Px", &P[2]);
  fLastFile->SetBranchStatus("Py", kTRUE);
  fLastFile->SetBranchAddress("Py", &P[1]);
  fLastFile->SetBranchStatus("Pz", kTRUE);
  fLastFile->SetBranchAddress("Pz", &P[0]);

  fLastFile->SetBranchStatus("Weight", kTRUE);
  fLastFile->SetBranchAddress("Weight", &weight);

  const auto nInside = fLastFile->GetEntries();
  for (auto i = 0; i < nInside; i++) {
    fLastFile->GetEntry(i);
    ene = GetEne(P);
    theta = P.Theta() * 180. / TMath::Pi();
    outTree->Fill();
  }

  // Write tree
  auto outFileName = fDirectory + "AnaOut_" + fParName + ".root";
  auto outFile = new TFile(outFileName, "RECREATE");
  outTree->Write();
  outFile->Close();

  DelPointer(outTree);
  DelPointer(outFile);
}
