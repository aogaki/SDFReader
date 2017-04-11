/*
   Obtaining histograms
   The mesh values (Electric field, Magnetic field, and etc.) are saved as histogram.
   Before this, please check the histograms saved in the file.
*/

#include <TFile.h>
#include <TH2.h>

TH2D *HisEx;
TH2D *HisEy;
TH2D *HisEz;

TH2D *HisBx;
TH2D *HisBy;
TH2D *HisBz;

TH2D *HisJx;
TH2D *HisJy;
TH2D *HisJz;


void GetHists(TString fileName = "out.root")
{
   TFile *file = new TFile(fileName, "READ");
   HisEx = (TH2D*)file->Get("Ex");
   HisEy = (TH2D*)file->Get("Ey");
   HisEz = (TH2D*)file->Get("Ez");

   HisBx = (TH2D*)file->Get("Bx");
   HisBy = (TH2D*)file->Get("By");
   HisBz = (TH2D*)file->Get("Bz");

   HisJx = (TH2D*)file->Get("Jx");
   HisJy = (TH2D*)file->Get("Jy");
   HisJz = (TH2D*)file->Get("Jz");

   HisBz->Draw("COLZ");
}
