/*
 For block manupilating class
*/

#include <iostream>

#include "TBlockPointVar.hpp"


using std::cout;
using std::endl;

TBlockPointVar::TBlockPointVar(std::ifstream *file, Long_t location,
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
   fNParticles =  0;

   fDataSize = 0;
   
   ReadHeader();
}

void TBlockPointVar::ReadMetadata()
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
   fInputFile->read((Char_t *)fNParticles, sizeof(Long64_t));
}

void TBlockPointVar::PrintMetadata()
{
   cout << "Normalization factor: " << fNormFactor <<"\n"
        << "Units: " << fUnits <<"\n"
        << "Mesh ID: " << fMeshID << endl;
   cout << "No. Particles: " << fNParticles << endl;
}
