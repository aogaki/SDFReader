#include <iostream>

#include "TSDFReader.hpp"
#include "TConverter.hpp"


using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  if(argc != 3 && argc != 4){
    cout << "Useage" <<endl;
    cout << "sdf2root \"input SDF file name\" \"output ROOT file name\" \"Particle name\"\n"
	 << endl;
    exit(0);
  }
  
   //TString fileName = "Data/0006.sdf";
   TString inputName = "Data/0100.sdf";
   TString outputName = "out.root";
   TString targetName = "";
   
   if(argc > 1) inputName = argv[1];
   if(argc > 2) outputName = argv[2];
   if(argc > 3) targetName = argv[3];
   
   // Reading file information
   TSDFReader *reader = new TSDFReader(inputName);

   Int_t counter = 0;
   for(auto block: reader->fBlock){
      cout << counter++ <<"\t"
           << block->GetBlockType() <<"\t"
           << block->GetDataType() <<"\t"
           << block->GetName() <<"\t"
           << block->GetID() <<"\t"
           << endl;
   }

   TConverter *converter = new TConverter(reader, outputName, targetName);
   converter->GetData();
   delete converter;

   delete reader;

   return 0;
}
