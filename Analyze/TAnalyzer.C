#define TAnalyzer_cxx
// The class definition in TAnalyzer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("TAnalyzer.C")
// root> T->Process("TAnalyzer.C","some options")
// root> T->Process("TAnalyzer.C+")
//

#include "TAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TDatabasePDG.h>

void TAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   fHisAll = nullptr;
   fHisIon = nullptr;
   fHisParticle = nullptr;
   fHisPrimal = nullptr;
}

void TAnalyzer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   InitEleMap();

   Double_t eneMax = 600.;
   Int_t nBins = eneMax * 10;
   
   fHisAll = new TH1D("HisAll",
                      "Energy distribution of all particles",
                      nBins, 0., eneMax);
   fHisAll->SetXTitle("Kinetic energy [MeV]");
   GetOutputList()->Add(fHisAll);

   fHisPrimal = new TH1D("HisPrimal",
                         "Energy distribution of all particles",
                         nBins, 0., eneMax);
   fHisPrimal->SetXTitle("Kinetic energy [MeV]");
   GetOutputList()->Add(fHisPrimal);

   fHisParticle = new TH2D("HisParticle",
                           "Energy distribution of each particle",
                           nBins, 0., eneMax, 10, 0., 0.);
   fHisParticle->SetXTitle("Kinetic energy [MeV]");
   GetOutputList()->Add(fHisParticle);

   fHisIon = new TH2D("HisIon",
                      "Energy distribution of each particle",
                      nBins, 0., eneMax, 10, 0., 0.);
   fHisIon->SetXTitle("Kinetic energy [MeV]");
   GetOutputList()->Add(fHisIon);
}

Bool_t TAnalyzer::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);

   fHisAll->Fill(*KineticEnergy);

   auto particle = TDatabasePDG::Instance()->GetParticle(*PDGCode);
   TString name;
   if(particle){
      name = particle->GetName();
      fHisParticle->Fill(*KineticEnergy, name, 1.);
      //fHisParticle->Fill(*KineticEnergy, *PDGCode, 1.);
   }
   else{
      name = GetIonName(*PDGCode);
      fHisIon->Fill(*KineticEnergy, name, 1.);
      //fHisIon->Fill(*KineticEnergy, *PDGCode, 1.);
   }
   if(*TrackID == 1) fHisPrimal->Fill(*KineticEnergy);
   
   return kTRUE;
}

void TAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void TAnalyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   TFile *file = new TFile("tmp.root", "RECREATE");
   fHisAll->Write();
   fHisPrimal->Write();
   fHisIon->Write();
   fHisParticle->Write();
   file->Close();
}

void TAnalyzer::InitEleMap()
{
   fEleMap[1] = "H";
   fEleMap[2] = "He";
   fEleMap[3] = "Li";
   fEleMap[4] = "Be";
   fEleMap[5] = "B";
   fEleMap[6] = "C";
   fEleMap[7] = "N";
   fEleMap[8] = "O";
   fEleMap[9] = "F";
   fEleMap[10] = "Ne";
   fEleMap[11] = "Na";
   fEleMap[12] = "Mg";
   fEleMap[13] = "Al";
   fEleMap[14] = "Si";
   fEleMap[15] = "P";
   fEleMap[16] = "S";
   fEleMap[17] = "Cl";
   fEleMap[18] = "Ar";
   fEleMap[19] = "K";
   fEleMap[20] = "Ca";
   fEleMap[21] = "Sc";
   fEleMap[22] = "Ti";
   fEleMap[23] = "V";
   fEleMap[24] = "Cr";
   fEleMap[25] = "Mn";
   fEleMap[26] = "Fe";
   fEleMap[27] = "Co";
   fEleMap[28] = "Ni";
   fEleMap[29] = "Cu";
   fEleMap[30] = "Zn";
   fEleMap[31] = "Ga";
   fEleMap[32] = "Ge";
   fEleMap[33] = "As";
   fEleMap[34] = "Se";
   fEleMap[35] = "Br";
   fEleMap[36] = "Kr";
   fEleMap[37] = "Rb";
   fEleMap[38] = "Sr";
   fEleMap[39] = "Y";
   fEleMap[40] = "Zr";
   fEleMap[41] = "Nb";
   fEleMap[42] = "Mo";
   fEleMap[43] = "Tc";
   fEleMap[44] = "Ru";
   fEleMap[45] = "Rh";
   fEleMap[46] = "Pd";
   fEleMap[47] = "Ag";
   fEleMap[48] = "Cd";
   fEleMap[49] = "In";
   fEleMap[50] = "Sn";
   fEleMap[51] = "Sb";
   fEleMap[52] = "Te";
   fEleMap[53] = "I";
   fEleMap[54] = "Xe";
   fEleMap[55] = "Cs";
   fEleMap[56] = "Ba";
   fEleMap[57] = "La";
   fEleMap[58] = "Ce";
   fEleMap[59] = "Pr";
   fEleMap[60] = "Nd";
   fEleMap[61] = "Pm";
   fEleMap[62] = "Sm";
   fEleMap[63] = "Eu";
   fEleMap[64] = "Gd";
   fEleMap[65] = "Tb";
   fEleMap[66] = "Dy";
   fEleMap[67] = "Ho";
   fEleMap[68] = "Er";
   fEleMap[69] = "Tm";
   fEleMap[70] = "Yb";
   fEleMap[71] = "Lu";
   fEleMap[72] = "Hf";
   fEleMap[73] = "Ta";
   fEleMap[74] = "W";
   fEleMap[75] = "Re";
   fEleMap[76] = "Os";
   fEleMap[77] = "Ir";
   fEleMap[78] = "Pt";
   fEleMap[79] = "Au";
   fEleMap[80] = "Hg";
   fEleMap[81] = "Tl";
   fEleMap[82] = "Pb";
   fEleMap[83] = "Bi";
   fEleMap[84] = "Po";
   fEleMap[85] = "At";
   fEleMap[86] = "Rn";
   fEleMap[87] = "Fr";
   fEleMap[88] = "Ra";
   fEleMap[89] = "Ac";
   fEleMap[90] = "Th";
   fEleMap[91] = "Pa";
   fEleMap[92] = "U";
   fEleMap[93] = "Np";
   fEleMap[94] = "Pu";
   fEleMap[95] = "Am";
   fEleMap[96] = "Cm";
   fEleMap[97] = "Bk";
   fEleMap[98] = "Cf";
}

TString TAnalyzer::GetIonName(Int_t pdgCode)
{
   TString code = TString::Itoa(pdgCode, 10);
   Int_t Z = TString(code(3, 3)).Atoi();
   Int_t A = TString(code(6, 3)).Atoi();
   
   return fEleMap[Z] + TString::Itoa(A, 10);
}
