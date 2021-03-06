{
//=========Macro generated from canvas: canvas/canvas
//=========  (Tue May 20 17:25:40 2014) by ROOT version5.34/09
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,776);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(0,0,1,1);
   canvas->SetFillColor(0);
   canvas->SetBorderMode(0);
   canvas->SetBorderSize(2);
   canvas->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: mainPad
   TPad *mainPad = new TPad("mainPad", "",0,0,0.9,1);
   mainPad->Draw();
   mainPad->cd();
   mainPad->Range(-0.6093202,-0.1893016,0.7596044,1.703714);
   mainPad->SetFillColor(0);
   mainPad->SetBorderMode(0);
   mainPad->SetBorderSize(2);
   mainPad->SetGridx();
   mainPad->SetGridy();
   mainPad->SetTickx(1);
   mainPad->SetTicky(1);
   mainPad->SetFrameBorderMode(0);
   mainPad->SetFrameBorderMode(0);
   
   THStack *hStack = new THStack();
   hStack->SetName("hStack");
   hStack->SetTitle("Q(b_{thad}) #times Q(l) [t#bar{t}];Q(b_{thad}) #times Q(l) [t#bar{t}];Probability density");
   Double_t xAxis121[16] = {-0.4724277, -0.2983002, -0.2075116, -0.1406082, -0.08430114, -0.03415296, 0.01257022, 0.05778509, 0.1015518, 0.1458842, 0.1921048, 0.2422705, 0.2989976, 0.366367, 0.4565092, 0.6227119}; 
   
   TH1F *hStack_stack_41 = new TH1F("hStack_stack_41","Q(b_{thad}) #times Q(l) [t#bar{t}]",15, xAxis121);
   hStack_stack_41->SetMinimum(0);
   hStack_stack_41->SetMaximum(1.514412);
   hStack_stack_41->SetDirectory(0);
   hStack_stack_41->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_41->SetLineColor(ci);
   hStack_stack_41->SetLineWidth(2);
   hStack_stack_41->GetXaxis()->SetTitle("Q(b_{thad}) #times Q(l) [t#bar{t}]");
   hStack_stack_41->GetXaxis()->SetLabelFont(42);
   hStack_stack_41->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_41->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_41->GetXaxis()->SetTitleFont(42);
   hStack_stack_41->GetYaxis()->SetTitle("Probability density");
   hStack_stack_41->GetYaxis()->SetLabelFont(42);
   hStack_stack_41->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_41->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_41->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_41->GetYaxis()->SetTitleFont(42);
   hStack_stack_41->GetZaxis()->SetLabelFont(42);
   hStack_stack_41->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_41->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_41->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_41);
   
   Double_t xAxis122[16] = {-0.4724277, -0.2983002, -0.2075116, -0.1406082, -0.08430114, -0.03415296, 0.01257022, 0.05778509, 0.1015518, 0.1458842, 0.1921048, 0.2422705, 0.2989976, 0.366367, 0.4565092, 0.6227119}; 
   
   TH1D *PDFSgn613810__81 = new TH1D("PDFSgn613810__81","",15, xAxis122);
   PDFSgn613810__81->SetBinContent(0,0.1839128);
   PDFSgn613810__81->SetBinContent(1,0.4160978);
   PDFSgn613810__81->SetBinContent(2,0.7289557);
   PDFSgn613810__81->SetBinContent(3,0.9656463);
   PDFSgn613810__81->SetBinContent(4,1.138005);
   PDFSgn613810__81->SetBinContent(5,1.250714);
   PDFSgn613810__81->SetBinContent(6,1.328291);
   PDFSgn613810__81->SetBinContent(7,1.385992);
   PDFSgn613810__81->SetBinContent(8,1.402995);
   PDFSgn613810__81->SetBinContent(9,1.373244);
   PDFSgn613810__81->SetBinContent(10,1.312477);
   PDFSgn613810__81->SetBinContent(11,1.191324);
   PDFSgn613810__81->SetBinContent(12,1.047537);
   PDFSgn613810__81->SetBinContent(13,0.8983783);
   PDFSgn613810__81->SetBinContent(14,0.6857701);
   PDFSgn613810__81->SetBinContent(15,0.3792572);
   PDFSgn613810__81->SetBinContent(16,0.154627);
   PDFSgn613810__81->SetBinError(0,0.00343933);
   PDFSgn613810__81->SetBinError(1,0.005144813);
   PDFSgn613810__81->SetBinError(2,0.009301731);
   PDFSgn613810__81->SetBinError(3,0.01255538);
   PDFSgn613810__81->SetBinError(4,0.01485234);
   PDFSgn613810__81->SetBinError(5,0.01645167);
   PDFSgn613810__81->SetBinError(6,0.01764504);
   PDFSgn613810__81->SetBinError(7,0.01837961);
   PDFSgn613810__81->SetBinError(8,0.01872291);
   PDFSgn613810__81->SetBinError(9,0.01842415);
   PDFSgn613810__81->SetBinError(10,0.01762407);
   PDFSgn613810__81->SetBinError(11,0.01606977);
   PDFSgn613810__81->SetBinError(12,0.01422766);
   PDFSgn613810__81->SetBinError(13,0.01215106);
   PDFSgn613810__81->SetBinError(14,0.009178599);
   PDFSgn613810__81->SetBinError(15,0.005027547);
   PDFSgn613810__81->SetBinError(16,0.003224171);
   PDFSgn613810__81->SetEntries(119361);
   PDFSgn613810__81->SetDirectory(0);
   PDFSgn613810__81->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn613810__81->SetLineColor(ci);
   PDFSgn613810__81->GetXaxis()->SetLabelFont(42);
   PDFSgn613810__81->GetXaxis()->SetLabelSize(0.035);
   PDFSgn613810__81->GetXaxis()->SetTitleSize(0.035);
   PDFSgn613810__81->GetXaxis()->SetTitleFont(42);
   PDFSgn613810__81->GetYaxis()->SetLabelFont(42);
   PDFSgn613810__81->GetYaxis()->SetLabelSize(0.035);
   PDFSgn613810__81->GetYaxis()->SetTitleSize(0.035);
   PDFSgn613810__81->GetYaxis()->SetTitleFont(42);
   PDFSgn613810__81->GetZaxis()->SetLabelFont(42);
   PDFSgn613810__81->GetZaxis()->SetLabelSize(0.035);
   PDFSgn613810__81->GetZaxis()->SetTitleSize(0.035);
   PDFSgn613810__81->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn613810,"hist");
   Double_t xAxis123[16] = {-0.4724277, -0.2983002, -0.2075116, -0.1406082, -0.08430114, -0.03415296, 0.01257022, 0.05778509, 0.1015518, 0.1458842, 0.1921048, 0.2422705, 0.2989976, 0.366367, 0.4565092, 0.6227119}; 
   
   TH1D *PDFBkg613810__82 = new TH1D("PDFBkg613810__82","",15, xAxis123);
   PDFBkg613810__82->SetBinContent(0,0.1155986);
   PDFBkg613810__82->SetBinContent(1,0.3161493);
   PDFBkg613810__82->SetBinContent(2,0.6478994);
   PDFBkg613810__82->SetBinContent(3,0.9082583);
   PDFBkg613810__82->SetBinContent(4,1.072975);
   PDFBkg613810__82->SetBinContent(5,1.261073);
   PDFBkg613810__82->SetBinContent(6,1.352943);
   PDFBkg613810__82->SetBinContent(7,1.386773);
   PDFBkg613810__82->SetBinContent(8,1.442297);
   PDFBkg613810__82->SetBinContent(9,1.439308);
   PDFBkg613810__82->SetBinContent(10,1.435642);
   PDFBkg613810__82->SetBinContent(11,1.323013);
   PDFBkg613810__82->SetBinContent(12,1.18643);
   PDFBkg613810__82->SetBinContent(13,1.01121);
   PDFBkg613810__82->SetBinContent(14,0.7496265);
   PDFBkg613810__82->SetBinContent(15,0.4031373);
   PDFBkg613810__82->SetBinContent(16,0.1564274);
   PDFBkg613810__82->SetBinError(0,0.003262648);
   PDFBkg613810__82->SetBinError(1,0.005174646);
   PDFBkg613810__82->SetBinError(2,0.01017268);
   PDFBkg613810__82->SetBinError(3,0.01394146);
   PDFBkg613810__82->SetBinError(4,0.01649726);
   PDFBkg613810__82->SetBinError(5,0.01912353);
   PDFBkg613810__82->SetBinError(6,0.02038038);
   PDFBkg613810__82->SetBinError(7,0.02082898);
   PDFBkg613810__82->SetBinError(8,0.02149397);
   PDFBkg613810__82->SetBinError(9,0.02145457);
   PDFBkg613810__82->SetBinError(10,0.02110831);
   PDFBkg613810__82->SetBinError(11,0.01942749);
   PDFBkg613810__82->SetBinError(12,0.01731173);
   PDFBkg613810__82->SetBinError(13,0.01468435);
   PDFBkg613810__82->SetBinError(14,0.01093849);
   PDFBkg613810__82->SetBinError(15,0.005904445);
   PDFBkg613810__82->SetBinError(16,0.003673884);
   PDFBkg613810__82->SetEntries(105134);
   PDFBkg613810__82->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg613810__82->SetFillColor(ci);
   PDFBkg613810__82->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg613810__82->SetLineColor(ci);
   PDFBkg613810__82->GetXaxis()->SetLabelFont(42);
   PDFBkg613810__82->GetXaxis()->SetLabelSize(0.035);
   PDFBkg613810__82->GetXaxis()->SetTitleSize(0.035);
   PDFBkg613810__82->GetXaxis()->SetTitleFont(42);
   PDFBkg613810__82->GetYaxis()->SetLabelFont(42);
   PDFBkg613810__82->GetYaxis()->SetLabelSize(0.035);
   PDFBkg613810__82->GetYaxis()->SetTitleSize(0.035);
   PDFBkg613810__82->GetYaxis()->SetTitleFont(42);
   PDFBkg613810__82->GetZaxis()->SetLabelFont(42);
   PDFBkg613810__82->GetZaxis()->SetLabelSize(0.035);
   PDFBkg613810__82->GetZaxis()->SetTitleSize(0.035);
   PDFBkg613810__82->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg613810,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3436789,0.8986269,0.6563211,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Q(b_{thad}) #times Q(l) [t#bar{t}]");
   pt->Draw();
   mainPad->Modified();
   canvas->cd();
   
   TLegend *leg = new TLegend(0.83,0.7,0.98,0.85,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("NULL"," sgn vs bkg ","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFSgn613810"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg613810"," bkg ","f");

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
   leg->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
