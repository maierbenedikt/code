{
//=========Macro generated from canvas: canvas/canvas
//=========  (Sun Sep 27 19:30:49 2015) by ROOT version5.34/32
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-0.8370852,-0.2596263,0.632847,1.737499);
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
   hStack->SetTitle(";Q(b_{t}) #times Q(l);Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(1.477873);
   Double_t xAxis37[16] = {-0.6018961, -0.4322744, -0.3409955, -0.2709977, -0.2113121, -0.1589332, -0.1099935, -0.06408846, -0.01812453, 0.02822646, 0.07731841, 0.1294035, 0.1890808, 0.2610315, 0.3606648, 0.5593504}; 
   
   TH1F *hStack_stack_13 = new TH1F("hStack_stack_13","",15, xAxis37);
   hStack_stack_13->SetMinimum(0);
   hStack_stack_13->SetMaximum(1.477873);
   hStack_stack_13->SetDirectory(0);
   hStack_stack_13->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_13->SetLineColor(ci);
   hStack_stack_13->SetLineWidth(2);
   hStack_stack_13->GetXaxis()->SetTitle("Q(b_{t}) #times Q(l)");
   hStack_stack_13->GetXaxis()->SetNdivisions(508);
   hStack_stack_13->GetXaxis()->SetLabelFont(42);
   hStack_stack_13->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_13->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_13->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_13->GetXaxis()->SetTitleFont(42);
   hStack_stack_13->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_13->GetYaxis()->SetNdivisions(508);
   hStack_stack_13->GetYaxis()->SetLabelFont(42);
   hStack_stack_13->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_13->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_13->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_13->GetYaxis()->SetTitleFont(42);
   hStack_stack_13->GetZaxis()->SetNdivisions(508);
   hStack_stack_13->GetZaxis()->SetLabelFont(42);
   hStack_stack_13->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_13->GetZaxis()->SetTitleSize(0.05);
   hStack_stack_13->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_13);
   
   Double_t xAxis38[16] = {-0.6018961, -0.4322744, -0.3409955, -0.2709977, -0.2113121, -0.1589332, -0.1099935, -0.06408846, -0.01812453, 0.02822646, 0.07731841, 0.1294035, 0.1890808, 0.2610315, 0.3606648, 0.5593504}; 
   
   TH1D *PDFSgn66601__25 = new TH1D("PDFSgn66601__25","",15, xAxis38);
   PDFSgn66601__25->SetBinContent(0,0.1748312);
   PDFSgn66601__25->SetBinContent(1,0.450828);
   PDFSgn66601__25->SetBinContent(2,0.7949016);
   PDFSgn66601__25->SetBinContent(3,1.015697);
   PDFSgn66601__25->SetBinContent(4,1.186512);
   PDFSgn66601__25->SetBinContent(5,1.300352);
   PDFSgn66601__25->SetBinContent(6,1.404026);
   PDFSgn66601__25->SetBinContent(7,1.397977);
   PDFSgn66601__25->SetBinContent(8,1.407498);
   PDFSgn66601__25->SetBinContent(9,1.339968);
   PDFSgn66601__25->SetBinContent(10,1.258874);
   PDFSgn66601__25->SetBinContent(11,1.15396);
   PDFSgn66601__25->SetBinContent(12,0.9758355);
   PDFSgn66601__25->SetBinContent(13,0.7963175);
   PDFSgn66601__25->SetBinContent(14,0.532919);
   PDFSgn66601__25->SetBinContent(15,0.2359755);
   PDFSgn66601__25->SetBinContent(16,0.07139384);
   PDFSgn66601__25->SetBinError(0,0.004549667);
   PDFSgn66601__25->SetBinError(1,0.007227991);
   PDFSgn66601__25->SetBinError(2,0.01297897);
   PDFSgn66601__25->SetBinError(3,0.01677783);
   PDFSgn66601__25->SetBinError(4,0.01962352);
   PDFSgn66601__25->SetBinError(5,0.02189966);
   PDFSgn66601__25->SetBinError(6,0.02354246);
   PDFSgn66601__25->SetBinError(7,0.02420789);
   PDFSgn66601__25->SetBinError(8,0.02418855);
   PDFSgn66601__25->SetBinError(9,0.02356769);
   PDFSgn66601__25->SetBinError(10,0.02212219);
   PDFSgn66601__25->SetBinError(11,0.02058338);
   PDFSgn66601__25->SetBinError(12,0.01778215);
   PDFSgn66601__25->SetBinError(13,0.01468905);
   PDFSgn66601__25->SetBinError(14,0.0101949);
   PDFSgn66601__25->SetBinError(15,0.004820056);
   PDFSgn66601__25->SetBinError(16,0.002644305);
   PDFSgn66601__25->SetEntries(58680);
   PDFSgn66601__25->SetDirectory(0);
   PDFSgn66601__25->SetStats(0);
   PDFSgn66601__25->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn66601__25->SetLineColor(ci);
   PDFSgn66601__25->GetXaxis()->SetNdivisions(508);
   PDFSgn66601__25->GetXaxis()->SetLabelFont(42);
   PDFSgn66601__25->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn66601__25->GetXaxis()->SetTitleSize(0.05);
   PDFSgn66601__25->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn66601__25->GetXaxis()->SetTitleFont(42);
   PDFSgn66601__25->GetYaxis()->SetNdivisions(508);
   PDFSgn66601__25->GetYaxis()->SetLabelFont(42);
   PDFSgn66601__25->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn66601__25->GetYaxis()->SetTitleSize(0.05);
   PDFSgn66601__25->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn66601__25->GetYaxis()->SetTitleFont(42);
   PDFSgn66601__25->GetZaxis()->SetNdivisions(508);
   PDFSgn66601__25->GetZaxis()->SetLabelFont(42);
   PDFSgn66601__25->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn66601__25->GetZaxis()->SetTitleSize(0.05);
   PDFSgn66601__25->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn66601,"hist");
   Double_t xAxis39[16] = {-0.6018961, -0.4322744, -0.3409955, -0.2709977, -0.2113121, -0.1589332, -0.1099935, -0.06408846, -0.01812453, 0.02822646, 0.07731841, 0.1294035, 0.1890808, 0.2610315, 0.3606648, 0.5593504}; 
   
   TH1D *PDFBkg66601__26 = new TH1D("PDFBkg66601__26","",15, xAxis39);
   PDFBkg66601__26->SetBinContent(0,0.1354279);
   PDFBkg66601__26->SetBinContent(1,0.3196479);
   PDFBkg66601__26->SetBinContent(2,0.5986693);
   PDFBkg66601__26->SetBinContent(3,0.7948219);
   PDFBkg66601__26->SetBinContent(4,0.928693);
   PDFBkg66601__26->SetBinContent(5,1.093495);
   PDFBkg66601__26->SetBinContent(6,1.195951);
   PDFBkg66601__26->SetBinContent(7,1.308969);
   PDFBkg66601__26->SetBinContent(8,1.336461);
   PDFBkg66601__26->SetBinContent(9,1.329552);
   PDFBkg66601__26->SetBinContent(10,1.304947);
   PDFBkg66601__26->SetBinContent(11,1.243402);
   PDFBkg66601__26->SetBinContent(12,1.115096);
   PDFBkg66601__26->SetBinContent(13,0.9963094);
   PDFBkg66601__26->SetBinContent(14,0.7367848);
   PDFBkg66601__26->SetBinContent(15,0.4225347);
   PDFBkg66601__26->SetBinContent(16,0.1697814);
   PDFBkg66601__26->SetBinError(0,0.003963294);
   PDFBkg66601__26->SetBinError(1,0.006078129);
   PDFBkg66601__26->SetBinError(2,0.0113042);
   PDFBkg66601__26->SetBinError(3,0.01483268);
   PDFBkg66601__26->SetBinError(4,0.01733316);
   PDFBkg66601__26->SetBinError(5,0.02010036);
   PDFBkg66601__26->SetBinError(6,0.02164423);
   PDFBkg66601__26->SetBinError(7,0.02341636);
   PDFBkg66601__26->SetBinError(8,0.0236851);
   PDFBkg66601__26->SetBinError(9,0.02351808);
   PDFBkg66601__26->SetBinError(10,0.02253907);
   PDFBkg66601__26->SetBinError(11,0.02148835);
   PDFBkg66601__26->SetBinError(12,0.01897909);
   PDFBkg66601__26->SetBinError(13,0.01633642);
   PDFBkg66601__26->SetBinError(14,0.01194918);
   PDFBkg66601__26->SetBinError(15,0.006446481);
   PDFBkg66601__26->SetBinError(16,0.004123466);
   PDFBkg66601__26->SetEntries(58680);
   PDFBkg66601__26->SetDirectory(0);
   PDFBkg66601__26->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg66601__26->SetFillColor(ci);
   PDFBkg66601__26->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg66601__26->SetLineColor(ci);
   PDFBkg66601__26->GetXaxis()->SetNdivisions(508);
   PDFBkg66601__26->GetXaxis()->SetLabelFont(42);
   PDFBkg66601__26->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg66601__26->GetXaxis()->SetTitleSize(0.05);
   PDFBkg66601__26->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg66601__26->GetXaxis()->SetTitleFont(42);
   PDFBkg66601__26->GetYaxis()->SetNdivisions(508);
   PDFBkg66601__26->GetYaxis()->SetLabelFont(42);
   PDFBkg66601__26->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg66601__26->GetYaxis()->SetTitleSize(0.05);
   PDFBkg66601__26->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg66601__26->GetYaxis()->SetTitleFont(42);
   PDFBkg66601__26->GetZaxis()->SetNdivisions(508);
   PDFBkg66601__26->GetZaxis()->SetLabelFont(42);
   PDFBkg66601__26->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg66601__26->GetZaxis()->SetTitleSize(0.05);
   PDFBkg66601__26->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg66601,"hist");
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
   
   TLegend *leg = new TLegend(0.73,0.67,0.92,0.84,NULL,"brNDC");
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
   entry=leg->AddEntry("PDFSgn66601"," correct ","f");
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
   entry=leg->AddEntry("PDFBkg66601"," wrong ","f");

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
      tex = new TLatex(0.96,0.13,"Underflow: #color[4]{2%} / #color[2]{2%},  Overflow: #color[4]{1%} / #color[2]{3%}");
tex->SetNDC();
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
