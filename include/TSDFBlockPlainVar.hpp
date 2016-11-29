#ifndef TSDFBLOCKPLAINVAR
#define TSDFBLOCKPLAINVAR 1

#include <fstream>

#include <TString.h>

#include "TSDFBlock.hpp"


class TSDFBlockPlainVar: public TSDFBlock
{
public:
   TSDFBlockPlainVar(std::ifstream *, Long_t, Int_t, Int_t);
   virtual ~TSDFBlockPlainVar();
   
   void ReadMetadata();
   void ReadData();
   
private:
   void PrintMetadata();
   Double_t fNormFactor;
   Char_t fUnits[33];
   Char_t fMeshID[33];
   Int_t fNGrid[3];
   Int_t fStagger;
};

#endif
