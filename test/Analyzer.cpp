#include <iostream>
#include <vector>

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TCollection.h>
#include <TH2.h>
#include <TH1.h>
#include <TVector3.h>
#include <TVectorD.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TStyle.h>


constexpr Double_t kgElectronMass = 9.10938291e-31;// These values are used in EPOCH.
constexpr Double_t kgLightSpeed = 2.99792458e8;
constexpr Double_t kgElectronFactor = 1.; // For electron mass
constexpr Double_t kgProtonFactor = 1836.2; // For proton mass
constexpr Double_t kgIonFactor = 1836.2*12; // For carbon mass, you should change these factor as same as EPOCH


class TAnalyzer
{
public:
   TAnalyzer(TFile *file);
   ~TAnalyzer();

   // When time or position range are over, all functions return 0
   // Think better value!
   Double_t GetEneMaxIon(Double_t t); // return max energy at time t
   Double_t GetHighEnergyIon(Double_t t); // return the number of particles higherr than Emax / 10 at t

   Double_t GetEneMaxElectron(Double_t t);
   Double_t GetHighEnergyElectron(Double_t t);

   Double_t GetDiffDensity(Double_t x, Double_t t); // return the difference of particle density
   Double_t GetEx(Double_t x, Double_t t); // return Ex
   
   // For easy debug, member variables of histograms and graphs are public
   std::vector<TH1D*> fHisIon;
   std::vector<TH1D*> fHisElectron;

   TGraph *fGrEneMaxIon;
   TGraph *fGrHighEneIon;
   TGraph *fGrEneMaxElectron;
   TGraph *fGrHighEneElectron;

   TGraph2D *fGrDiffDensity;
   TGraph2D *fGrEx;

   TGraph *fGrPx;
   
private:
   TFile *fInputFile;

   // The result file has directories
   // Each directory represents each SDF file
   void GetDirList();
   std::vector<TString> fDirList;

   void GetTimeInfo();
   std::vector<Double_t> fTime; // in fs

   // Make histograms to obtain the maximum energy and more info
   void FillHisEne();
   Double_t GetKineEne(const TVector3 &P, const Double_t &mass);

   void MakeGraph();
   void GetGraphInfo();
   std::vector<Double_t> fEMaxIon;
   std::vector<Double_t> fHighEneIon;
   std::vector<Double_t> fEMaxElectron;
   std::vector<Double_t> fHighEneElectron;

   Bool_t CheckTime(Double_t t);
};

TAnalyzer::TAnalyzer(TFile *file)
   :fInputFile(file)
{
   GetDirList();
   GetTimeInfo();
   FillHisEne();
   GetGraphInfo();
   MakeGraph();
}

TAnalyzer::~TAnalyzer()
{

}

void TAnalyzer::GetDirList()
{
   auto list = fInputFile->GetListOfKeys();
   TIter next(list);
   TDirectory *dir;
   while((dir = (TDirectory*)next())){
      fDirList.push_back(dir->GetName());
   }
}

void TAnalyzer::GetTimeInfo()
{
   for(auto &dirName: fDirList){
      auto time = (TVectorD*)fInputFile->Get(dirName + "/Time");
      fTime.push_back((*time)[0] * 1.e15);
   }
   for(auto time: fTime) cout << time << endl;
}

void TAnalyzer::FillHisEne()
{
   constexpr Double_t eleMass = kgElectronMass * kgElectronFactor;
   constexpr Double_t ionMass = kgElectronMass * kgIonFactor;

   UInt_t counter = 0;
   for(auto &dirName: fDirList){
      auto hisEle = new TH1D("HisEle" + dirName,
                             Form("Energy distribution at %f fs", fTime[counter]),
                             1000, 0., 10.);
      hisEle->SetXTitle("Kinetic Energy [MeV]");
      
      auto hisIon = new TH1D("HisIon" + dirName,
                             Form("Energy distribution at %f fs", fTime[counter++]),
                             1000, 0., 10.);
      hisIon->SetXTitle("Kinetic Energy [MeV]");
      
      auto tree = (TTree*)fInputFile->Get(dirName + "/particles");
      tree->SetBranchStatus("*", kFALSE);

      TString *type{nullptr};
      tree->SetBranchStatus("Ptype", kTRUE);
      tree->SetBranchAddress("Ptype", &type);

      TVector3 P;
      tree->SetBranchStatus("Px", kTRUE);
      tree->SetBranchStatus("Py", kTRUE);
      tree->SetBranchStatus("Pz", kTRUE);
      tree->SetBranchAddress("Px", &P[0]);
      tree->SetBranchAddress("Py", &P[1]);
      tree->SetBranchAddress("Pz", &P[2]);

      Double_t weight;
      tree->SetBranchStatus("Weight", kTRUE);
      tree->SetBranchAddress("Weight", &weight);      

      const auto kNoPar = tree->GetEntries();
      for(auto i = 0; i < kNoPar; i++){
         tree->GetEntry(i);
         if(*type == "electron"){
            hisEle->Fill(GetKineEne(P, eleMass), weight);
         }
         else if(*type == "ion"){
            hisIon->Fill(GetKineEne(P, ionMass), weight);
         }
         else{
            cout << "What is this particle type?: " << *type << endl;
         }
      }
      
      fHisElectron.push_back(hisEle);
      fHisIon.push_back(hisIon);
   }
}

