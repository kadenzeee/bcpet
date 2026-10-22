//
// Spawns particles uniformly within the tumour.
//

#include "G4IonTable.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Exception.hh"
#include "G4RandomTools.hh"


#include "PrimaryGeneratorAction.hh"
#include "SimulationConfig.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction() 
    : fParticleGun(0)
{
    fParticleGun = new G4ParticleGun(1);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}


void PrimaryGeneratorAction::SetIsotope(G4int Z, G4int A) {
    fZ = Z;
    fA = A;
}


G4ThreeVector PrimaryGeneratorAction::GetPointInEllipsoid(const G4ThreeVector& axes) {
    // Generate a random position within the tumour ellipsoid
    G4double u   = G4UniformRand();          // 0..1
    G4double r   = std::cbrt(u);             // cube root for uniform volume

    // Random direction on the unit sphere
    G4ThreeVector dir = G4RandomDirection();

    return G4ThreeVector(
        dir.x() * r * axes.x(),
        dir.y() * r * axes.y(),
        dir.z() * r * axes.z()
    );
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    auto* ionTable = G4ParticleTable::GetParticleTable()->GetIonTable();
    G4ParticleDefinition* ion = ionTable->GetIon(fZ, fA, 0.0);

    auto* config = SimulationConfig::Instance();
    G4ThreeVector tumourPosition = config->tumourPosition;
    G4ThreeVector tumourSemiAxes = config->tumourSemiAxes;

    G4ThreeVector localPosition = GetPointInEllipsoid(tumourSemiAxes);

    G4ThreeVector worldPosition = tumourPosition + localPosition;

    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleEnergy(0.0*keV);
    fParticleGun->SetParticlePosition(worldPosition);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}



