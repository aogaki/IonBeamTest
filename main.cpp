#include <random>

#include <Randomize.hh>

#ifdef G4MULTITHREADED
#include <G4MTRunManager.hh>
#include <G4Threading.hh>
#else
#include <G4RunManager.hh>
#endif

#include <G4UImanager.hh>
#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#include <G4TrajectoryParticleFilter.hh>
#endif
#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#endif

#include <QGSP_BIC_HP.hh>

#include "IBTDetectorConstruction.hpp"
#include "IBTActionInitialization.hpp"
#include "IBTPhysicsList.hpp"


int main(int argc, char **argv)
{
   G4String macro = "";
   G4bool monoFlag = false;
   G4double ene = 0.;
   for(G4int i = 1; i < argc; i++) {
      if(G4String(argv[i]) == "-m"){
         if(++i < argc) macro = argv[i];
         else G4cout << "eneter macro file name!" << G4endl;
      }
      else if(G4String(argv[i]) == "-e"){
         monoFlag = true;
         if(++i < argc) ene = std::stod(argv[i]);
         else G4cout << "eneter kinetic energy!" << G4endl;
      }
   }
   
   // Choose the Random engine
   // Need both?
   std::random_device rndSeed; // Use C++11!
   CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine(rndSeed()));
   G4Random::setTheEngine(new CLHEP::MTwistEngine(rndSeed()));

   // Construct the default run manager
#ifdef G4MULTITHREADED
   G4MTRunManager *runManager = new G4MTRunManager();
   runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
#else
   G4RunManager *runManager = new G4RunManager();
#endif

   // Detector construction
   runManager->SetUserInitialization(new IBTDetectorConstruction());

   // Physics list
   //G4VModularPhysicsList *physicsList = new QGSP_BIC_HP();
   G4VModularPhysicsList *physicsList = new IBTPhysicsList();
   physicsList->SetVerboseLevel(0);
   runManager->SetUserInitialization(physicsList);

   // Primary generator action and User action intialization
   runManager->SetUserInitialization(new IBTActionInitialization(monoFlag, ene));

   // Initialize G4 kernel
   //
   runManager->Initialize();

#ifdef G4VIS_USE
   // Initialize visualization
   G4VisManager *visManager = new G4VisExecutive();
   visManager->Initialize();
#endif

   // Get the pointer to the User Interface manager
   G4UImanager *UImanager = G4UImanager::GetUIpointer();

   if (macro != "") {
      // batch mode
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command + macro);
   } else {
      // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
      UImanager->ApplyCommand("/control/execute init.mac");
#endif
      if (ui->IsGUI()) {
         UImanager->ApplyCommand("/control/execute icons.mac");
      }
      ui->SessionStart();
      delete ui;
#endif
   }

#ifdef G4VIS_USE
   delete visManager;
#endif

   delete runManager;

   return 0;
}
