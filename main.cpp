#include <iostream>

#include "TSDFReader.hpp"
#include "TConverter.hpp"


using std::cout;
using std::endl;

int main(int argc, char **argv)
{
   //TString fileName = "Data/0006.sdf";
   TString fileName = "Data/0010.sdf";

   if(argc == 2) fileName = argv[1];
   
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

   TConverter *converter = new TConverter(reader);
   converter->GetData();
   
   delete converter;
   delete reader;

   return 0;
}
