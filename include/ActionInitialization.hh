
#ifndef B2ActionInitialization_h
#define B2ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

namespace project
{

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
};

}

#endif


