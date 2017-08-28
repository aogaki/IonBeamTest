TH1D *HisAll;
TH1D *HisPri;
TH2D *HisPar;
TH2D *HisIon;

std::vector<TH1D*> HisParticles;
TCanvas *canv;


void GetInfo()
{
   TFile *file = new TFile("proton.root", "READ");
   HisAll = (TH1D*)file->Get("HisAll");
   HisPri = (TH1D*)file->Get("HisPrimal");
   HisPar = (TH2D*)file->Get("HisParticle");
   HisIon = (TH2D*)file->Get("HisIon");

   HisPar->GetYaxis()->LabelsOption(">");
   HisIon->GetYaxis()->LabelsOption(">");
   
   for(Int_t i = 1; i <= HisPar->GetYaxis()->GetNbins(); i++){
      TString name = HisPar->GetYaxis()->GetBinLabel(i);
      if(name != ""){
         TH1D *his = HisPar->ProjectionX(name, i, i);
         his->SetTitle(name);
         his->SetLineColor(i + 1);
         his->SetLineWidth(2);
         HisParticles.push_back(his);
      }
   }

   for(Int_t i = 1; i <= HisIon->GetYaxis()->GetNbins(); i++){
      TString label = HisIon->GetYaxis()->GetBinLabel(i);
      if(label != "") cout << label << endl;
   }
   
   canv = new TCanvas();
   for(Int_t i = 0; i < HisParticles.size(); i++){
      if(i == 0) HisParticles[i]->Draw();
      else HisParticles[i]->Draw("SAME");
   }
   canv->BuildLegend();
   gStyle->SetOptStat(kFALSE);
   
}
