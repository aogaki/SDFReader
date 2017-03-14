#include <iostream>

#include "TSDFReader.hpp"
#include "TMakeTree.hpp"


using std::cout;
using std::endl;

int main(int argc, char **argv)
{
   //TString fileName = "Data/0006.sdf";
   TString fileName = "Data/0030.sdf";

   // Reading file information
   TSDFReader *reader = new TSDFReader(fileName);
   Int_t counter = 0;
   for(auto block: reader->fBlock){
      cout << counter++ <<"\t"
           << block->GetBlockType() <<"\t"
           << block->GetDataType() <<"\t"
           << block->GetName() <<"\t"
           << block->GetID() <<"\t"
           << endl;
   }

   //block->PrintData();
   
   // Filling root file
   // NYI

   TMakeTree *makeTree = new TMakeTree(reader);
   makeTree->GetData();
   
   delete makeTree;
   delete reader;

   return 0;
}
