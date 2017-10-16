/*
 For block manupilating class
*/

#include <iostream>

#include "TBlockPointVar.hpp"

using std::cout;
using std::endl;

TBlockPointVar::TBlockPointVar(std::ifstream *file, Long_t location,
                               Int_t stringLength, Int_t headerLength)
    : TBlock()
{
  fInputFile = file;
  fBlockLocation = location;
  fStringLength = stringLength;
  fHeaderLength = headerLength;
  fMetadataLocation = fBlockLocation + fHeaderLength;

  fNormFactor = 0.;
  fUnits = "";
  fMeshID = "";
  fNParticles = 0;

  fDataSize = 0;

  ReadHeader();

  if (fDataType == 4)
    fDataSize = fDataLength / sizeof(Double_t);
  else if (fDataType == 3)
    fDataSize = fDataLength / sizeof(Float_t);
  else {
    // cout << "DataType error in GetHeader@BlockPointVar" << endl;
    // cout << "Now, only double and float data types are implemented" << endl;
    // exit(0);
  }
}

void TBlockPointVar::ReadMetadata()
{
  fInputFile->seekg(fMetadataLocation, std::ios::beg);
  fInputFile->read((Char_t *)&fNormFactor, sizeof(fNormFactor));
  const Int_t charSize = 32;
  Char_t buf[charSize];
  fInputFile->read(buf, sizeof(Char_t) * charSize);
  fUnits = TString(buf);
  fInputFile->read(buf, sizeof(Char_t) * charSize);
  fMeshID = TString(buf);
  fInputFile->read((Char_t *)&fNParticles, sizeof(Long64_t));
}

void TBlockPointVar::PrintMetadata()
{
  cout << "Normalization factor: " << fNormFactor << "\n"
       << "Units: " << fUnits << "\n"
       << "Mesh ID: " << fMeshID << endl;
  cout << "No. Particles: " << fNParticles << endl;
}

void TBlockPointVar::ReadData() {}  // Do nothing

Double_t TBlockPointVar::GetData(Int_t i)
{
  if (fDataType == 4)
    return GetData64(i);
  else if (fDataType == 3)
    return GetData32(i);
  else {
    cout << "DataType error in ReadData@BlockPlaneVar" << endl;
    cout << "Now, only double and float data types are implemented" << endl;
    cout << "Particle ID showd be given by GetID(Int_t i)" << endl;
    exit(0);
  }
}

Double_t TBlockPointVar::GetData32(Int_t i)
{
  Float_t buf;
  fInputFile->seekg(fDataLocation + i * sizeof(buf), std::ios::beg);
  fInputFile->read((Char_t *)&buf, sizeof(buf));
  return buf;
}

Double_t TBlockPointVar::GetData64(Int_t i)
{
  Double_t buf;
  fInputFile->seekg(fDataLocation + i * sizeof(buf), std::ios::beg);
  fInputFile->read((Char_t *)&buf, sizeof(buf));
  return buf;
}

Long64_t TBlockPointVar::GetID(Int_t i)
{
  Long64_t buf;
  fInputFile->seekg(fDataLocation + i * sizeof(buf), std::ios::beg);
  fInputFile->read((Char_t *)&buf, sizeof(buf));
  return buf;
}
