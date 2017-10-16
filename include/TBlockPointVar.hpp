#ifndef TBLOCKPOINTVAR
#define TBLOCKPOINTVAR 1

#include <fstream>
#include <vector>

#include <TString.h>

#include "TBlock.hpp"

class TBlockPointVar : public TBlock
{
 public:
  TBlockPointVar(std::ifstream *, Long_t, Int_t, Int_t);

  void ReadMetadata();
  void PrintMetadata();

  void ReadData();
  Double_t GetData(Int_t i);
  Long64_t GetID(Int_t i);  // Only ID info uses integer

 private:
  Double_t fNormFactor;
  TString fUnits;
  TString fMeshID;
  Long64_t fNParticles;

  Double_t GetData64(Int_t i);
  Double_t GetData32(Int_t i);
};

#endif
