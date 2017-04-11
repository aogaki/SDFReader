/*
   The all particle information are saved as tree.
   The number of entry means the number of particles.  
*/

#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TH2.h>
#include <TMath.h>


TH2D *HisPosition;
TH2D *HisEneTheta;

void InitHists()
{
   // Histogram size and range should be taken from mesh data.
   // NYI
   HisPosition = new TH2D("HisPosition", "Position distribution of proton",
                          400, -10.e-6, 30.e-6, 200, -10.e-6, 10.e-6);
   HisPosition->SetXTitle("X-coordinate [m]");
   HisPosition->SetYTitle("Y-coordinate [m]");

   HisEneTheta = new TH2D("HisEneTheta", "Kinetic energy VS #it{#theta}",
                          900, 0., 180., 1000, 0., 1000.);
   HisEneTheta->SetXTitle("#it{#theta} [deg]");
   HisEneTheta->SetYTitle("Kinetic energy [MeV]");
}

void GetParInfo(TString fileName = "out.root")
{
   InitHists();
   
   TFile *file = new TFile(fileName, "READ");
   TTree *tree = (TTree*)file->Get("proton");

   tree->SetBranchStatus("*", kFALSE);
   Double_t ene; // kinetic energy
   tree->SetBranchStatus("Ek", kTRUE);
   tree->SetBranchAddress("Ek", &ene);
      
   Double_t weight; // weight of macro particle
   tree->SetBranchStatus("Weight", kTRUE);
   tree->SetBranchAddress("Weight", &weight);
   
   TVector3 pos; // position
   tree->SetBranchStatus("x", kTRUE);
   tree->SetBranchAddress("x", &pos[0]);
   tree->SetBranchStatus("y", kTRUE);
   tree->SetBranchAddress("y", &pos[1]);
   tree->SetBranchStatus("z", kTRUE);
   tree->SetBranchAddress("z", &pos[2]);

   // I need divergence respect to X-axis
   // Thus, Pz and Px are changed
   TVector3 P; // momentum
   tree->SetBranchStatus("Px", kTRUE);
   tree->SetBranchAddress("Px", &P[2]);
   tree->SetBranchStatus("Py", kTRUE);
   tree->SetBranchAddress("Py", &P[1]);
   tree->SetBranchStatus("Pz", kTRUE);
   tree->SetBranchAddress("Pz", &P[0]);
/*
   TVector3 V; // velocity
   tree->SetBranchStatus("Vx", kTRUE);
   tree->SetBranchAddress("Vx", &V[0]);
   tree->SetBranchStatus("Vy", kTRUE);
   tree->SetBranchAddress("Vy", &V[1]);
   tree->SetBranchStatus("Vz", kTRUE);
   tree->SetBranchAddress("Vz", &V[2]);
*/
   const Long64_t kNoPar = tree->GetEntries();
   cout << kNoPar <<" particles"<< endl;
   for(Long64_t i = 0; i < kNoPar; i++){
      if(i % 100000 == 0)
         cout << i <<" / "<< kNoPar << endl;
      tree->GetEntry(i);
      HisPosition->Fill(pos.x(), pos.y(), weight);
      HisEneTheta->Fill(P.Theta() * 180. / TMath::Pi(), ene * 6.242E12, weight);
   }

   //HisPosition->Draw("COLZ");
   HisEneTheta->Draw("COLZ");
}
