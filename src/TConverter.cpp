#include <iostream>

#include "TConverter.hpp"
#include "TBlockPlainMesh.hpp"
#include "TBlockPlainVar.hpp"
#include "TBlockPointMesh.hpp"
#include "TBlockPointVar.hpp"


using std::cout;
using std::endl;

TConverter::TConverter(TSDFReader *reader)
   : fReader(reader),
     fOutput(nullptr),
     fMeshValue(nullptr)
{
   fMeshValue = new TMeshValue(fReader);
   fMeshValue->ReadMeshGrid();
   
   fOutput = new TFile("out.root", "RECREATE");

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
   //fMeshValue->GetMeshData();
   //fMeshValue->Save();

   // Particle data
   for(auto name: fParName) cout << name << endl;
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

