////////////////////////////////////////////////////////////////////////////////
//
// th_lepscales_x
// -----------------
//
//            02/09/2014 Benedikt Maier <benedikt.maier@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"
#include <assert.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper

#define MU_ID_SYS 0.005
#define MU_ISO_SYS 0.002
#define MU_TRIG_SYS 0.002


TH1F* h_muon_id;
TH1F* h_muon_iso;
TH1F* h_muon_trigger;
void muonTrackerScaleFactor(float eta, float* eff, float* err);
void muonIDScaleFactor(float eta, float* eff, float* err);
void muonIsoScaleFactor(float eta, float* eff, float* err);
void muonTriggerScaleFactor(float eta, float* eff, float* err);
void electronIDAndIsoScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down);
void electronTriggerScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down);
void readHistoFromGraph(TGraphAsymmErrors* graph, TH1F** h, TString name);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  //
  // evaluate command line options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs      = cl.getVector<string>("inputs");
  string         treename    = cl.getValue<string> ("treename",  "t");
  string         datapath    = cl.getValue<string> ("datapath",  "/");
  string         mode_as_str = cl.getValue<string> ("mode","dijetpt");
  int            shift       = cl.getValue<bool>   ("shift",       0);
  
  if (!cl.check()) return 0;
  cl.print();

  
  //
  // loop over all input files
  //
  for (unsigned iinput=0;iinput<inputs.size();iinput++) {
  
    // open input file
    string input = inputs[iinput]; //input = datapath + "/" + input;
    TFile* ifile = new TFile(input.c_str(),"READ");
    if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

    // create temporary output file
    size_t pos = input.find(".root");
    string output = input.substr(0,pos);
    output = output + ".lsf.root";
    TFile* ofile = new TFile(output.c_str(),"CREATE");
    if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
    
    // loop over input directories
    TIter next(ifile->GetListOfKeys());
    TKey* key(0);
    while ((key=(TKey*)next())) {

      if (strcmp(key->GetClassName(),"TDirectoryFile")!=0) continue;
      
      // read input directory
      TDirectory* idir    = (TDirectory*)key->ReadObj();
      string      dirname = idir->GetName();
      cout<<input<<"/"<<dirname<<":"<<endl;
      
      // create output directory
      TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
      if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
      
      // open input tree
      TTree* itree = (TTree*)idir->Get(treename.c_str());
      if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}
      
      // check wether relevant branches are present
      if (0==itree->FindBranch("nlepw")){ cout<<"no W->lnu branches stored!"<<endl; return 0; }

      // don't copy branches if they already exist
      if (0!=itree->FindBranch("weight_lsf"))      itree->SetBranchStatus("weight_lsf",     0);
      
 
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

      
      // create H->bb branches in output tree
       
      Float_t weight_lsf;
      

      TBranch* b_weight_lsf        =otree->Branch("weight_lsf",      &weight_lsf,           "weight_lsf/F");
      

      // setup input branches needed to fill H->bb branches
      
      Int_t   njt;             itree->SetBranchAddress("njt",          &njt);
      Float_t jtpt[140];       itree->SetBranchAddress("jtpt",         jtpt);
      Float_t jteta[140];      itree->SetBranchAddress("jteta",       jteta);
      Int_t   nmu;             itree->SetBranchAddress("nmu",          &nmu);
      Float_t mueta[110];      itree->SetBranchAddress("mueta",       mueta);
      Float_t mupt[110];       itree->SetBranchAddress("mupt",         mupt);
      Float_t mucharge[110];   itree->SetBranchAddress("mucharge", mucharge);
      Int_t   nel;             itree->SetBranchAddress("nel",          &nel);
      Float_t eleta[110];      itree->SetBranchAddress("eleta",       eleta);
      Float_t elpt[110];       itree->SetBranchAddress("elpt",         elpt);
      Float_t elcharge[110];   itree->SetBranchAddress("elcharge", elcharge);


      // declare files and fill histograms

      string rootfile_dir = "/portal/ekpcms5/home/bmaier/thanalysis/STANDALONE/THAnalysis/";

      TFile mu_id_file((rootfile_dir+"mu_id_eff.root").c_str());
      TGraphAsymmErrors* g_muon_id = (TGraphAsymmErrors*) mu_id_file.Get("DATA_over_MC_Tight_eta_pt20-500");
      readHistoFromGraph(g_muon_id, &h_muon_id, "h_muon_id");
      
      TFile mu_iso_file((rootfile_dir + "/mu_iso_eff.root").c_str());
      TGraphAsymmErrors* g_muon_iso = (TGraphAsymmErrors*) mu_iso_file.Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_eta_pt20-500");
      readHistoFromGraph(g_muon_iso, &h_muon_iso, "h_muon_iso");
      
      TFile mu_trigger_file((rootfile_dir + "/mu_trigger_eff.root").c_str());
      TGraphAsymmErrors* g_muon_trigger = (TGraphAsymmErrors*) mu_trigger_file.Get("IsoMu24_eta2p1_DATA_over_MC_TightID_ETA_pt25-500_2012ABCD");
      readHistoFromGraph(g_muon_trigger, &h_muon_trigger, "h_muon_trigger");

      // loop over all events and fill branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

	itree->GetEntry(ievt);
	
	weight_lsf = 1.;

	float eta = -99;
	float abseta = fabs(eta);
	float pt = -99;
	float charge = -99; 
	
	float max_reweight_eta = 0;  // Up to which eta the systematic reweighting should vary.
	// Depends on the lepton flavour.


	// these three are to be calculated now
	float overall_eff = 0;
	float overall_abserr_up = 0;
	float overall_abserr_down = 0;


	if(nmu)
	  {
	    max_reweight_eta = 2.1;
	    
	    eta = mueta[0];
	    abseta = fabs(eta);
	    pt = mupt[0];
	    charge = mucharge[0]; 
	
	    float tracker_eff, tracker_err;
	    muonTrackerScaleFactor(eta, &tracker_eff, &tracker_err);
	    
	    float id_eff, id_err;
	    muonIDScaleFactor(eta, &id_eff, &id_err);
	    
	    float iso_eff, iso_err;
	    muonIsoScaleFactor(eta, &iso_eff, &iso_err);
	    
	    float trigger_eff, trigger_err;
	    muonTriggerScaleFactor(eta, &trigger_eff, &trigger_err);
	    

	    // calculate mean value of overall efficiency
	    overall_eff = tracker_eff * id_eff * iso_eff * trigger_eff;
	    
	    // calculate relative errors
	    const float tracker_relerr = tracker_err / tracker_eff;
	    const float id_relerr = sqrt(pow(id_err / id_eff, 2) + pow(MU_ID_SYS, 2));
	    const float iso_relerr = sqrt(pow(iso_err / iso_eff, 2) + pow(MU_ISO_SYS, 2));
	    const float trigger_relerr = sqrt(pow(trigger_err / trigger_eff, 2) + pow(MU_TRIG_SYS, 2));
	    
	    // relative uncertainty on overall efficiency
	    const float overall_relerr = sqrt( pow(tracker_relerr, 2.) + pow(id_relerr, 2.) +
					       pow(iso_relerr, 2.) + pow(trigger_relerr, 2.) );
	    
	    // absolute uncertainty on overall efficiency
	    overall_abserr_up = overall_abserr_down = overall_relerr * overall_eff;
	  }
	else //electron
	  {
	    max_reweight_eta = 2.5;
	    
	    eta = eleta[0];
	    abseta = fabs(eta);
	    pt = elpt[0];
	    charge = elcharge[0];
	    
	    // call functions to get us efficiency and uncertainty for the different
	    // tag-and-probe steps
	    float id_eff, id_errup, id_errdown;
	    electronIDAndIsoScaleFactor(eta, pt, &id_eff, &id_errup, &id_errdown);
	    
	    float trigger_eff, trigger_errup, trigger_errdown;
	    electronTriggerScaleFactor(eta, pt, &trigger_eff, &trigger_errup, &trigger_errdown);
	    
	    // calculate mean value of overall efficiency
	    overall_eff = id_eff * trigger_eff;

	    // calculate absolute errors on efficiency
	    // up
	    {
	      // calculate relative errors
	      const float id_relerr = id_errup / id_eff;
	      const float trigger_relerr = trigger_errup / trigger_eff;
	      
	      // relative uncertainty on overall efficiency
	      const float overall_relerr = sqrt( pow(id_relerr, 2.) + pow(trigger_relerr, 2.) );
	      
	      // absolute uncertainty on overall efficiency
	      overall_abserr_up = overall_relerr * overall_eff;
	    }

	    // down
	    {
	      // calculate relative errors
	      const float id_relerr = id_errdown / id_eff;
	      const float trigger_relerr = trigger_errdown / trigger_eff;
	      
	      // relative uncertainty on overall efficiency
	      const float overall_relerr = sqrt( pow(id_relerr, 2.) + pow(trigger_relerr, 2.) );
	      
	      // absolute uncertainty on overall efficiency
	      overall_abserr_down = overall_relerr * overall_eff;
	    }
	  }

	

    if(shift == 0)
	      weight_lsf = overall_eff;
	    else // do systematic shift
	      {
		const float delta = overall_abserr_up + overall_abserr_down; // magnitude of variation
		const float low_eff  = overall_eff - overall_abserr_down; // complete down-variation
		const float high_eff = overall_eff + overall_abserr_up;   // complete up-variation

		if(shift > 0)
		  {
		    if(charge > 0)
		      weight_lsf =  low_eff + delta * (abseta/max_reweight_eta);
		    else
		      weight_lsf = high_eff - delta * (abseta/max_reweight_eta);
		  }
		else
		  {
		    if(charge < 0)
		      weight_lsf =  low_eff + delta * (abseta/max_reweight_eta);
		    else
		      weight_lsf = high_eff - delta * (abseta/max_reweight_eta);
		  }
	      }
	
	b_weight_lsf           ->Fill();
	
      } // ievt
      
    } // idir
    
    cout<<"Save extended tree to "<<input<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv " + output + " " + input;
    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;
    
  } // ifile
  

  return 0;
}

