#ifndef RootOutput_h
#define RootOutput_h 1

#include "G4String.hh"

class TFile;
class TTree;

class RootOutput {
public:
    static RootOutput* Instance();

    void Open(const G4String& filename);
    void Close();
    void FillCrossing(G4int eventID, G4int trackID, G4int parentID, G4int pdg,
                      G4double x, G4double y, G4double z,
                      G4double dx, G4double dy, G4double dz,
                      G4double energyKeV, G4double timeNs);

private:
    RootOutput() = default;
    ~RootOutput();
    RootOutput(const RootOutput&) = delete;
    RootOutput& operator=(const RootOutput&) = delete;

    TFile* file_ = nullptr;
    TTree* crossings_ = nullptr;

    G4int eventID_ = -1;
    G4int trackID_ = -1;
    G4int parentID_ = -1;
    G4int pdg_ = 0;
    G4double x_ = 0., y_ = 0., z_ = 0.;
    G4double dx_ = 0., dy_ = 0., dz_ = 0.;
    G4double energyKeV_ = 0.;
    G4double timeNs_ = 0.;
};

#endif
