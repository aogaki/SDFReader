#include <iostream>

#include <TVector.h>

#include "TConverter.hpp"
#include "TMacroParticle.hpp"


using std::cout;
using std::endl;

TConverter::TConverter(TSDFReader *reader, TString outputName)
   : fReader(reader),
     fOutput(nullptr),
     fMeshValue(nullptr)
{
   fMeshValue = new TMeshValue(fReader);
   fMeshValue->ReadMeshGrid();
   
   fOutput = new TFile(outputName, "RECREATE");

   // Save time info
   TVectorD time(1);
   time[0] = fReader->GetTime();
   time.Write("Time");
   
   FindPar();
}

TConverter::~TConverter()
{
   fOutput->Close();
   
   delete fMeshValue;
}

void TConverter::GetData()
{
   // Mesh data
  fMeshValue->GetMeshData();
  fMeshValue->Save();

   // Particle data
  for(auto name: fParName){
    cout << name << endl;
    TMacroParticle *par = new TMacroParticle(fReader, name);
    par->MakeTree();
   
    delete par;
  }
}

void TConverter::FindPar()
{
   // Using CPU split information.
   // Unfortunately, I don't know what is the CPU split information.
   // But, this shows the particle names
   for(Int_t i = 0; i < fReader->GetNBlocks(); i++){
      TString id = fReader->fBlock[i]->GetID();
      if(id.Contains("cpu/")){
         id.Remove(0, 4); // 4 means size of "cpu/"
         fParName.push_back(id);
      }
   }
}

