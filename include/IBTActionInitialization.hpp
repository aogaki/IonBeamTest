#ifndef IBTActionInitialization_h
#define IBTActionInitialization_h 1

#include <G4VUserActionInitialization.hh>
#include <globals.hh>


class IBTActionInitialization : public G4VUserActionInitialization
{
public:
   IBTActionInitialization(G4bool monoFlag, G4double ene);
   virtual ~IBTActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
   G4bool fMonoFlag;
   G4double fEnergy;
      
};

#endif
