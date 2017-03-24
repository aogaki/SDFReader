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
     fTree(nullptr),
     fMeshValue(nullptr)
{
   for(Int_t i = 0; i < 3; i++){
      fHisMin[i] = 0;
      fHisMax[i] = 0;
      fHisBinWidth[i] = 0;
      fHisUnit[i] = "";
      fHisLabel[i] = "";
   }
   fMeshValue = new TMeshValue();
   fMeshValue->SetStep(fReader->GetStep());
   fMeshValue->SetTime(fReader->GetTime());
   ReadMeshGrid();
   
   fOutput = new TFile("out.root", "RECREATE");
   fTree= new TTree("EPOCH", "EPOCH simulation data");

   FindPar();
   GetParGrid();
}

TConverter::~TConverter()
{
   SaveTree();
   fOutput->Close();
   
   delete fMeshValue;
}

void TConverter::GetData()
{
   GetMeshData();
}


void TConverter::GetMeshData()
{
   fMeshValue->SetEx(GetMeshHis("ex", "Ex", "Electric field x"));
   fMeshValue->SetEy(GetMeshHis("ey", "Ey", "Electric field y"));
   fMeshValue->SetEz(GetMeshHis("ez", "Ez", "Electric field z"));
   
   fMeshValue->SetBx(GetMeshHis("bx", "Bx", "Magnetic field x"));
   fMeshValue->SetBy(GetMeshHis("by", "By", "Magnetic field y"));
   fMeshValue->SetBz(GetMeshHis("bz", "Bz", "Magnetic field z"));

   fMeshValue->SetJx(GetMeshHis("jx", "Jx", "Current x"));
   fMeshValue->SetJy(GetMeshHis("jy", "Jy", "Current y"));
   fMeshValue->SetJz(GetMeshHis("jz", "Jz", "Current z"));
}

TH1 *TConverter::GetMeshHis(TString blockName, TString hisName, TString hisTitle)
{
   TH1 *his{nullptr};

   Int_t index = GetBlockIndex(blockName);
   if(index < 0) return new TH1D();// nullptr is better?
   
   TBlockPlainVar *block = (TBlockPlainVar*)fReader->fBlock[index];
   block->ReadData();

   TString name = hisName;
   TString title = hisTitle;
   Double_t norm = block->GetNormFactor();

   Int_t nBins[3] = {
      block->GetNGrids(0),
      block->GetNGrids(1),
      block->GetNGrids(2),
   };
   
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
                     nBins[0], fHisMin[0] - delta[0] + shift[0], fHisMax[0] + delta[0] + shift[0]);
      his->SetXTitle(fHisLabel[0] + " [" + fHisUnit[0] + "]");
      for(Int_t x = 1; x <= nBins[0]; x++){
         his->SetBinContent(x, norm * block->GetData(x - 1));
      }
   }
   else if(dim == 2){
      his = new TH2D(name, title,
                     nBins[0], fHisMin[0] - delta[0] + shift[0], fHisMax[0] + delta[0] + shift[0],
                     nBins[1], fHisMin[1] - delta[1] + shift[1], fHisMax[1] + delta[1] + shift[1]);
      his->SetXTitle(fHisLabel[0] + " [" + fHisUnit[0] + "]");
      his->SetYTitle(fHisLabel[1] + " [" + fHisUnit[1] + "]");
      for(Int_t x = 1; x <= nBins[0]; x++){
         for(Int_t y = 1; y <= nBins[1]; y++){
            Int_t i = (x - 1) + ((y - 1) * nBins[0]);
            his->SetBinContent(x, y, norm * block->GetData(i));
         }
      }
   }
   else if(dim == 3){
      his = new TH3D(name, title,
                     nBins[0], fHisMin[0] - delta[0] + shift[0], fHisMax[0] + delta[0] + shift[0],
                     nBins[1], fHisMin[1] - delta[1] + shift[1], fHisMax[1] + delta[1] + shift[1],
                     nBins[2], fHisMin[2] - delta[2] + shift[2], fHisMax[2] + delta[2] + shift[2]);
      his->SetXTitle(fHisLabel[0] + " [" + fHisUnit[0] + "]");
      his->SetYTitle(fHisLabel[1] + " [" + fHisUnit[1] + "]");
      his->SetZTitle(fHisLabel[2] + " [" + fHisUnit[2] + "]");
      for(Int_t x = 1; x <= nBins[0]; x++){
         for(Int_t y = 1; y <= nBins[1]; y++){
            for(Int_t z = 1; z <= nBins[2]; z++){
               Int_t i = (x - 1) + ((y - 1) * nBins[0]) + ((z - 1) * nBins[0] * nBins[1]);
               his->SetBinContent(x, y, z, norm * block->GetData(i));
            }
         }
      }
   }

   //block->PrintHeader();
   //block->PrintMetadata();
   
   return his;
}

void TConverter::ReadMeshGrid()
{
   Int_t index = GetBlockIndex("grid");
   if(index < 0) return;
   
   TBlockPlainMesh *block = (TBlockPlainMesh*)fReader->fBlock[index];

   block->ReadMetadata();
   Int_t nGrids[3];
   for(Int_t i = 0; i < 3; i++){
      fHisMin[i] = block->GetMinVal(i);
      fHisMax[i] = block->GetMaxVal(i);
      nGrids[i] = block->GetNGrids(i);
      fHisUnit[i] = block->GetUnits(i);
      fHisLabel[i] = block->GetAxisLabel(i);
   }

   block->ReadData();
   fHisBinWidth[0] = fabs(block->GetData(1) - block->GetData(0));
   if(nGrids[1] > 1) fHisBinWidth[1] = fabs(block->GetData(1 + nGrids[0]) - block->GetData(nGrids[0]));
   if(nGrids[2] > 1) fHisBinWidth[2] = fabs(block->GetData(1 + nGrids[0] + nGrids[1])
                                            - block->GetData(nGrids[0] + nGrids[1]));
}

void TConverter::SaveTree()
{
   fMeshValue->Write("MeshValue");
   fTree->Write();
}

Int_t TConverter::GetBlockIndex(TString ID)
{
   for(Int_t i = 0; i < fReader->GetNBlocks(); i++)
      if(ID == fReader->fBlock[i]->GetID()) return i;

   return -1;
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

void TConverter::GetParGrid()
{
   Int_t i = 0;
   
   Int_t index = GetBlockIndex("grid/" + fParName[i]);
   if(index < 0) return;

   cout << index << endl;
   
   TBlockPointMesh *block = (TBlockPointMesh*)fReader->fBlock[index];
   block->ReadMetadata();
   block->ReadData();
   
   block->PrintHeader();
   block->PrintMetadata();

}