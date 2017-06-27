#ifndef TCONVERTER
#define TCONVERTER 1

/*
    Converter class
*/

#include <vector>

#include <TFile.h>
#include <TTree.h>

#include "TSDFReader.hpp"
#include "TBlockPlainMesh.hpp"
#include "TMeshValue.hpp"
#include "TMacroParticle.hpp"
#include "RunMode.hpp"


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
   std::vector<std::vector<TMacroParticle>> fMacroPar;
   std::vector<TString> fParName;
   TString fTargetName;
   void FindPar();
   void GetParGrid();
   void GetParData();
};


#endif

