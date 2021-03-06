///////////////////////////////////////////////////////////////////////////////////
//
//th_recexpert_x
// ------------
//
// tool to reconstuct a single top higgs event with popov's weights
//
// 
//
//    
//    ::05/14/2012 Benedikt Maier <bmaier@ekp.uni-karlsruhe.de>:: Simon :: Christian
/////////////////////////////////////////////////////////////////////////////////////

//#include "SchieferD/AnaUtils/interface/MCSample.h"

#include "../interface/CommandLine.h"
#include "../interface/THStatus.h"


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <map>
#include <TLorentzVector.h>
#include <math.h>

#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TKey.h"
#include "TControlBar.h"
#include "TSystem.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TChain.h"
//#include "/portal/ekpcms5/home/held/code/root_v5.28.00c/tmva/test/TMVAGui.C"
//#include "/portal/ekpcms5/home/held/code/root_v5.28.00c/tmva/test/tmvaglob.C"
//#include "TStopwatch.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
//////////////////////////////////////////////////////////////////////////////////______________________________________________________________________________

struct GreaterByVal{
  vector<float>  pts;
  GreaterByVal(vector<float>  pts_) : pts(pts_){
    //void
  }
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) > pts.at(j);
  }
};

struct SmallerByVal{
  vector<float>  pts;
  SmallerByVal(vector<float>  pts_) : pts(pts_) {
    //void
  }
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) < pts.at(j);
  }
};

double deltaPhi(double phi1, double phi2);
double deltaR(double eta1, double phi1, double eta2, double phi2);
void master_reader_th(TMVA::Reader *reader_th, string weight, TString weightfile);

// Create a set of variables and declare them to the reader
  // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
Float_t var1=0., var2=0.;
Float_t var3=0., var4=0.;
Float_t var5=0., var6=0.;
Float_t var7=0., var8=0.;
Float_t var9=0., var10=0.;
Float_t var11=0., var12=0.;
Float_t var13=0., var14=0.;
Float_t var15=0., var16=0.;
Float_t var17=0., var18=0.;

Int_t spec1=0;
Int_t spec2=0;
Int_t spec3=0;
Int_t spec4=0;
Float_t spec5=0.0;
Float_t spec6=0.0;
Float_t spec7=0.0;
Float_t spec8=0.0;

