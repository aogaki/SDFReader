#include <iostream>

#include "TSDFReader.hpp"


using std::cout;
using std::endl;

int main(int argc, char **argv)
{
   TString fileName = "Data/0006.sdf";
   TSDFReader *reader = new TSDFReader(fileName);

   for(auto block: reader->fBlock){
      cout << block->GetBlockType() <<"\t"
           << block->GetDataType() <<"\t"
           << block->GetName() <<"\t"
           << block->GetID() <<"\t"
           << endl;
   }
   
   delete reader;

   return 0;
}
