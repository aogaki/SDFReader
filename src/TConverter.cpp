#include <iostream>

#include <TVector.h>

#include "TConverter.hpp"
#include "TMacroParticle.hpp"


using std::cout;
using std::endl;

TConverter::TConverter(TSDFReader *reader, TString outputName, TString targetName = "")
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

   fTargetName = targetName;
}

TConverter::~TConverter()
{
   fOutput->Close();
   
   delete fMeshValue;
}

void TConverter::GetData()
{
   if(fTargetName == ""){
      // Mesh data
      fMeshValue->GetMeshData();
      fMeshValue->Save();

      // Particle data
      for(auto name: fParName){
         cout << name << endl;
         //TMacroParticle *par = new TMacroParticle(fReader, name);
         //par->MakeTree();
         //delete par;
      }
   }
   else{
      // Particle data
      for(auto name: fParName){
         if(name == fTargetName){
            cout << name << endl;
            TMacroParticle *par = new TMacroParticle(fReader, name);
            par->MakeTree();
            delete par;
         }
      }
   }
}

void TConverter::FindPar()
{
   // Using Px for finding particles.
   // In case of not recording Px, what is the best?
   for(Int_t i = 0; i < fReader->GetNBlocks(); i++){
      TString id = fReader->fBlock[i]->GetID();
      TString px = id(0, 3);
      if(px == "px/"){
         id.Remove(0, 3); // 4 means size of "px/"
         fParName.push_back(id);
      }
   }
}

