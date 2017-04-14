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

      anaMan->FillNtupleIColumn(0, 0, eventID); // EventID

      G4int trackID = newHit->GetTrackID();
      anaMan->FillNtupleIColumn(0, 1, trackID);

      G4String volumeName = newHit->GetVolumeName();
      anaMan->FillNtupleSColumn(0, 2, volumeName);

      G4double kineticEnergy = newHit->GetKineticEnergy();
      anaMan->FillNtupleDColumn(0, 3, kineticEnergy);

      G4ThreeVector position = newHit->GetPosition();
      anaMan->FillNtupleDColumn(0, 4, position.x());
      anaMan->FillNtupleDColumn(0, 5, position.y());
      anaMan->FillNtupleDColumn(0, 6, position.z());

      G4ThreeVector momentum = newHit->GetMomentum();
      anaMan->FillNtupleDColumn(0, 7, momentum.x());
      anaMan->FillNtupleDColumn(0, 8, momentum.y());
      anaMan->FillNtupleDColumn(0, 9, momentum.z());

      G4double depositEnergy = newHit->GetDepositEnergy();
      anaMan->FillNtupleDColumn(0, 10, depositEnergy);

      G4double incidentEnergy = newHit->GetIncidentEnergy();
      anaMan->FillNtupleDColumn(0, 11, incidentEnergy);

      G4int isExit = newHit->GetIsExit();
      anaMan->FillNtupleIColumn(0, 12, isExit);

      G4double time = newHit->GetTime();
      anaMan->FillNtupleDColumn(0, 13, time);

      anaMan->AddNtupleRow(0);
   }

}

