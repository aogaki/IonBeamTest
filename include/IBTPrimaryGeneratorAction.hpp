#ifndef IBTPrimaryGeneratorAction_h
#define IBTPrimaryGeneratorAction_h 1

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4ThreeVector.hh>
#include <G4Threading.hh>
#include <G4Event.hh>
#include <G4GenericMessenger.hh>


class IBTPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
   IBTPrimaryGeneratorAction();
   virtual ~IBTPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   G4ParticleGun *fParticleGun;
   G4double fZPosition;
   G4bool fFirstFlag;

   // Ion parameters
   void SetIon();
   G4int fZ;
   G4int fA;
   G4double fIonCharge;
   G4double fExcitEnergy;

   // Commands
   void DefineCommands();
   G4GenericMessenger *fMessenger;
   // Changing Ion 
   void SetIonCmd(G4String name);
   
};

#endif
