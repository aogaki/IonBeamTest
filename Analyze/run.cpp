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

void run()
{   
   TChain *chain = new TChain("IBT");
   chain->Add("Data/proton*.root");

   ActivatePROOF(chain);

   chain->Process("TAnalyzer.C+O");
}
