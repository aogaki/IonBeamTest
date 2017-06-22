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
   IBTPrimaryGeneratorAction(G4bool monoFlag, G4double ene);
   virtual ~IBTPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   G4ParticleGun *fParticleGun;

   G4bool fMonoFlag;
   G4double fEnergy;

};

#endif
