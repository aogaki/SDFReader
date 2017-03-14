#ifndef TSDFBLOCKPOINTMESH
#define TSDFBLOCKPOINTMESH 1

#include <fstream>
#include <vector>

#include <TString.h>

#include "TSDFBlock.hpp"


class TSDFBlockPointMesh: public TSDFBlock
{
public:
   TSDFBlockPointMesh(std::ifstream *, Long_t, Int_t, Int_t);
   
   void ReadMetadata();
   void PrintMetadata();

private:
   Double_t fNormFactor[3];
   TString fAxisLabel[3];
   TString fUnits[3];
   Int_t fGeoType;
   Double_t fMinVal[3];
   Double_t fMaxVal[3];
   Long64_t fNParticles;
   
};

#endif
