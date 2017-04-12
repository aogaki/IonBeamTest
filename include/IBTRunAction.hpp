#ifndef IBTRunAction_h
#define IBTRunAction_h 1

#include <G4UserRunAction.hh>
#include <G4Run.hh>


class IBTRunAction: public G4UserRunAction
{
public:
   IBTRunAction();
   virtual ~IBTRunAction();

   virtual void BeginOfRunAction(const G4Run *);
   virtual void EndOfRunAction(const G4Run *);

private:

};

#endif
