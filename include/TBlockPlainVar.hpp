#ifndef TBLOCKPLAINVAR
#define TBLOCKPLAINVAR 1

#include <fstream>
#include <vector>

#include <TString.h>

#include "TBlock.hpp"


class TBlockPlainVar: public TBlock
{
public:
   TBlockPlainVar(std::ifstream *, Long_t, Int_t, Int_t);
      
   void ReadMetadata();
   void PrintMetadata();

   Double_t GetNormFactor(){return fNormFactor;};
   Int_t GetNGrids(Int_t i){return fNGrids[i];};
   Int_t GetStagger(){return fStagger;};
   
private:
   Double_t fNormFactor;
   TString fUnits;
   TString fMeshID;
   Int_t fNGrids[3];
   Int_t fStagger;
};

#endif
