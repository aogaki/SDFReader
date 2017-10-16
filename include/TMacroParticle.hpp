#ifndef TMACROPARTICLE
#define TMACROPARTICLE 1
/*
    Macroparticle tree class.
*/

#include <TString.h>
#include <TTree.h>

#include "TBlockPointMesh.hpp"
#include "TBlockPointVar.hpp"
#include "TSDFReader.hpp"

class TMacroParticle
{
 public:
  TMacroParticle(TSDFReader *reader, TString parName);
  ~TMacroParticle();

  void MakeTree();
  void AddTree(TTree *tree);

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

  TBlockPointVar *fEk;  // What is the E? this block shows the kinetic ene.

  TBlockPointVar *fWeight;

  TBlockPointVar *fOptDep;

  TBlockPointVar *fQEDEne;

  TBlockPointVar *fID;
};

#endif
