#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4TouchableHistory.hh>
#include <G4Step.hh>
#include <G4ios.hh>
#include <g4root.hh>
#include <G4SystemOfUnits.hh>
#include <G4Material.hh>
#include <G4VProcess.hh>

#include "IBTSD.hpp"
#include "IBTHit.hpp"


IBTSD::IBTSD(const G4String &name,
             const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

IBTSD::~IBTSD()
{}

void IBTSD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new IBTHitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool IBTSD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   G4Track *track = step->GetTrack();   
   G4int trackID = track->GetTrackID();

   G4StepPoint *postStepPoint = step->GetPostStepPoint();
   if(postStepPoint->GetStepStatus() == fGeomBoundary) return false; // only going out particle

   IBTHit *newHit = new IBTHit();
   
   newHit->SetTrackID(trackID);
   
   G4ParticleDefinition *particle = track->GetDefinition();
   G4int pdgCode = particle->GetPDGEncoding();
   //if(pdgCode != 22) return false;
   newHit->SetPDGCode(pdgCode);

   G4ThreeVector position =  postStepPoint->GetPosition();
   newHit->SetPosition(position);
   
   G4double kineticEnergy = postStepPoint->GetKineticEnergy();
   newHit->SetKineticEnergy(kineticEnergy);

   G4ThreeVector momentum =  postStepPoint->GetMomentum();
   newHit->SetMomentum(momentum);

   fHitsCollection->insert(newHit);
   return true;
}
