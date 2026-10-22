#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4TouchableHistory;

class SensitiveDetector : public G4VSensitiveDetector {
public:
    explicit SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override = default;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
};

#endif