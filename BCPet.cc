#include <iostream>
#include <vector>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "SimulationConfig.hh"

void test() {
    SimulationConfig* x = SimulationConfig::Instance();
}

#include "FTFP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"


void printHelp() {
    std::cout << "Usage: ./build/bcpet [options]" << std::endl
    << "Options:" << std::endl
    << " --help: Show this help message" << std::endl
    << " -n <integer>: Number of events to simulate" << std::endl
    << " -tr <vector>: Tumour position (x, y, z) in mm" << std::endl
    << " -ts <vector>: Tumour semi-axes (a, b, c) in mm" << std::endl
    << " --gui: Activate the GUI" << std::endl;
}


#include <sstream>
#include <stdexcept>
#include <string>

G4ThreeVector parseVector3(const std::string& arg)
{
    std::istringstream stream(arg);

    G4double x, y, z;
    char comma1, comma2;

    if (!(stream >> x >> comma1 >> y >> comma2 >> z) ||
        comma1 != ',' || comma2 != ',')
    {
        throw std::invalid_argument(
            "Expected three comma-separated values, e.g. 10,20,30"
        );
    }

    // Reject trailing characters or extra values.
    stream >> std::ws;
    if (!stream.eof())
    {
        throw std::invalid_argument(
            "Unexpected characters in vector: " + arg
        );
    }

    return G4ThreeVector(x * mm, y * mm, z * mm);
}

int main(int argc, char** argv) {

    // Initialize default parameters
    G4int nEvents;
    G4ThreeVector tumourPosition;
    G4ThreeVector tumourSemiAxes;
    G4bool useGUI = true;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help") {
            printHelp();
            return 0;
        } else if (arg == "-n" && i + 1 < argc) {
            nEvents = std::stoi(argv[++i]);
        } else if (arg == "-tr" && i + 1 < argc) {
            tumourPosition = parseVector3(argv[++i]);
        } else if (arg == "-ts" && i + 1 < argc) {
            tumourSemiAxes = parseVector3(argv[++i]);
        } else if (arg == "--gui") {
            useGUI = true;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }
    

    auto* detector = new DetectorConstruction();

    std::cout << tumourPosition << std::endl;
    std::cout << tumourSemiAxes << std::endl;
    
    SimulationConfig::Instance()->tumourPosition = tumourPosition;
    SimulationConfig::Instance()->tumourSemiAxes = tumourSemiAxes;

    detector->SetTumourPosition(tumourPosition);
    detector->SetTumourSemiAxes(tumourSemiAxes);

    G4RunManager *runManager = new G4RunManager();
    runManager->SetUserInitialization(detector);

    auto* physicsList = new FTFP_BERT;
    
    physicsList->RegisterPhysics(
        new G4RadioactiveDecayPhysics()
    );

    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization());
    runManager->Initialize();

    G4UIExecutive *ui = new G4UIExecutive(argc, argv);


    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
    UImanager->ApplyCommand("/vis/modeling/trajectories/create/drawByParticleID");

    ui->SessionStart();

    return 0;   
}