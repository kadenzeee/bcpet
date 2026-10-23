#include "RunAction.hh"
#include "RootOutput.hh"
#include "SimulationConfig.hh"

#include "G4Run.hh"

void RunAction::BeginOfRunAction(const G4Run*) {
    RootOutput::Instance()->Open(SimulationConfig::Instance()->outputFile);
}

void RunAction::EndOfRunAction(const G4Run*) {
    RootOutput::Instance()->Close();
}
