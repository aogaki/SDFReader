#include <iostream>

#include "TSDFReader.hpp"


int main(int argc, char **argv)
{
   TString fileName = "Data/0300.sdf";
   TSDFReader *reader = new TSDFReader(fileName);

   for(auto block: reader->fBlock){
      //if(block->GetDataSize() > 0.)
         std::cout << block->GetName() <<"\t"
                   << block->GetID() <<"\t"
                   << std::endl;
   }
   
   delete reader;

   return 0;
}
