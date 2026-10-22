#include "SimulationConfig.hh"

SimulationConfig* SimulationConfig::Instance() {
    static SimulationConfig instance;
    return &instance;
}