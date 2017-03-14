/*
 For block manupilating class
*/

#include <iostream>

#include "TSDFReader.hpp"
#include "TSDFBlock.hpp"


using std::cout;
using std::endl;

TSDFBlock::TSDFBlock()
{
   fDataSize = 0;
   fInputFile = nullptr;
   fBlockLocation = 0;
   fMetadataLocation = 0;
   fDataLocation = 0;
   fNextLocation = 0;
   fDataLength = 0;
   fBlockType = 0;
   fDataType = 0;
   fNDims = 0;
   fBlockInfoLength = 0;
   fHeaderLength = 0;
   fStringLength = 0;

   fDataSize = 0;

   fBlockID = "";
   fBlockName = "";
}

TSDFBlock::TSDFBlock(std::ifstream *file, Long_t location,
                     Int_t stringLength, Int_t headerLength)
{
   fInputFile = file;
   fBlockLocation = location;
   fStringLength = stringLength;
   fHeaderLength = headerLength;

   fMetadataLocation = 0;
   fDataLocation = 0;
   fNextLocation = 0;
   fDataLength = 0;
   fBlockType = 0;
   fDataType = 0;
   fNDims = 0;
   fBlockInfoLength = 0;
   fDataSize = 0;
   fBlockID = "";
   fBlockName = "";

   ReadHeader();
}

TSDFBlock::~TSDFBlock()
{}

void TSDFBlock::ReadHeader()
{
   fInputFile->seekg(fBlockLocation, std::ios::beg);
   fInputFile->read((Char_t *)&fNextLocation, sizeof(fNextLocation));
   fInputFile->read((Char_t *)&fDataLocation, sizeof(fDataLocation));
   Char_t *buf1 = new Char_t[32];
   fInputFile->read(buf1, sizeof(Char_t) * 32);
   fBlockID = buf1;
   delete buf1;
   fInputFile->read((Char_t *)&fDataLength, sizeof(fDataLength));
   fInputFile->read((Char_t *)&fBlockType, sizeof(fBlockType));
   fInputFile->read((Char_t *)&fDataType, sizeof(fDataType));
   fInputFile->read((Char_t *)&fNDims, sizeof(fNDims));
   Char_t *buf2 = new Char_t[fStringLength];
   fInputFile->read(buf2, sizeof(Char_t) * fStringLength);
   fBlockName = buf2;
   delete buf2;
   fInputFile->read((Char_t *)&fBlockInfoLength, sizeof(fBlockInfoLength));
}

void TSDFBlock::PrintHeader()
{
   cout << "\n\tBlock header" <<"\n"
        << "block_start = " << fBlockLocation <<"\n"
        << "data_location = " << fDataLocation <<"\n"
        << "block_id = " << fBlockID <<"\n"
        << "data_length = " << fDataLength <<"\n"
        << "blocktype = " << fBlockType <<"\n"
        << "datatype = " << fDataType <<"\n"
        << "ndims = " << fNDims <<"\n"
        << "block_name = " << fBlockName <<"\n"
        << "block_info_length = " << fBlockInfoLength <<"\n"
        << "next_block_location = " << fNextLocation << endl;
}
