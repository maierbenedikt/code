////////////////////////////////////////////////////////////////////////////////
//
// TTbarHFFinder
// --------
//
//            09/04/2014 Simon Fink <simon.fink@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/TTbarHFFinder.h"
#include <Math/VectorUtil.h>

#include <iostream>
#include <math.h>
#include <vector>


using namespace std;


TTbarHFFinder::TTbarHFFinder(TTree* _itree, KITAJets* _mykitajets):
  matchConeSize(0.5)
{
  itree = _itree;
  
  isUnknown = true;
  isTwoBottom = false; 
  isDoubleBottomJet = false;
  isSingleBottom = false;   
  isTwoCharm = false;       
  isDoubleCharmJet = false; 
  isSingleCharm = false;    
  isUnmatchedBottom = false;
  isUnmatchedCharm = false; 
  isNoHF = false;

  mykitagenstatus2 = new KITAGenParticles();
  mykitagenstatus3 = new KITAGenParticles();
  mykitajets = _mykitajets;
  itree->SetBranchAddress("KITAGenFinalState_status2", &mykitagenstatus2);
  itree->SetBranchAddress("KITAGenFinalState_status3", &mykitagenstatus3);
  //  itree->SetBranchAddress("KITAJets", &mykitajets);

  unmatchedPartonCount= {0, 0};

}


