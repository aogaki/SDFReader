#include <TChain.h>
#include <TProof.h>
#include <TProofLite.h>
#include <TString.h>


void ActivatePROOF(TChain *chain, Int_t nThreads = 0)
{
   TProof *proof = TProof::Open("");
   proof->SetProgressDialog(kFALSE);
   if(nThreads > 0) proof->SetParallel(nThreads);

   chain->SetProof();
}

void ParallelAna(Int_t fileIndex = 0)
{
   TChain *chain = new TChain("electron");
   chain->Add("../ozgur.root");
   
   //const Int_t kNThreads = chain->GetNtrees();
   //ActivatePROOF(chain, kNThreads);
   ActivatePROOF(chain);
   //ActivatePROOF(chain, 5);

   chain->Process("MacroParticle.C+O(nullptr, 1.)");
}
