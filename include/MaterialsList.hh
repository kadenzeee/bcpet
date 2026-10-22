#ifndef MaterialsList_h
#define MaterialsList_h 1

#include "G4Material.hh"
#include "G4NistManager.hh"
#include <map>
#include <string>

class MaterialsList
{
  public:
    MaterialsList();
    ~MaterialsList();

    void DefineMaterials();

    G4Material* GetMaterial(const G4String& name) const;

  private:
    std::map<std::string, G4Material*> materials;
};

#endif