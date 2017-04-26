/*
   Calculating kinetic energy.
   Par means Particle or Parameter.  Stupid code! Be careful!
*/

#include <iostream>

#include <TVector3.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>

constexpr Double_t kgElectronMass = 9.10938291e-31;// These values are used in EPOCH. The unit is kg
constexpr Double_t kgLightSpeed = 2.99792458e8;

class EpochPar
{
public:
   EpochPar();
   EpochPar(Double_t mass);
   ~EpochPar();

   Double_t GetEne() const;
   Double_t Theta() const;
   
   // These public variables are for tree branch
   Double_t x;
   Double_t y;
   Double_t z;
   
   TVector3 P;
   TVector3 V;

   // The value Ene uses the kinetic energy calculated by EPOCH
   Double_t Ene;
   Double_t Weight;

private:
   Double_t fMass;

};

EpochPar::EpochPar()
{
   x = y = z = 0.;
   P = TVector3(0., 0., 0.);
   V = TVector3(0., 0., 0.);
   Ene = 0.;
   Weight = 0.;

   fMass = kgElectronMass;
}

EpochPar::EpochPar(Double_t mass)
{
   EpochPar();

   fMass = mass;
}

EpochPar::~EpochPar()
{}

Double_t EpochPar::GetEne() const
{
   Double_t magP = P.Mag2(); // magnitude * magnitude
   if(magP == 0.) return Ene; // or return 0.;

   constexpr Double_t c2 = kgLightSpeed*kgLightSpeed;
   Double_t K = sqrt(fMass*fMass * c2*c2 + magP * c2) - fMass * c2; // in J

   constexpr Double_t MeV = 6.241509e12;
   return K*MeV;
}

Double_t EpochPar::Theta() const
{
   return P.Theta();
}

TH2D *HisTest;

void InitHist()
{
   HisTest = new TH2D("HisTest", "Kinetic energy VS divergence",
                      1800, 0., 180., 1000, 0., 10000.);
}

void AnalyzePar(TString fileName = "out.root")
{
   TFile *file = new TFile(fileName, "READ");
   TTree *tree = (TTree*)file->Get("electron");
   tree->SetBranchStatus("*", kFALSE);
   
   EpochPar electron;

   // Swapping x and z.  the beam direction of 2D EPOCH is x
   tree->SetBranchStatus("Px", kTRUE);
   tree->SetBranchAddress("Px", &electron.P[2]);
   tree->SetBranchStatus("Py", kTRUE);
   tree->SetBranchAddress("Py", &electron.P[1]);
   tree->SetBranchStatus("Pz", kTRUE);
   tree->SetBranchAddress("Pz", &electron.P[0]);

   tree->SetBranchStatus("Ek", kTRUE);
   tree->SetBranchAddress("Ek", &electron.Ene);   

   tree->SetBranchStatus("Weight", kTRUE);
   tree->SetBranchAddress("Weight", &electron.Weight);   

   InitHist();
   
   const Long64_t kNoParticles = tree->GetEntries();
   for(Long64_t i = 0; i < kNoParticles; i++){
      tree->GetEntry(i);
      HisTest->Fill(electron.Theta() * 180 / TMath::Pi(), electron.GetEne(), electron.Weight);
   }
   
}
