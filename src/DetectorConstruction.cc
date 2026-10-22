//
//  Defines breast tissue phantom, generates random ellipsoidal tumours,
//  builds PET ring of scintillator crystals.
//

#include "G4Ellipsoid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Box.hh"

#include "MaterialsList.hh"
#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction() = default;

void DetectorConstruction::SetTumourPosition(const G4ThreeVector& position) {
    tumourPosition = position;
}

G4ThreeVector DetectorConstruction::GetTumourPosition() const {
    return tumourPosition;
}

void DetectorConstruction::SetTumourSemiAxes(const G4ThreeVector& semiAxes) {
    tumourSemiAxes = semiAxes;
}

G4ThreeVector DetectorConstruction::GetTumourSemiAxes() const {
    return tumourSemiAxes;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    MaterialsList materialsList;
    materialsList.DefineMaterials();

    G4Material* water = materialsList.GetMaterial("water");
    G4Material* air = materialsList.GetMaterial("air");
    G4Material* breastTissue = materialsList.GetMaterial("breastTissue");

    // World Volume
    auto worldSolid = new G4Box("worldSolid", 1*m, 1*m, 1*m);
    auto worldLogical = new G4LogicalVolume(worldSolid, air, "worldLogical");
    auto worldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), worldLogical, "worldPV", nullptr, false, 0, true);

    // Breast Phantom
    auto breastSolid = new G4Ellipsoid("breastSolid", 60*mm, 50*mm, 40*mm);
    auto breastLogical = new G4LogicalVolume(breastSolid, breastTissue, "breastLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), breastLogical, "breastPhysical", worldLogical, false, 0, true);

    // Tumour Phantom
    std::cout << "Tumour position: " << tumourPosition << std::endl;
    std::cout << "Tumour semi-axes: " << tumourSemiAxes << std::endl;
    auto tumourSolid = new G4Ellipsoid("tumourSolid", tumourSemiAxes.x(), tumourSemiAxes.y(), tumourSemiAxes.z());
    auto tumourLogical = new G4LogicalVolume(tumourSolid, water, "tumourLogical");
    new G4PVPlacement(nullptr, tumourPosition, tumourLogical, "tumourPV", breastLogical, false, 0, true);

    // ----------------
    //  GUI
    // ----------------

    worldLogical->SetVisAttributes(
        G4VisAttributes::GetInvisible()
    );

    auto breastVis = new G4VisAttributes();
    breastVis->SetVisibility(true);
    breastVis->SetForceSolid(true);
    breastVis->SetColour(108/255., 166/255., 188/255., 0.3);
    breastLogical->SetVisAttributes(breastVis);

    auto tumourVis = new G4VisAttributes();
    tumourVis->SetVisibility(true);
    tumourVis->SetForceSolid(true);
    tumourVis->SetColour(100/255., 100/255., 100/255, 0.5);
    tumourLogical->SetVisAttributes(tumourVis);

    return worldPhysical;
}





