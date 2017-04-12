#ifndef IBTEventAction_h
#define IBTEventAction_h 1

#include <G4UserEventAction.hh>

#include "IBTHit.hpp"

class IBTEventAction : public G4UserEventAction
{
public:
   IBTEventAction();
   virtual ~IBTEventAction();

   virtual void BeginOfEventAction(const G4Event *);
   virtual void EndOfEventAction(const G4Event *);

private:
   IBTHitsCollection *GetHitsCollection(G4int hcID, const G4Event *event) const;
   
   G4int fHitsCollectionID;
};

#endif
