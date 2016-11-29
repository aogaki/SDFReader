/*
 For block manupilating class
*/

#include <iostream>

#include "TSDFReader.hpp"
#include "TSDFBlock.hpp"


TSDFBlock::TSDFBlock()
{}

TSDFBlock::TSDFBlock(std::ifstream *file, Long_t location,
                     Int_t stringLength, Int_t headerLength)
{
   fInputFile = file;
   fBlockLocation = location;
   fStringLength = stringLength;
   fHeaderLength = headerLength;
}

TSDFBlock::~TSDFBlock()
{}

void TSDFBlock::ReadHeader()
{
   fInputFile->seekg(fBlockLocation, std::ios::beg);
   fInputFile->read((Char_t *)&fNextLocation, sizeof(fNextLocation));
   fInputFile->read((Char_t *)&fDataLocation, sizeof(fDataLocation));
   fInputFile->read(fBlockID, sizeof(Char_t) * 32);
   fInputFile->read((Char_t *)&fDataLength, sizeof(fDataLength));
   fInputFile->read((Char_t *)&fBlockType, sizeof(fBlockType));
   fInputFile->read((Char_t *)&fDataType, sizeof(fDataType));
   fInputFile->read((Char_t *)&fNDims, sizeof(fNDims));
   fInputFile->read(fBlockName, sizeof(Char_t) * fStringLength);
   fInputFile->read((Char_t *)&fBlockInfoLength, sizeof(fBlockInfoLength));

   PrintHeader();
}

void TSDFBlock::PrintHeader()
{
  std::cout << "\n\tBlock header" <<"\n"
        << "block_start = " << fBlockLocation <<"\n"
        << "data_location = " << fDataLocation <<"\n"
        << "block_id = " << fBlockID <<"\n"
        << "data_length = " << fDataLength <<"\n"
        << "blocktype = " << fBlockType <<"\n"
        << "datatype = " << fDataType <<"\n"
        << "ndims = " << fNDims <<"\n"
        << "block_name = " << fBlockName <<"\n"
        << "block_info_length = " << fBlockInfoLength <<"\n"
        << "next_block_location = " << fNextLocation << std::endl;
}

void TSDFBlock::ReadMetadata()
{
   PrintMetadata();
}

void TSDFBlock::PrintMetadata()
{}

void TSDFBlock::ReadData()
{}
