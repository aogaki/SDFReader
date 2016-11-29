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

   PrintMetadata();
}

void TSDFBlockPlainVar::PrintMetadata()
{
   std::cout << fNormFactor <<"\t"
             << fUnits <<"\t"
             << fMeshID << std::endl;
   std::cout << fNGrid[0] <<"\t"
             << fNGrid[1] <<"\t"
             << fNGrid[2] << std::endl;

}

void TSDFBlockPlainVar::ReadData()
{
   fInputFile->seekg(fDataLocation, std::ios::beg);
}
