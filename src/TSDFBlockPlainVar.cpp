/*
 For block manupilating class
*/

#include <iostream>

#include "TSDFReader.hpp"
#include "TSDFBlockPlainVar.hpp"


TSDFBlockPlainVar::TSDFBlockPlainVar(std::ifstream *file, Long_t location,
                                     Int_t stringLength, Int_t headerLength)
   :TSDFBlock()
{
   fInputFile = file;
   fBlockLocation = location;
   fStringLength = stringLength;
   fHeaderLength = headerLength;
   fMetadataLocation = fBlockLocation + fHeaderLength;

   fDataSize = 0;
   
   ReadHeader();
}

TSDFBlockPlainVar::~TSDFBlockPlainVar()
{}

void TSDFBlockPlainVar::ReadMetadata()
{
   fInputFile->seekg(fMetadataLocation, std::ios::beg);
   fInputFile->read((Char_t *)&fNormFactor, sizeof(fNormFactor));
   fInputFile->read(fUnits, sizeof(Char_t) * 32);
   fInputFile->read(fMeshID, sizeof(Char_t) * 32);
   fInputFile->read((Char_t *)fNGrid, sizeof(Int_t) * fNDims);
   fInputFile->read((Char_t *)&fStagger, sizeof(fStagger));
}

void TSDFBlockPlainVar::PrintMetadata()
{
   std::cout << fNormFactor <<"\t"
             << fUnits <<"\t"
             << fMeshID << std::endl;
   std::cout << fNGrid[0] <<"\t"
             << fNGrid[1] <<"\t"
             << fNGrid[2] << std::endl;
   std::cout << fStagger << std::endl;
}

void TSDFBlockPlainVar::ReadData()
{
   fInputFile->seekg(fDataLocation, std::ios::beg);

   if(fNDims == 1) fDataSize = fNGrid[0];
   else if(fNDims == 2) fDataSize = fNGrid[0] * fNGrid[1];
   else if(fNDims == 3) fDataSize = fNGrid[0] * fNGrid[1] * fNGrid[2];
   else{
      std::cout << "dimension size error in BlockPlaneVar" << std::endl;
      exit(0);
   }

   if(fDataType == 4) ReadData64();
   else if(fDataType == 3) ReadData32();
   else{
      std::cout << "DataType error in ReadData@BlockPlaneVar" << std::endl;
      exit(0);
   }
}

void TSDFBlockPlainVar::ReadData64()
{
   Double_t buf;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData64.push_back(buf);
   }
}

void TSDFBlockPlainVar::ReadData32()
{
   Float_t buf;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData32.push_back(buf);
   }
}

Double_t TSDFBlockPlainVar::GetData(Int_t i)
{
   if(fDataType == 4) return fData64[i];
   else if(fDataType == 3) return fData32[i];
   else{
      std::cout << "DataType error in GetData@BlockPlaneVar" << std::endl;
      exit(0);
   }
}
