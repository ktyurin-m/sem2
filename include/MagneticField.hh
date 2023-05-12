#ifndef B5MagneticField_H
#define B5MagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

#include <CLHEP/Units/SystemOfUnits.h>

class G4GenericMessenger;

namespace project
{

/// Magnetic field

class MagneticField : public G4MagneticField
{
  public:
    MagneticField();
    ~MagneticField() override;

    void GetFieldValue(const G4double point[4],double* bField ) const override;

    void SetField(G4double val) { fBy = val; }
    G4double GetField() const { return fBy; }

  private:
    void DefineCommands();

    G4double fBy = 1.0*CLHEP::tesla;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
