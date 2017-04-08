#ifndef TMACROPARTICLE
#define TMACROPARTICLE 1
/*
    Macroparticle tree class.  
*/

#include <TString.h>

#include "TSDFReader.hpp"
#include "TBlockPointMesh.hpp"
#include "TBlockPointVar.hpp"


class TMacroParticle
{
public:
  TMacroParticle(TSDFReader *reader, TString parName);
   ~TMacroParticle();

  void MakeTree();
  
private:
  TSDFReader *fReader;
  TString fParName;

  // position information
  void FindMesh();
  TBlockPointMesh *fGrid;

  // particle parameters
  void FindVar();
  TBlockPointVar *FindBlockPointVar(TString id);
 
  TBlockPointVar *fPx;
  TBlockPointVar *fPy;
  TBlockPointVar *fPz;
  
  TBlockPointVar *fVx;
  TBlockPointVar *fVy;
  TBlockPointVar *fVz;

  TBlockPointVar *fEk; // What is the E? this block shows the kinetic ene.

  TBlockPointVar *fWeight;
};

#endif
