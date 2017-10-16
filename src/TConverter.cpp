#include <iostream>

#include <TThread.h>
#include <TVector.h>

#include "TConverter.hpp"
#include "TMacroParticle.hpp"

using std::cout;
using std::endl;

TConverter::TConverter(TSDFReader *reader, TString outName, RunMode runMode)
    : fReader(reader), fRunMode(runMode), fOutName(outName)
{
  FindPar();
}

TConverter::~TConverter() {}

void TConverter::GetData()
{
  TString name = fOutName + ".root";
  TFile *file = new TFile(name, "RECREATE");

  // Save time info
  TVectorD time(1);
  time[0] = fReader->GetTime();
  time.Write("Time");

  // Mesh data
  cout << "\n Mesh info" << endl;
  auto mesh = new TMeshValue(fReader);
  mesh->SetAllOn();
  mesh->GetMeshData();
  delete mesh;

  // Particle data
  cout << "\n Particle info" << endl;
  TTree *parTree = new TTree("particles", "particle information");
  for (Int_t i = 0; i < fParName.size(); i++) {
    TString name = fParName[i];
    auto par = new TMacroParticle(fReader, name);
    par->AddTree(parTree);
    delete par;
  }
  parTree->Write();
  delete parTree;

  file->Close();
  delete file;
}

void TConverter::FindPar()
{
  // Using Px for finding particles.
  // In case of not recording Px, what is the best?
  for (Int_t i = 0; i < fReader->GetNBlocks(); i++) {
    TString id = fReader->fBlock[i]->GetID();
    TString px = id(0, 3);
    if (px == "px/") {
      id.Remove(0, 3);  // 3 means size of "px/"
      fParName.push_back(id);
    }
  }
}