// Book method(s)   
TString methodName = TString("BDT") + TString(" method");


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char**argv)
{
  
  // This loads the library
  TMVA::Tools::Instance();
  
  
  // Default MVA methods to be trained + tested
  std::map<std::string,int> Use;
  
  // --- Boosted Decision Trees
  Use["BDT"]             = 1; // uses Adaptive Boost
  Use["BDTG"]            = 0; // uses Gradient Boost
  Use["BDTB"]            = 0; // uses Bagging
  Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
  std::cout << std::endl;
  std::cout << "==> Start TMVAClassificationApplication" << std::endl;
  
  CommandLine cl;
  if (!cl.parse(argc,argv))
    return 0;
  
  string         input     = cl.getValue <string>  ("input");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         ipath     = cl.getValue <string>  ("ipath",              ".");
  string         opath     = cl.getValue <string>  ("opath",              ".");
  bool           signal  = cl.getValue <bool>      ("signal",         false);
  vector<string> bdtpath   = cl.getVector<string>  ("bdtpath"              ""); // was empty before (why?)
  bool           takerandom = cl.getValue<bool>    ("takerandom",       false);
  float          workingpoint=cl.getValue<float>   ("workingpoint",     0.935);


  if(!cl.check()){};
  cl.print();
    
  
  // How many different BDTs will be evaluated?

  Int_t nbdts = bdtpath.size();

  string cpmap[21]={"weights_syst[0]/weights_syst[0]",
		      "weights_syst[446]/weights_syst[0]",
		      "weights_syst[447]/weights_syst[0]",
		      "weights_syst[448]/weights_syst[0]",
		      "weights_syst[449]/weights_syst[0]",
		      "weights_syst[450]/weights_syst[0]",
		      "weights_syst[451]/weights_syst[0]",
		      "weights_syst[452]/weights_syst[0]",
		      "weights_syst[453]/weights_syst[0]",
		      "weights_syst[454]/weights_syst[0]",
		      "weights_syst[455]/weights_syst[0]",
		      "weights_syst[456]/weights_syst[0]",
		      "weights_syst[457]/weights_syst[0]",
		      "weights_syst[458]/weights_syst[0]",
		      "weights_syst[459]/weights_syst[0]",
		      "weights_syst[460]/weights_syst[0]",
		      "weights_syst[461]/weights_syst[0]",
		      "weights_syst[462]/weights_syst[0]",
		      "weights_syst[463]/weights_syst[0]",
		      "weights_syst[464]/weights_syst[0]",
		      "weights_syst[465]/weights_syst[0]"};


  

  TMVA::Reader *reader_th_0 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_1 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_2 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_3 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_4 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_5 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_6 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_7 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_8 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_9 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_10 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_11 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_12 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_13 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_14 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_15 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_16 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_17 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_18 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_19 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_20 = new TMVA::Reader( "!Color:!Silent" );


  TString weightfiles_th[21];
  
  for (Int_t z=0;z<nbdts;z++)
    weightfiles_th[z]=TString(bdtpath[z]) +"/"+ TString("BDTreco_expert_BDT.weights.xml");


    
  master_reader_th(reader_th_0,cpmap[0],weightfiles_th[0]);
  master_reader_th(reader_th_1,cpmap[1],weightfiles_th[1]);
  master_reader_th(reader_th_2,cpmap[2],weightfiles_th[2]);
  master_reader_th(reader_th_3,cpmap[3],weightfiles_th[3]);
  master_reader_th(reader_th_4,cpmap[4],weightfiles_th[4]);
  master_reader_th(reader_th_5,cpmap[5],weightfiles_th[5]);
  master_reader_th(reader_th_6,cpmap[6],weightfiles_th[6]);
  master_reader_th(reader_th_7,cpmap[7],weightfiles_th[7]);
  master_reader_th(reader_th_8,cpmap[8],weightfiles_th[8]);
  master_reader_th(reader_th_9,cpmap[9],weightfiles_th[9]);
  master_reader_th(reader_th_10,cpmap[10],weightfiles_th[10]);
  master_reader_th(reader_th_11,cpmap[11],weightfiles_th[11]);
  master_reader_th(reader_th_12,cpmap[12],weightfiles_th[12]);
  master_reader_th(reader_th_13,cpmap[13],weightfiles_th[13]);
  master_reader_th(reader_th_14,cpmap[14],weightfiles_th[14]);
  master_reader_th(reader_th_15,cpmap[15],weightfiles_th[15]);
  master_reader_th(reader_th_16,cpmap[16],weightfiles_th[16]);
  master_reader_th(reader_th_17,cpmap[17],weightfiles_th[17]);
  master_reader_th(reader_th_18,cpmap[18],weightfiles_th[18]);
  master_reader_th(reader_th_19,cpmap[19],weightfiles_th[19]);
  master_reader_th(reader_th_20,cpmap[20],weightfiles_th[20]);
  

  
  // --- Create the Reader object
  		      
  string prefix = "TMVAClassification";
  

  
  // read input file
  TFile* ifile = new TFile((ipath+"/"+input).c_str(),"READ");
  if (!ifile->IsOpen()) {
    cout<<"Failed to open "<<input<<endl;
    return 0;
  }
  
  // create temporary output file
  size_t pos = input.find(".root");
  string output = input.substr(0,pos);
  output = output + ".cpreco.root";
  TFile* ofile = new TFile((opath+"/"+output).c_str(),"RECREATE");
  if (!ofile->IsOpen()) {
    cout<<"Failed to create "<<output<<endl;
    return 0;
  }
  
  // read input directory
  TDirectory* idir = (TDirectory*)ifile->Get(dirname.c_str());
  cout << "Input file: " << input << "/" << dirname << ": (@" << idir << ")" << endl;
  
  // create output directory
  TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
  if(0==odir){
    cout << "Failed to make " << dirname << " in " << output << endl;
    return 0;
  }
  cout << "Output file: " << output << "/" << dirname << ": (@" << odir << ")" << endl;

  // open input tree
  TTree* itree = (TTree*)idir->Get(treename.c_str());
  if(0==itree){
    cout << "No " << treename << " in " << output << ":" << dirname << endl;
    return 0;
  }
  
  // don't copy popout branch if it already exists
  if (0!=itree->FindBranch("best_recbdtout_cp"))           itree->SetBranchStatus("best_recbdtout_cp",0);
  if (0!=itree->FindBranch("hyp_posbdt_cp"))               itree->SetBranchStatus("hyp_posbdt_cp",0);
  if (0!=itree->FindBranch("hyp_posdR_cp"))                itree->SetBranchStatus("hyp_posdR_cp",0);
  if (0!=itree->FindBranch("hbbm_cp"))                     itree->SetBranchStatus("hbbm_cp",0); 
  if (0!=itree->FindBranch("hbbpt_cp"))                    itree->SetBranchStatus("hbbpt_cp",0);
  if (0!=itree->FindBranch("hbbphi_cp"))                   itree->SetBranchStatus("hbbphi_cp",0);
  if (0!=itree->FindBranch("hbbeta_cp"))                   itree->SetBranchStatus("hbbeta_cp",0);
  if (0!=itree->FindBranch("hbbdr_cp"))                    itree->SetBranchStatus("hbbdr_cp",0); 
  if (0!=itree->FindBranch("hbbjtidx_cp"))                 itree->SetBranchStatus("hbbjtidx_cp",0);
  if (0!=itree->FindBranch("topm_cp"))                     itree->SetBranchStatus("topm_cp",0);   
  if (0!=itree->FindBranch("toppt_cp"))                    itree->SetBranchStatus("toppt_cp",0);  
  if (0!=itree->FindBranch("topphi_cp"))                   itree->SetBranchStatus("topphi_cp",0); 
  if (0!=itree->FindBranch("topeta_cp"))                   itree->SetBranchStatus("topeta_cp",0); 
  if (0!=itree->FindBranch("topjtidx_cp"))                 itree->SetBranchStatus("topjtidx_cp",0);
  if (0!=itree->FindBranch("ljtidx_cp"))                   itree->SetBranchStatus("ljtidx_cp",0);
  

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout << "Output file: " << output << "/" << dirname << "/" << treename << ": (@" << otree << ")" << endl;

  if (0!=itree->FindBranch("hbbm_cp"))                   itree->SetBranchStatus("hbbm_cp",1);
  if (0!=itree->FindBranch("hbbpt_cp"))                  itree->SetBranchStatus("hbbpt_cp",1);
  if (0!=itree->FindBranch("hbbphi_cp"))                 itree->SetBranchStatus("hbbphi_cp",1);
  if (0!=itree->FindBranch("hbbeta_cp"))                 itree->SetBranchStatus("hbbeta_cp",1);
  if (0!=itree->FindBranch("hbbdr_cp"))                  itree->SetBranchStatus("hbbdr_cp",1);
  if (0!=itree->FindBranch("hbbjtidx_cp"))               itree->SetBranchStatus("hbbjtidx_cp",1);
  if (0!=itree->FindBranch("topm_cp"))                   itree->SetBranchStatus("topm_cp",1);
  if (0!=itree->FindBranch("toppt_cp"))                  itree->SetBranchStatus("toppt_cp",1);
  if (0!=itree->FindBranch("topphi_cp"))                 itree->SetBranchStatus("topphi_cp",1);
  if (0!=itree->FindBranch("topeta_cp"))                 itree->SetBranchStatus("topeta_cp",1);
  if (0!=itree->FindBranch("topjtidx_cp"))               itree->SetBranchStatus("topjtidx_cp",1);
  if (0!=itree->FindBranch("ljtidx_cp"))                 itree->SetBranchStatus("ljtidx_cp",1);

  // create bdtout branches in output tree
  Float_t  best_recbdtout[21]={-99};      otree->Branch("best_recbdtout_cp", best_recbdtout, "best_recbdtout_cp[21]/F");
  Int_t  hyp_posdR=-99;             otree->Branch("hyp_posdR_cp",     &hyp_posdR,           "hyp_posdR_cp/I");
  Int_t  hyp_posbdt=-99;            otree->Branch("hyp_posbdt_cp",    &hyp_posbdt,         "hyp_posbdt_cp/I");

  Float_t hbbm[21]={-99};                 otree->Branch("hbbm_cp",          hbbm,                     "hbbm_cp[21]/F");
  Float_t hbbpt[21]={-99};                otree->Branch("hbbpt_cp",         hbbpt,                   "hbbpt_cp[21]/F");
  Float_t hbbphi[21]={-99};               otree->Branch("hbbphi_cp",        hbbphi,                 "hbbphi_cp[21]/F");
  Float_t hbbeta[21]={-99};               otree->Branch("hbbeta_cp",        hbbeta,                 "hbbeta_cp[21]/F");
  Float_t hbbdr[21]={-99};                otree->Branch("hbbdr_cp",         hbbdr,                   "hbbdr_cp[21]/F");
  Int_t hbbjtidx[3][21]={-99,-99,-99};  otree->Branch("hbbjtidx_cp",      hbbjtidx,           "hbbjtidx_cp[3][21]/I");
  Float_t topm[21]={-99};                 otree->Branch("topm_cp",          topm,                     "topm_cp[21]/F");
  Float_t toppt[21]={-99};                otree->Branch("toppt_cp",         toppt,                   "toppt_cp[21]/F");
  Float_t topphi[21]={-99};               otree->Branch("topphi_cp",        topphi,                 "topphi_cp[21]/F");
  Float_t topeta[21]={-99};               otree->Branch("topeta_cp",        topeta,                 "topeta_cp[21]/F");
  Int_t topjtidx[21]={-99};               otree->Branch("topjtidx_cp",      topjtidx,             "topjtidx_cp[21]/I");
  Int_t ljtidx[21]={-99};                 otree->Branch("ljtidx_cp",        ljtidx,                 "ljtidx_cp[21]/I");
  
  //Get relevant information for creating hypothesis
  Int_t i_njt=-99;                 itree->SetBranchAddress("njt",            &i_njt);
  Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
  Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta",         i_jteta);
  Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
  Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
  Float_t i_jtcsvt[40]={-99};      itree->SetBranchAddress("jtcsvt",       i_jtcsvt);
  Float_t i_jtcharge[40]={-99};    itree->SetBranchAddress("jtcharge",   i_jtcharge);
  Float_t i_jtcostheta_l[40]={-99};itree->SetBranchAddress("jtcostheta_l",   i_jtcostheta_l);
  
  Int_t i_nlepw={-99};           itree->SetBranchAddress("nlepw",        &i_nlepw);
  Float_t i_lepwm={-99};           itree->SetBranchAddress("lepwm",        &i_lepwm);
  Float_t i_lepwpt={-99};          itree->SetBranchAddress("lepwpt",      &i_lepwpt);
  Float_t i_lepwphi={-99};         itree->SetBranchAddress("lepwphi",    &i_lepwphi);
  Float_t i_lepweta={-99};         itree->SetBranchAddress("lepweta",    &i_lepweta);
  
  Int_t i_nel={-99};               itree->SetBranchAddress("nel",            &i_nel);
  Float_t i_elpt[10]={-99};        itree->SetBranchAddress("elpt",           i_elpt);
  Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
  Float_t i_elphi[10]={-99};       itree->SetBranchAddress("elphi",         i_elphi);
  Float_t i_ele[10]={-99};         itree->SetBranchAddress("ele",             i_ele);
  Float_t i_elcharge[10]={-99};    itree->SetBranchAddress("elcharge",   i_elcharge);  
  
  Int_t i_nmu={-99};               itree->SetBranchAddress("nmu",            &i_nmu);
  Float_t i_mupt[10]={-99};        itree->SetBranchAddress("mupt",           i_mupt);
  Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
  Float_t i_muphi[10]={-99};       itree->SetBranchAddress("muphi",         i_muphi);
  Float_t i_mue[10]={-99};         itree->SetBranchAddress("mue",             i_mue);
  Float_t i_mucharge[10]={-99};    itree->SetBranchAddress("mucharge",   i_mucharge);
  
  Float_t i_sumHt={-99};           itree->SetBranchAddress("sumHt",        &i_sumHt);
  Float_t i_met={-99};             itree->SetBranchAddress("met",            &i_met);

  Float_t i_Hpt={-99};
  Float_t i_Heta={-99};          
  Float_t i_Hphi={-99};
  Float_t i_Hm={-99}; 

  Float_t i_tpt={-99}; 
  Float_t i_teta={-99};       
  Float_t i_tphi={-99};
  Float_t i_tm={-99};        

  Float_t i_lqpt={-99};
  Float_t i_lqeta={-99};
  Float_t i_lqphi={-99};
  Float_t i_lqm={-99};

  if(signal){
    itree->SetBranchAddress("Hpt",            &i_Hpt);
    itree->SetBranchAddress("Heta",          &i_Heta);
    itree->SetBranchAddress("Hphi",          &i_Hphi);
    itree->SetBranchAddress("Hm",              &i_Hm);
    itree->SetBranchAddress("tpt",            &i_tpt);
    itree->SetBranchAddress("teta",          &i_teta);
    itree->SetBranchAddress("tphi",          &i_tphi);
    itree->SetBranchAddress("tm",              &i_tm);
    itree->SetBranchAddress("lqpt",          &i_lqpt);
    itree->SetBranchAddress("lqeta",        &i_lqeta);
    itree->SetBranchAddress("lqphi",        &i_lqphi);
    itree->SetBranchAddress("lqm",            &i_lqm);
  }

  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  THStatus * status= new THStatus(nevt);
  cout << nevt << " events will be processed" << endl;




  Float_t rndm_recbdtout[21];
  Int_t best_i[21];
  Int_t best_j[21];
  Int_t best_k[21];
  Int_t best_l[21];
  TLorentzVector p4jets[100];
  Float_t jetscsv[100];
  vector<float> vec_bdtout;
  vector<float> vec_deltaR;
  Float_t recbdtout[21];


  //event loop
  for (unsigned int ievt=0; ievt<nevt; ievt++) {

    if (ievt %20 == 0 || ievt <= 10 || nevt - ievt < 10){
      cout << "\r  > Processing events from input tree:  (\033[33m" << ievt+1 << "/" << nevt << "\033[0m)  -> \033[32m";
      cout << setprecision(3) << setiosflags(std::ios_base::left) << setfill('0') << setw(2)\
	   << ((ievt*1.0+1))/nevt*100.0 << "\033[0m%    \b\b\b" << flush;
    }
    //status->print_status(ievt);
    

    itree->GetEntry(ievt);
    Int_t rndm_i=-1;
    Int_t rndm_j=-1;
    Int_t rndm_k=-1;
    Int_t rndm_l=-1;
 
    // unnecessary because they get overwritten either way, and nothing checks for the default value of -1
    //std::fill_n(best_i, 21, -1);
    //std::fill_n(best_j, 21, -1);
    //std::fill_n(best_k, 21, -1);
    //std::fill_n(best_l, 21, -1);
    
    for(int z=0;z<21;z++)
      {
	ljtidx[z]=-1;
	topjtidx[z]=-1;
	hbbjtidx[0][z]=-1;
	hbbjtidx[1][z]=-1;
	best_recbdtout[z] = -99; 
	rndm_recbdtout[z] = -99; 
      }
	  
    vec_bdtout.clear();
    vec_deltaR.clear();

    // unused:
    //vector<int>  vec_i;
    //vector<int>  vec_j;
    //vector<int>  vec_k;
    //vector<int>  vec_l;

    Bool_t isForbidden = false;
    Int_t hyp_counter=0;
    Int_t n_central=0;
    for (int jets=0; jets<i_njt; jets++){
      if (abs(i_jteta[jets])<2.4)
        n_central++;
    }
    
    if (i_nlepw == 1 && i_njt>=4 && n_central>2) {
      TLorentzVector p4top, p4higgs, p4light;
      TLorentzVector p4lepw;
      for (int jets=0; jets<i_njt; jets++){
	p4jets[jets].SetPtEtaPhiE(i_jtpt[jets],i_jteta[jets],i_jtphi[jets],i_jte[jets]);
	jetscsv[jets]=i_jtcsvt[jets];
      }

      p4lepw.SetPtEtaPhiM(i_lepwpt,i_lepweta,i_lepwphi,i_lepwm);
     

      bool has_wenu = (i_nel==1);
      TLorentzVector p4lepton;
      float lepcharge = -99; 

      if (has_wenu){
	      p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
	      lepcharge = i_elcharge[0];
      }else{
	      p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);
	      lepcharge = i_mucharge[0];
      }

      if(signal){
	      p4higgs.SetPtEtaPhiM(i_Hpt,i_Heta,i_Hphi,i_Hm);
	      p4top.SetPtEtaPhiM(i_tpt,i_teta,i_tphi,i_tm);
	      p4light.SetPtEtaPhiM(i_lqpt, i_lqeta,i_lqphi,i_lqm);
      }


      int facul = i_njt ;
      int resultfac = 1;
      for(int f=0; f<4; f++){
        resultfac *= facul;
	      facul -= 1;
      }
      resultfac /= 2;

      for (Int_t i=0; i<i_njt; i++) {  // b from top
	if(fabs(p4jets[i].Eta())>2.4 || p4jets[i].Pt()<30)
	  continue;

	for (Int_t j=0; j<i_njt; j++) {  // b1 from Higgs
          if (j!=i) {
            if(fabs(p4jets[j].Eta())>2.4 || p4jets[j].Pt()<30)
              continue;

            for (Int_t k=0; k<i_njt; k++) {  // b2 from Higgs
              if ((k!=i )&&(k>j)) {  // switching of b1 & b2 does not change Higgs, so take only one
                if(fabs(p4jets[k].Eta())>2.4 || p4jets[k].Pt()<30)
                  continue;

                TLorentzVector rechiggs = p4jets[j] + p4jets[k];
                for (Int_t l=0; l<i_njt; l++) {  // light jet
                  if(l!=k && l!=j && l!=i) {
                    if((i_jtcsvt[l] > workingpoint&&fabs(p4jets[l].Eta())<2.4) || (fabs(p4jets[l].Eta())>2.4 && p4jets[l].Pt()<40 ))
                      continue;

                    hyp_counter++;
                  }
                }//for ...  light jet
              }
            }//for ... b2 from Higgs
          }
        }//for ... b1 from Higgs
      }//for ... b from top

      
      TRandom3 random = 0;
      int randomhyp = random.Integer(hyp_counter-1);
      hyp_counter =0 ;
      for (Int_t i=0; i<i_njt; i++) {  // b from top
        if(fabs(p4jets[i].Eta())>2.4 || p4jets[i].Pt()<30)
          continue;
        TLorentzVector rectop = p4lepw + p4jets[i];

        for (Int_t j=0; j<i_njt; j++){  // b1 from Higgs
          if (j!=i){
            if(fabs(p4jets[j].Eta())>2.4 || p4jets[j].Pt()<30)
              continue;

            for (Int_t k=0; k<i_njt; k++) {  // b2 from Higgs
	      
              if ((k!=i )&&(k>j)) {  // switching of b1 & b2 does not change Higgs, so take only one

		if(fabs(p4jets[k].Eta())>2.4 || p4jets[k].Pt()<30)
                  continue;
                TLorentzVector rechiggs = p4jets[j] + p4jets[k];
		
                for (Int_t l=0; l<i_njt; l++) {  // light jet
                  if(l!=k && l!=j && l!=i) {
                    if((i_jtcsvt[l] > workingpoint&& fabs(p4jets[l].Eta())<2.4) || (fabs(p4jets[l].Eta())>2.4 && p4jets[l].Pt()<40))
                      continue;
                    if(hyp_counter == randomhyp){
                      rndm_i  =  i;
                      rndm_j  =  j;
                      rndm_k  =  k;
                      rndm_l  =  l;
                    }

                    TLorentzVector reclight=p4jets[l];
                    /*Lookup:
                     * i -> topjtidx
                     * j -> hbbjtidx[0]
                     * k -> hbbjtidx[1]
                     * l -> ljtidx
                     */
                    var1  =  i_jtcsvt[i];
                    var2  =  TMath::Abs(i_jteta[l]);
                    var3  =  i_jtcsvt[j];
		    var4  =  i_jtcsvt[k];
                    var5  =  (rectop.M() > 0) ? TMath::Log(rectop.M()) : -1 ;
                    var6  =  (rechiggs.M() > 0) ? TMath::Log(rechiggs.M()) : -1;
                    var7  =  TMath::Log(TMath::Min(i_jtpt[j],i_jtpt[k]));
                    var8  =  deltaR(i_jteta[j],i_jtphi[j], i_jteta[k], i_jtphi[k]);
                    var9  =  TMath::Max(TMath::Abs(i_jteta[j]),TMath::Abs(i_jteta[k]));
		    var10 =   deltaR(i_jteta[i], i_jtphi[i], p4lepw.Eta(), p4lepw.Phi());
                    var11 =  (rectop.Pt()+rechiggs.Pt()+i_jtpt[l])/(i_sumHt + i_met + (i_nel==1 ? i_elcharge[0] : i_mucharge[0]));
                    var12 =  deltaR(rectop.Eta(), rectop.Phi(), rechiggs.Eta(), rechiggs.Phi());
                    var13 =  i_jtcharge[i]*(i_nel==1 ? i_elcharge[0] : i_mucharge[0]);
                    var14 =  TMath::Abs(i_jteta[l]-i_jteta[i]);
                    var15 =  i_jte[l]-i_jte[i];
                    var16 =  TMath::Abs(i_jteta[i]);
                    var17 =  TMath::Abs(rectop.Eta()-rechiggs.Eta());
		    var18 =  i_jtcostheta_l[i];

		    if(isnan(var5))
		      cout << "Higgs jets: " << p4jets[j].Pt() << " " << p4jets[k].Pt() << endl;
		    if(isnan(var4))
		      cout << "top jets: " << p4jets[i].Pt() << " W: " << p4lepw.Pt() << endl;
	
		    recbdtout[0]=reader_th_0->EvaluateMVA( methodName  );
		    recbdtout[1]=reader_th_1->EvaluateMVA( methodName  );
		    recbdtout[2]=reader_th_2->EvaluateMVA( methodName  );
		    recbdtout[3]=reader_th_3->EvaluateMVA( methodName  );
		    recbdtout[4]=reader_th_4->EvaluateMVA( methodName  );
		    recbdtout[5]=reader_th_5->EvaluateMVA( methodName  );
		    recbdtout[6]=reader_th_6->EvaluateMVA( methodName  );
		    recbdtout[7]=reader_th_7->EvaluateMVA( methodName  );
		    recbdtout[8]=reader_th_8->EvaluateMVA( methodName  );
		    recbdtout[9]=reader_th_9->EvaluateMVA( methodName  );
		    recbdtout[10]=reader_th_10->EvaluateMVA( methodName  );
		    recbdtout[11]=reader_th_11->EvaluateMVA( methodName  );
		    recbdtout[12]=reader_th_12->EvaluateMVA( methodName  );
		    recbdtout[13]=reader_th_13->EvaluateMVA( methodName  );
		    recbdtout[14]=reader_th_14->EvaluateMVA( methodName  );
		    recbdtout[15]=reader_th_15->EvaluateMVA( methodName  );
		    recbdtout[16]=reader_th_16->EvaluateMVA( methodName  );
		    recbdtout[17]=reader_th_17->EvaluateMVA( methodName  );
		    recbdtout[18]=reader_th_18->EvaluateMVA( methodName  );
		    recbdtout[19]=reader_th_19->EvaluateMVA( methodName  );
		    recbdtout[20]=reader_th_20->EvaluateMVA( methodName  );


		    for (Int_t z=0;z<nbdts;z++){
		    			      
		      vec_bdtout.push_back(recbdtout[0]);
		      if(signal){
			Float_t DRtop = deltaR(rectop.Eta(),rectop.Phi(),p4top.Eta(),p4top.Phi()) + fabs(rectop.Pt() - p4top.Pt())/p4top.Pt();
			Float_t DRhiggs = deltaR(rechiggs.Eta(),rechiggs.Phi(),p4higgs.Eta(),p4higgs.Phi()) + fabs(rechiggs.Pt() - p4higgs.Pt())/p4higgs.Pt();
			Float_t DRlight = deltaR(reclight.Eta(),reclight.Phi(),p4light.Eta(),p4light.Phi()) + fabs(reclight.Pt() - p4light.Pt())/p4light.Pt();
			Float_t DeltaRval = DRtop + DRhiggs + DRlight;
			vec_deltaR.push_back(DeltaRval);
		      }
		      if(hyp_counter == randomhyp)
			rndm_recbdtout[z] = recbdtout[z];
		      
		      
		      if(recbdtout[z] > best_recbdtout[z]){
			best_recbdtout[z]  =  recbdtout[z];
			best_i[z]  =  i;
			best_j[z]  =  j;
			best_k[z]  =  k;
			best_l[z]  =  l;
		      }

		    } // number of BDTs evaluated: for (Int_t z=0;i<nbdts;i++){ ...

                    hyp_counter++;
		    
                  }
                }//end light jet
              }
            }//end b2 from Higgs
          }
        }//end b1 from Higgs
      }  // end b from top
      
      // Fill in which position max(BDT) and min(deltaR) ar
      if (signal){
        vec_bdtout.resize(hyp_counter);
        vec_deltaR.resize(hyp_counter);
        vector<size_t> indices_bdt;
        vector<size_t> indices_dR;
        indices_bdt.resize(hyp_counter);
        indices_dR.resize(hyp_counter);
        for(Int_t i=0; i<hyp_counter; ++i) {
          indices_bdt[i] = i;
          indices_dR[i] = i;
        }
        sort(indices_bdt.begin(),indices_bdt.end(),GreaterByVal(vec_bdtout));
        sort(indices_dR.begin(),indices_dR.end(),SmallerByVal(vec_deltaR));

        hyp_posbdt = std::find(indices_dR.begin(), indices_dR.end(), indices_bdt.at(0)) - indices_dR.begin();
        hyp_posdR = std::find(indices_bdt.begin(), indices_bdt.end(), indices_dR.at(0)) - indices_bdt.begin();
      }

      // !!! Take random as "best" !!
      if (takerandom) {
	for (Int_t z=0;z<nbdts;z++){
	  best_recbdtout[z] = rndm_recbdtout[z];
	  best_i[z] = rndm_i;
	  best_j[z] = rndm_j;
	  best_k[z] = rndm_k;
	  best_l[z] = rndm_l;
	}
      }

      for (Int_t z=0;z<nbdts;z++){
	TLorentzVector rectop    =  p4lepw + p4jets[best_i[z]];
	TLorentzVector rechiggs  =  p4jets[best_j[z]] + p4jets[best_k[z]];
	TLorentzVector reclight  =  p4jets[best_l[z]];
	ljtidx[z]      =    best_l[z];
	hbbm[z]        =    rechiggs.M();
	hbbpt[z]       =    rechiggs.Pt();
	hbbphi[z]      =    rechiggs.Phi();
	hbbeta[z]      =    rechiggs.Eta();
	hbbdr[z]       =    p4jets[best_j[z]].DeltaR(p4jets[best_k[z]]);
	hbbjtidx[0][z] =    best_j[z];
	hbbjtidx[1][z] =    best_k[z];
	topm[z]        =    rectop.M();
	toppt[z]       =    rectop.Pt();
	topphi[z]      =    rectop.Phi();
	topeta[z]      =    rectop.Eta();
	topjtidx[z]    =    best_i[z];
      }

      otree->Fill();

    }else{ //FROM: if (i_nlep == 1 && i_njt > 4 && n_central > 2)
      //      cout << "No valid hypothesis found!\n";
      for (Int_t z=0;z<nbdts;z++){

	ljtidx[z]      =  -99;
	hbbm[z]        =   -99;
	hbbpt[z]       =   -99;
	hbbphi[z]      =   -99; 
	hbbeta[z]      =   -99;
	hbbdr[z]       =   -99;
	hbbjtidx[0][z] =   -99;
	hbbjtidx[1][z] =   -99;
	topm[z]        =   -99;
	toppt[z]       =   -99;
	topphi[z]      =   -99;
	topeta[z]      =   -99;
	topjtidx[z]    =   -99;
	best_recbdtout[z] = -99;
      }
    }
  } // event loop
    
  cout << "End of event loop \n";
  
  otree->Write();
  ofile->Close();
  ifile->Close();
  cout << "Wrote file *.cpreco.root \n";
  delete reader_th_0;  
  delete reader_th_1;  
  delete reader_th_2;  
  delete reader_th_3;  
  delete reader_th_4;  
  delete reader_th_5;  
  delete reader_th_6;  
  delete reader_th_7;  
  delete reader_th_8;  
  delete reader_th_9;  
  delete reader_th_10;  
  delete reader_th_11;  
  delete reader_th_12;  
  delete reader_th_13;  
  delete reader_th_14;  
  delete reader_th_15;  
  delete reader_th_16;  
  delete reader_th_17;  
  delete reader_th_18;  
  delete reader_th_19;  
  delete reader_th_20;  
  
  delete ifile;
  delete ofile;
  
  string cmd_str = "mv -f " + opath + "/" + output + " " + opath + "/" + input;
  gSystem->Exec(cmd_str.c_str());
  
  cout << "Override original file... !" << endl;
  cout << "==> TMVAClassificationApplication is done! Hell Yeah!" << endl;

  return 0;

  //  std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
    
 
}

