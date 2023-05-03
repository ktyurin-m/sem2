
/// \file EventAction.hh
/// \brief Definition of the B2::EventAction class

#ifndef B2EventAction_h
#define B2EventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"

namespace project
{

/// Event action class

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction() override;

    void  BeginOfEventAction(const G4Event* ) override;
    void    EndOfEventAction(const G4Event* ) override;
};

}

#endif
