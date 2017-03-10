#ifndef TSDFBLOCKPLAINVAR
#define TSDFBLOCKPLAINVAR 1

#include <fstream>
#include <vector>

#include <TString.h>

#include "TSDFBlock.hpp"


class TSDFBlockPlainVar: public TSDFBlock
{
public:
   TSDFBlockPlainVar(std::ifstream *, Long_t, Int_t, Int_t);
   virtual ~TSDFBlockPlainVar();
   
   void ReadMetadata();
   void PrintMetadata();

   void ReadData();

   Double_t GetData(Int_t i);
   
private:
   Double_t fNormFactor;
   Char_t fUnits[33];
   Char_t fMeshID[33];
   Int_t fNGrid[3];
   Int_t fStagger;

   void ReadData64();
   std::vector<Double_t> fData64;

   void ReadData32();
   std::vector<Float_t> fData32;
};

#endif
