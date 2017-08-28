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
     fWallMat(nullptr)
{
   fCheckOverlap = true;
   
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
   fWallMat = manager->FindOrBuildMaterial("G4_Al");
}

G4VPhysicalVolume *IBTDetectorConstruction::Construct()
{
   // world volume
   G4double worldX = 1.*m;
   G4double worldY = 1.*m;
   G4double worldZ = 1.*m;

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
   G4double wallW = worldX;
   G4double wallH = worldY;
   G4double wallT = 10.*cm;

   G4Box *wallS = new G4Box("Wall", wallW / 2., wallH / 2., wallT / 2.);
   G4LogicalVolume *wallLV = new G4LogicalVolume(wallS, fWallMat, "Wall");
   visAttributes = new G4VisAttributes(G4Colour::Cyan());
   wallLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4ThreeVector wallPos = G4ThreeVector(0., 0., wallT / 2.);
   new G4PVPlacement(nullptr, wallPos, wallLV, "Wall", worldLV,
                     false, 0, fCheckOverlap);

   return worldPV;
}

void IBTDetectorConstruction::ConstructSDandField()
{

   // Sensitive Detectors
   G4VSensitiveDetector *SD = new IBTSD("SD", "HC");
   G4SDManager::GetSDMpointer()->AddNewDetector(SD);
   
   G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
   for(auto &&lv: *lvStore){
      if(lv->GetName().contains("Wall") ||
         lv->GetName().contains("Kapton"))
         SetSensitiveDetector(lv->GetName(), SD);
   }

}
