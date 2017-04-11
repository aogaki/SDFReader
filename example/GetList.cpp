/*
   Getting the list included in the file
   Using TBrowser is better
*/

#include <TFile.h>
#include <TROOT.h>


void GetList(TString fileName = "out.root")
{
   TFile *file = new TFile(fileName, "READ");
   gROOT->ProcessLine(".ls");
}
