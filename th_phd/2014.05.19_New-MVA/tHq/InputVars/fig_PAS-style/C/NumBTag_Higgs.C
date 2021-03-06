{
//=========Macro generated from canvas: canvas/canvas
//=========  (Wed Sep 24 11:55:57 2014) by ROOT version5.34/20
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-1.107595,-0.1344877,2.689873,0.900033);
   canvas->SetFillColor(0);
   canvas->SetBorderMode(0);
   canvas->SetBorderSize(2);
   canvas->SetTickx(1);
   canvas->SetTicky(1);
   canvas->SetLeftMargin(0.16);
   canvas->SetRightMargin(0.05);
   canvas->SetTopMargin(0.13);
   canvas->SetBottomMargin(0.13);
   canvas->SetFrameBorderMode(0);
   canvas->SetFrameBorderMode(0);
   
   THStack *hStack = new THStack();
   hStack->SetName("hStack");
   hStack->SetTitle(";#tagged(b_{H});Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(0.7655453);
   Double_t xAxis64[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1F *hStack_stack_22 = new TH1F("hStack_stack_22","",3, xAxis64);
   hStack_stack_22->SetMinimum(0);
   hStack_stack_22->SetMaximum(0.7655453);
   hStack_stack_22->SetDirectory(0);
   hStack_stack_22->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_22->SetLineColor(ci);
   hStack_stack_22->SetLineWidth(2);
   hStack_stack_22->GetXaxis()->SetTitle("#tagged(b_{H})");
   hStack_stack_22->GetXaxis()->SetNdivisions(508);
   hStack_stack_22->GetXaxis()->SetLabelFont(42);
   hStack_stack_22->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_22->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_22->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_22->GetXaxis()->SetTitleFont(42);
   hStack_stack_22->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_22->GetYaxis()->SetNdivisions(508);
   hStack_stack_22->GetYaxis()->SetLabelFont(42);
   hStack_stack_22->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_22->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_22->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_22->GetYaxis()->SetTitleFont(42);
   hStack_stack_22->GetZaxis()->SetNdivisions(508);
   hStack_stack_22->GetZaxis()->SetLabelFont(42);
   hStack_stack_22->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_22->GetZaxis()->SetTitleSize(0.05);
   hStack_stack_22->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_22);
   
   Double_t xAxis65[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFSgn724286__43 = new TH1D("PDFSgn724286__43","",3, xAxis65);
   PDFSgn724286__43->SetBinContent(1,0.002832246);
   PDFSgn724286__43->SetBinContent(2,0.268077);
   PDFSgn724286__43->SetBinContent(3,0.7290907);
   PDFSgn724286__43->SetBinError(1,0.0002481597);
   PDFSgn724286__43->SetBinError(2,0.002321928);
   PDFSgn724286__43->SetBinError(3,0.003728913);
   PDFSgn724286__43->SetEntries(58680);
   PDFSgn724286__43->SetDirectory(0);
   PDFSgn724286__43->SetStats(0);
   PDFSgn724286__43->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn724286__43->SetLineColor(ci);
   PDFSgn724286__43->GetXaxis()->SetNdivisions(508);
   PDFSgn724286__43->GetXaxis()->SetLabelFont(42);
   PDFSgn724286__43->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn724286__43->GetXaxis()->SetTitleSize(0.05);
   PDFSgn724286__43->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn724286__43->GetXaxis()->SetTitleFont(42);
   PDFSgn724286__43->GetYaxis()->SetNdivisions(508);
   PDFSgn724286__43->GetYaxis()->SetLabelFont(42);
   PDFSgn724286__43->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn724286__43->GetYaxis()->SetTitleSize(0.05);
   PDFSgn724286__43->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn724286__43->GetYaxis()->SetTitleFont(42);
   PDFSgn724286__43->GetZaxis()->SetNdivisions(508);
   PDFSgn724286__43->GetZaxis()->SetLabelFont(42);
   PDFSgn724286__43->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn724286__43->GetZaxis()->SetTitleSize(0.05);
   PDFSgn724286__43->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn724286,"hist");
   Double_t xAxis66[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFBkg724286__44 = new TH1D("PDFBkg724286__44","",3, xAxis66);
   PDFBkg724286__44->SetBinContent(1,0.07044354);
   PDFBkg724286__44->SetBinContent(2,0.4461847);
   PDFBkg724286__44->SetBinContent(3,0.4833718);
   PDFBkg724286__44->SetBinError(1,0.001180266);
   PDFBkg724286__44->SetBinError(2,0.002956183);
   PDFBkg724286__44->SetBinError(3,0.00303738);
   PDFBkg724286__44->SetEntries(58680);
   PDFBkg724286__44->SetDirectory(0);
   PDFBkg724286__44->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg724286__44->SetFillColor(ci);
   PDFBkg724286__44->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg724286__44->SetLineColor(ci);
   PDFBkg724286__44->GetXaxis()->SetNdivisions(508);
   PDFBkg724286__44->GetXaxis()->SetLabelFont(42);
   PDFBkg724286__44->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg724286__44->GetXaxis()->SetTitleSize(0.05);
   PDFBkg724286__44->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg724286__44->GetXaxis()->SetTitleFont(42);
   PDFBkg724286__44->GetYaxis()->SetNdivisions(508);
   PDFBkg724286__44->GetYaxis()->SetLabelFont(42);
   PDFBkg724286__44->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg724286__44->GetYaxis()->SetTitleSize(0.05);
   PDFBkg724286__44->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg724286__44->GetYaxis()->SetTitleFont(42);
   PDFBkg724286__44->GetZaxis()->SetNdivisions(508);
   PDFBkg724286__44->GetZaxis()->SetLabelFont(42);
   PDFBkg724286__44->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg724286__44->GetZaxis()->SetTitleSize(0.05);
   PDFBkg724286__44->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg724286,"hist");
   hStack->Draw("nostack");
   TLatex *   tex = new TLatex(0.2,0.89,"#scale[1.2]{#font[62]{CMS}} #font[52]{Simulation Preliminary}");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.95,0.89,"8 TeV");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
   
   TLegend *leg = new TLegend(0.19,0.67,0.38,0.84,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("NULL","Interpretation","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFSgn724286"," correct ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFBkg724286"," wrong ","f");

   ci = TColor::GetColor("#ff0000");
   entry->SetFillColor(ci);
   entry->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
