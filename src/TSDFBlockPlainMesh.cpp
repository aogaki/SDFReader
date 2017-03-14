/*
 For block manupilating class
*/

#include <iostream>

#include "TSDFReader.hpp"
#include "TSDFBlockPlainMesh.hpp"


using std::cout;
using std::endl;

TSDFBlockPlainMesh::TSDFBlockPlainMesh(std::ifstream *file, Long_t location,
                                       Int_t stringLength, Int_t headerLength)
   :TSDFBlock()
{
   fInputFile = file;
   fBlockLocation = location;
   fStringLength = stringLength;
   fHeaderLength = headerLength;
   fMetadataLocation = fBlockLocation + fHeaderLength;

   fDataSize = 0;

   fGeoType = 0;
   for(Int_t i = 0; i < 3; i++){// These have 3 dimensions
      fNormFactor[i] = 0.;
      fAxisLabel[i] = "";
      fUnits[i] = "";
      fMinVal[i] = 0.;
      fMaxVal[i] = 0.;
      fNGrid[i] = 0;
   }
   
   ReadHeader();
}

void TSDFBlockPlainMesh::ReadMetadata()
{
   fInputFile->seekg(fMetadataLocation, std::ios::beg);
   fInputFile->read((Char_t *)fNormFactor, sizeof(Double_t) * fNDims);
   for(Int_t i = 0; i < fNDims; i++){
      Char_t *buf = new Char_t[32];
      fInputFile->read(buf, sizeof(Char_t) * 32);
      fAxisLabel[i] = buf;
      delete buf;
   }
   for(Int_t i = 0; i < fNDims; i++){
      Char_t *buf = new Char_t[32];
      fInputFile->read(buf, sizeof(Char_t) * 32);
      fUnits[i] = buf;
      delete buf;  
   }
   fInputFile->read((Char_t *)&fGeoType, sizeof(Int_t));
   fInputFile->read((Char_t *)fMinVal, sizeof(Double_t) * fNDims);
   fInputFile->read((Char_t *)fMaxVal, sizeof(Double_t) * fNDims);
   fInputFile->read((Char_t *)fNGrid, sizeof(Int_t) * fNDims);
}

void TSDFBlockPlainMesh::PrintMetadata()
{
   cout << "Normalization factor: "
        << fNormFactor[0] <<"\t"
        << fNormFactor[1] <<"\t"
        << fNormFactor[2] << endl;
   cout << "Axis label: "
        << fAxisLabel[0] <<"\t"
        << fAxisLabel[1] <<"\t"
        << fAxisLabel[2] << endl;
   cout << "Units: "
        << fUnits[0] <<"\t"
        << fUnits[1] <<"\t"
        << fUnits[2] << endl;
   cout << "Geometry type: " << fGeoType << endl;
   cout << "Min value: "
        << fMinVal[0] <<"\t"
        << fMinVal[1] <<"\t"
        << fMinVal[2] << endl;
   cout << "Max value: "
        << fMaxVal[0] <<"\t"
        << fMaxVal[1] <<"\t"
        << fMaxVal[2] << endl;
   cout << "No. Grids: "
        << fNGrid[0] <<"\t"
        << fNGrid[1] <<"\t"
        << fNGrid[2] << endl;
}

void TSDFBlockPlainMesh::ReadData()
{
   fInputFile->seekg(fDataLocation, std::ios::beg);

   if(fNDims == 1) fDataSize = fNGrid[0];
   else if(fNDims == 2) fDataSize = fNGrid[0] * fNGrid[1];
   else if(fNDims == 3) fDataSize = fNGrid[0] * fNGrid[1] * fNGrid[2];
   else{
      cout << "dimension size error in BlockPlaneMesh" << endl;
      exit(0);
   }

   if(fDataType == 4) ReadData64();
   else if(fDataType == 3) ReadData32();
   else{
      cout << "DataType error in ReadData@BlockPlaneMesh" << endl;
      exit(0);
   }
}

void TSDFBlockPlainMesh::ReadData64()
{
   Double_t buf;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData.push_back(buf);
   }
}

void TSDFBlockPlainMesh::ReadData32()
{
   Float_t buf;

   for(Int_t i = 0; i < fDataSize; i++){
      fInputFile->read((Char_t *)&buf, sizeof(buf));
      fData.push_back(Double_t(buf));
   }
}

Double_t TSDFBlockPlainMesh::GetData(Int_t i)
{
   // Check the vector size
   return fData[i];
}