//______________________________________________________________________________
double deltaPhi(double phi1, double phi2)
{
  double result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

//______________________________________________________________________________
double deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return std::sqrt(deta*deta + dphi*dphi);
}

//______________________________________
void master_reader_th(TMVA::Reader *reader_th, string weight, TString weightfile)
{
  
  reader_th->AddSpectator("topjtidx", &spec1);
  reader_th->AddSpectator("ljtidx", &spec2);
  reader_th->AddSpectator("hbbjtidx[0]", &spec3);
  reader_th->AddSpectator("hbbjtidx[1]", &spec4);
  reader_th->AddSpectator(weight, &spec5);
  

  
  reader_th->AddVariable("jtcsvt[topjtidx]", 			 &var1 );
  reader_th->AddVariable("abs(jteta[ljtidx])",           &var2 );
  reader_th->AddVariable("jtcsvt[hbbjtidx[0]]", 			 &var3 );
  reader_th->AddVariable("jtcsvt[hbbjtidx[1]]", 			 &var4 );
  reader_th->AddVariable("log(topm)",                    &var5 );
  reader_th->AddVariable("log(hbbm)",                    &var6 );
  reader_th->AddVariable("log(min(jtpt[hbbjtidx[0]],jtpt[hbbjtidx[1]]))", 			 &var7 );
  reader_th->AddVariable("deltaR(jteta[hbbjtidx[0]],jtphi[hbbjtidx[0]],jteta[hbbjtidx[1]],jtphi[hbbjtidx[1]])", 			 &var8 );
  //  reader_th->AddVariable("max(abs(jteta[hbbjtidx[0]]),abs(jteta[hbbjtidx[1]]))", 			 &var9 );
  reader_th->AddVariable("deltaR(jteta[topjtidx],jtphi[topjtidx],lepweta,lepwphi)", 			 &var10 );
  reader_th->AddVariable("(toppt+hbbpt+jtpt[ljtidx])/(sumHt+met+(nel==1 && nmu==0)*Alt$(elpt[0],0)"
			 "+(nel==0 && nmu==1)*Alt$(mupt[0],0))",				 &var11 );
  //  reader_th->AddVariable("deltaR(topeta,topphi,hbbeta,hbbphi)",				 &var12 );
  //  reader_th->AddVariable("jtcharge[topjtidx]*(((nel==1 && nmu==0)*Alt$(elcharge[0],0))"
			 //			 "+((nel==0 && nmu==1)*Alt$(mucharge[0],0)))",				 &var13 );
  reader_th->AddVariable("abs(jteta[ljtidx]-jteta[topjtidx])",				 &var14 );
  reader_th->AddVariable("jte[ljtidx]-jte[topjtidx]",				 &var15 );
  reader_th->AddVariable("abs(jteta[topjtidx])",				 &var16 );
  reader_th->AddVariable("abs(topeta-hbbeta)",					 &var17 ); //new
  reader_th->AddVariable("jtcostheta_l[topjtidx]",					 &var18 ); //new

  
  reader_th->BookMVA( methodName, weightfile );

}
