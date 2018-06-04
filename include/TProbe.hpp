#ifndef TPROBE
#define TPROBE 1
/*
    Macroparticle tree class.
*/

#include <TString.h>

#include "TBlockPointMesh.hpp"
#include "TBlockPointVar.hpp"
#include "TSDFReader.hpp"

class TProbe
{
 public:
  TProbe(TSDFReader *reader, TString probeName);
  ~TProbe();

  void MakeTree();

 private:
  TSDFReader *fReader;
  TString fProbeName;

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
