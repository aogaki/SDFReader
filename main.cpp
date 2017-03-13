#include <iostream>

#include "TSDFReader.hpp"


int main(int argc, char **argv)
{
   TString fileName = "Data/0010.sdf";
   TSDFReader *reader = new TSDFReader(fileName);

   for(auto block: reader->fBlock){
         std::cout << block->GetBlockType() <<"\t"
                   << block->GetDataType() <<"\t"
                   << block->GetName() <<"\t"
                   << block->GetID() <<"\t"
                   << std::endl;
   }
   
   delete reader;

   return 0;
}
