#include <iostream>

#include "TMeshValue.hpp"
#include "TBlockPlainMesh.hpp"
#include "TBlockPlainVar.hpp"


using std::cout;
using std::endl;

TMeshValue::TMeshValue(TSDFReader *reader)
   : fReader(reader),
     fEx(nullptr),
     fEy(nullptr),
     fEz(nullptr),   
     fBx(nullptr),
     fBy(nullptr),
     fBz(nullptr),
     fJx(nullptr),
     fJy(nullptr),
     fJz(nullptr),
     fEkBar(nullptr),
     fEkFlux(nullptr),
     fMassDensity(nullptr),
     fChargeDensity(nullptr),
     fNumberDensity(nullptr),
     fPoyntFlux(nullptr)
{
   for(Int_t i = 0; i < 3; i++){
      fHisMin[i] = 0;
      fHisMax[i] = 0;
      fHisBinWidth[i] = 0;
      fHisUnit[i] = "";
      fHisLabel[i] = "";
   }
}

TMeshValue::~TMeshValue()
{
   // Those are automatically deleted by ROOT?
/*
   if(fEx != nullptr) delete fEx;
   if(fEy != nullptr) delete fEy;
   if(fEz != nullptr) delete fEz;

   if(fBx != nullptr) delete fBx;
   if(fBy != nullptr) delete fBy;
   if(fBz != nullptr) delete fBz;

   if(fJx != nullptr) delete fJx;
   if(fJy != nullptr) delete fJy;
   if(fJz != nullptr) delete fJz;

   if(fEkBar != nullptr) delete fEkBar;
   if(fEkFlux != nullptr) delete fEkFlux;
   if(fMassDensity != nullptr) delete fMassDensity;
   if(fChargeDensity != nullptr) delete fChargeDensity;
   if(fNumberDensity != nullptr) delete fNumberDensity;
   if(fPoyntFlux != nullptr) delete fPoyntFlux;
*/
}

void TMeshValue::Save()
{
   if(fEx != nullptr) fEx->Write();
   if(fEy != nullptr) fEy->Write();
   if(fEz != nullptr) fEz->Write();

   if(fBx != nullptr) fBx->Write();
   if(fBy != nullptr) fBy->Write();
   if(fBz != nullptr) fBz->Write();

   if(fJx != nullptr) fJx->Write();
   if(fJy != nullptr) fJy->Write();
   if(fJz != nullptr) fJz->Write();

   if(fEkBar != nullptr) fEkBar->Write();
   if(fEkFlux != nullptr) fEkFlux->Write();
   if(fMassDensity != nullptr) fMassDensity->Write();
   if(fChargeDensity != nullptr) fChargeDensity->Write();
   if(fNumberDensity != nullptr) fNumberDensity->Write();
   if(fPoyntFlux != nullptr) fPoyntFlux->Write();

}

void TMeshValue::ReadMeshGrid()
{
   Int_t index = fReader->GetBlockIndex("grid");
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


void TMeshValue::GetMeshData()
{
   fEx = GetMeshHis("ex", "Ex", "Electric field x");
   fEy = GetMeshHis("ey", "Ey", "Electric field y");
   fEz = GetMeshHis("ez", "Ez", "Electric field z");
   
   fBx = GetMeshHis("bx", "Bx", "Magnetic field x");
   fBy = GetMeshHis("by", "By", "Magnetic field y");
   fBz = GetMeshHis("bz", "Bz", "Magnetic field z");

   fJx = GetMeshHis("jx", "Jx", "Current x");
   fJy = GetMeshHis("jy", "Jy", "Current y");
   fJz = GetMeshHis("jz", "Jz", "Current z");

   fNumberDensity = GetMeshHis("number_density/electron", "number_density", "Number density");
}

TH1 *TMeshValue::GetMeshHis(TString blockName, TString hisName, TString hisTitle)
{
   TH1 *his{nullptr};

   Int_t index = fReader->GetBlockIndex(blockName);
   if(index < 0) return nullptr;
   
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

