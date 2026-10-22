#ifndef SimulationConfig_h
#define SimulationConfig_h 1

#include "G4ThreeVector.hh"

class SimulationConfig {
public:
    static SimulationConfig* Instance();

    G4ThreeVector tumourPosition;
    G4ThreeVector tumourSemiAxes;
private:
    SimulationConfig() = default;
};

#endif