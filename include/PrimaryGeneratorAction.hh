#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "DetectorConstruction.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* anEvent) override;
    void SetIsotope(G4int Z, G4int A);
    G4ThreeVector GetPointInEllipsoid(const G4ThreeVector& axes);

  private:
    G4ParticleGun* fParticleGun = nullptr;

    G4int fZ = 9;
    G4int fA = 18;

    G4ThreeVector fPosition;
};

#endif