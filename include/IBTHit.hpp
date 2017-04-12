#ifndef IBTHit_h
#define IBTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class IBTHit : public G4VHit
{
public:
   IBTHit();
   virtual ~IBTHit();
   IBTHit(const IBTHit &right);
   const IBTHit &operator=(const IBTHit &right);
   int operator==(const IBTHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods
   void SetTrackID(G4int id) {fTrackID = id;};
   G4int GetTrackID() {return fTrackID;};

   void SetKineticEnergy(G4double ene) {fKineticEnergy = ene;};
   G4double GetKineticEnergy() {return fKineticEnergy;};

   void SetIncidentEnergy(G4double ene) {fIncidentEnergy = ene;};
   G4double GetIncidentEnergy() {return fIncidentEnergy;};

   void SetDepositEnergy(G4double ene) {fDepositEnergy = ene;};
   G4double GetDepositEnergy() {return fDepositEnergy;};

   void SetPosition(G4ThreeVector pos) {fPosition = pos;};
   G4ThreeVector GetPosition() {return fPosition;};
   
   void SetMomentum(G4ThreeVector p) {fMomentum = p;};
   G4ThreeVector GetMomentum() {return fMomentum;};
   
   void SetIsExit(G4int flag) {fIsExit = flag;};
   G4int GetIsExit() {return fIsExit;};
   
   void SetVolumeName(G4String volumeName) {fVolumeName = volumeName;};
   G4String GetVolumeName() {return fVolumeName;};

   void SetTime(G4double time) {fTime = time;};
   G4double GetTime() {return fTime;};

private:
   G4int fTrackID;
   G4double fKineticEnergy;
   G4double fIncidentEnergy;
   G4double fDepositEnergy;
   G4double fTime;
   G4ThreeVector fPosition;
   G4ThreeVector fMomentum;
   G4int fIsExit;
   G4String fVolumeName;
};

typedef G4THitsCollection<IBTHit> IBTHitsCollection;

extern G4ThreadLocal G4Allocator<IBTHit> *IBTHitAllocator;

inline void *IBTHit::operator new(size_t)
{
   if (!IBTHitAllocator)
      IBTHitAllocator = new G4Allocator<IBTHit>;
   return (void *)IBTHitAllocator->MallocSingle();
}

inline void IBTHit::operator delete(void *hit)
{
   IBTHitAllocator->FreeSingle((IBTHit *) hit);
}

#endif