//
//
//
void muonTrackerScaleFactor(float eta, float* eff, float* err)
{
  assert( fabs(eta) <= 2.4 );

  if(eta <= -2.1) {*eff = 98.69/100; *err = 0.07/100; return;}
  if(eta <= -1.6) {*eff = 99.48/100; *err = 0.02/100; return;}
  if(eta <= -1.2) {*eff = 99.67/100; *err = 0.02/100; return;}
  if(eta <= -0.9) {*eff = 99.74/100; *err = 0.02/100; return;}
  if(eta <= -0.6) {*eff = 99.80/100; *err = 0.01/100; return;}
  if(eta <= -0.3) {*eff = 99.80/100; *err = 0.01/100; return;}
  if(eta <= -0.2) {*eff = 99.72/100; *err = 0.02/100; return;}
  if(eta <= +0.2) {*eff = 99.63/100; *err = 0.01/100; return;}
  if(eta <= +0.3) {*eff = 99.78/100; *err = 0.02/100; return;}
  if(eta <= +0.6) {*eff = 99.77/100; *err = 0.01/100; return;}
  if(eta <= +0.9) {*eff = 99.76/100; *err = 0.01/100; return;}
  if(eta <= +1.2) {*eff = 99.68/100; *err = 0.02/100; return;}
  if(eta <= +1.6) {*eff = 99.59/100; *err = 0.03/100; return;}
  if(eta <= +2.1) {*eff = 99.70/100; *err = 0.02/100; return;}
  if(eta <= +2.4) {*eff = 98.36/100; *err = 0.08/100; return;}
}

