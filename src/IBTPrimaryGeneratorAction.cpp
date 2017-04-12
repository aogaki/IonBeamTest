#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4AutoLock.hh>
#include <G4ParticleTable.hh>
#include <G4ChargedGeantino.hh>
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
   fFirstFlag = true;
   fZPosition = -300.*mm;
   
   G4int nPar = 1;
   fParticleGun = new G4ParticleGun(nPar);

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4ParticleDefinition* particle
      = particleTable->FindParticle("chargedgeantino");
   fParticleGun->SetParticleDefinition(particle);
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fParticleGun->SetParticleEnergy(1.*eV);    
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

IBTPrimaryGeneratorAction::~IBTPrimaryGeneratorAction()
{
   //G4AutoLock lock(&mutexInPGA);
   if(fParticleGun != nullptr) {delete fParticleGun; fParticleGun = nullptr;}
}

void IBTPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   if(fFirstFlag){
      fFirstFlag = false;
      //fluorine 
      G4int Z = 2;
      G4int A = 4;
      G4double ionCharge   = 2.*eplus;
      G4double excitEnergy = 0.*keV;
        
      G4ParticleDefinition *ion
         = G4IonTable::GetIonTable()->GetIon(Z, A, excitEnergy);
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(ionCharge);
   }

   G4double ene = G4RandExponential::shoot(100)*MeV;
   fParticleGun->SetParticleEnergy(ene);
   //create vertex
   //
   fParticleGun->GeneratePrimaryVertex(event);

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->FillNtupleIColumn(1, 0, event->GetEventID());
   anaMan->FillNtupleDColumn(1, 1, ene);
   anaMan->AddNtupleRow(1);

   G4AutoLock lock(&mutexInPGA);
   if (nEveInPGA++ % 10000 == 0)
      G4cout << nEveInPGA - 1 << " events done" << G4endl;

}
