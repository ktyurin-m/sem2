
/// \file TrackerSD.hh
/// \brief Definition of the B2::TrackerSD class

#ifndef B2TrackerSD_h
#define B2TrackerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "TrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

namespace project
{

/// Tracker sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non zero
/// energy deposit.

class TrackerSD : public G4VSensitiveDetector
{
  public:
    TrackerSD(const G4String& name,
                const G4String& hitsCollectionName);
    ~TrackerSD() override;

    // methods from base class
    void   Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hitCollection) override;
    void SetTouch(G4bool t){touch = t;}
    G4bool GetTouch(){return touch;};
    void AddEdep(G4double de){Edep += de;};
    void SetEdep(G4double de){Edep = de;};
    G4double GetEdep(){return Edep;};

  private:
    TrackerHitsCollection* fHitsCollection = nullptr;
    G4bool touch = true;
    G4double Edep = 0;
};

}

#endif
