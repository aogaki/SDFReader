#ifndef TBLOCKPOINTMESH
#define TBLOCKPOINTMESH 1

#include <fstream>
#include <vector>

#include <TString.h>

#include "TBlock.hpp"


class TBlockPointMesh: public TBlock
{
public:
   TBlockPointMesh(std::ifstream *, Long_t, Int_t, Int_t);
   
   void ReadMetadata();
   void PrintMetadata();

   void ReadData();
   Double_t GetData(Int_t i);
   
   Long64_t GetNParticles() {return fNParticles;};
   Double_t GetMinVal(Int_t i) {return fMinVal[i] * fNormFactor[i];};
   Double_t GetMaxVal(Int_t i) {return fMaxVal[i] * fNormFactor[i];};
   TString GetAxisLabel(Int_t i) {return fAxisLabel[i];};
   TString GetUnits(Int_t i) {return fUnits[i];};
   
private:
   Double_t fNormFactor[3];
   TString fAxisLabel[3];
   TString fUnits[3];
   Int_t fGeoType;
   Double_t fMinVal[3];
   Double_t fMaxVal[3];
   Long64_t fNParticles;

   Double_t GetData64(Int_t i);
   Double_t GetData32(Int_t i);
   
};

#endif
