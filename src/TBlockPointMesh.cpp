/*
 For block manupilating class
*/

#include <iostream>

#include "TBlockPointMesh.hpp"


using std::cout;
using std::endl;

TBlockPointMesh::TBlockPointMesh(std::ifstream *file, Long_t location,
                                       Int_t stringLength, Int_t headerLength)
   :TBlock()
{
   fInputFile = file;
   fBlockLocation = location;
   fStringLength = stringLength;
   fHeaderLength = headerLength;
   fMetadataLocation = fBlockLocation + fHeaderLength;

   fDataSize = 0;

   fGeoType = 0;
   fNParticles = 0;
   for(Int_t i = 0; i < 3; i++){// These have 3 dimensions
      fNormFactor[i] = 0.;
      fAxisLabel[i] = "";
      fUnits[i] = "";
      fMinVal[i] = 0.;
      fMaxVal[i] = 0.;
   }
   
   ReadHeader();
}

void TBlockPointMesh::ReadMetadata()
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
   fInputFile->read((Char_t *)fNParticles, sizeof(Long64_t));
}

void TBlockPointMesh::PrintMetadata()
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
   cout << "No. Particles: " << fNParticles << endl;
}
