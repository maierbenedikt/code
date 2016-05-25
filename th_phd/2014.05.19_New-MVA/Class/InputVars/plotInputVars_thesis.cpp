/**
 * \author Andrey Popov
 * 
 * The script plots distributions over input variables normalised to unit area. The binning is
 * adapted in such a way that every bin contains similar number of events taken from 1:1 signal and
 * background mixture. Several criteria of similarity between signal and background PDFs are
 * evaluated. The code can be compiled with
 *   g++ -o plotInputVars_thesis plotInputVars_thesis.cpp --std=c++11 `root-config --cflags --libs` -lTreePlayer -I$BOOST_ROOT/include/ -O2
 */

#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Alpha/2013.08.23_Full-analysis-CSVM/Step1_MVA-reco/plotInputVars/NamedFormula.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Alpha/2013.08.23_Full-analysis-CSVM/Step1_MVA-reco/plotInputVars/BinnedVariable.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/PlotTools/Util/TreeReader.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/PlotTools/Util/BinaryClassPerformance.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Bravo/2014.05.19_New-MVA/tHq/InputVars/convUserNDC.hpp"

#include <THStack.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>
#include <TROOT.h>
#include <TError.h>

#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sys/stat.h>


using namespace std;



/// An aggregate to run BinaryClassPerformance for each variable
struct VarSeparation
{
    /// Constructor
    VarSeparation(NamedFormula const &var_, vector<double> const &edges):
        var(var_), perf(edges.begin(), edges.end())
    {}
    
    /// Constructor
    VarSeparation(NamedFormula const &var_, initializer_list<double> const &edges):
        var(var_), perf(edges)
    {}
    
    /// Associated object with TTreeFormula expression and title
    NamedFormula const &var;
    
    /// An object to evaluate the performance
    BinaryClassPerformance perf;
};


/**
 * Loops over the given files and fills AdaptiveBinning objects in the provided binnings. Only
 * nEntries entires passing the selection are read (splitted evenly among all the input files)
 */
void FillBinners(string const &filePrefix, list<string> const &fileNames, string const &treeName,
 string const &commonWeight, string const &selection, unsigned long nEntries,
 vector<AdaBinnedVariable> &vars)
{
    unsigned long const nEntriesToRead = nEntries / fileNames.size();
    
    for (auto const &fName: fileNames)
    {
        TreeReader reader(filePrefix + fName, treeName);
        reader.BookVariable(commonWeight, "weight");
        reader.BookVariable(selection, "sel");
        
        for (auto const &v: vars)
            reader.BookVariable(v.GetFormula());
        
        unsigned long const nEntriesAvailable = reader.GetEntries();
        
        for (unsigned long ev = 0, entriesPassed = 0;
         entriesPassed < nEntriesToRead and ev < nEntriesAvailable; ++ev)
        {
            reader.ReadEntry(ev);
            
            if (reader["sel"] == 0. or reader["weight"] == 0.)
                continue;
            
            for (auto &v: vars)
                v.Fill(reader[v.GetFormula()]);
            
            ++entriesPassed;
        }
    }
}


/**
 * Loops over the given files and fills BinaryClassPerformance objects in the provided separations
 */
void FillSeparations(string const &filePrefix, list<string> const &fileNames,
 string const &treeName, string const &commonWeight, string const &selection,
 vector<VarSeparation> &separations, bool isSignal)
{
    for (auto const &fName: fileNames)
    {
        TreeReader reader(filePrefix + fName, treeName);
        reader.BookVariable(commonWeight, "weight");
        reader.BookVariable(selection, "sel");
        
        for (auto const &sep: separations)
            reader.BookVariable(sep.var.GetFormula());
        
        while (reader.ReadNextEntry())
        {
            double const weight = reader["weight"];
            
            if (reader["sel"] == 0. or weight == 0.)
                continue;
            
            for (auto &sep: separations)
                sep.perf.Fill(isSignal, reader[sep.var.GetFormula()], weight);
        }
    }
}


/**
 * An aggregate to store evaluated criteria for a variable
 */
struct DiscrCriteria
{
    DiscrCriteria(string const &name_, string const &title_):
        name(name_), title(title_)
    {}
    
