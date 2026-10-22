#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4SystemOfUnits.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;

    void SetTumourPosition(const G4ThreeVector& position);
    G4ThreeVector GetTumourPosition() const;

    void SetTumourSemiAxes(const G4ThreeVector& semiAxes);
    G4ThreeVector GetTumourSemiAxes() const;

  private:
    // Global magnetic field messenger
    static G4ThreadLocal G4GlobalMagFieldMessenger* m_magneticFieldMessenger;

    G4ThreeVector tumourPosition; 
    G4ThreeVector tumourSemiAxes;
    

};

#endif