#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace B5
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  MagneticField::MagneticField()
  {
    // define commands for this class
    DefineCommands();
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  MagneticField::~MagneticField()
  {
    delete fMessenger;
  }

  void MagneticField::GetFieldValue(const G4double[4], double *bField) const
  {
    bField[0] = 0.;
    bField[1] = fBy;
    bField[2] = 0.;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void MagneticField::DefineCommands()
  {
    // Define /B5/field command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this,
                                        "/B5/field/",
                                        "Field control");

    // fieldValue command
    auto &valueCmd = fMessenger->DeclareMethodWithUnit("value", "tesla",
                                                       &MagneticField::SetField,
                                                       "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
