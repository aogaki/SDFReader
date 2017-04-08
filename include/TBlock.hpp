#ifndef TBLOCK
#define TBLOCK 1

#include <iostream>
#include <fstream>

#include <TString.h>


class TBlock
{
public:
   TBlock();
   TBlock(std::ifstream *, Long_t, Int_t, Int_t);
   virtual ~TBlock();

   virtual Long_t GetNextLocation() {return fNextLocation;};
   
   virtual void ReadHeader();
   virtual void PrintHeader();

   virtual void ReadMetadata(){};
   virtual void PrintMetadata(){};

   virtual void ReadData();
   virtual void PrintData(){for(auto v: fData) std::cout << v << std::endl;};
  virtual Int_t GetDataSize(){return fData.size();};
  virtual Double_t GetData(Int_t i);
   

   //Int_t GetDataSize(){return fDataSize;};

   TString GetName(){return fBlockName;};
   TString GetID(){return fBlockID;};
   Int_t GetBlockType(){return fBlockType;};
   Int_t GetDataType(){return fDataType;};
   Int_t GetNDims(){return fNDims;};

protected:
   // For header and general information
   std::ifstream *fInputFile;
   Long_t fBlockLocation;
   Long_t fMetadataLocation;
   Long_t fDataLocation;
   Long_t fNextLocation;
   TString fBlockID;
   Long_t fDataLength;
   Int_t fBlockType;
   Int_t fDataType;
   Int_t fNDims;
   TString fBlockName;
   Int_t fBlockInfoLength;
   
   Int_t fHeaderLength;
   Int_t fStringLength;

   Int_t fDataSize;

   void ReadData64();
   void ReadData32();
   std::vector<Double_t> fData;
};

#endif
