#include <TTree.h>

#include "TMacroParticle.hpp"
#include "TBlockPointMesh.hpp"
#include "TBlockPointVar.hpp"


using std::cout;
using std::endl;

TMacroParticle::TMacroParticle(TSDFReader *reader, TString parName)
  : fReader(reader),
    fParName(parName),
    fGrid(nullptr),
    fPx(nullptr),
    fPy(nullptr),
    fPz(nullptr),
    fVx(nullptr),
    fVy(nullptr),
    fVz(nullptr),
    fEk(nullptr),
    fWeight(nullptr),
    fOptDep(nullptr),
    fQEDEne(nullptr)
{
  FindMesh();
  FindVar();
}

TMacroParticle::~TMacroParticle()
{}

void TMacroParticle::FindMesh()
{
  TString meshID = "grid/" + fParName;
  Int_t index = fReader->GetBlockIndex(meshID);
  if(index < 0) return;
  fGrid = (TBlockPointMesh*)fReader->fBlock[index];
  fGrid->ReadMetadata();
  fGrid->ReadData(); // This shoud be changed.
}

void TMacroParticle::FindVar()
{
  fPx = FindBlockPointVar("px/" + fParName);
  fPy = FindBlockPointVar("py/" + fParName);
  fPz = FindBlockPointVar("pz/" + fParName);

  fVx = FindBlockPointVar("vx/" + fParName);
  fVy = FindBlockPointVar("vy/" + fParName);
  fVz = FindBlockPointVar("vz/" + fParName);

  fEk = FindBlockPointVar("ek/" + fParName);

  fWeight = FindBlockPointVar("weight/" + fParName);

  fOptDep = FindBlockPointVar("optical depth/" + fParName);

  fQEDEne = FindBlockPointVar("qed energy/" + fParName);
}

TBlockPointVar *TMacroParticle::FindBlockPointVar(TString id)
{
  Int_t index = fReader->GetBlockIndex(id);
  if(index < 0) return nullptr;
  TBlockPointVar *block = (TBlockPointVar*)fReader->fBlock[index];
  block->ReadMetadata();
  block->ReadData(); // This shoud be changed.

  return block;
}

void TMacroParticle::MakeTree()
{
  TTree *tree = new TTree(fParName, "particle information");

  // Using similar name is not so good.  Block: fPx, Variable: Px.
  // Position
  Double_t x;
  tree->Branch("x", &x, "x/D");
  Double_t y;
  tree->Branch("y", &y, "y/D");
  Double_t z;
  tree->Branch("z", &z, "z/D");
  
  // Momentum
  Double_t Px;
  tree->Branch("Px", &Px, "Px/D");
  Double_t Py;
  tree->Branch("Py", &Py, "Py/D");
  Double_t Pz;
  tree->Branch("Pz", &Pz, "Pz/D");
  
  // Velocity
  Double_t Vx;
  tree->Branch("Vx", &Vx, "Vx/D");
  Double_t Vy;
  tree->Branch("Vy", &Vy, "Vy/D");
  Double_t Vz;
  tree->Branch("Vz", &Vz, "Vz/D");

  // Kinetic energy
  Double_t Ek;
  tree->Branch("Ek", &Ek, "Ek/D");

  // Weight
  Double_t Weight;
  tree->Branch("Weight", &Weight, "Weight/D");

  // Optical depth
  Double_t optDep;
  tree->Branch("OptDep", &optDep, "OptDep/D");

  // QED energy
  Double_t QEDEne;
  tree->Branch("QEDEne", &QEDEne, "QEDEne/D");


  const Long64_t kNoPar = fPx->GetDataSize();
  cout << kNoPar << endl;
  for(Long64_t i = 0; i < kNoPar; i++){
     if(i % 1000000 == 0)
        cout << i <<" / "<< kNoPar <<" ("<< i * 100. / kNoPar <<" %)"<< endl;
     
     if(fPx) Px = fPx->GetData(i);
     if(fPy) Py = fPy->GetData(i);
     if(fPz) Pz = fPz->GetData(i);
    
     if(fVx) Vx = fVx->GetData(i);
     if(fVy) Vy = fVy->GetData(i);
     if(fVz) Vz = fVz->GetData(i);
    
     if(fEk) Ek = fEk->GetData(i);

     if(fWeight) Weight = fWeight->GetData(i);

     if(fOptDep) optDep = fOptDep->GetData(i);
     
     if(fQEDEne) QEDEne = fQEDEne->GetData(i);

     if(fGrid){
        x = fGrid->GetData(i);
        if(fGrid->GetNDims() > 1) y = fGrid->GetData(i + kNoPar);
        if(fGrid->GetNDims() > 2) z = fGrid->GetData(i + kNoPar + kNoPar);
     }
     tree->Fill();
  }

  tree->Write();
  delete tree;
}
