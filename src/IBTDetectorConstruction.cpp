#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4LogicalVolumeStore.hh>

#include "IBTDetectorConstruction.hpp"
#include "IBTSD.hpp"


IBTDetectorConstruction::IBTDetectorConstruction()
   : fVacuumMat(nullptr),
     fAirMat(nullptr),
     fKaptonMat(nullptr)
{
   fCheckOverlap = true;
   fAirT = 5.*mm;
   fKaptonT = 15.*um;
   
   DefineMaterials();
}

IBTDetectorConstruction::~IBTDetectorConstruction()
{
   
}

void IBTDetectorConstruction::DefineMaterials()
{
   G4NistManager *manager = G4NistManager::Instance();

   // NIST database materials
   fVacuumMat = manager->FindOrBuildMaterial("G4_Galactic");
   fAirMat = manager->FindOrBuildMaterial("G4_AIR");
   fKaptonMat = manager->FindOrBuildMaterial("G4_KAPTON");
}

G4VPhysicalVolume *IBTDetectorConstruction::Construct()
{
   // world volume
   G4double worldX = 0.1*m;
   G4double worldY = 0.1*m;
   G4double worldZ = 0.6*m;

   G4Box *worldS = new G4Box("World", worldX / 2., worldY / 2., worldZ / 2.);
   G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, fVacuumMat, "World");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   worldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4VPhysicalVolume *worldPV
      = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", 0,
                          false, 0, fCheckOverlap);

   // Air layer
   G4double airW = worldX;
   G4double airH = worldY;

   G4Box *airS = new G4Box("Air", airW / 2., airH / 2., fAirT / 2.);
   G4LogicalVolume *airLV = new G4LogicalVolume(airS, fAirMat, "Air");
   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   airLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4ThreeVector airPos = G4ThreeVector(0., 0., 0.);
   new G4PVPlacement(nullptr, airPos, airLV, "Air", worldLV,
                     false, 0, fCheckOverlap);


   // Kapton layer
   G4double kaptonW = worldX;
   G4double kaptonH = worldY;

   G4Box *kaptonS = new G4Box("Kapton", kaptonW / 2., kaptonH / 2., fKaptonT / 2.);
   G4LogicalVolume *kaptonLV = new G4LogicalVolume(kaptonS, fKaptonMat, "Kapton");
   visAttributes = new G4VisAttributes(G4Colour::Magenta());
   kaptonLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4ThreeVector kaptonPos1 = G4ThreeVector(0., 0., -(fAirT + fKaptonT) / 2.);
   new G4PVPlacement(nullptr, kaptonPos1, kaptonLV, "Kapton1", worldLV,
                     false, 0, fCheckOverlap);

   G4ThreeVector kaptonPos2 = G4ThreeVector(0., 0., (fAirT + fKaptonT) / 2.);
   new G4PVPlacement(nullptr, kaptonPos2, kaptonLV, "Kapton2", worldLV,
                     false, 1, fCheckOverlap);


   return worldPV;
}

void IBTDetectorConstruction::ConstructSDandField()
{

   // Sensitive Detectors
   G4VSensitiveDetector *SD = new IBTSD("SD", "HC");
   G4SDManager::GetSDMpointer()->AddNewDetector(SD);
   
   G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
   for(auto &&lv: *lvStore){
      if(lv->GetName().contains("Air") ||
         lv->GetName().contains("Kapton"))
         SetSensitiveDetector(lv->GetName(), SD);
   }

}
