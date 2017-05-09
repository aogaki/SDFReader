#include <TChain.h>
#include <TProof.h>
#include <TProofLite.h>
#include <TString.h>


void ActivatePROOF(TChain *chain, Int_t nThreads = 0)
{
   TProof *proof = TProof::Open("");
   //proof->SetProgressDialog(kFALSE);
   if(nThreads > 0) proof->SetParallel(nThreads);

   chain->SetProof();
}

void ParallelAna()
{
   TChain *chain = new TChain("electron1");
   chain->Add("../electron1.root");
   
   //const Int_t kNThreads = chain->GetNtrees();
   //ActivatePROOF(chain, kNThreads);
   ActivatePROOF(chain);
   //ActivatePROOF(chain, 5);

   chain->Process("MacroParticle.C+O");
}
