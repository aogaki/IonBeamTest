const Int_t kNoIon = 8;

TH1D *HisPri[kNoIon];

void FitHist(TH1D *his)
{
   TF1 *f1 = new TF1("f1" + TString(his->GetName()), "gaus");
   his->Fit(f1);

   Double_t mean = f1->GetParameter(1);
   Double_t sigma = f1->GetParameter(2);
   Double_t HWHM = sigma * sqrt(2.*log(2.));
   f1->SetRange(mean - HWHM, mean + HWHM);
   his->Fit(f1, "R");
   
   mean = f1->GetParameter(1);
   sigma = f1->GetParameter(2);
   HWHM = sigma * sqrt(2.*log(2.));
   f1->SetRange(mean - HWHM, mean + HWHM);
   his->Fit(f1, "R");

   TAxis *xAxis = his->GetXaxis();
   xAxis->SetRange(xAxis->FindBin(mean - 10.*HWHM), xAxis->FindBin(mean + 10.*HWHM));
   
}

void FitPrimary()
{
   TCanvas *canv = new TCanvas();
   map<TString, Int_t> fA;
   fA["H"] = 1;
   fA["He"] = 4;
   fA["C"] = 12;
   fA["O"] = 16;
   fA["Ne"] = 20;
   fA["Si"] = 28;
   fA["Ca"] = 40;
   fA["Fe"] = 56;
   
   TString ionName[kNoIon] = {"H", "He", "C", "O", "Ne", "Si", "Ca", "Fe"};
   for(Int_t i = 0; i < kNoIon; i++){
      auto file = new TFile(ionName[i] + ".root", "READ");
      HisPri[i] = (TH1D*)file->Get("HisPrimal" + ionName[i] + "1");
      HisPri[i]->SetDirectory(0);
      HisPri[i]->SetTitle(TString::Itoa(fA[ionName[i]], 10) + TString(" GeV"));
      file->Close();
      delete file;
      FitHist(HisPri[i]);
   }

   canv->Print("attenuation.pdf[");
   for(auto &his: HisPri){
      his->Draw();
      canv->Print("attenuation.pdf");
   }
   canv->Print("attenuation.pdf]");
   
}
