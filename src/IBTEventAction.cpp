#include <G4Event.hh>
#include <G4SDManager.hh>
#include <g4root.hh>

#include "IBTEventAction.hpp"


IBTEventAction::IBTEventAction()
   : G4UserEventAction(),
     fHitsCollectionID(-1)
{}

IBTEventAction::~IBTEventAction()
{}

IBTHitsCollection *IBTEventAction::GetHitsCollection(G4int hcID, const G4Event *event)
const
{
   IBTHitsCollection *hitsCollection 
      = static_cast<IBTHitsCollection *>(
         event->GetHCofThisEvent()->GetHC(hcID));
  
   if ( ! hitsCollection ) {
      G4ExceptionDescription msg;
      msg << "Cannot access hitsCollection ID " << hcID;
      // check how to use G4Exception
      G4Exception("IBTEventAction::GetHitsCollection()",
                  "MyCode0003", FatalException, msg);
   }         

   return hitsCollection;
}

void IBTEventAction::BeginOfEventAction(const G4Event *)
{}

void IBTEventAction::EndOfEventAction(const G4Event *event)
{

   if (fHitsCollectionID == -1)
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("HC");
   
   IBTHitsCollection *hc = GetHitsCollection(fHitsCollectionID, event);
   
   G4int eventID = event->GetEventID();

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();

   const G4int kHit = hc->entries();
   for (G4int iHit = 0; iHit < kHit; iHit++) {
      IBTHit *newHit = (*hc)[iHit];
      
      anaMan->FillNtupleIColumn(0, eventID); // EventID

      G4int trackID = newHit->GetTrackID();
      anaMan->FillNtupleIColumn(1, trackID);

      G4double kineticEnergy = newHit->GetKineticEnergy();
      anaMan->FillNtupleDColumn(2, kineticEnergy);

      G4ThreeVector position = newHit->GetPosition();
      anaMan->FillNtupleDColumn(3, position.x());
      anaMan->FillNtupleDColumn(4, position.y());
      anaMan->FillNtupleDColumn(5, position.z());

      G4ThreeVector momentum = newHit->GetMomentum();
      anaMan->FillNtupleDColumn(6, momentum.x());
      anaMan->FillNtupleDColumn(7, momentum.y());
      anaMan->FillNtupleDColumn(8, momentum.z());

      G4int code = newHit->GetPDGCode();
      anaMan->FillNtupleIColumn(9, code);

      anaMan->AddNtupleRow();
   }

}

