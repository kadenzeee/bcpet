
#ifndef DecayTimerAction_h
#define DecayTimerAction_h 1

#include "G4UserStackingAction.hh"
#include "G4Types.hh"

class G4Track;

class DecayTimerAction : public G4UserStackingAction
{
public:
    DecayTimerAction();
    ~DecayTimerAction() override = default;

    G4ClassificationOfNewTrack
    ClassifyNewTrack(const G4Track* track) override; // G4 internal named tracker

    G4double GetDecayTime() const { return decayTicker_; }

private:
    G4double decayTicker_ = 0.0;
    G4bool positronFound_ = false;
};

#endif
