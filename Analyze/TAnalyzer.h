//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Aug 21 09:04:33 2017 by ROOT version 6.11/01
// from TChain IBT/
//////////////////////////////////////////////////////////

#ifndef TAnalyzer_h
#define TAnalyzer_h

#include <map>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TString.h>
#include <TH2.h>
#include <TH1.h>

// Headers needed by this particular selector

class TAnalyzer : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> TrackID = {fReader, "TrackID"};
   TTreeReaderValue<Double_t> KineticEnergy = {fReader, "KineticEnergy"};
   TTreeReaderValue<Int_t> PDGCode = {fReader, "PDGCode"};


   TAnalyzer(TTree * /*tree*/ =0) { }
   virtual ~TAnalyzer() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   void InitEleMap();
   TString GetIonName(Int_t pdgCode);
   std::map<Int_t, TString> fEleMap;

   TH1D *fHisAll;
   TH1D *fHisPrimal;
   TH2D *fHisParticle;
   TH2D *fHisIon;
   
   ClassDef(TAnalyzer,0);

};

#endif

#ifdef TAnalyzer_cxx
void TAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t TAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef TAnalyzer_cxx
