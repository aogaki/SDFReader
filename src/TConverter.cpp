#include <iostream>

#include <TVector.h>
#include <TThread.h>

#include "TConverter.hpp"
#include "TMacroParticle.hpp"


using std::cout;
using std::endl;

TConverter::TConverter(TSDFReader *reader, TString outName, RunMode runMode)
   : fReader(reader),
     fRunMode(runMode),
     fOutName(outName)
{
   
   FindPar();

}

TConverter::~TConverter()
{}

void TConverter::GetData()
{
   if(fRunMode == RunMode::AllInfo || fRunMode == RunMode::AllMeshes){
      TString name = fOutName + "_MeshInfo.root";
      TFile *file = new TFile(name, "RECREATE");

      // Save time info
      TVectorD time(1);
      time[0] = fReader->GetTime();
      time.Write("Time");
   
      // Mesh data
      auto mesh = new TMeshValue(fReader);
      mesh->SetAllOn();
      mesh->GetMeshData();

      file->Close();

      delete file;
      delete mesh;
   }
   if(fRunMode == RunMode::AllInfo || fRunMode == RunMode::AllParticles){
      //for(auto name: fParName){
      for(Int_t i = 0; i < fParName.size(); i++){
         TString name = fParName[i];
         TString fileName = fOutName + "_" + name + ".root";
         TFile *file = new TFile(fileName, "RECREATE");
         cout << name << endl;
         auto par = new TMacroParticle(fReader, name);
         par->MakeTree();
         file->Close();
         delete file;
         delete par;
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
         id.Remove(0, 3); // 3 means size of "px/"
         fParName.push_back(id);
      }
   }
}
