#include <iostream>

#include <TFile.h>

#include "TMakeTree.hpp"
#include "TBlockPlainVar.hpp"

using std::cout;
using std::endl;

TMakeTree::TMakeTree(TSDFReader *reader)
   : fReader(reader)
{
   fField = new TFieldValue();
   
   Int_t index = GetBlockIndex("grid");
   if(index > 0) fFieldGrid = (TBlockPlainMesh*)fReader->fBlock[index];
   else{
      cout << "No field grid" << endl;
      fFieldGrid = nullptr;
   }
   
   fFieldGrid->ReadData();
   
}

TMakeTree::~TMakeTree()
{
   SaveTree();

   delete fField;
}

void TMakeTree::GetData()
{
   fField->Ex = GetEx();
}

TH1 *TMakeTree::GetEx()
{
   TH1 *his{nullptr};

   Int_t index = GetBlockIndex("ex");
   if(index < 0) return nullptr;
   
   TBlockPlainVar *block = (TBlockPlainVar*)fReader->fBlock[index];
   block->ReadData();

   TString name = "Ex";
   TString title = "Electric Field x";
   Double_t norm = block->GetNormFactor();
   Int_t nBins[3]{
      block->GetNGrids(0),
      block->GetNGrids(1),
      block->GetNGrids(2)
   };
   Int_t stagger = block->GetStagger();
   Int_t dim = block->GetNDims();

   if(dim == 1){
      his = new TH1D(name, title,
                     nBins[0], 0, nBins[0]);
      for(Int_t x = 1; x <= nBins[0]; x++){
         his->SetBinContent(x, norm * block->GetData(x - 1));
      }
   }
   else if(dim == 2){
      his = new TH2D(name, title,
                     nBins[0], 0, nBins[0],
                     nBins[1], 0, nBins[1]);
      for(Int_t x = 1; x <= nBins[0]; x++){
         for(Int_t y = 1; y <= nBins[1]; y++){
            Int_t i = (x - 1) + ((y - 1) * nBins[0]);
            his->SetBinContent(x, y, norm * block->GetData(i));
         }
      }
   }
   else if(dim == 3){
      his = new TH3D(name, title,
                     nBins[0], 0, nBins[0],
                     nBins[1], 0, nBins[1],
                     nBins[2], 0, nBins[2]);
      for(Int_t x = 1; x <= nBins[0]; x++){
         for(Int_t y = 1; y <= nBins[1]; y++){
            for(Int_t z = 1; z <= nBins[2]; z++){
               Int_t i = (x - 1) + ((y - 1) * nBins[0]) + ((y - 1) * nBins[0] * nBins[1]);
               his->SetBinContent(x, y, z, norm * block->GetData(i));
            }
         }
      }
   }

   block->PrintHeader();
   block->PrintMetadata();
   
   return his;
}

void TMakeTree::SaveTree()
{
   
   TFile *file = new TFile("out.root", "RECREATE");

   fField->Ex->Write();

   file->Close();
}

Int_t TMakeTree::GetBlockIndex(TString ID)
{
   for(Int_t i = 0; i < fReader->GetNBlocks(); i++)
      if(ID == fReader->fBlock[i]->GetID()) return i;

   return -1;
}
