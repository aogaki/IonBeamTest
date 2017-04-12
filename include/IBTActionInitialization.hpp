#ifndef IBTActionInitialization_h
#define IBTActionInitialization_h 1

#include <G4VUserActionInitialization.hh>


class IBTActionInitialization : public G4VUserActionInitialization
{
public:
   IBTActionInitialization();
   virtual ~IBTActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
   
};

#endif