Double_t TAnalyzer::GetKineEne(const TVector3 &P, const Double_t &mass)
{
   auto magP = P.Mag2(); // magnitude * magnitude
   if(magP == 0.) return 0.;

   constexpr Double_t c2 = kgLightSpeed*kgLightSpeed;
   auto K = sqrt(mass*mass * c2*c2 + magP * c2) - mass * c2; // in J

   constexpr Double_t MeV = 6.241509e12;
   return K*MeV;
}

void TAnalyzer::GetGraphInfo()
{
   // Ion
   for(auto &his: fHisIon){
      auto nBins = his->GetNbinsX();
      auto maxBin = nBins;
      
      for(; maxBin > 0; maxBin--) if(his->GetBinContent(maxBin) > 0.) break;
      auto eneMax = his->GetBinCenter(maxBin);
      fEMaxIon.push_back(eneMax);

      auto thBin = his->FindBin(eneMax / 10.);
      auto nHighEnePar = his->Integral(thBin, nBins);
      fHighEneIon.push_back(nHighEnePar);
   }
   // Electron
   for(auto &his: fHisElectron){
      auto nBins = his->GetNbinsX();
      auto maxBin = nBins;
      
      for(; maxBin > 0; maxBin--) if(his->GetBinContent(maxBin) > 0.) break;
      auto eneMax = his->GetBinCenter(maxBin);
      fEMaxElectron.push_back(eneMax);

      auto thBin = his->FindBin(eneMax / 10.);
      auto nHighEnePar = his->Integral(thBin, nBins);
      fHighEneElectron.push_back(nHighEnePar);
   }
}

void TAnalyzer::MakeGraph()
{
   // 1D Graphs
   const Int_t nData = fTime.size();
   fGrEneMaxIon = new TGraph(nData, &fTime[0], &fEMaxIon[0]);
   fGrHighEneIon = new TGraph(nData, &fTime[0], &fHighEneIon[0]);
   fGrEneMaxElectron = new TGraph(nData, &fTime[0], &fEMaxElectron[0]);
   fGrHighEneElectron = new TGraph(nData, &fTime[0], &fHighEneElectron[0]);

   // 2D Graphs
   // Too long!!
   fGrDiffDensity = new TGraph2D();
   fGrDiffDensity->SetTitle("Electron density - Ion density;position [m];time [fs];Difference [m^{-3}]");
   
   fGrEx = new TGraph2D();
   fGrEx->SetTitle("Electric field x;position [m];time [fs];Difference [m^{-3}]");
   
   for(auto iStep = 0, counter = 0; iStep < fTime.size(); iStep++){ // SDF is better name?
      auto time = fTime[iStep];
      
      auto dirName = fDirList[iStep];
      auto hisIon = (TH1D*)fInputFile->Get(dirName + "/number_density_Ion");
      auto hisEle = (TH1D*)fInputFile->Get(dirName + "/number_density_Electron");
      auto hisEx = (TH1D*)fInputFile->Get(dirName + "/Ex");

      const auto nBins = hisIon->GetNbinsX();
      for(auto iBin = 0; iBin < nBins; iBin++ ){
         auto diff = hisEle->GetBinContent(iBin) - hisIon->GetBinContent(iBin);
         auto position = hisEle->GetBinCenter(iBin);
         auto ex = hisEx->GetBinContent(iBin);
         fGrDiffDensity->SetPoint(counter, position, time, diff);
         fGrEx->SetPoint(counter, position, time, ex);
         counter++;
      }
   }
}

Double_t TAnalyzer::GetEneMaxIon(Double_t t)
{
   if(CheckTime(t)) return fGrEneMaxIon->Eval(t);
   return 0.;
}

Double_t TAnalyzer::GetHighEnergyIon(Double_t t)
{
   if(CheckTime(t)) return fGrHighEneIon->Eval(t);
   return 0.;
}

Double_t TAnalyzer::GetEneMaxElectron(Double_t t)
{
   if(CheckTime(t)) return fGrEneMaxElectron->Eval(t);
   return 0.;
}

Double_t TAnalyzer::GetHighEnergyElectron(Double_t t)
{
   if(CheckTime(t)) return fGrHighEneElectron->Eval(t);   
   return 0.;
}

Double_t TAnalyzer::GetDiffDensity(Double_t x, Double_t t)
{// Position range check NYI
   if(CheckTime(t)) return fGrDiffDensity->Eval(x, t);
   return 0.;
}

Double_t TAnalyzer::GetEx(Double_t x, Double_t t)
{
   if(CheckTime(t)) return fGrEx->Eval(x, t);
   return 0.;
}

Bool_t TAnalyzer::CheckTime(Double_t t)
{
   if(t < fTime[0] || t > fTime[fTime.size() - 1]){
      cout << "Time range is from " << fTime[0] <<" to "<< fTime[fTime.size() - 1] << endl;
      return kFALSE;
   }

   return kTRUE;
}



TAnalyzer *test;

void Analyzer()
{
   auto file = TFile::Open("result.root");

   test = new TAnalyzer(file);
   gStyle->SetPalette(kRainBow);
   gStyle->SetNumberContours(99);
   test->fGrEx->Draw("SURF1Z");
   
   // ROOT delete all objects.  Maybe...
   //delete test;
   //file->Close();
   //delete file;
}
