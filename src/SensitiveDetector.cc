#include "SensitiveDetector.hh"
#include "RootOutput.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"
#include "G4StepStatus.hh"

SensitiveDetector::SensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    if (!step) return false;

    // Count a crossing only on entry into the shell. This avoids recording
    // every subsequent step while a photon traverses the sensitive volume.
    const auto* pre = step->GetPreStepPoint();
    if (pre->GetStepStatus() != fGeomBoundary) return false;

    const auto* track = step->GetTrack();
    if (track->GetDefinition() != G4Gamma::Gamma()) return false;

    const auto* event = G4EventManager::GetEventManager()->GetConstCurrentEvent();
    if (!event) return false;

    const G4ThreeVector position = pre->GetPosition();
    const G4ThreeVector direction = track->GetMomentumDirection();
    RootOutput::Instance()->FillCrossing(
        event->GetEventID(),
        track->GetTrackID(),
        track->GetParentID(),
        track->GetDefinition()->GetPDGEncoding(),
        position.x()/mm, position.y()/mm, position.z()/mm,
        direction.x(), direction.y(), direction.z(),
        track->GetKineticEnergy()/keV,
        track->GetGlobalTime()/ns
    );
    return true;
}
