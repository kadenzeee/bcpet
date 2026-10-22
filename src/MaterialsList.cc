//
//  Defines all the materials needed in the simulation.
//


// https://aapm.onlinelibrary.wiley.com/doi/10.1002/mp.14226 tests breast tissue at 50% adipose and 50% fibroglandular tissue. This is the composition of the breast phantom used in this simulation.


#include "MaterialsList.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4Exception.hh"

#include <string>

MaterialsList::MaterialsList() {}
MaterialsList::~MaterialsList() {}

void MaterialsList::DefineMaterials()
{
    auto* nist = G4NistManager::Instance();

    materials["air"] = nist->FindOrBuildMaterial("G4_AIR");
    materials["water"] = nist->FindOrBuildMaterial("G4_WATER");
    materials["adipose"] = nist->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");

    //ICRU Report , 1992, Vol.os-24(1), p.11-13
    auto* fibroglandular = new G4Material("fibroglandular", 1.04 * g / cm3, 8);
    fibroglandular->AddElement(nist->FindOrBuildElement("H"), 0.102);
    fibroglandular->AddElement(nist->FindOrBuildElement("C"), 0.267);
    fibroglandular->AddElement(nist->FindOrBuildElement("N"), 0.028);
    fibroglandular->AddElement(nist->FindOrBuildElement("O"), 0.539);
    fibroglandular->AddElement(nist->FindOrBuildElement("Na"), 0.0004);
    fibroglandular->AddElement(nist->FindOrBuildElement("P"), 0.017);
    fibroglandular->AddElement(nist->FindOrBuildElement("S"), 0.004);
    fibroglandular->AddElement(nist->FindOrBuildElement("Ca"), 0.0004);

    materials["fibroglandular"] = fibroglandular;

    auto* breastTissue =
        new G4Material("BreastTissue", 1.0 * g / cm3, 2);

    breastTissue->AddMaterial(materials["adipose"], 50 * perCent);
    breastTissue->AddMaterial(materials["fibroglandular"], 50 * perCent);

    materials["breastTissue"] = breastTissue;
}

G4Material* MaterialsList::GetMaterial(const G4String& name) const {
    auto it = materials.find(name);
    if (it != materials.end()) {
        return it->second;
    }
    return nullptr;
}

