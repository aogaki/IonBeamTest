#include <map>
#include <iostream>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TDatabasePDG.h>

#include "eleMap.h"


const Int_t kNoFile = 6;
TString kEne[kNoFile] = {"1", "10", "20", "30", "40", "50"};

class TMakeHist
{
public:
   TMakeHist(TString name);
   ~TMakeHist();

   void FillHists();
   
private:
   TChain *fChain[kNoFile];

   void InitHists();
   TString fIonName;
   TH1D *fHisAll[kNoFile];
   TH1D *fHisPrimal[kNoFile];
   TH2D *fHisParticle[kNoFile];
   TH2D *fHisIon[kNoFile];

   TString GetIonName(Int_t pdgCode);
   
   map<TString, Int_t> fA;
};

TMakeHist::TMakeHist(TString name)
{
   fIonName = name;
   
   for(Int_t i = 0; i < kNoFile; i++){
      fChain[i] = new TChain("IBT");
      TString fileName = "Data/" + fIonName + kEne[i] + "GeV*";
      fChain[i]->Add(fileName);
   }

   fA["H"] = 1;
   fA["He"] = 4;
   fA["C"] = 12;
   fA["O"] = 16;
   fA["Ne"] = 20;
   fA["Si"] = 28;
   fA["Ca"] = 40;
   fA["Fe"] = 56;
   
   InitHists();
}

TMakeHist::~TMakeHist()
{
   auto file = new TFile(fIonName + ".root", "RECREATE");
   for(auto &&his: fHisAll) his->Write();
   for(auto &&his: fHisPrimal) his->Write();
   for(auto &&his: fHisParticle) his->Write();
   for(auto &&his: fHisIon) his->Write();
   file->Close();
   delete file;
}

void TMakeHist::InitHists()
{
   Double_t eneMax = fA[fIonName] * 50;
   Int_t nBins = 10000;
   for(Int_t i = 0; i < kNoFile; i++){
      eneMax = fA[fIonName] * kEne[i].Atof();
      nBins = eneMax * 1000;
      if(nBins > 100000) nBins = 100000;
      
      fHisAll[i] = new TH1D("HisAll" + fIonName + kEne[i],
                            "Energy distribution of all particles",
                            nBins, 0., eneMax);
      fHisAll[i]->SetXTitle("Kinetic energy [GeV]");

      fHisPrimal[i] = new TH1D("HisPrimal" + fIonName + kEne[i],
                               "Energy distribution of all particles",
                               nBins, 0., eneMax);
      fHisPrimal[i]->SetXTitle("Kinetic energy [GeV]");

      fHisParticle[i] = new TH2D("HisParticle" + fIonName + kEne[i],
                                 "Energy distribution of each particle",
                                 nBins, 0., eneMax, 10, 0., 0.);
      fHisParticle[i]->SetXTitle("Kinetic energy [GeV]");

      fHisIon[i] = new TH2D("HisIon" + fIonName + kEne[i],
                            "Energy distribution of each particle",
                            nBins, 0., eneMax, 10, 0., 0.);
      fHisIon[i]->SetXTitle("Kinetic energy [GeV]");

   }
}

void TMakeHist::FillHists()
{
   for(Int_t iFile = 0; iFile < kNoFile; iFile++){
      auto chain = fChain[iFile];
      chain->SetBranchStatus("*", kFALSE);

      Double_t ene;
      chain->SetBranchStatus("KineticEnergy", kTRUE);
      chain->SetBranchAddress("KineticEnergy", &ene);

      Double_t z;
      chain->SetBranchStatus("z", kTRUE);
      chain->SetBranchAddress("z", &z);

      Int_t trackID;
      chain->SetBranchStatus("TrackID", kTRUE);
      chain->SetBranchAddress("TrackID", &trackID);

      Int_t pdgCode;
      chain->SetBranchStatus("PDGCode", kTRUE);
      chain->SetBranchAddress("PDGCode", &pdgCode);

      const Long64_t kMaxEve = chain->GetEntries();
      for(Int_t iEve = 0; iEve < kMaxEve; iEve++){
         chain->GetEntry(iEve);
         if(z > 100.) continue;
         ene /= 1000.;
         fHisAll[iFile]->Fill(ene);

         auto particle = TDatabasePDG::Instance()->GetParticle(pdgCode);
         TString name;
         if(particle){
            name = particle->GetName();
            fHisParticle[iFile]->Fill(ene, name, 1.);
         }
         else{
            name = GetIonName(pdgCode);
            fHisIon[iFile]->Fill(ene, name, 1.);
         }
         if(trackID == 1) fHisPrimal[iFile]->Fill(ene);
      }
   }
}

TString TMakeHist::GetIonName(Int_t pdgCode)
{
   TString code = TString::Itoa(pdgCode, 10);
   Int_t Z = TString(code(3, 3)).Atoi();
   Int_t A = TString(code(6, 3)).Atoi();
   
   return gEleMap[Z] + TString::Itoa(A, 10);
}

void MakeHist(TString ion = "He")
{
   InitEleMap();
   
   TMakeHist *test = new TMakeHist(ion);
   test->FillHists();
   delete test;
}
