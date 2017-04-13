#include "IBTActionInitialization.hpp"
#include "IBTPrimaryGeneratorAction.hpp"
#include "IBTRunAction.hpp"
#include "IBTEventAction.hpp"


IBTActionInitialization::IBTActionInitialization(G4bool monoFlag, G4double ene)
   : G4VUserActionInitialization()
{
   fMonoFlag = monoFlag;
   fEnergy = ene;
}

IBTActionInitialization::~IBTActionInitialization()
{}

void IBTActionInitialization::BuildForMaster() const
{
   SetUserAction(new IBTRunAction());
}

void IBTActionInitialization::Build() const
{
   SetUserAction(new IBTPrimaryGeneratorAction(fMonoFlag, fEnergy));
   SetUserAction(new IBTRunAction());
   SetUserAction(new IBTEventAction());
}
