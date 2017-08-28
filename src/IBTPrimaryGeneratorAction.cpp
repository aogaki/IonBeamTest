#include <TFile.h>

#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4AutoLock.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>
#include <Randomize.hh>
#include <g4root.hh>

#include "IBTPrimaryGeneratorAction.hpp"

static G4int nEveInPGA = 0; // Global variable change to local? 
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;


IBTPrimaryGeneratorAction::IBTPrimaryGeneratorAction()
   : G4VUserPrimaryGeneratorAction(),
     fParticleGun(nullptr)
{
   G4int nPar = 1;
   fParticleGun = new G4ParticleGun(nPar);

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4ParticleDefinition* particle
      = particleTable->FindParticle("proton");
   fParticleGun->SetParticleDefinition(particle);
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -300.*mm));
   fParticleGun->SetParticleEnergy(1.*GeV);    
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

   fDx = (487 - 163) / (log10(100.) - log10(0.1));
   fTh = 100.;

   G4AutoLock lock(&mutexInPGA);
   auto file = new TFile("Sun/dist.root", "READ");
   //fHisProton = (TH1D*)file->Get("HisProton");
   //fHisProton->SetDirectory(0);
   //fHisFe = (TH1D*)file->Get("HisFe");
   //fHisFe->SetDirectory(0);
   fHisEne = (TH1D*)file->Get("HisProton");
   fHisEne->SetDirectory(0);
   file->Close();

}

IBTPrimaryGeneratorAction::~IBTPrimaryGeneratorAction()
{
   if(fParticleGun != nullptr) {delete fParticleGun; fParticleGun = nullptr;}
}

Double_t IBTPrimaryGeneratorAction::GetEne()
{   
   Double_t ene;
   while(1){
      ene = pow(10., (fHisEne->GetRandom() - 163.) / fDx) * 0.1;
      if(ene <= fTh) break;
   }
   return ene;
}

void IBTPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   //G4double ene = GetEne() * MeV;
   G4double ene = GetEne() * 56*MeV; // Fe ion
   fParticleGun->SetParticleEnergy(ene);
   fParticleGun->GeneratePrimaryVertex(event);

   //G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   //anaMan->FillNtupleIColumn(1, 0, event->GetEventID());
   //anaMan->FillNtupleDColumn(1, 1, ene);
   //anaMan->AddNtupleRow(1);

   //G4AutoLock lock(&mutexInPGA);
   //if (nEveInPGA++ % 10000 == 0)
   //G4cout << nEveInPGA - 1 << " events done" << G4endl;
}
