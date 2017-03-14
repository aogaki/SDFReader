#include <iostream>

#include "TSDFReader.hpp"


using std::cout;
using std::endl;

int main(int argc, char **argv)
{
   TString fileName = "Data/0006.sdf";

   // Reading file information
   TSDFReader *reader = new TSDFReader(fileName);
   Int_t counter = 0;
   for(auto block: reader->fBlock){
      cout << counter++ <<"\t"
           <<block->GetBlockType() <<"\t"
           << block->GetDataType() <<"\t"
           << block->GetName() <<"\t"
           << block->GetID() <<"\t"
           << endl;
   }

   // Filling root file
   // NYI
   
   delete reader;

   return 0;
}