    bool operator<(DiscrCriteria const &rhs) const
    {
        return complArea < rhs.complArea;
    }
    
    void Print() const
    {
        cout << name << " &  $" << title << "$ &  " <<
         setprecision(3) << complArea << " &  " << setprecision(3) << ksStat << " &  " <<
         setprecision(3) << symChi2 << " &  " << setprecision(3) << jsDiv << " \\\\\n"; 
    }
    
    string name;
    string title;
    
    double complArea;
    double ksStat;
    double symChi2;
    double jsDiv;
};


void plotInputVars()
{
    // Set style to resemble TDR
    gStyle->SetOptStat(0);
    gStyle->SetStripDecimals(false);
    gStyle->SetErrorX(0.);
    TGaxis::SetMaxDigits(3);

    gStyle->SetTitleFont(42);
    gStyle->SetTitleFontSize(0.04);  // was 0.05
    gStyle->SetTitleFont(42, "XYZ");
    gStyle->SetTitleXOffset(0.9);
    gStyle->SetTitleYOffset(1.25);
    gStyle->SetTitleSize(0.05, "XYZ");  // was 0.06
    gStyle->SetLabelFont(42, "XYZ");
    gStyle->SetLabelOffset(0.007, "XYZ");
    gStyle->SetLabelSize(0.04, "XYZ");  // was 0.05
    gStyle->SetNdivisions(508, "XYZ");
    gStyle->SetLegendFont(42);

    gStyle->SetPadTopMargin(0.13);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetPadRightMargin(0.05);
    
    
    // Set the desired number of bins (same for all the variables). There are some rules of thumb
    //how to do it [1], but here the value is just set manually
    //[1] http://en.wikipedia.org/wiki/Histogram#Number_of_bins_and_width
    unsigned const nBins = 15;
    //unsigned const nBins = 10;
    
    
    // Continuous variables to plot
    vector<AdaBinnedVariable> contVars;
    
    contVars.emplace_back("glb_Pt_J1", "p_{T}(j_{1})", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_Pt_J2", "p_{T}(j_{2})", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_Ht", "Total H_{T}", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_MET", "MET", "GeV");
    contVars.back().SetHistParams(nBins, 0.015, 0.04);
    
    contVars.emplace_back("glb_SqrtSHat", "#sqrt{#hat{s}}", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_Sphericity", "Sphericity");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_Mass_Top", "m(t) [tHq]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tHq_Pt_Top", "p_{T}(t) [tHq]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tHq_Eta_Top)", "|#eta(t)| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_Pt_BTop", "p_{T}(b_{t}) [tHq]", "GeV");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("abs(tHq_Eta_BTop)", "|#eta(b_{t})| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_Charge_BTop", "Q(b_{t}) #times Q(l) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_DeltaR_BTopW", "#DeltaR(b_{t},W) [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_DEta_TopLep", "|#eta(t) - #eta(l)| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_Mass_BTopLep", "m(b_{t}+l) [tHq]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tHq_Mass_Higgs", "m(H) [tHq]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Pt_Higgs", "p_{T}(H) [tHq]", "GeV");
    // contVars.emplace_back("tHq_Pt_Higgs", "p_{T}(higgs) [tHq]", "GeV");  // KIT-style name
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tHq_Eta_Higgs)", "|#eta(H)| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_DeltaR_BJetsHiggs", "#DeltaR(b_{H}, b_{H}) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_AbsCharge_Higgs", "|#SigmaQ(b_{H})| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_MaxEta_BHiggs", "max |#eta(b_{H})| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_MinPt_BHiggs", "min p_{T}(b_{H}) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Pt_Recoil", "p_{T}(j') [tHq]", "GeV");
    // contVars.emplace_back("tHq_Pt_Recoil", "p_{T}(light jet) [tHq]", "GeV");  // KIT-style name
    contVars.back().SetHistParams(nBins, 0.01, 0.04);
    
    contVars.emplace_back("tHq_DeltaR_TopHiggs", "#DeltaR(t, H) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Cos_LepRecoil_TH", "cos_{t+H}(l, j') [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Mass_TopHiggs", "m(t+H) [tHq]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_RelHt", "(p_{T}(t) + p_{T}(H) + p_{T}(j')) / H_{T} [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.);
    
    
    contVars.emplace_back("tt_Mass_TopLep", "m(t_{lep}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tt_Pt_TopLep", "p_{T}(t_{lep}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_TopLep)", "|#eta(t_{lep})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tt_Pt_BTopLep", "p_{T}(b_{tlep}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("abs(tt_Eta_BTopLep)", "|#eta(b_{tlep})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tt_Charge_BTopLep", "Q(b_{tlep}) #times Q(l) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_DeltaR_BTopLepWLep", "#DeltaR(b_{tlep},W_{lep}) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.03);
    
    contVars.emplace_back("tt_Mass_BTopLepLep", "m(b_{tlep}+l) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tt_Mass_TopHad", "m(t_{had}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.12);
    
    contVars.emplace_back("tt_Pt_TopHad", "p_{T}(t_{had}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_TopHad)", "|#eta(t_{had})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_Pt_BTopHad", "p_{T}(b_{thad}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_BTopHad)", "|#eta(b_{thad})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_Charge_BTopHad", "Q(b_{thad}) #times Q(l) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_DeltaR_BTopHadWHad", "#DeltaR(b_{thad},W_{had}) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_Mass_WHad", "m(W_{had}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.12);
    
    contVars.emplace_back("tt_Pt_WHad", "p_{T}(W_{had}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_WHad)", "|#eta(W_{had})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tt_DeltaR_Light", "#DeltaR(q_{Whad}, q_{Whad}) [t#bar{t}]");
    // contVars.emplace_back("tt_DeltaR_Light", "#DeltaR(t_{had} jets) [t#bar{t}]");  // KIT-style name
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_SumCharge_Light", "#SigmaQ(q_{Whad}) #times Q(l) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_DMass_TopHadWHad", "m(t_{had}) - m(W_{had}) [t#bar{t}]", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.10);
    
    contVars.emplace_back("tt_RelHt", "(p_{T}(t_{lep}) + p_{T}(t_{had})) / H_{T} [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.);
    
    contVars.emplace_back("tt_MaxMass_BTopHadLight", "max m(b_{thad}, q_{Whad}) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.10);
    
    
    // Variables with manual binning
    vector<BinnedVariable> discrVars;
    
    discrVars.emplace_back("glb_Charge_Lep", "Q(l)");
    // discrVars.emplace_back("glb_Charge_Lep", "lepton charge");  // KIT-style name
    discrVars.back().SetBinning(initializer_list<double>({-1.5, -0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tHq_PassBTag_BTop", "tagged(b_{t}) [tHq]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tHq_NumBTag_Higgs", "#tagged(b_{H}) [tHq]");
    // discrVars.emplace_back("tHq_NumBTag_Higgs", "# b-tags of higgs jets [tHq]");  // KIT-style name
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5, 2.5}));
    
    discrVars.emplace_back("tt_PassBTag_BTopLep", "tagged(b_{tlep}) [t#bar{t}]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tt_PassBTag_BTopHad", "tagged(b_{thad}) [t#bar{t}]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tt_NumPassBTag_Light", "#tagged(q_{Whad}) [t#bar{t}]");
    // discrVars.emplace_back("tt_NumPassBTag_Light", "# b-tags of light t_{had} jets [t#bar{t}]");  // KIT-style name
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5, 2.5}));
    
    
    vector<double> etaBins;
    
    for (double eta = 0.; eta < 2.401; eta += 0.2)
        etaBins.push_back(eta);
    
    etaBins.push_back(3.2);
    etaBins.push_back(4.7);
    
    discrVars.emplace_back("abs(tHq_Eta_Recoil)", "|#eta(j')| [tHq]");
    // discrVars.emplace_back("abs(tHq_Eta_Recoil)", "|#eta(light jet)| [tHq]");  // KIT-style name
    discrVars.back().SetBinning(etaBins.begin(), etaBins.end());
    
    discrVars.emplace_back("tt_MaxEta_Light", "max |#eta(q_{Whad})| [t#bar{t}]");
    discrVars.back().SetBinning(etaBins.begin(), etaBins.end());
    
    
    /*
    list<double> binsUnitDomain;
    
    for (int i = 0; i <= nBins; ++i)
        binsUnitDomain.push_back(-0.01 + 1.02 / nBins * i);
    
    discrVars.emplace_back("CSVBJetTop", "CSV(j_{t})");
    discrVars.back().SetBinning(binsUnitDomain.begin(), binsUnitDomain.end());
    
    discrVars.emplace_back("CSVLJet", "CSV(j')");
    discrVars.back().SetBinning(binsUnitDomain.begin(), binsUnitDomain.end());
    
    discrVars.emplace_back("MinCSVBJetsHiggs", "min CSV(j_{h})");
    discrVars.back().SetBinning(binsUnitDomain.begin(), binsUnitDomain.end());
    */
    
    
    // The common event weight and selection
    string const commonWeight("Weight");
    string const sgnWeight("1");
    string const bkgWeight("1");
    
    
    // Define and create directories to store the output
    string const outputDir("fig_thesis/");
    struct stat dirStat;
    
    if (stat(outputDir.c_str(), &dirStat) != 0)  // the directory does not exist
        mkdir(outputDir.c_str(), 0755);
    if (stat((outputDir + "png/").c_str(), &dirStat) != 0)
        mkdir((outputDir + "png/").c_str(), 0755);
    if (stat((outputDir + "pdf/").c_str(), &dirStat) != 0)
        mkdir((outputDir + "pdf/").c_str(), 0755);
    if (stat((outputDir + "C/").c_str(), &dirStat) != 0)
        mkdir((outputDir + "C/").c_str(), 0755);
    if (stat((outputDir + "root/").c_str(), &dirStat) != 0)
        mkdir((outputDir + "root/").c_str(), 0755);
    
    
    // Define the input files. For the sake of generality, files containing signal and background
    //are treated as if they are independent
    string const filePrefix("../Train/tuples/");
    list<string> const sgnFileNames = {"sgn_train.root", "sgn_exam.root"};
    list<string> const bkgFileNames = {"bkg_train.root", "bkg_exam.root"};
    string const treeName("Vars");
    
    
    // Construct binnings for the continuous variables
    cout << "Construction of adaptive binnings started.\n";
    FillBinners(filePrefix, sgnFileNames, treeName, commonWeight, sgnWeight, -1, contVars);
    cout << "Adaptive binnings have been filled with signal events.\n";
    FillBinners(filePrefix, bkgFileNames, treeName, commonWeight, bkgWeight, -1, contVars);
    cout << "Adaptive binnings have been filled with background events.\n";
    
    
    /*
    for (auto const &b: binnings)
    {
        cout << "Binning: ";
        
        for (auto &edge: b.binner.GetBinBoundaries())
            cout << edge << ", ";
        
        cout << '\n';
    }
    */
    
    // Constuct performance-evaluating objects for the continuous variables
    vector<VarSeparation> separations;
    
    for (auto const &v: contVars)
        separations.emplace_back(v, v.GetBinning());
    
    for (auto const &v: discrVars)
        separations.emplace_back(v, v.GetBinning());
    
    
    // Prepare for drawing
    gStyle->SetHistLineWidth(2);
    gErrorIgnoreLevel = kWarning;  // suppress info messages
    
    TCanvas canvas("canvas", "canvas", 1200, 1000);
    canvas.SetTicks();
    
    // TPad mainPad("mainPad", "", 0., 0., 0.9, 1., 0);
    // mainPad.SetTicks(1, 1);
    // mainPad.SetGrid();
    // mainPad.Draw();
    
    // Change position of the common factor for the y axis
    TGaxis::SetExponentOffset(-0.06, 0.02, "y");
    
    
    // CMS label
    //TLatex cmsLabel(0.25, 0.89, "#font[62]{CMS} Simulation Preliminary");  // leave a room for common exponent
    TLatex cmsLabel(0.20, 0.89, "#scale[1.2]{#font[62]{CMS}} #font[52]{Simulation Preliminary}");
    cmsLabel.SetNDC();
    cmsLabel.SetTextFont(42);
    cmsLabel.SetTextSize(0.05);  // was 0.06
    cmsLabel.SetTextAlign(11);

    TLatex energyLabel(0.95, 0.89, "8 TeV");
    energyLabel.SetNDC();
    energyLabel.SetTextFont(42);
    energyLabel.SetTextSize(0.05);  // was 0.06
    energyLabel.SetTextAlign(31);
    
    
    // Fill performance-evaluating objects
    cout << "Filling of performance-evaluating objects started.\n";
    FillSeparations(filePrefix, sgnFileNames, treeName, commonWeight, sgnWeight,
     separations, true);
    cout << "Signal files have been processed.\n";
    FillSeparations(filePrefix, bkgFileNames, treeName, commonWeight, bkgWeight,
     separations, false);
    cout << "Background files have been processed.\n";
    
    
    // Draw the variables and fill discrimination criteria
    vector<DiscrCriteria> criteria;
    
    for (auto &sep: separations)
    {
        // Get and adjust the histograms
        THStack hStack("hStack",
         (";" + sep.var.GetTitleDim('[') + ";Probability density [a. u.]").c_str());
        
        TH1D &sgnHist = sep.perf.GetSgnHist();
        sgnHist.Scale(1. / sgnHist.Integral(0, -1), "width");
        sgnHist.SetLineColor(kBlue);
        sgnHist.SetFillColor(38);
        sgnHist.SetFillStyle(1001);
        hStack.Add(&sgnHist, "hist");
        
        TH1D &bkgHist = sep.perf.GetBkgHist();
        bkgHist.Scale(1. / bkgHist.Integral(0, -1), "width");
        bkgHist.SetLineColor(kRed);
        bkgHist.SetFillColor(kRed);
        bkgHist.SetFillStyle(3354);
        hStack.Add(&bkgHist, "hist");
        
        
        // mainPad.cd();
        hStack.Draw("nostack");
        hStack.SetMinimum(0.);
        // hStack.GetYaxis()->SetTitleOffset(1.5);
        
        canvas.Update();  // needed to convert coordinates below
        
        
        // Find a suitable place for the legend
        double const legendWidth = 0.15;
        double const legendHeight = 0.12;
        double const legendInnerMargin = 0.03;  // distance between legend and frame
        
        bool legendRightSide = true;  // legend can be placed in upper left or right corner
        double yRescaleFactor = 1.;
        //^ It might be needed to reset maximum of hStack to give some place for the legend
        
        // First try the right corner
        double xEdgeSpace = undc::UtoX(canvas, 1. - canvas.GetRightMargin() - legendWidth -
         2 * legendInnerMargin);  // left edge of the region to host the legend
        double const yEdgeSpace = undc::VtoY(canvas, 1. - canvas.GetTopMargin() - legendHeight -
         2 * legendInnerMargin);
        double maxY = 0.;  // will be maximal y of a histogram bin that gets into the region
        
        for (auto const &h: {&sgnHist, &bkgHist})
            for (int bin = h->FindFixBin(xEdgeSpace); bin <= h->GetNbinsX(); ++bin)
            {
                double const y = h->GetBinContent(bin);
                
                if (y > maxY)
                    maxY = y;
            }
        
        if (maxY > yEdgeSpace)
        //^ If not, the legend can be hosted at the right corner without a problem
        {
            yRescaleFactor = maxY / yEdgeSpace;  // assuming hStack.GetMiminum() == 0.
            
            // Since would need to reset the maximum to place the legend in the right corner, check
            //the left corner. Maybe, there is enough place there
            xEdgeSpace = undc::UtoX(canvas, canvas.GetLeftMargin() + legendWidth +
             2 * legendInnerMargin);
            maxY = 0.;
            
            for (auto const &h: {&sgnHist, &bkgHist})
            {
                int maxBin = h->FindFixBin(xEdgeSpace);
                
                for (int bin = 1; bin <= maxBin; ++bin)
                {
                    double const y = h->GetBinContent(bin);
                    
                    if (y > maxY)
                        maxY = y;
                }
            }
            
            
            // Check if the left corner is better
            double const proposedRescaleFactor = max(maxY / yEdgeSpace, 1.);
            
            if (proposedRescaleFactor < yRescaleFactor)
            {
                legendRightSide = false;
                yRescaleFactor = proposedRescaleFactor;
            }
        }
        
        
        // Reset the maximum to host the legend. There is also an additional margin between the
        //histogram and the upper axis of the pad, which is accounted for. The statement below
        //exploits the fact that the histogram miminum is set to zero
        hStack.SetMaximum(hStack.GetMaximum("nostack") * (1. + gStyle->GetHistTopMargin()) *
         yRescaleFactor);
        
        
        // Create the legend
        double const legendX = ((legendRightSide) ?
         1. - canvas.GetRightMargin() - legendWidth - legendInnerMargin :
         canvas.GetLeftMargin() + legendInnerMargin);
        
        TLegend legend(legendX, 1. - canvas.GetTopMargin() - legendInnerMargin - legendHeight,
         legendX + legendWidth, 1. - canvas.GetTopMargin() - legendInnerMargin);
        legend.SetFillColor(kWhite);
        legend.SetTextSize(0.04);
        
        // legend.SetHeader(" sgn vs bkg ");
        legend.AddEntry(&sgnHist, " tHq ", "f");
        legend.AddEntry(&bkgHist, " bkg ", "f");
        
        
        // Print under- and overflows. Note that the integrals of both histograms are unit
        unsigned const actualNBins = sgnHist.GetNbinsX();
        ostringstream overflowLabelText;
        overflowLabelText << "Underflow: #color[4]{" <<
         int(100. * sgnHist.GetBinContent(0) * sgnHist.GetBinWidth(0)) << "%} / #color[2]{" <<
         int(100. * bkgHist.GetBinContent(0) * bkgHist.GetBinWidth(0)) <<
         "%},  Overflow: #color[4]{" <<
         int(100. * sgnHist.GetBinContent(actualNBins + 1) *
          sgnHist.GetBinWidth(actualNBins + 1)) << "%} / #color[2]{" <<
         int(100. * bkgHist.GetBinContent(actualNBins + 1) *
          bkgHist.GetBinWidth(actualNBins + 1)) << "%}";
        //^ Widths of under- and overflow bins are set to widths of the neighbouring bins. When a
        //histogram is rescaled with the "width" option, bin contents are divided by the bin widts,
        //and thus here they should be multiplied by the widths
        
        TLatex overflowLabel(0.96, 0.13, overflowLabelText.str().c_str());
        overflowLabel.SetNDC();
        overflowLabel.SetTextAngle(90);
        overflowLabel.SetTextFont(42);
        overflowLabel.SetTextSize(0.04);
        overflowLabel.SetTextAlign(13);
        
        
        // canvas.cd();
        
        cmsLabel.Draw();
        energyLabel.Draw();
        legend.Draw();
        overflowLabel.Draw();
        
        
        
        // Print to files
        string const outFileName(sep.var.GetEscapedName());
        canvas.Print((outputDir + "png/" + outFileName + ".png").c_str());
        canvas.Print((outputDir + "pdf/" + outFileName + ".pdf").c_str());
        canvas.Print((outputDir + "C/" + outFileName + ".C").c_str());
        canvas.Print((outputDir + "root/" + outFileName + ".root").c_str());
        
        
        // Fill discrimination criteria
        criteria.emplace_back(sep.var.GetFormula(), sep.var.GetTitle());
        criteria.back().complArea = 1. - sep.perf.GetCrossArea();
        criteria.back().ksStat = sep.perf.GetKSStatistics();
        criteria.back().symChi2 = sep.perf.GetSymmetricChi2();
        criteria.back().jsDiv = sep.perf.GetJensenShannonDivergence();
    }
    
    
    // Print discrimination criteria
    sort(criteria.rbegin(), criteria.rend());
    
    cout << '\n';
    cout << "Formula   Title   (1 - CrossArea)   KS statistis   Sym. chi2   Jensen-Shannon\n";
    cout << fixed;
    
    for (auto const &c: criteria)
        c.Print();
    cout << '\n';
    
}


int main()
{
    plotInputVars();
    
    return 0;
}
