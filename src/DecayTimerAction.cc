
#include "DecayTimerAction.hh"

#include "G4Track.hh"
#include "G4Positron.hh"

DecayTimerAction::DecayTimerAction() = default;

G4ClassificationOfNewTrack
DecayTimerAction::ClassifyNewTrack(const G4Track* track)
{
    if (!positronFound_ &&
        track->GetDefinition() == G4Positron::Positron()) {

        decayTicker_ = track->GetGlobalTime();
        positronFound_ = true;
    }

    return fUrgent; // i had to spend so long figuring out what fUrgent was. i still don't know. but everything breaks if this is not here. so like. it stays i guess?
}
