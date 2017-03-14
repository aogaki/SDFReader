/*
 For block manupilating class
*/

#include <iostream>

#include "TSDFReader.hpp"
#include "TSDFBlockPlainVar.hpp"


using std::cout;
using std::endl;

TSDFBlockPlainVar::TSDFBlockPlainVar(std::ifstream *file, Long_t location,
                                     Int_t stringLength, Int_t headerLength)
   :TSDFBlock()
{
   fInputFile = file;
   fBlockLocation = location;
   fStringLength = stringLength;
   fHeaderLength = headerLength;
   fMetadataLocation = fBlockLocation + fHeaderLength;

   fNormFactor = 0.;
   fUnits = "";
   fMeshID = "";
   fNGrid[0] = fNGrid[1] = fNGrid[2] =  0;
   fStagger = -1;

   fDataSize = 0;
   
   ReadHeader();
}

void TSDFBlockPlainVar::ReadMetadata()
{
   fInputFile->seekg(fMetadataLocation, std::ios::beg);
   fInputFile->read((Char_t *)&fNormFactor, sizeof(fNormFactor));
   const Int_t charSize = 32;
   Char_t *buf = new Char_t[charSize];
   fInputFile->read(buf, sizeof(Char_t) * charSize);
   fUnits = buf;
   fInputFile->read(buf, sizeof(Char_t) * charSize);
   fMeshID = buf;
   delete buf;
   fInputFile->read((Char_t *)fNGrid, sizeof(Int_t) * fNDims);
   fInputFile->read((Char_t *)&fStagger, sizeof(fStagger));
}

void TSDFBlockPlainVar::PrintMetadata()
{
   cout << "Normalization factor: " << fNormFactor <<"\n"
        << "Units: " << fUnits <<"\n"
        << "Mesh ID: " << fMeshID << endl;
   cout << "No. Grids" <<"\t"
        << fNGrid[0] <<"\t"
        << fNGrid[1] <<"\t"
        << fNGrid[2] << endl;
   cout << "Stagger: " << fStagger << endl;
}

void TSDFBlockPlainVar::ReadData()
{
   fInputFile->seekg(fDataLocation, std::ios::beg);

   if(fNDims == 1) fDataSize = fNGrid[0];
   else if(fNDims == 2) fDataSize = fNGrid[0] * fNGrid[1];
   else if(fNDims == 3) fDataSize = fNGrid[0] * fNGrid[1] * fNGrid[2];
   else{
      cout << "dimension size error in BlockPlaneVar" << endl;
      exit(0);
   }

   if(fDataType == 4) ReadData64();
   else if(fDataType == 3) ReadData32();
   else{
      cout << "DataType error in ReadData@BlockPlaneVar" << endl;
      exit(0);
   }
}

void TSDFBlockPlainVar::ReadData64()
{
   Double_t buf;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData.push_back(buf);
   }
}

void TSDFBlockPlainVar::ReadData32()
{
   Float_t buf;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData.push_back(Double_t(buf));
   }
}

Double_t TSDFBlockPlainVar::GetData(Int_t i)
{
   // Check the vector size
   return fData[i];
}
