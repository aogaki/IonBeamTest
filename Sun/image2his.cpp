void image2his()
{
   TASImage image("ions.BMP");
   UInt_t yPixels = image.GetHeight();
   UInt_t xPixels = image.GetWidth();
   UInt_t *argb   = image.GetArgbArray();

   TH2D *h = new TH2D("h", "test", xPixels, -0.5, xPixels - 0.5, yPixels, -0.5, yPixels - 0.5);

   for (int row = 0; row < xPixels; ++row) {
      for (int col = 0; col < yPixels; ++col) {
         int index = col * xPixels + row;
         float grey = float(argb[index] & 0xff) / 256;
         UInt_t a = ((argb[index] >> 24) & 0xff);
         UInt_t r = ((argb[index] >> 16) & 0xff);
         UInt_t g = ((argb[index] >> 8) & 0xff);
         UInt_t b = ((argb[index]) & 0xff);
         //h->SetBinContent(row + 1, yPixels - col, b);
         h->SetBinContent(row + 1, yPixels - col, r + g + b);
      }
   }

   h->Draw("colz");

   auto file = new TFile("image.root", "RECREATE");
   h->Write();
   file->Close();
}
