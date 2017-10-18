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

// If you use ROOT5 use following defines
//#define constexpr const
//#define nullptr NULL


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

   void Initialize();
   
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
   Bool_t CheckPosition(Double_t x, TGraph2D *gr);
};

TAnalyzer::TAnalyzer(TFile *file)
   : fGrEneMaxIon(nullptr),
     fGrHighEneIon(nullptr),
     fGrEneMaxElectron(nullptr),
     fGrHighEneElectron(nullptr),
     fGrDiffDensity(nullptr),
     fGrEx(nullptr),
     fGrPx(nullptr),
     fInputFile(file)
{
   fHisIon.clear();
}

TAnalyzer::~TAnalyzer()
{
   for(UInt_t i = 0; i < fHisIon.size(); i++) delete fHisIon[i];
   for(UInt_t i = 0; i < fHisElectron.size(); i++) delete fHisElectron[i];
   
   delete fGrEneMaxIon;
   delete fGrHighEneIon;
   delete fGrEneMaxElectron;
   delete fGrHighEneElectron;

   delete fGrDiffDensity;
   delete fGrEx;

   delete fGrPx;

   fInputFile->Close();
}

void TAnalyzer::Initialize()
{
   GetDirList();
   GetTimeInfo();
   FillHisEne();
   GetGraphInfo();
   MakeGraph();
}

void TAnalyzer::GetDirList()
{
   TList *list = fInputFile->GetListOfKeys();
   TIter next(list);
   TDirectory *dir;
   while((dir = (TDirectory*)next())){
      fDirList.push_back(dir->GetName());
   }
}

void TAnalyzer::GetTimeInfo()
{
   for(UInt_t i = 0; i < fDirList.size(); i++){
      TVectorD *time = (TVectorD*)fInputFile->Get(fDirList[i] + "/Time");
      fTime.push_back((*time)[0] * 1.e15);
      cout << (*time)[0] * 1.e15 << endl;
   }
}

void TAnalyzer::FillHisEne()
{
   constexpr Double_t eleMass = kgElectronMass * kgElectronFactor;
   constexpr Double_t ionMass = kgElectronMass * kgIonFactor;

   UInt_t counter = 0;
   for(UInt_t i = 0; i < fDirList.size(); i++){
      TH1D *hisEle = new TH1D("HisEle" + fDirList[i],
                             Form("Energy distribution at %f fs", fTime[counter]),
                             1000, 0., 10.);
      hisEle->SetXTitle("Kinetic Energy [MeV]");
      
      TH1D *hisIon = new TH1D("HisIon" + fDirList[i],
                             Form("Energy distribution at %f fs", fTime[counter++]),
                             1000, 0., 10.);
      hisIon->SetXTitle("Kinetic Energy [MeV]");
      
      TTree *tree = (TTree*)fInputFile->Get(fDirList[i] + "/particles");
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

      const Long64_t kNoPar = tree->GetEntries();
      for(Long64_t i = 0; i < kNoPar; i++){
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
   Double_t magP = P.Mag2(); // magnitude * magnitude
   if(magP == 0.) return 0.;

   constexpr Double_t c2 = kgLightSpeed*kgLightSpeed;
   Double_t K = sqrt(mass*mass * c2*c2 + magP * c2) - mass * c2; // in J

   constexpr Double_t MeV = 6.241509e12;
   return K*MeV;
}

void TAnalyzer::GetGraphInfo()
{
   // Ion
   for(UInt_t i = 0; i < fHisIon.size(); i++){
      Int_t nBins = fHisIon[i]->GetNbinsX();
      Int_t maxBin = nBins;
      
      for(; maxBin > 0; maxBin--) if(fHisIon[i]->GetBinContent(maxBin) > 0.) break;
      Double_t eneMax = fHisIon[i]->GetBinCenter(maxBin);
      fEMaxIon.push_back(eneMax);

      Int_t thBin = fHisIon[i]->FindBin(eneMax / 10.);
      Double_t nHighEnePar = fHisIon[i]->Integral(thBin, nBins);
      fHighEneIon.push_back(nHighEnePar);
   }
   // Electron
   for(UInt_t i = 0; i < fHisElectron.size(); i++){
      Int_t nBins = fHisElectron[i]->GetNbinsX();
      Int_t maxBin = nBins;
      
      for(; maxBin > 0; maxBin--) if(fHisElectron[i]->GetBinContent(maxBin) > 0.) break;
      Double_t eneMax = fHisElectron[i]->GetBinCenter(maxBin);
      fEMaxElectron.push_back(eneMax);

      Int_t thBin = fHisElectron[i]->FindBin(eneMax / 10.);
      Double_t nHighEnePar = fHisElectron[i]->Integral(thBin, nBins);
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
   fGrDiffDensity = new TGraph2D();
   fGrDiffDensity->SetTitle("Electron density - Ion density;position [m];time [fs];Difference [m^{-3}]");
   
   fGrEx = new TGraph2D();
   fGrEx->SetTitle("Electric field x;position [m];time [fs];Ex [V/m]");
   
   for(UInt_t iStep = 0, counter = 0; iStep < fTime.size(); iStep++){ // SDF is better name?
      Double_t time = fTime[iStep];
      
      TString dirName = fDirList[iStep];
      TH1D *hisIon = (TH1D*)fInputFile->Get(dirName + "/number_density_Ion");
      TH1D *hisEle = (TH1D*)fInputFile->Get(dirName + "/number_density_Electron");
      TH1D *hisEx = (TH1D*)fInputFile->Get(dirName + "/Ex");

      const Int_t nBins = hisIon->GetNbinsX();
      for(Int_t iBin = 0; iBin < nBins; iBin++ ){
         Double_t diff = hisEle->GetBinContent(iBin) - hisIon->GetBinContent(iBin);
         Double_t position = hisEle->GetBinCenter(iBin);
         Double_t ex = hisEx->GetBinContent(iBin);
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
{
   if(CheckTime(t) && CheckPosition(x, fGrDiffDensity))
      return fGrDiffDensity->Interpolate(x, t);
   
   return 0.;
}

Double_t TAnalyzer::GetEx(Double_t x, Double_t t)
{
   if(CheckTime(t) && CheckPosition(x, fGrEx))
      return fGrEx->Interpolate(x, t);

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

Bool_t TAnalyzer::CheckPosition(Double_t x, TGraph2D *gr)
{
   const Int_t nData = gr->GetN();
   const Double_t *position = gr->GetX(); // Is this sorted?

   if(x > position[nData - 1] || x < position[0]){
      cout << "Position range is from " << position[0] <<" to "<< position[nData - 1] << endl;
      return kFALSE;
   }
   return kTRUE;
}


TAnalyzer *test = nullptr;

void Analyzer()
{
   TFile *file = TFile::Open("result.root");

   if(test) delete test;
   test = new TAnalyzer(file);
   test->Initialize();
   
   gStyle->SetPalette(kRainBow);
   gStyle->SetNumberContours(99);
   test->fGrEx->Draw("SURF1Z");
   
   // ROOT delete all objects.  Maybe...
   //delete test;
   //file->Close();
   //delete file;
}