Bool_t TTbarHFFinder::FindHF()
{

  // Fill the collection of pointers to additional partons with appropriate partons
  additionalPartons.clear();
  for (unsigned i=0; i < mykitagenstatus2->size(); ++i)
    {
      KITAGenParticle & p = mykitagenstatus2->at(i);
      //cout << "AGAIN FEST " << p.pid << "  of total " << mykitagenstatus2->size() << endl;
      int const absPdgId = abs(p.pid);
        
      if (absPdgId != 4 && absPdgId != 5)  // skip all but b and c quarks
	//cout <<"Skipped LF particle"<< endl;
	continue;
	
      //cout << "DID NOT SKIP HF  PID: " << p.pid << endl;
	if (p.MoID == 2212)
	  //cout << "Skipped Beam particle" << endl;
	continue;
      //^ Skip immediate daughters of beam particles
        
        
      additionalPartons.push_back(p);
    }
    
    
  // Find quarks whose descendants should be removed from the above list. They are quarks produced
  //in electroweak interactions, namely b quarks from decays of tops and c quarks from decays of
  //W bosons
  ewkPartons.clear();
  unsigned numBTopsFound = 0;
    
  for (unsigned i=0; i < mykitagenstatus3->size(); ++i)
    {
      KITAGenParticle & p = mykitagenstatus3->at(i);
      if (abs(p.pid) == 5 and abs(p.MoID) == 6)
	//^ b quarks from decays of tops
        {
	  //cout << "B from top found   " <<endl; 
	  ewkPartons.push_back(p);
	  ++numBTopsFound;
        }
        
      if (abs(p.pid) == 4 and abs(p.MoID) == 24)
        //^ Charms from decays of W bosons from top quarks
	{
	  ewkPartons.push_back(p);
	  //  cout << "Found Charm from W decay" << endl;
	}
    }
  //  cout << " Found a total of " << numBTopsFound << " b quarks from the Top" << endl;
  
  // A sanity check
  if (numBTopsFound != 2)
    throw runtime_error("TTbarHFFinder::ProcessEvent: Failed to find two b quarks from top decays.");
    
    
  // Remove from the collection of additional partons those partons that are closest to the quarks
  //produced in electroweak interactions
  //cout << "################" << endl << "   Size of ewk partons : " << ewkPartons.size() << "    Size of additional partons: " << additionalPartons.size() << endl;
  
  for (unsigned i=0; i < ewkPartons.size(); ++i)
    {   
      vector<KITAGenParticle>::iterator closestPartonIt = additionalPartons.end();
      KITAGenParticle & ewkParton = ewkPartons.at(i);
      double minDistance = 9999;
        
      for (vector<KITAGenParticle>::iterator it = additionalPartons.begin(); it != additionalPartons.end(); ++it)
        {
	  if ((*it).pid != ewkParton.pid)
	    {
	      //cout << "ungleich" << endl;
	      continue;
            }
	    
	  //	  cout << "GLEICH!!!" << endl;
	  double const dR = ROOT::Math::VectorUtil::DeltaR(ewkParton.vec,(*it).vec);
          //cout << "Computed dR = " <<   dR << endl;
	  if (dR < minDistance)
	    {
	      closestPartonIt = it;
	      minDistance = dR;
	    }
        }
        
      // Sanity check
      if (closestPartonIt == additionalPartons.end())
      	throw runtime_error("ClassTTbarHFPlugin::ProcessEvent: Cannot match a status-3 quark "
			    "produced in electroweak interaction to any quark from the parton shower.");
        
      //cout << "Removing Parton with dR= " << minDistance << endl;  
      // Remove this parton
      additionalPartons.erase(closestPartonIt);
    }
    
  
  // Now match reconstructed jets to remaining partons
  
  partonCounts.clear();
  unmatchedPartonCount = {0,0};
  
  for (unsigned iJet = 0; iJet < mykitajets->size(); ++iJet)
    partonCounts.push_back(HFCounter{0, 0});
    
  for (unsigned i=0; i < additionalPartons.size(); ++i)
    {
      KITAGenParticle & p = additionalPartons.at(i);
      // Find index of the closest jet
      unsigned iClosestJet = -1;
      double minDistance = 9999;
      
      for (unsigned iJet = 0; iJet < mykitajets->size(); ++iJet)
        {
	  KITAJet & kjet = mykitajets->at(iJet);
	  double const dR = ROOT::Math::VectorUtil::DeltaR(kjet.vec,p.vec);
	  //cout << "!!!!!!!!!!Computed dR = " <<   dR << endl;
	  
	  if (dR < minDistance)
            {
	      iClosestJet = iJet;
	      minDistance = dR;
            }
        }
      
      //  cout << endl << "Closest Jet : " << iClosestJet << " with conesize " << matchConeSize << " and dR = " << minDistance << "and pi = " << p.pid << endl;

      // Check if the closest jet is close enough
      if (minDistance < matchConeSize)
        {
	  if (abs(p.pid) == 5)
	    ++partonCounts.at(iClosestJet).nb;
	  else
	    ++partonCounts.at(iClosestJet).nc;
        }
      else
	{
	  if (abs(p.pid) == 5)
	    ++unmatchedPartonCount.nb;
	  else
	    ++unmatchedPartonCount.nc;
        }
      
    }
      
  //cout << "||||  " << unmatchedPartonCount.nb << "  ||  " << unmatchedPartonCount.nc << endl;
  // Count different types of jets
  unsigned nJetB = 0, nJetDoubleB = 0, nJetC = 0, nJetDoubleC = 0;
    
  for (unsigned i=0; i < partonCounts.size(); ++i)
    {
      if (partonCounts.at(i).nb > 0)
	++nJetB;
        
      if (partonCounts.at(i).nb > 1)
	++nJetDoubleB;
        
      if (partonCounts.at(i).nc > 0)
	++nJetC;
        
      if (partonCounts.at(i).nc > 1)
	++nJetDoubleC;

      //cout << "^^^ Jet " << i << "   nb = " << partonCounts.at(i).nb << "    nc = " << partonCounts.at(i).nc << " ^^^" << endl;
    }
    
  //cout << "<<< Jets with min 1 b: " << nJetB << "    Jets with min 2 b: " << nJetDoubleB << "    Jets with min 1 c: " << nJetC << "    Jets with min 2 c: " << nJetDoubleC << " >>> " << endl;
    
  isUnknown = true;
  isTwoBottom = false;
  isDoubleBottomJet = false;
  isSingleBottom = false;
  isTwoCharm = false;
  isDoubleCharmJet = false;
  isSingleCharm = false;
  isUnmatchedBottom = false;
  isUnmatchedCharm = false;
  isNoHF = false;



  // Finally, classify the event
  if (nJetB >= 2)
    isTwoBottom = true;
  else if (nJetDoubleB > 0)
    isDoubleBottomJet = true;
  else if (nJetB > 0)
    isSingleBottom = true;
  else if (nJetC >= 2)
    isTwoCharm = true;
  else if (nJetDoubleC > 0)
    isDoubleCharmJet = true;
  else if (nJetC > 0)
    isSingleCharm = true;
  else if (unmatchedPartonCount.nb > 0)
    isUnmatchedBottom = true;
  else if (unmatchedPartonCount.nc > 0)
    isUnmatchedCharm = true;
  else
    isNoHF = true;
  
  //cout << "####### ENDGUELTIGE ENTSCHEIDUNG: " << GetDecision() << endl << endl; 
  return true; 
}


Int_t TTbarHFFinder::GetDecision()
{
  if (isTwoBottom) return 1; 
  if (isDoubleBottomJet) return 2; 
  if (isSingleBottom) return 3; 
  if (isTwoCharm) return 4; 
  if (isDoubleCharmJet) return 5; 
  if (isSingleCharm) return 6; 
  if (isUnmatchedBottom) return 7; 
  if (isUnmatchedCharm) return 8; 
  if (isNoHF) return 9;

  else return 0;
}