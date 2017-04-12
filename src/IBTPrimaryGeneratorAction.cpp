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
     fParticleGun(nullptr),
     fMessenger(nullptr)
{
   fFirstFlag = true;
   fZPosition = -300.*mm;
   
   fZ = 6;
   fA = 12;
   fIonCharge   = 0.*eplus;
   fExcitEnergy = 0.*keV;

   G4int nPar = 1;
   fParticleGun = new G4ParticleGun(nPar);

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4ParticleDefinition* particle
      = particleTable->FindParticle("chargedgeantino");
   fParticleGun->SetParticleDefinition(particle);
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., fZPosition));
   fParticleGun->SetParticleEnergy(1.*eV);    
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

   DefineCommands();
}

IBTPrimaryGeneratorAction::~IBTPrimaryGeneratorAction()
{
   if(fParticleGun != nullptr) {delete fParticleGun; fParticleGun = nullptr;}
   if(fMessenger != nullptr) {delete fMessenger; fMessenger = nullptr;}
}

void IBTPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   if(fFirstFlag){
      fFirstFlag = false;
      SetIon();
   }

   G4double ene = G4RandExponential::shoot(100)*MeV;
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

void IBTPrimaryGeneratorAction::SetIon()
{
   G4ParticleDefinition *ion
      = G4IonTable::GetIonTable()->GetIon(fZ, fA, fExcitEnergy);
   fParticleGun->SetParticleDefinition(ion);
   fParticleGun->SetParticleCharge(fIonCharge);
}

void IBTPrimaryGeneratorAction::DefineCommands()
{
   fMessenger = new G4GenericMessenger(this, "/IBT/Primary/", 
                                       "Beam control");

   // z position
   G4GenericMessenger::Command &ionCmd
      = fMessenger->DeclareMethod("ionName",
                                  &IBTPrimaryGeneratorAction::SetIonCmd, 
                                  "Set the ion type");

   ionCmd.SetParameterName("ionName", true);
}

void IBTPrimaryGeneratorAction::SetIonCmd(G4String ionName)
{
   if(ionName == "test"){
      fZ = 100;
      fA = 200;
   }      
   else if(ionName == "He"){
      fZ = 2;
      fA = 4;
   }
   else if(ionName == "C"){
      fZ = 6;
      fA = 12;
   }
   else if(ionName == "O"){
      fZ = 8;
      fA = 16;
   }
   else if(ionName == "Fe"){
      fZ = 26;
      fA = 56; // Only integer is accepted.  A is 55.845
   }
   else{
      G4cout << "Now, only He, C, O, and Fe is acceptable." << G4endl;
   }
   
   SetIon();
}
