#ifndef IBTPrimaryGeneratorAction_h
#define IBTPrimaryGeneratorAction_h 1

#include <TH1D.h>

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
   
   G4double fDx;
   G4double fTh;
   
   Double_t GetEne();
   TH1D *fHisEne;
};

#endif
