#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "FTFP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "SimulationConfig.hh"

namespace {
void printHelp() {
    std::cout
        << "Usage: ./bcpet [options]\n"
        << "Options:\n"
        << "  --help                 Show this help message\n"
        << "  -n <integer>           Number of events to simulate (batch mode)\n"
        << "  -tr <x,y,z>            Tumour position in mm (relative to breast centre)\n"
        << "  -ts <a,b,c>            Tumour semi-axes in mm\n"
        << "  --detector-radius <mm> Inner radius of spherical detector (default 100)\n"
        << "  --detector-thickness <mm> Detector shell thickness (default 1)\n"
        << "  --output <file.root>   ROOT output filename (default bcpet.root)\n"
        << "  --gui                  Start interactive visualisation\n";
}

G4ThreeVector parseVector3(const std::string& arg) {
    std::istringstream stream(arg);
    G4double x, y, z;
    char comma1, comma2;
    if (!(stream >> x >> comma1 >> y >> comma2 >> z) ||
        comma1 != ',' || comma2 != ',') {
        throw std::invalid_argument(
            "Expected three comma-separated values, e.g. 10,20,30");
    }
    stream >> std::ws;
    if (!stream.eof()) {
        throw std::invalid_argument("Unexpected characters in vector: " + arg);
    }
    return G4ThreeVector(x * mm, y * mm, z * mm);
}
}

int main(int argc, char** argv) {
    G4int nEvents = -1;
    G4ThreeVector tumourPosition(0., 0., 0.);
    G4ThreeVector tumourSemiAxes(5. * mm, 5. * mm, 5. * mm);
    G4double detectorRadius = 100. * mm;
    G4double detectorThickness = 1. * mm;
    G4String outputFile = "bcpet.root";
    G4bool useGUI = false;

    try {
        for (int i = 1; i < argc; ++i) {
            const std::string arg = argv[i];
            auto requireValue = [&](const char* option) -> std::string {
                if (i + 1 >= argc) {
                    throw std::invalid_argument(std::string("Missing value for ") + option);
                }
                return argv[++i];
            };
            if (arg == "--help") {
                printHelp();
                return 0;
            } else if (arg == "-n") {
                nEvents = std::stoi(requireValue("-n"));
            } else if (arg == "-tr") {
                tumourPosition = parseVector3(requireValue("-tr"));
            } else if (arg == "-ts") {
                tumourSemiAxes = parseVector3(requireValue("-ts"));
            } else if (arg == "--detector-radius") {
                detectorRadius = std::stod(requireValue("--detector-radius")) * mm;
            } else if (arg == "--detector-thickness") {
                detectorThickness = std::stod(requireValue("--detector-thickness")) * mm;
            } else if (arg == "--output") {
                outputFile = requireValue("--output");
            } else if (arg == "--gui") {
                useGUI = true;
            } else {
                throw std::invalid_argument("Unknown argument: " + arg);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Argument error: " << e.what() << "\n";
        printHelp();
        return 1;
    }

    if (nEvents < -1 || detectorRadius <= 0. || detectorThickness <= 0.) {
        std::cerr << "Invalid event count or detector dimensions.\n";
        return 1;
    }

    auto* config = SimulationConfig::Instance();
    config->tumourPosition = tumourPosition;
    config->tumourSemiAxes = tumourSemiAxes;
    config->detectorRadius = detectorRadius;
    config->detectorThickness = detectorThickness;
    config->outputFile = outputFile;

    auto* detector = new DetectorConstruction();
    detector->SetTumourPosition(tumourPosition);
    detector->SetTumourSemiAxes(tumourSemiAxes);

    auto* runManager = new G4RunManager();
    runManager->SetUserInitialization(detector);

    auto* physicsList = new FTFP_BERT;
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization());
    runManager->Initialize();

    if (useGUI) {
        auto* ui = new G4UIExecutive(argc, argv);
        auto* visManager = new G4VisExecutive();
        visManager->Initialize();
        auto* uiManager = G4UImanager::GetUIpointer();
        uiManager->ApplyCommand("/vis/open OGL");
        uiManager->ApplyCommand("/vis/drawVolume");
        uiManager->ApplyCommand("/vis/scene/add/trajectories smooth");
        uiManager->ApplyCommand("/vis/viewer/set/autoRefresh true");
        uiManager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
        uiManager->ApplyCommand("/vis/modeling/trajectories/create/drawByParticleID");
        if (nEvents >= 0) {
            uiManager->ApplyCommand("/run/beamOn " + std::to_string(nEvents));
        }
        ui->SessionStart();
        delete ui;
        delete visManager;
    } else if (nEvents >= 0) {
        G4UImanager::GetUIpointer()->ApplyCommand(
            "/run/beamOn " + std::to_string(nEvents));
    } else {
        std::cerr << "Batch mode requires -n <events>; use --gui for interactive mode.\n";
        delete runManager;
        return 1;
    }

    delete runManager;
    return 0;
}
