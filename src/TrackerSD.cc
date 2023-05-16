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

  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep==0.) return true;
  TrackerHit* newHit = new TrackerHit();
  auto touchable = aStep->GetPreStepPoint()->GetTouchable();
  auto transform = touchable->GetHistory()->GetTopTransform();
  auto worldPos = aStep->GetPreStepPoint()->GetPosition();
  auto localPos
    = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
  newHit->SetPos (
    localPos
    );
  fHitsCollection->insert( newHit );
  newHit->AddEdep(edep);
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int Hits = fHitsCollection->entries();
  G4cout << "Hits: " << Hits << G4endl;
  if (Hits > 0){
      auto analysisManager = G4AnalysisManager::Instance();
      G4double FullEnergy = 0;
      for (G4int i = 0; i < Hits; i++)
        {
          FullEnergy = FullEnergy + (*fHitsCollection)[i]->GetEdep();
        }
      analysisManager->FillNtupleDColumn(0, (*fHitsCollection)[0]->GetPos().getX());
      analysisManager->FillNtupleDColumn(1, (*fHitsCollection)[0]->GetPos().getY());
      analysisManager->FillNtupleDColumn(2, (*fHitsCollection)[0]->GetPos().getZ());  
      analysisManager->FillNtupleDColumn(3, FullEnergy);
      analysisManager->AddNtupleRow();
  }

   

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

