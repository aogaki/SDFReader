#ifndef TSDFBLOCKPOINTVAR
#define TSDFBLOCKPOINTVAR 1

#include <fstream>
#include <vector>

#include <TString.h>

#include "TSDFBlock.hpp"


class TSDFBlockPointVar: public TSDFBlock
{
public:
   TSDFBlockPointVar(std::ifstream *, Long_t, Int_t, Int_t);
      
   void ReadMetadata();
   void PrintMetadata();

private:
   Double_t fNormFactor;
   TString fUnits;
   TString fMeshID;
   Long64_t fNParticles;
};

#endif
