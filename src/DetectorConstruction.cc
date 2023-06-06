
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the B2a::DetectorConstruction class

#include "DetectorConstruction.hh"
#include "TrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "MagneticField.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "G4FieldManager.hh"
#include "G4EqMagElectricField.hh"
using namespace project;

namespace project
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;
G4ThreadLocal MagneticField* DetectorConstruction::magField = 0;

DetectorConstruction::DetectorConstruction()
{
  

  // fNbOfChambers = 5;
  // fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  // delete [] fLogicChamber;
  // delete fStepLimit;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // Material definition

  G4NistManager* nistManager = G4NistManager::Instance();

  // Air defined using NIST Manager
  nistManager->FindOrBuildMaterial("G4_Galactic");

  // Lead defined using NIST Manager
  fTargetMaterial  = nistManager->FindOrBuildMaterial("G4_Pb");

  // Xenon gas defined using NIST Manager
  fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");

  Fe = nistManager->FindOrBuildMaterial("G4_Fe");
  // Print materials
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  G4Material* air  = G4Material::GetMaterial("G4_Galactic");
  
  // Sizes of the principal geometrical components (solids)



  // Definitions of Solids, Logical Volumes, Physical Volumes
  G4double boxsize = 5*m;
  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(boxsize);

  // G4cout << "Computed tolerance = "
  //        << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
  //        << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
                boxsize,boxsize,boxsize); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 air,      //its material
                 "World"); //its name

  //  Must place the World Physical volume unrotated at (0,0,0).
  //
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps

  // Target


  // Tracker



  // Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));

  worldLV      ->SetVisAttributes(boxVisAtt);

  // trackerLV    ->SetVisAttributes(boxVisAtt);

  // Tracker segments





  //detector

  G4Box* solidbox = new G4Box("Box", 0.1*m,0.1*m,10*cm);
  Box = new G4LogicalVolume(solidbox,fTargetMaterial,"Box");
  G4RotationMatrix* rotate = new G4RotationMatrix();
  rotate->rotateY(6.8427734* deg);

  new G4PVPlacement(rotate,
                    G4ThreeVector(-300*mm, 0, 2800*mm),
                    Box,
                    "Box",
                    worldLV,
                    false,
                    0,
                    fCheckOverlaps);
                    

  //~~~~
  //magnit
  G4double lengthM = 1600/2*mm;
  G4double widthM  = 500/2*mm;
  G4double heightM = 280/2*mm;

  G4Box* magnet = new G4Box("Box1", widthM, heightM, lengthM);
  
  Magnet = new G4LogicalVolume(magnet, Fe,"Box1");
  
  new G4PVPlacement(0,
                    G4ThreeVector(0,0,0),
                    Magnet,
                    "Box1",
                    worldLV,
                    false,
                    0,
                    fCheckOverlaps);
                    
  //gap
  G4double lengthG = 1600/2*mm;
  G4double widthG  = 310/2*mm;
  G4double heightG = 20/2*mm;
  G4Box* gap = new G4Box("Box2", widthG,heightG,lengthG + 1*mm);

  Gap = new G4LogicalVolume(gap, air,"Box2");
  
  new G4PVPlacement(0,
                    G4ThreeVector(0,0,0),
                    Gap,
                    "Box2",
                    worldLV,
                    false,
                    2,
                    fCheckOverlaps);


  // Example of User Limits
  //
  // Below is an example of how to set tracking constraints in a given
  // logical volume
  //
  // Sets a max step length in the tracker region, with G4StepLimiter

  G4double maxStep = 2*mm;
  fStepLimit = new G4UserLimits(maxStep);
  worldLV->SetUserLimits(fStepLimit);

  /// Set additional contraints on the track, with G4UserSpecialCuts
  ///
  /// G4double maxLength = 2*trackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
  /// trackerLV->SetUserLimits(new G4UserLimits(maxStep,
  ///                                           maxLength,
  ///                                           maxTime,
  ///                                           minEkin));

  // Always return the physical world

  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "/TrackerChamberSD";
  TrackerSD* aTrackerSD = new TrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
  // Setting aTrackerSD to all logical volumes with the same name
  // of "Chamber_LV".
  SetSensitiveDetector("Box", aTrackerSD, false);

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.

  //MAGNETIC FIELD
  magField = new MagneticField();
  fFieldMgr = new G4FieldManager();
  fFieldMgr->SetDetectorField(magField);
  fFieldMgr->CreateChordFinder(magField);
  
  Gap->SetFieldManager(fFieldMgr,true);

  // Register the field messenger for deleting
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetMaterial(G4String materialName)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetChamberMaterial(G4String materialName)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
