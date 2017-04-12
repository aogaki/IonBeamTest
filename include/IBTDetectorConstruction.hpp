#ifndef IBTDetectorConstruction_h
#define IBTDetectorConstruction_h 1

#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Material.hh>
#include <G4VisAttributes.hh>


class IBTDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   IBTDetectorConstruction();
   virtual ~IBTDetectorConstruction();

   virtual G4VPhysicalVolume *Construct();
   virtual void ConstructSDandField();

private:
   G4bool fCheckOverlap;
   
   // Materials
   void DefineMaterials();
   G4Material *fVacuumMat;
   G4Material *fAirMat;
   G4Material *fKaptonMat;
   
   G4double fKaptonT;
   G4double fAirT;
   
   std::vector<G4VisAttributes *> fVisAttributes;

};

#endif
