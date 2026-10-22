#ifndef SimulationConfig_h
#define SimulationConfig_h 1

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

class SimulationConfig {
public:
    static SimulationConfig* Instance();

    G4ThreeVector tumourPosition;
    G4ThreeVector tumourSemiAxes;
    G4double detectorRadius = 100.*mm;
    G4double detectorThickness = 1.*mm;
    G4String outputFile = "bcpet.root";
private:
    SimulationConfig() = default;
};

#endif