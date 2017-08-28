#include <TFile.h>
#include <TH1.h>


class TRanGen
{
public:
   TRanGen();
   ~TRanGen();

   Double_t GetProtonEne();
   Double_t GetFeEne();

private:
   Double_t fDx;
   Double_t fTh;
   
   TH1D *fHisProton;
   TH1D *fHisFe;
};

TRanGen::TRanGen()
{
   auto file = new TFile("dist.root", "READ");
   fHisProton = (TH1D*)file->Get("HisProton");
   fHisProton->SetDirectory(0);
   fHisFe = (TH1D*)file->Get("HisFe");
   fHisFe->SetDirectory(0);
   file->Close();

   fDx = (487 - 163) / (log10(100.) - log10(0.1));
   fTh = 100.;
}

TRanGen::~TRanGen()
{
   delete fHisProton;
   delete fHisFe;
}

Double_t TRanGen::GetProtonEne()
{
   Double_t ene;
   while(1){
      ene = pow(10., (fHisProton->GetRandom() - 163.) / fDx) * 0.1;
      if(ene <= fTh) break;
   }
   return ene;
}

Double_t TRanGen::GetFeEne()
{
   Double_t ene;
   while(1){
      ene = pow(10., (fHisFe->GetRandom() - 163.) / fDx) * 0.1;
      if(ene <= fTh) break;
   }
   return ene;
}


void RanGen()
{
   auto test = new TRanGen();

   auto his = new TH1D("his", "test", 20000, 0., 200.);
   for(Int_t i = 0; i < 100000000; i++)
      his->Fill(test->GetProtonEne());
   
   delete test;
   
   his->Draw();
}
