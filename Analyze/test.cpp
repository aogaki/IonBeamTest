void test()
{
   auto file = new TFile("test.root", "READ");
   auto tree = (TTree*)file->Get("IBT");

   Int_t pdgCode;
   tree->SetBranchAddress("PDGCode", &pdgCode);

   const Long64_t kMaxEve = tree->GetEntries();
   for(Int_t iEve = 0; iEve < kMaxEve; iEve++){
      tree->GetEntry(iEve);
      if(pdgCode != 22 &&
         pdgCode != 12 &&
         pdgCode != -12 &&
         pdgCode != 11 &&
         pdgCode != -11 &&
         pdgCode != 2212 &&
         pdgCode != 2112)
      cout << pdgCode << endl;
   }
}

