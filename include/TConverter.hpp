#ifndef TCONVERTER
#define TCONVERTER 1

/*
    Converter class
*/

#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

#include "TSDFReader.hpp"
#include "TBlockPlainMesh.hpp"
#include "TMeshValue.hpp"
#include "TMacroParticle.hpp"


class TConverter
{
public:
  TConverter(TSDFReader *reader, TString outName);
   ~TConverter();

   void GetData();
   
private:
   TSDFReader *fReader;

   TFile *fOutput;
   
   // Mesh values
   TMeshValue *fMeshValue;

   // Particle values
   std::vector<std::vector<TMacroParticle>> fMacroPar;
   std::vector<TString> fParName;
   void FindPar();
   void GetParGrid();
   void GetParData();
};


#endif

