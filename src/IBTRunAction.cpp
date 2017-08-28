#include <g4root.hh>

#include "IBTRunAction.hpp"


IBTRunAction::IBTRunAction()
   : G4UserRunAction()
{
   auto anaMan = G4AnalysisManager::Instance();
   //anaMan->SetNtupleMerging(true);
   anaMan->SetVerboseLevel(1);
   G4String fileName = "result";
   anaMan->SetFileName(fileName);

   // Particle information, when exit
   anaMan->CreateNtuple("IBT", "Ion beam test");
   // anaMan->CreateNtupleIColumn("EventID");
   anaMan->CreateNtupleIColumn("TrackID");
   
   anaMan->CreateNtupleDColumn("KineticEnergy");
   
   //anaMan->CreateNtupleDColumn("x");
   //anaMan->CreateNtupleDColumn("y");
   //anaMan->CreateNtupleDColumn("z");

   //anaMan->CreateNtupleDColumn("Px");
   //anaMan->CreateNtupleDColumn("Py");
   //anaMan->CreateNtupleDColumn("Pz");
   
   anaMan->CreateNtupleIColumn("PDGCode");


   anaMan->FinishNtuple();

   // Init parameters
   //anaMan->CreateNtuple("InitPar", "Initial Parameters");
   //anaMan->CreateNtupleIColumn("EventID");
   //anaMan->CreateNtupleDColumn("KineticEnergy");

   anaMan->FinishNtuple();

}

IBTRunAction::~IBTRunAction()
{
   delete G4AnalysisManager::Instance();
}

void IBTRunAction::BeginOfRunAction(const G4Run *)
{
   auto analysisManager = G4AnalysisManager::Instance();
   analysisManager->OpenFile();
}

void IBTRunAction::EndOfRunAction(const G4Run *)
{
   auto anaMan = G4AnalysisManager::Instance();
   anaMan->Write();
   anaMan->CloseFile();
}