// HINT This could also be done pt-dependent using other histos
void muonIDScaleFactor(float eta, float* eff, float* err)
{
  const int bin_index = h_muon_id->FindBin(eta);
  *eff = h_muon_id->GetBinContent(bin_index);
  *err = h_muon_id->GetBinError(bin_index);
}

// HINT This could also be done pt-dependent using other histos
// WARNING Using scale factor for dBeta-corrected reliso <0.12
void muonIsoScaleFactor(float eta, float* eff, float* err)
{
  const int bin_index = h_muon_iso->FindBin(eta);
  *eff = h_muon_iso->GetBinContent(bin_index);
  *err = h_muon_iso->GetBinError(bin_index);
}

// HINT This could also be done pt-dependent using other histos
void muonTriggerScaleFactor(float eta, float* eff, float* err)
{
  const int bin_index = h_muon_trigger->FindBin(eta);
  *eff = h_muon_trigger->GetBinContent(bin_index);
  *err = h_muon_trigger->GetBinError(bin_index);
}


void electronIDAndIsoScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down)
{
  const float abseta = fabs(eta);

  assert(pt >= 20);
  assert(abseta <= 2.5);


  if(pt < 30)
  {
    if(abseta < 0.800) {*eff = 0.953; *err_up = 0.185; *err_down = 0.007; return;} // sic. if it's an error, it's theirs
    if(abseta < 1.478) {*eff = 0.904; *err_up = 0.010; *err_down = 0.010; return;}
    if(abseta < 2.500) {*eff = 0.863; *err_up = 0.008; *err_down = 0.008; return;}
  }
  if(pt < 40)
  {
    if(abseta < 0.800) {*eff = 0.945; *err_up = 0.003; *err_down = 0.003; return;}
    if(abseta < 1.478) {*eff = 0.916; *err_up = 0.002; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 0.896; *err_up = 0.004; *err_down = 0.004; return;}
  }
  else if(pt < 50)
  {
    if(abseta < 0.800) {*eff = 0.948; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 1.478) {*eff = 0.959; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 0.944; *err_up = 0.002; *err_down = 0.005; return;}
  }
  else
  {
    if(abseta < 0.800) {*eff = 0.960; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 1.478) {*eff = 0.957; *err_up = 0.002; *err_down = 0.002; return;}
    if(abseta < 2.500) {*eff = 0.953; *err_up = 0.008; *err_down = 0.007; return;}
  }

}


void electronTriggerScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down)
{
  const float abseta = fabs(eta);

  assert(pt >= 30);
  assert(abseta <= 2.5);


  if(pt < 40)
  {
    if(abseta < 0.800) {*eff = 0.987; *err_up = 0.012; *err_down = 0.017; return;}
    if(abseta < 1.478) {*eff = 0.964; *err_up = 0.002; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 1.004; *err_up = 0.006; *err_down = 0.006; return;}
  }
  else if(pt < 50)
  {
    if(abseta < 0.800) {*eff = 0.997; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 1.478) {*eff = 0.980; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 1.033; *err_up = 0.007; *err_down = 0.007; return;}
  }
  else
  {
    if(abseta < 0.800) {*eff = 0.998; *err_up = 0.002; *err_down = 0.002; return;}
    if(abseta < 1.478) {*eff = 0.988; *err_up = 0.002; *err_down = 0.002; return;}
    if(abseta < 2.500) {*eff = 0.976; *err_up = 0.015; *err_down = 0.012; return;}
  }

}



void readHistoFromGraph(TGraphAsymmErrors* graph, TH1F** h, TString name)
{
  const int npoints = graph->GetN();
  const double* x_centers = graph->GetX();
  const double* y_centers = graph->GetY();
  double x_lows[npoints];
  double x_highs[npoints];
  double y_lows[npoints];
  double y_highs[npoints];
  for(int i=0; i<npoints; ++i)
  {
    x_lows[i] = graph->GetErrorXlow(i);
    x_highs[i] = graph->GetErrorXhigh(i);
    y_lows[i] = graph->GetErrorYlow(i);
    y_highs[i] = graph->GetErrorYhigh(i);
  }

  double x_edges[npoints+1];
  for(int i=0; i<npoints; ++i)
  {
    x_edges[i] = x_centers[i] - x_lows[i];
  }
  x_edges[npoints] = x_centers[npoints-1] + x_highs[npoints-1];


  *h = new TH1F(name, name, npoints, x_edges);
  (*h)->SetDirectory(0); // without this the histo will get deleted when a currently open TFile is closed

  for(int i=0; i<npoints; ++i)
  {
    (*h)->SetBinContent(i+1, y_centers[i]);
    (*h)->SetBinError(i+1, max(y_lows[i], y_highs[i]));
  }

}