#include <iostream>

#include "TDataTree.hpp"

void PrintUsage()
{
  std::cout << "Usage\n"
            << "parTree [options]\n"
            << "-p: Particle name (default: electron)\n"
            << "-m: Particle mass normalized by electron (default: 1.)\n"
            << "-c: PDG code (default: 11)\n"
            << "-d: Directory for input and output (default: ./)\n"
            << std::endl;
}

int main(int argc, char **argv)
{
  TString parName{"electron"};
  Double_t parMass{kgElectronMass * 1.};
  Int_t PDGCode{11};
  TString dirName{"./"};

  // check arguments
  for (int i = 1; i < argc; i++) {
    TString arg{argv[i]};
    if (arg == "-h") {
      PrintUsage();
      return 0;
    } else if (arg == "-p") {
      parName = argv[++i];
    } else if (arg == "-m") {
      parMass = kgElectronMass * atof(argv[++i]);
    } else if (arg == "-c") {
      PDGCode = atoi(argv[++i]);
    } else if (arg == "-d") {
      dirName = argv[++i];
    } else {
      PrintUsage();
      return 0;
    }
  }

  auto writer = new TDataTree();
  writer->SetParName(parName);
  writer->SetParMass(parMass);
  writer->SetPDGCode(PDGCode);  // PDG code of electron.  Proton is 2212
  writer->SetDirectory(dirName);
  writer->AddFiles();

  writer->GetOutput();

  delete writer;

  return 0;
}
