#include "RootOutput.hh"

#include "TFile.h"
#include "TTree.h"

#include "G4Exception.hh"

RootOutput* RootOutput::Instance() {
    static RootOutput output;
    return &output;
}

RootOutput::~RootOutput() {
    Close();
}

void RootOutput::Open(const G4String& filename) {
    Close();
    file_ = TFile::Open(filename.c_str(), "RECREATE");
    if (!file_ || file_->IsZombie()) {
        G4Exception("RootOutput::Open", "BCPET_ROOT_001", FatalException,
                    "Could not create ROOT output file.");
        return;
    }

    crossings_ = new TTree("crossings", "Gamma crossings into spherical detector");
    crossings_->Branch("eventID", &eventID_, "eventID/I");
    crossings_->Branch("trackID", &trackID_, "trackID/I");
    crossings_->Branch("parentID", &parentID_, "parentID/I");
    crossings_->Branch("pdg", &pdg_, "pdg/I");
    crossings_->Branch("x_mm", &x_, "x_mm/D");
    crossings_->Branch("y_mm", &y_, "y_mm/D");
    crossings_->Branch("z_mm", &z_, "z_mm/D");
    crossings_->Branch("dx", &dx_, "dx/D");
    crossings_->Branch("dy", &dy_, "dy/D");
    crossings_->Branch("dz", &dz_, "dz/D");
    crossings_->Branch("energy_keV", &energyKeV_, "energy_keV/D");
    crossings_->Branch("time_ns", &timeNs_, "time_ns/D");
}

void RootOutput::Close() {
    if (file_) {
        file_->cd();
        if (crossings_) crossings_->Write();
        file_->Close();
        delete file_;
    }
    file_ = nullptr;
    crossings_ = nullptr;
}

void RootOutput::FillCrossing(G4int eventID, G4int trackID, G4int parentID, G4int pdg,
                              G4double x, G4double y, G4double z,
                              G4double dx, G4double dy, G4double dz,
                              G4double energyKeV, G4double timeNs) {
    if (!crossings_) return;
    eventID_ = eventID;
    trackID_ = trackID;
    parentID_ = parentID;
    pdg_ = pdg;
    x_ = x; y_ = y; z_ = z;
    dx_ = dx; dy_ = dy; dz_ = dz;
    energyKeV_ = energyKeV;
    timeNs_ = timeNs;
    crossings_->Fill();
}
