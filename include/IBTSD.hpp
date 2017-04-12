#ifndef IBTSD_h
#define IBTSD_h 1

#include <G4VSensitiveDetector.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "IBTHit.hpp"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class IBTSD: public G4VSensitiveDetector
{
public:
   IBTSD(const G4String &name,
            const G4String &hitsCollectionName);
   virtual ~IBTSD();

   virtual void Initialize(G4HCofThisEvent *hce);
   virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
   IBTHitsCollection *fHitsCollection;
};

#endif
