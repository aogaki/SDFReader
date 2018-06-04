#ifndef TCONVERTER
#define TCONVERTER 1

/*
    Converter class
*/

#include <vector>

#include <TFile.h>
#include <TTree.h>

#include "RunMode.hpp"
#include "TBlockPlainMesh.hpp"
#include "TMacroParticle.hpp"
#include "TMeshValue.hpp"
#include "TSDFReader.hpp"

class TConverter
{
 public:
  TConverter(TSDFReader *reader, TString outName, RunMode runMode);
  ~TConverter();

  void GetData();

 private:
  TSDFReader *fReader;

  TString fOutName;

  RunMode fRunMode;

  // Particle values
  std::vector<TString> fParName;
  void FindPar();

  // For probe
  std::vector<TString> fProbeName;
  void FindProbe();
};

#endif
