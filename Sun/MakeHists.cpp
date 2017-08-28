#include <TASImage.h>
#include <TH2.h>
#include <TH1.h>
#include <TFile.h>


class TImgConverter
{
public:
   TImgConverter();
   ~TImgConverter();

   void MakeData();

   void SaveData();
   
private:
   void ReadImg();
   TH2D *fHisImg;

   void InitHists();
   
   void FillProton();
   TH1D *fHisProton;

   void FillFe();
   TH1D *fHisFe;

   Double_t GetNoPar(Int_t pix);

};

TImgConverter::TImgConverter()
{
   ReadImg();
   InitHists();
}

TImgConverter::~TImgConverter()
{}

void TImgConverter::ReadImg()
{
   TASImage image("ions.BMP");
   UInt_t yPixels = image.GetHeight();
   UInt_t xPixels = image.GetWidth();
   UInt_t *argb   = image.GetArgbArray();

   fHisImg = new TH2D("h", "test", xPixels, -0.5, xPixels - 0.5, yPixels, -0.5, yPixels - 0.5);

   for (int row = 0; row < xPixels; ++row) {
      for (int col = 0; col < yPixels; ++col) {
         int index = col * xPixels + row;
         float grey = float(argb[index] & 0xff) / 256;
         UInt_t a = ((argb[index] >> 24) & 0xff);
         UInt_t r = ((argb[index] >> 16) & 0xff);
         UInt_t g = ((argb[index] >> 8) & 0xff);
         UInt_t b = ((argb[index]) & 0xff);
         fHisImg->SetBinContent(row + 1, yPixels - col, r + g + b);
      }
   }
}

void TImgConverter::InitHists()
{
   fHisProton = new TH1D("HisProton", "Proton energy distribution", 388, 143.5, 531.5);
   //fHisProton->SetXTitle("");

   fHisFe = new TH1D("HisFe", "Fe ion energy distribution", 388, 143.5, 531.5);
}

void TImgConverter::MakeData()
{
   FillProton();
   FillFe();
}

void TImgConverter::FillProton()
{
   for(Int_t bin = 1; bin <= 388; bin++){
      Int_t x = bin + 144;
      Int_t pix = 0;
      for(Int_t y = 662; y >= 102; y--){
         if(fHisImg->GetBinContent(x, y) == 0){
            pix = y - 1;
            break;
         }
      }
      fHisProton->SetBinContent(bin, GetNoPar(pix));
   }
}

void TImgConverter::FillFe()
{
   for(Int_t bin = 1; bin <= 388; bin++){
      Int_t x = bin + 144;
      Int_t pix = 0;
      for(Int_t y = 102; y <= 662; y++){
         if(fHisImg->GetBinContent(x, y) == 0){
            pix = y - 1;
            break;
         }
      }
      fHisFe->SetBinContent(bin, GetNoPar(pix));
   }
}

void TImgConverter::SaveData()
{
   auto file = new TFile("dist.root", "RECREATE");
   fHisProton->Write();
   fHisFe->Write();
   file->Close();
}

Double_t TImgConverter::GetNoPar(Int_t pix)
{
   //constexpr Double_t dy = (673 - 142) / (log10(1.E10) - log10(1.));
   constexpr Double_t dy = (673 - 142) / (10. - 0.);
   Double_t n = pow(10., (pix - 142.) / dy);
   return n;
}


void MakeHists()
{
   TImgConverter *test = new TImgConverter();
   test->MakeData();
   test->SaveData();
}
