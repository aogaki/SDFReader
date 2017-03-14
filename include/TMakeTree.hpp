#ifndef TMAKETREE
#define TMAKETREE 1

/*
Making tree class
*/

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

#include "TSDFReader.hpp"
#include "TFieldValue.hpp"
#include "TBlockPlainMesh.hpp"


class TMakeTree
{
public:
   TMakeTree(TSDFReader *reader);
   ~TMakeTree();

   void GetData();
   
private:
   TSDFReader *fReader;
   TFieldValue *fField;

   TBlockPlainMesh *fFieldGrid;
   
   void SaveTree();

   Int_t GetBlockIndex(TString ID);


   TH1 *GetEx();
   
};


#endif

