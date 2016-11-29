/*
 For Reading SDF file, output of EPOCH.  
 Almost variable names are same as file format description. 
*/

#include <iostream>

#include "TSDFReader.hpp"
#include "TSDFBlock.hpp"
#include "TSDFBlockPlainVar.hpp"


TSDFReader::TSDFReader(TString fileName)
{
   fFileName = fileName;
   fInputFile = new std::ifstream(fFileName, std::ios::in | std::ios::binary);
   if(fInputFile->fail()){
      std::cerr << "file " << fFileName << " is not found" << std::endl;
      exit(0);
   }

   ReadFileHeader();
   if(fNBlocks > kMaxBlock) {
      std::cerr << "Enlarge the kMaxBlock!" << std::endl;
      exit(0);
   }
   
   fBlockIndex = 0;
   fBlock[fBlockIndex] = new TSDFBlock(fInputFile, fNextBlockLocation,
                                       fStringLength, fBlockHeaderLength);
}

TSDFReader::~TSDFReader()
{
   fInputFile->close();
}

void TSDFReader::ReadFileHeader()
{
   Char_t sdf[5];
   fInputFile->read(sdf, sizeof(Char_t) * 4);
   std::cout << "sdf = " << sdf << std::endl;
   if(strcmp(sdf, "SDF1")) {
      std::cerr << "file " << fFileName << " is not SDF file" << std::endl;
      exit(0);
   }
  
   Int_t endianness;
   fInputFile->read((Char_t *)(&endianness), sizeof(endianness));
   std::cout << "endianness = " << endianness << std::endl;
   if(endianness != 16911887) {
      std::cerr << "This file have not same endian.  Converter is not implemented now." << std::endl;
      exit(0);
   }

   Int_t sdf_version;
   fInputFile->read((Char_t *)(&sdf_version), sizeof(sdf_version));
   std::cout << "sdf_version = " << sdf_version << std::endl;
   if(sdf_version > kSDFVersion) {
      std::cerr << "The version of file is newer than this reader. The reader version = "
           << kSDFVersion << std::endl;
      exit(0);
   }
   
   Int_t sdf_revision;
   fInputFile->read((Char_t *)(&sdf_revision), sizeof(sdf_revision));
   std::cout << "sdf_revision = " << sdf_revision << std::endl;
   if(sdf_version == kSDFVersion && sdf_revision > kSDFRevision) {
      std::cerr << "The revision of file is newer than this reader. The reader revision = "
           << kSDFRevision << std::endl;
   }

   Char_t code_name[33];
   fInputFile->read(code_name, sizeof(Char_t) * 32);
   std::cout << "code_name = " << code_name << std::endl;
   
   fInputFile->read((Char_t *)(&fNextBlockLocation), sizeof(fNextBlockLocation));
   std::cout << "first_block_location = " << fNextBlockLocation << std::endl;
   
   Long_t summary_location;
   fInputFile->read((Char_t *)(&summary_location), sizeof(summary_location));
   std::cout << "summary_location = " << summary_location << std::endl;

   Int_t summary_size;
   fInputFile->read((Char_t *)(&summary_size), sizeof(summary_size));
   std::cout << "summary_size = " << summary_size << std::endl;

   Int_t nblocks;
   fInputFile->read((Char_t *)(&nblocks), sizeof(nblocks));
   std::cout << "nblocks = " << nblocks << std::endl;
   if(nblocks == 0) {
      std::cerr << "This file is not finished." << std::endl;
      exit(0);
   }
   fNBlocks = nblocks;
   
   fInputFile->read((Char_t *)(&fBlockHeaderLength), sizeof(fBlockHeaderLength));
   std::cout << "block_header_length = " << fBlockHeaderLength << std::endl;

   Int_t step;
   fInputFile->read((Char_t *)(&step), sizeof(step));
   std::cout << "step = " << step << std::endl;

   Double_t time;
   fInputFile->read((Char_t *)(&time), sizeof(time));
   std::cout << "time = " << time << std::endl;

   fInputFile->read((Char_t *)(&fJobID1), sizeof(fJobID1));
   std::cout << "jobid1 = " << fJobID1 << std::endl;

   fInputFile->read((Char_t *)(&fJobID2), sizeof(fJobID2));
   std::cout << "jobid1 = " << fJobID2 << std::endl;

   fInputFile->read((Char_t *)(&fStringLength), sizeof(fStringLength));
   std::cout << "string_length = " << fStringLength << std::endl;

   Int_t code_io_version;
   fInputFile->read((Char_t *)(&code_io_version), sizeof(code_io_version));
   std::cout << "code_io_version = " << code_io_version << std::endl;

   Char_t restart_flag;
   fInputFile->read(&restart_flag, sizeof(restart_flag));
   std::cout << "restart_flag = " << Int_t(restart_flag) << std::endl;

   Char_t subdomain_file;
   fInputFile->read(&subdomain_file, sizeof(subdomain_file));
   std::cout << "subdomain_file = " << Int_t(subdomain_file) << std::endl;
}

void TSDFReader::ReadBlockHeader()
{
   fBlock[fBlockIndex]->ReadHeader();
}

void TSDFReader::ReadBlockMetadata()
{
   fBlock[fBlockIndex]->ReadMetadata();
}

void TSDFReader::ReadBlockData()
{
   fBlock[fBlockIndex]->ReadData();
}

void TSDFReader::GoNextBlock()
{
   fNextBlockLocation = fBlock[fBlockIndex]->GetNextLocation();
   Int_t blockType = GetNextBlockType();
   
   fBlockIndex++;

   switch(blockType){
      case c_blocktype_scrubbed:
         break;
      case c_blocktype_null:
         break;
      case c_blocktype_plain_mesh:
         break;
      case c_blocktype_point_mesh:
         break;
      case c_blocktype_plain_variable:
         fBlock[fBlockIndex] = new TSDFBlockPlainVar(fInputFile, fNextBlockLocation,
                                                     fStringLength, fBlockHeaderLength);
         break;
      case c_blocktype_point_variable:
         break;
      case c_blocktype_constant:
         break;
      case c_blocktype_array:
         break;
      case c_blocktype_run_info:
         fBlock[fBlockIndex] = new TSDFBlock(fInputFile, fNextBlockLocation,
                                             fStringLength, fBlockHeaderLength);
         break;
      case c_blocktype_source:
         break;
      case c_blocktype_stitched_tensor:
         break;
      case c_blocktype_stitched_material:
         break;
      case c_blocktype_stitched_matvar:
         break;
      case c_blocktype_stitched_species:
         break;
      case c_blocktype_species:
         break;
      default:
         fBlock[fBlockIndex] = new TSDFBlock(fInputFile, fNextBlockLocation,
                                             fStringLength, fBlockHeaderLength);
         break;
   }
}

Int_t TSDFReader::GetNextBlockType()
{
   Int_t blockType;
   fInputFile->seekg(fNextBlockLocation + kBlockTypeOffset,
                     std::ios::beg);
   fInputFile->read((Char_t *)&blockType, sizeof(blockType));
   std::cout << "blockType = " << blockType << std::endl;

   return blockType;
}
