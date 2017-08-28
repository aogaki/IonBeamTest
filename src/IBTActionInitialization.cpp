#include "IBTActionInitialization.hpp"
#include "IBTPrimaryGeneratorAction.hpp"
#include "IBTRunAction.hpp"
#include "IBTEventAction.hpp"


IBTActionInitialization::IBTActionInitialization()
   : G4VUserActionInitialization()
{}

IBTActionInitialization::~IBTActionInitialization()
{}

void IBTActionInitialization::BuildForMaster() const
{
   SetUserAction(new IBTRunAction());
}

void IBTActionInitialization::Build() const
{
   SetUserAction(new IBTPrimaryGeneratorAction());
   SetUserAction(new IBTRunAction());
   SetUserAction(new IBTEventAction());
}
