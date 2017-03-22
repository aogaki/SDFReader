#include <iostream>

#include <TFile.h>

#include "TMakeTree.hpp"
#include "TBlockPlainVar.hpp"

using std::cout;
using std::endl;

TMakeTree::TMakeTree(TSDFReader *reader)
   : fReader(reader),
     fEx(nullptr), 
     fEy(nullptr), 
     fEz(nullptr), 
     fBx(nullptr), 
     fBy(nullptr), 
     fBz(nullptr), 
     fJx(nullptr), 
     fJy(nullptr), 
     fJz(nullptr)
{
   for(Int_t i = 0; i < 3; i++){
      fHisMin[i] = 0;
      fHisMax[i] = 0;
      fHisNbins[i] = 0;
      fHisBinWidth[i] = 0;
      fHisUnit[i] = "";
      fHisLabel[i] = "";
   }
   ReadFieldGrid();
}

TMakeTree::~TMakeTree()
{
   SaveTree();
}

void TMakeTree::GetData()
{
   GetFieldData();
}


void TMakeTree::GetFieldData()
{
   fEx = GetFieldHis("ex", "Ex", "Electric field x");
   fEy = GetFieldHis("ey", "Ey", "Electric field y");
   fEz = GetFieldHis("ez", "Ez", "Electric field z");
   
   fBx = GetFieldHis("bx", "Bx", "Magnetic field x");
   fBy = GetFieldHis("by", "By", "Magnetic field y");
   fBz = GetFieldHis("bz", "Bz", "Magnetic field z");

   fJx = GetFieldHis("jx", "Jx", "Current x");
   fJy = GetFieldHis("jy", "Jy", "Current y");
   fJz = GetFieldHis("jz", "Jz", "Current z");
}

TH1 *TMakeTree::GetFieldHis(TString blockName, TString hisName, TString hisTitle)
{
   TH1 *his{nullptr};

   Int_t index = GetBlockIndex(blockName);
   if(index < 0) return nullptr;
   
   TBlockPlainVar *block = (TBlockPlainVar*)fReader->fBlock[index];
   block->ReadData();

   TString name = hisName;
   TString title = hisTitle;
   Double_t norm = block->GetNormFactor();

   Int_t stagger = block->GetStagger();
   Int_t dim = block->GetNDims();

   Double_t delta[3] = {
      fHisBinWidth[0] / 2.,
      fHisBinWidth[1] / 2.,
      fHisBinWidth[2] / 2.,      
   };

   // stagger.  Don't use bit mask.  Use number
   // Check is this right or not.
   Double_t shift[3] = {0., 0., 0.};
   if(stagger == 1)
      shift[0] += delta[0];
   else if(stagger == 2)
      shift[1] += delta[1];
   else if(stagger == 4)
      shift[2] += delta[2];
   else if(stagger == 3){
      shift[0] += delta[0];
      shift[1] += delta[1];
   }   
   else if(stagger == 5){
      shift[0] += delta[0];
      shift[2] += delta[2];
   }   
   else if(stagger == 6){
      shift[1] += delta[1];
      shift[2] += delta[2];
   }   
   else if(stagger == 7){
      shift[0] += delta[0];
      shift[1] += delta[1];
      shift[2] += delta[2];
   }
   else if(stagger != 0){
      cout << "stagger error: " << stagger << endl;
   }
   
   if(dim == 1){
      his = new TH1D(name, title,
                     fHisNbins[0], fHisMin[0] - delta[0] + shift[0], fHisMax[0] + delta[0] + shift[0]);
      for(Int_t x = 1; x <= fHisNbins[0]; x++){
         his->SetBinContent(x, norm * block->GetData(x - 1));
      }
   }
   else if(dim == 2){
      his = new TH2D(name, title,
                     fHisNbins[0], fHisMin[0] - delta[0] + shift[0], fHisMax[0] + delta[0] + shift[0],
                     fHisNbins[1], fHisMin[1] - delta[1] + shift[1], fHisMax[1] + delta[1] + shift[1]);
      for(Int_t x = 1; x <= fHisNbins[0]; x++){
         for(Int_t y = 1; y <= fHisNbins[1]; y++){
            Int_t i = (x - 1) + ((y - 1) * fHisNbins[0]);
            his->SetBinContent(x, y, norm * block->GetData(i));
         }
      }
   }
   else if(dim == 3){
      his = new TH3D(name, title,
                     fHisNbins[0], fHisMin[0] - delta[0] + shift[0], fHisMax[0] + delta[0] + shift[0],
                     fHisNbins[1], fHisMin[1] - delta[1] + shift[1], fHisMax[1] + delta[1] + shift[1],
                     fHisNbins[2], fHisMin[2] - delta[2] + shift[2], fHisMax[2] + delta[2] + shift[2]);
      for(Int_t x = 1; x <= fHisNbins[0]; x++){
         for(Int_t y = 1; y <= fHisNbins[1]; y++){
            for(Int_t z = 1; z <= fHisNbins[2]; z++){
               Int_t i = (x - 1) + ((y - 1) * fHisNbins[0]) + ((y - 1) * fHisNbins[0] * fHisNbins[1]);
               his->SetBinContent(x, y, z, norm * block->GetData(i));
            }
         }
      }
   }

   //block->PrintHeader();
   //block->PrintMetadata();
   
   return his;
}

void TMakeTree::ReadFieldGrid()
{
   Int_t index = GetBlockIndex("grid");
   if(index < 0) return;
   
   TBlockPlainMesh *block = (TBlockPlainMesh*)fReader->fBlock[index];

   block->ReadMetadata();
   for(Int_t i = 0; i < 3; i++){
      fHisMin[i] = block->GetMinVal(i);
      fHisMax[i] = block->GetMaxVal(i);
      fHisNbins[i] = block->GetNGrids(i);
      fHisUnit[i] = block->GetUnits(i);
      fHisLabel[i] = block->GetAxisLabel(i);
   }

   block->ReadData();
   fHisBinWidth[0] = fabs(block->GetData(1) - block->GetData(0));
   if(fHisNbins[1] > 1) fHisBinWidth[1] = fabs(block->GetData(1 + fHisNbins[0]) - block->GetData(fHisNbins[0]));
   if(fHisNbins[2] > 1) fHisBinWidth[2] = fabs(block->GetData(1 + fHisNbins[0] * fHisNbins[1])
                                               - block->GetData(fHisNbins[0] * fHisNbins[1]));
}

void TMakeTree::SaveTree()
{
   
   TFile *file = new TFile("out.root", "RECREATE");

   if(fEx != nullptr) fEx->Write();
   if(fEy != nullptr) fEy->Write();
   if(fEz != nullptr) fEz->Write();

   if(fBx != nullptr) fBx->Write();
   if(fBy != nullptr) fBy->Write();
   if(fBz != nullptr) fBz->Write();

   if(fJx != nullptr) fJx->Write();
   if(fJy != nullptr) fJy->Write();
   if(fJz != nullptr) fJz->Write();

   file->Close();
}

Int_t TMakeTree::GetBlockIndex(TString ID)
{
   for(Int_t i = 0; i < fReader->GetNBlocks(); i++)
      if(ID == fReader->fBlock[i]->GetID()) return i;

   return -1;
}
