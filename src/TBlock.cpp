/*
 For block manupilating class
*/

#include <iostream>

#include "TBlock.hpp"


using std::cout;
using std::endl;

TBlock::TBlock()
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

TBlock::TBlock(std::ifstream *file, Long_t location,
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

TBlock::~TBlock()
{}

void TBlock::ReadHeader()
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

void TBlock::PrintHeader()
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

void TBlock::ReadData()
{
   if(fData.size() == 0){
      fInputFile->seekg(fDataLocation, std::ios::beg);
      
      if(fDataType == 4) ReadData64();
      else if(fDataType == 3) ReadData32();
      else{
         cout << "DataType error in ReadData@BlockPlaneMesh" << endl;
         cout << "Now, only double and float data types are implemented" << endl;
         exit(0);
      }
   }
   //else cout << "Already readed" << endl;
}

void TBlock::ReadData64()
{
   Double_t buf;
   fDataSize = fDataLength / 8;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData.push_back(buf);
   }
}

void TBlock::ReadData32()
{
   Float_t buf;
   fDataSize = fDataLength / 4;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData.push_back(Double_t(buf));
   }
}

Double_t TBlock::GetData(Int_t i)
{
   if(i < fData.size())
      return fData[i];
   else std::cerr << "GetData@TBlock, out of range" << std::endl;
   return 0.;
};

