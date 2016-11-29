#include <iostream>

#include "TSDFReader.hpp"


int main(int argc, char **argv)
{
   TString fileName = "data/0300.sdf";
   TSDFReader *reader = new TSDFReader(fileName);

   std::cout << reader->GetJobID1() <<"\t"
             << reader->GetJobID2() <<"\t"
             << reader->GetNBlocks() << std::endl;

   //for(Int_t i = 0; i < reader->GetNBlocks(); i++){
   for(Int_t i = 0; i < 11; i++){
      reader->ReadBlockHeader();
      reader->ReadBlockMetadata();
      reader->ReadBlockData();
      reader->GoNextBlock();
   }
   
   delete reader;

   return 0;
}
