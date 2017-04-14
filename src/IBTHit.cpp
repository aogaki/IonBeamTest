#include <iomanip>

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "IBTHit.hpp"


G4ThreadLocal G4Allocator<IBTHit> *IBTHitAllocator = 0;


IBTHit::IBTHit()
   : G4VHit(),
     fTrackID(0),
     fKineticEnergy(0),
     fIncidentEnergy(0),
     fDepositEnergy(0),
     fTime(0),
     fPosition(0),
     fMomentum(0),
     fIsExit(false),
     fVolumeName("")
{}

IBTHit::~IBTHit()
{}

IBTHit::IBTHit(const IBTHit & /*right*/)
   : G4VHit()
{}

const IBTHit &
IBTHit::operator=(const IBTHit & /*right*/)
{
   return *this;
}

int IBTHit::operator==(const IBTHit & /*right*/) const
{
   return 0;
}
