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


IBTPrimaryGeneratorAction::IBTPrimaryGeneratorAction(G4bool monoFlag, G4double ene)
   : G4VUserPrimaryGeneratorAction(),
     fParticleGun(nullptr)
{
   fMonoFlag = monoFlag;
   fEnergy = ene;

   G4int nPar = 1;
   fParticleGun = new G4ParticleGun(nPar);

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4ParticleDefinition* particle
      = particleTable->FindParticle("gamma");
   fParticleGun->SetParticleDefinition(particle);
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -300.*mm));
   fParticleGun->SetParticleEnergy(1.*GeV);    
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

IBTPrimaryGeneratorAction::~IBTPrimaryGeneratorAction()
{
   if(fParticleGun != nullptr) {delete fParticleGun; fParticleGun = nullptr;}
}

void IBTPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   G4double ene;
   if(fMonoFlag) ene = fEnergy;
   else ene = G4RandExponential::shoot(fEnergy)*MeV;
   fParticleGun->SetParticleEnergy(ene);
   fParticleGun->GeneratePrimaryVertex(event);

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->FillNtupleIColumn(1, 0, event->GetEventID());
   anaMan->FillNtupleDColumn(1, 1, ene);
   anaMan->AddNtupleRow(1);

   G4AutoLock lock(&mutexInPGA);
   if (nEveInPGA++ % 10000 == 0)
      G4cout << nEveInPGA - 1 << " events done" << G4endl;
}
