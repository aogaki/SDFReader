/*
 For block manupilating class
*/

#include <iostream>

#include "TBlockPlainVar.hpp"


using std::cout;
using std::endl;

TBlockPlainVar::TBlockPlainVar(std::ifstream *file, Long_t location,
                                     Int_t stringLength, Int_t headerLength)
   :TBlock()
{
   fInputFile = file;
   fBlockLocation = location;
   fStringLength = stringLength;
   fHeaderLength = headerLength;
   fMetadataLocation = fBlockLocation + fHeaderLength;

   fNormFactor = 0.;
   fUnits = "";
   fMeshID = "";
   fNGrids[0] = fNGrids[1] = fNGrids[2] =  0;
   fStagger = -1;

   fDataSize = 0;
   
   ReadHeader();


   if(fDataType == 4) fDataSize = fDataLength / sizeof(Double_t);
   else if(fDataType == 3) fDataSize = fDataLength / sizeof(Float_t);
   else{
      cout << "DataType error in GetHeader@BlockPlaneVar" << endl;
      cout << "Now, only double and float data types are implemented" << endl;
      exit(0);
   }
}

void TBlockPlainVar::ReadMetadata()
{
   fInputFile->seekg(fMetadataLocation, std::ios::beg);
   fInputFile->read((Char_t *)&fNormFactor, sizeof(fNormFactor));
   const Int_t charSize = 32;
   Char_t *buf = new Char_t[charSize];
   fInputFile->read(buf, sizeof(Char_t) * charSize);
   fUnits = buf;
   fInputFile->read(buf, sizeof(Char_t) * charSize);
   fMeshID = buf;
   delete[] buf;
   fInputFile->read((Char_t *)fNGrids, sizeof(Int_t) * fNDims);
   fInputFile->read((Char_t *)&fStagger, sizeof(fStagger));
}

void TBlockPlainVar::PrintMetadata()
{
   cout << "Normalization factor: " << fNormFactor <<"\n"
        << "Units: " << fUnits <<"\n"
        << "Mesh ID: " << fMeshID << endl;
   cout << "No. Grids" <<"\t"
        << fNGrids[0] <<"\t"
        << fNGrids[1] <<"\t"
        << fNGrids[2] << endl;
   cout << "Stagger: " << fStagger << endl;
}
