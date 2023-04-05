/// \file TrackerSD.cc
/// \brief Implementation of the B2::TrackerSD class

#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include "G4UnitsTable.hh"
namespace project
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::TrackerSD(const G4String& name,
                     const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::~TrackerSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection
    = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce

  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TrackerSD::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
  // energy deposit
  if (aStep->GetTrack()->GetTrackID() != 1) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  
  if (edep==0.) return false;

  TrackerHit* newHit = new TrackerHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                                               ->GetCopyNumber());
  newHit->SetEdep(edep);
  auto touchable = aStep->GetPreStepPoint()->GetTouchable();
  newHit->SetPos (touchable->GetHistory()->GetTopTransform().TransformPoint(
    aStep->GetPreStepPoint()->GetPosition())
    );
  
  
  fHitsCollection->insert( newHit );
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  // if ( true ) {
  G4int nofHits = fHitsCollection->entries();
    //  G4cout << G4endl
    //         << "-------->Hits Collection: in this event they are " << nofHits
    //         << " hits in the tracker chambers: " << G4endl;
    //  for ( G4int i=0; i<nofHits; i++ ) {
    //    G4cout << i << " " << (*fHitsCollection)[i]->GetEdep() << G4endl;
    //   };
  // }
  auto analysisManager = G4AnalysisManager::Instance();
  for (G4int i = 0; i < nofHits; i++)
  {
    analysisManager->FillNtupleDColumn(0, (*fHitsCollection)[i]->GetPos().getX()); 
    analysisManager->FillNtupleDColumn(1, (*fHitsCollection)[i]->GetPos().getY());
    analysisManager->FillNtupleDColumn(2, (*fHitsCollection)[i]->GetPos().getZ());  
    analysisManager->FillNtupleDColumn(3, (*fHitsCollection)[i]->GetEdep()); 

    analysisManager->AddNtupleRow();
  }
   

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

