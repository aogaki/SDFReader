#define MacroParticle_cxx
// The class definition in MacroParticle.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("MacroParticle.C")
// root> T->Process("MacroParticle.C","some options")
// root> T->Process("MacroParticle.C+")
//

#include <iostream>
#include "MacroParticle.h"
#include <TH2.h>
#include <TStyle.h>

void MacroParticle::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   fHis2D = nullptr;
   fHisEne = nullptr;
   fHisTheta = nullptr;
   fHisTheta360 = nullptr;
   fGrPolar = nullptr;
}

void MacroParticle::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   // When I use those values.  Setting in SlaveBegin is mandatory.
   // Why?
   fEneMax = 15000.;
   fThetaMax = 180.;

   TString eneLabel = "Kinetic energy [MeV]";
   TString thetaLabel = "#it{#theta} [deg]";
   
   fHis2D = new TH2D("His2D", "Kinetic energy VS divergence",
                     Int_t(fThetaMax * 10), 0., fThetaMax, Int_t(fEneMax / 10), 0., fEneMax);
   fHis2D->SetXTitle(thetaLabel);
   fHis2D->SetYTitle(eneLabel);
   GetOutputList()->Add(fHis2D);

   fHisEne = new TH1D("HisEne", "", fEneMax, 0., fEneMax);
   fHisEne->SetXTitle(eneLabel);
   fHisEne->SetYTitle("Counts");
   GetOutputList()->Add(fHisEne);

   fHisTheta = new TH1D("HisTheta", "", fThetaMax * 10, 0., fThetaMax);
   fHisTheta->SetXTitle(thetaLabel);
   fHisTheta->SetYTitle("Counts");
   GetOutputList()->Add(fHisTheta);

   fHisTheta360 = new TH1D("HisTheta360", "", 3600, 0., 360.);
   fHisTheta360->SetXTitle(thetaLabel);
   fHisTheta360->SetYTitle("Counts");
   GetOutputList()->Add(fHisTheta360);

   fHis2D->GetXaxis()->CenterTitle(kTRUE);
   fHis2D->GetYaxis()->CenterTitle(kTRUE);
   fHisEne->GetXaxis()->CenterTitle(kTRUE);
   fHisEne->GetYaxis()->CenterTitle(kTRUE);
   fHisTheta->GetXaxis()->CenterTitle(kTRUE);
   fHisTheta->GetYaxis()->CenterTitle(kTRUE);
   fHisTheta360->GetXaxis()->CenterTitle(kTRUE);
   fHisTheta360->GetYaxis()->CenterTitle(kTRUE);
}

Bool_t MacroParticle::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);
   fP[0] = *Pz;
   fP[1] = *Py;
   fP[2] = *Px;
   Double_t ene = GetEne();
   if(ene > 0.){
      Double_t theta = GetTheta() * 180 / TMath::Pi();
      fHis2D->Fill(theta, ene, *Weight);
      fHisEne->Fill(ene, *Weight);
      fHisTheta->Fill(theta, *Weight);
      if(GetPhi() < 0.) theta = 360. - theta;
      fHisTheta360->Fill(theta, *Weight);
   }
   return kTRUE;
}

void MacroParticle::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void MacroParticle::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   GetPolarGraph();
   TFile *file = new TFile("tmp.root", "RECREATE");
   fHis2D->Write();
   fHisEne->Write();
   fHisTheta->Write();
   fHisTheta360->Write();
   fGrPolar->Write();
   file->Close();
}

Double_t MacroParticle::GetEne() const
{
   Double_t magP = fP.Mag2(); // magnitude * magnitude
   if(magP == 0.) return 0.;

   constexpr Double_t c2 = kgLightSpeed*kgLightSpeed;
   constexpr Double_t mass = kgElectronMass * kgMassFactor;
   Double_t K = sqrt(mass*mass * c2*c2 + magP * c2) - mass * c2; // in J

   constexpr Double_t MeV = 6.241509e12;
   return K*MeV;
}

Double_t MacroParticle::GetTheta() const
{
   return fP.Theta();
}

Double_t MacroParticle::GetPhi() const
{
   return fP.Phi();
}

void MacroParticle::GetPolarGraph()
{
   TGraphErrors *gr = new TGraphErrors(fHisTheta360);
   const Int_t n = gr->GetN();
   Double_t *y = gr->GetY();
   Double_t err[n];
   Double_t max = fHisTheta360->GetMaximum();
   for(Int_t i = 0; i < n; i++){
      y[i] /= max;
      err[i] = 0.;
   }
   fGrPolar = new TGraphPolar(n, gr->GetX(), y, err, err);
   fGrPolar->SetTitle("");
   fGrPolar->SetLineColor(kRed);
   fGrPolar->SetLineWidth(2);
   //delete gr;
}
