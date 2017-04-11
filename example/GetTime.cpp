/*
   Obtaining simulation time information
*/
#include <iostream>

#include <TFile.h>
#include <TString.h>
#include <TVector.h>


void GetTime(TString fileName = "out.root")
{
   TFile *file = new TFile(fileName, "READ");
   TVectorD *time = (TVectorD*)file->Get("Time");
   cout << (*time)[0] << endl;
}
