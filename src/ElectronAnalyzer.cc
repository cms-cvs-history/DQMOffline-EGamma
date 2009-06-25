// -*- C++ -*-
//
// Package:    DQMOffline/EGamma
// Class:      ElectronAnalyzer
// 
/**\class ElectronAnalyzer  DQMOffline/EGamma/src/ElectronAnalyzer.cc

 Description: GsfElectrons analyzer using reco data

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Claude Charlot
//         Created:  Mon Mar 27 13:22:06 CEST 2006
// $Id: ElectronAnalyzer.cc,v 1.9 2009/06/24 14:52:32 dellaric Exp $
//
//

// user include files
#include "DQMOffline/EGamma/interface/ElectronAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include <iostream>
#include "TMath.h"
#include <iostream>

using namespace reco;
 
ElectronAnalyzer::ElectronAnalyzer(const edm::ParameterSet& conf)
{

  outputFile_ = conf.getParameter<std::string>("outputFile");
  verbosity_ = conf.getUntrackedParameter<int>("Verbosity");
  electronCollection_=conf.getParameter<edm::InputTag>("electronCollection");
  matchingObjectCollection_ = conf.getParameter<edm::InputTag>("matchingObjectCollection");
  maxPt_ = conf.getParameter<double>("MaxPt");
  maxAbsEta_ = conf.getParameter<double>("MaxAbsEta");
  deltaR_ = conf.getParameter<double>("DeltaR");
  etamin=conf.getParameter<double>("Etamin");
  etamax=conf.getParameter<double>("Etamax");
  phimin=conf.getParameter<double>("Phimin");
  phimax=conf.getParameter<double>("Phimax");
  ptmax=conf.getParameter<double>("Ptmax");
  pmax=conf.getParameter<double>("Pmax");
  eopmax=conf.getParameter<double>("Eopmax");
  eopmaxsht=conf.getParameter<double>("Eopmaxsht");
  detamin=conf.getParameter<double>("Detamin");
  detamax=conf.getParameter<double>("Detamax");
  dphimin=conf.getParameter<double>("Dphimin");
  dphimax=conf.getParameter<double>("Dphimax");
  detamatchmin=conf.getParameter<double>("Detamatchmin");
  detamatchmax=conf.getParameter<double>("Detamatchmax");
  dphimatchmin=conf.getParameter<double>("Dphimatchmin");
  dphimatchmax=conf.getParameter<double>("Dphimatchmax");
  fhitsmax=conf.getParameter<double>("Fhitsmax");
  lhitsmax=conf.getParameter<double>("Lhitsmax");
  nbineta=conf.getParameter<int>("Nbineta");
  nbineta2D=conf.getParameter<int>("Nbineta2D");
  nbinp=conf.getParameter<int>("Nbinp");
  nbinpt=conf.getParameter<int>("Nbinpt");
  nbinp2D=conf.getParameter<int>("Nbinp2D");
  nbinpt2D=conf.getParameter<int>("Nbinpt2D");
  nbinpteff=conf.getParameter<int>("Nbinpteff");
  nbinphi=conf.getParameter<int>("Nbinphi");
  nbinphi2D=conf.getParameter<int>("Nbinphi2D");
  nbineop=conf.getParameter<int>("Nbineop");
  nbineop2D=conf.getParameter<int>("Nbineop2D");
  nbinfhits=conf.getParameter<int>("Nbinfhits");
  nbinlhits=conf.getParameter<int>("Nbinlhits");
  nbinxyz=conf.getParameter<int>("Nbinxyz");
  nbindeta=conf.getParameter<int>("Nbindeta");
  nbindphi=conf.getParameter<int>("Nbindphi");
  nbindetamatch=conf.getParameter<int>("Nbindetamatch");
  nbindphimatch=conf.getParameter<int>("Nbindphimatch");
  nbindetamatch2D=conf.getParameter<int>("Nbindetamatch2D");
  nbindphimatch2D=conf.getParameter<int>("Nbindphimatch2D");
  parameters_ = conf;
}  
  
ElectronAnalyzer::~ElectronAnalyzer()
{
 
}

void ElectronAnalyzer::beginJob(edm::EventSetup const&iSetup){

  dbe_ = 0;
  dbe_ = edm::Service<DQMStore>().operator->();
  
 if (dbe_) {
    if (verbosity_ > 0 ) {
      dbe_->setVerbose(1);
    } else {
      dbe_->setVerbose(0);
    }
  }
  if (dbe_) {
    if (verbosity_ > 0 ) dbe_->showDirStructure();
  }
  
  
  // matching object
  std::string::size_type locSC = matchingObjectCollection_.label().find( "SuperCluster", 0 );
  std::string type_;
  if ( locSC != std::string::npos ) {
    //std::cout << "Matching objects are SuperClusters "<< std::endl;
    type_ = "SC";
  } else {
    //std::cout << "Didn't recognize input matching objects!! " << std::endl; 
  }
  
  if (dbe_) {
    dbe_->setCurrentFolder("Egamma/ElectronAnalyzer");
  
    std::string htitle, hlabel;
    // matching object distributions 
    hlabel="h_"+type_+"_eta"; htitle=type_+" #eta";
    h_ele_matchingObjectEta             = dbe_->book1D( hlabel.c_str(), htitle.c_str(), nbineta,etamin,etamax); 
    hlabel="h_"+type_+"_Pt"; htitle=type_+" pt";
    h_ele_matchingObjectPt               = dbe_->book1D( hlabel.c_str(),htitle.c_str(),            nbinpteff,5.,ptmax); 
    hlabel="h_"+type_+"_phi"; htitle=type_+" phi";
    h_ele_matchingObjectPhi               = dbe_->book1D( hlabel.c_str(), htitle.c_str(),        nbinphi,phimin,phimax); 
    hlabel="h_"+type_+"_z"; htitle=type_+" z";
    h_ele_matchingObjectZ      = dbe_->book1D( hlabel.c_str(), htitle.c_str(),    50, -25, 25 );


    // matched electrons
    h_ele_charge         = dbe_->book1D( "h_ele_charge",         "ele charge",             5,-2.,2.);   
    h_ele_vertexP        = dbe_->book1D("h_ele_vertexP",        "ele p at vertex",       nbinp,0.,pmax);
    h_ele_vertexPt       = dbe_->book1D( "h_ele_vertexPt",       "ele p_{T} at vertex",  nbinpt,0.,ptmax);

    h_ele_vertexEta      = dbe_->book1D( "h_ele_vertexEta",      "ele #eta at vertex",    nbineta,etamin,etamax);

    h_ele_vertexPhi      = dbe_->book1D( "h_ele_vertexPhi",      "ele #phi at vertex",    nbinphi,phimin,phimax);
    h_ele_vertexX      = dbe_->book1D( "h_ele_vertexX",      "ele x at vertex",    nbinxyz,-0.1,0.1 );
    h_ele_vertexY      = dbe_->book1D( "h_ele_vertexY",      "ele y at vertex",    nbinxyz,-0.1,0.1 );
    h_ele_vertexZ      = dbe_->book1D( "h_ele_vertexZ",      "ele z at vertex",    nbinxyz,-25, 25 );
    h_ele_matchingObjectPt_matched   = dbe_->book1D( "h_ele_matchingObjectPt_matched",       "matching SC p_{T}",  nbinpteff,5.,ptmax);
    h_ele_ptEff  = dbe_->book1D( "h_ele_ptEff",       "fraction of reco ele matching a reco SC",  nbinpteff,5.,ptmax);

    h_ele_matchingObjectEta_matched  = dbe_->book1D( "h_ele_matchingObjectEta_matched",      "matching SC #eta",    nbineta,etamin,etamax);
    h_ele_etaEff     = dbe_->book1D( "h_ele_etaEff", "fraction of reco ele matching a reco SC",    nbineta,etamin,etamax);

    h_ele_matchingObjectPhi_matched  = dbe_->book1D( "h_ele_matchingObjectPhi_matched", "matching SC phi",        nbinphi,phimin,phimax); 
    h_ele_phiEff     = dbe_->book1D( "h_ele_phiEff", "fraction of reco ele matching a reco SC",  nbinphi,phimin,phimax);

    h_ele_matchingObjectZ_matched    = dbe_->book1D( "h_ele_matchingObjectZ_matched",      "matching SC z",    nbinxyz,-25,25);
    h_ele_zEff    = dbe_->book1D( "h_ele_zEff",      "fraction of reco ele matching a reco SC",    nbinxyz,-25,25);

    // matched electron, gsf tracks
    h_ele_foundHits      = dbe_->book1D( "h_ele_foundHits",      "ele track # found hits",      nbinfhits,0.,fhitsmax);
    h_ele_chi2           = dbe_->book1D( "h_ele_chi2",           "ele track #chi^{2}/ndf",         100,0.,15.);   
    h_ele_PinMnPout_mode      = dbe_->book1D( "h_ele_PinMnPout_mode",      "ele track inner p - outer p, mode"   ,nbinp,0.,100.);
  
    // matched electrons, matching 

  //    h_ele_EoP            = dbe_->book1D( "h_ele_EoP",            "ele E/P_{vertex}",        nbineop,0.,eopmax);
    h_ele_EoP            = dbe_->book1D( "h_ele_EoP",            "ele E_{sc}/P_{vertex}" ,        nbineop,0.,eopmax);
    //    h_ele_EoPout         = dbe_->book1D( "h_ele_EoPout",         "ele E/P_{out}",           nbineop,0.,eopmax);
    h_ele_EoPout         = dbe_->book1D( "h_ele_EoPout",         "ele E_{seed}/P_{out}",           nbineop,0.,eopmax);
    h_ele_dEtaSc_propVtx = dbe_->book1D( "h_ele_dEtaSc_propVtx", "ele #eta_{sc} - #eta_{tr},prop from vertex",      nbindetamatch,detamatchmin,detamatchmax);
    h_ele_dPhiSc_propVtx = dbe_->book1D( "h_ele_dPhiSc_propVtx", "ele #phi_{sc} - #phi_{tr},prop from vertex",      nbindphimatch,dphimatchmin,dphimatchmax);
    h_ele_dPhiCl_propOut = dbe_->book1D( "h_ele_dPhiCl_propOut", "ele #phi_{cl} - #phi_{tr},prop from outermost",   nbindphimatch,dphimatchmin,dphimatchmax);
    h_ele_HoE = dbe_->book1D("h_ele_HoE", "ele H/E", 55,-0.05,0.5) ;
 
    // classes  
    h_ele_classes = dbe_->book1D( "h_ele_classes", "ele electron classes",      150,0.0,150.);
    h_ele_eta = dbe_->book1D( "h_ele_eta", "ele electron eta",  nbineta/2,0.0,etamax);
    h_ele_eta_golden = dbe_->book1D( "h_ele_eta_golden", "ele electron eta golden",  nbineta/2,0.0,etamax);
    h_ele_eta_goldenFrac = dbe_->book1D( "h_ele_eta_goldenFrac", "fraction of golden electrons",  nbineta/2,0.0,etamax);
    h_ele_eta_shower = dbe_->book1D( "h_ele_eta_show", "ele electron eta showering",  nbineta/2,0.0,etamax);
    h_ele_eta_showerFrac = dbe_->book1D( "h_ele_eta_showerFrac", "fraction of showering electrons",  nbineta/2,0.0,etamax);
    h_ele_zEff       = dbe_->book1D( "h_ele_zEff",   "matching SC z",    nbinxyz,-25,25);  }

    // histos titles
    h_ele_matchingObjectEta             -> setAxisTitle("SC #eta",1);
    h_ele_vertexP        -> setAxisTitle("GeV/c",1);
    h_ele_vertexPt       -> setAxisTitle("GeV/c",1);
    h_ele_vertexX        -> setAxisTitle("cm",1);
    h_ele_vertexY        -> setAxisTitle("cm",1);
    h_ele_vertexZ        -> setAxisTitle("cm",1);
    h_ele_vertexPhi      -> setAxisTitle("rad",1);   
    h_ele_PinMnPout_mode      -> setAxisTitle("GeV/c",1);
    h_ele_dPhiCl_propOut ->setAxisTitle("rad",1);
    h_ele_dPhiSc_propVtx ->setAxisTitle("rad",1);
    h_ele_etaEff         ->setAxisTitle("#eta",1);
    h_ele_eta_goldenFrac ->setAxisTitle("#eta",1);
    h_ele_eta_showerFrac ->setAxisTitle("#eta",1);
    h_ele_phiEff         ->setAxisTitle("#phi (rad)",1);
    h_ele_ptEff          ->setAxisTitle("p_{T} (GeV/c)",1);
    h_ele_zEff           ->setAxisTitle("cm",1);
}     

void
ElectronAnalyzer::endJob(){

  h_ele_eta = dbe_->get("Egamma/ElectronAnalyzer/h_ele_etaEff");
  h_ele_eta_golden = dbe_->get("Egamma/ElectronAnalyzer/h_ele_eta_golden");
  h_ele_eta_shower = dbe_->get("Egamma/ElectronAnalyzer/h_ele_eta_shower");
  h_ele_eta_goldenFrac = dbe_->get("Egamma/ElectronAnalyzer/h_ele_eta_goldenFrac");
  h_ele_eta_showerFrac = dbe_->get("Egamma/ElectronAnalyzer/h_ele_eta_showerFrac");

  dbe_->setCurrentFolder("Egamma/ElectronAnalyzer");
  if (h_ele_eta) {
    int nbins=h_ele_eta->getNbinsX();
    for (int nb=0;nb<nbins;++nb) {
      float content=h_ele_eta->getBinContent(nb);
      if (content>0.) {
        if (h_ele_eta_golden) {
          float contgold =( h_ele_eta_golden->getBinContent(nb))/content;
          if (h_ele_eta_shower) {
            float contshow =( h_ele_eta_shower->getBinContent(nb))/content;
            if (h_ele_eta_goldenFrac) h_ele_eta_goldenFrac ->setBinContent(nb,contgold);
            if (h_ele_eta_showerFrac) h_ele_eta_showerFrac ->setBinContent(nb,contshow);
          }
        }
      }
    }
  }

  h_ele_matchingObjectEta = dbe_->get("Egamma/ElectronAnalyzer/h_ele_matchingObjectEta");
  h_ele_etaEff = dbe_->get("Egamma/ElectronAnalyzer/h_ele_etaEff");

  if (h_ele_matchingObjectEta) {
    int nbins=h_ele_matchingObjectEta->getNbinsX();
    for (int nb=0;nb<nbins;++nb) {
      float content=h_ele_matchingObjectEta->getBinContent(nb);
      if (content>0.) {
        if (h_ele_etaEff) h_ele_etaEff ->setBinContent(nb,h_ele_matchingObjectEta_matched->getBinContent(nb)/content);
      }
    }
  }

  h_ele_matchingObjectPt = dbe_->get("Egamma/ElectronAnalyzer/h_ele_matchingObjectPt");
  h_ele_ptEff = dbe_->get("Egamma/ElectronAnalyzer/h_ele_ptEff");

  if (h_ele_matchingObjectPt) {
    int nbins=h_ele_matchingObjectPt->getNbinsX();
    for (int nb=0;nb<nbins;++nb) {
      float content=h_ele_matchingObjectPt->getBinContent(nb);
      if (content>0.) {
        if (h_ele_ptEff) h_ele_ptEff ->setBinContent(nb,h_ele_matchingObjectPt_matched->getBinContent(nb)/content);
      }
    }
  }

  h_ele_matchingObjectPhi = dbe_->get("Egamma/ElectronAnalyzer/h_ele_matchingObjectPhi");
  h_ele_phiEff = dbe_->get("Egamma/ElectronAnalyzer/h_ele_phiEff");

  if (h_ele_matchingObjectPhi) {
    int nbins=h_ele_matchingObjectPhi->getNbinsX();
    for (int nb=0;nb<nbins;++nb) {
      float content=h_ele_matchingObjectPhi->getBinContent(nb);
      if (content>0.) {
        if (h_ele_phiEff) h_ele_phiEff ->setBinContent(nb,h_ele_matchingObjectPhi_matched->getBinContent(nb)/content);
      }
    }
  }

  h_ele_matchingObjectZ = dbe_->get("Egamma/ElectronAnalyzer/h_ele_matchingObjectZ");
  h_ele_zEff = dbe_->get("Egamma/ElectronAnalyzer/h_ele_zEff");

  if (h_ele_matchingObjectZ) {
    int nbins=h_ele_matchingObjectZ->getNbinsX();
    for (int nb=0;nb<nbins;++nb) {
      float content=h_ele_matchingObjectZ->getBinContent(nb);
      if (content>0.) {
        if (h_ele_zEff) h_ele_zEff ->setBinContent(nb,h_ele_matchingObjectZ_matched->getBinContent(nb)/content);
      }
    }
  }


  bool outputMEsInRootFile = parameters_.getParameter<bool>("OutputMEsInRootFile");
  std::string outputFileName = parameters_.getParameter<std::string>("outputFile");
  
  if(outputMEsInRootFile){
    dbe_->save(outputFile_);
  }
}

void
ElectronAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // get reco electrons  
  edm::Handle<reco::GsfElectronCollection> gsfElectrons;
  iEvent.getByLabel(electronCollection_,gsfElectrons); 
  edm::LogInfo("")<<"\n\n =================> Treating event "<<iEvent.id()<<" Number of electrons "<<gsfElectrons.product()->size();

  // get reco supercluster collection
  edm::Handle<reco::SuperClusterCollection> recoClusters;
  iEvent.getByLabel(matchingObjectCollection_,recoClusters);

  // association matching object-reco electrons
  int matchingObjectNum=0;

  for ( reco::SuperClusterCollection::const_iterator moIter=recoClusters->begin();
   moIter!=recoClusters->end(); moIter++ ) {
    
    // number of matching objects
    matchingObjectNum++;

      if (moIter->energy()/cosh(moIter->eta())> maxPt_ || fabs(moIter->eta())> maxAbsEta_) continue;
      
      h_ele_matchingObjectEta -> Fill( moIter->eta() );
      h_ele_matchingObjectPt   -> Fill( moIter->energy()/cosh(moIter->eta()) );
      h_ele_matchingObjectPhi   -> Fill( moIter->phi() );
      h_ele_matchingObjectZ   -> Fill(  moIter->z() );
     	
       // looking for the best matching gsf electron
      bool okGsfFound = false;
      double gsfOkRatio = 999999.;

      // find best matched electron
      reco::GsfElectron bestGsfElectron;
      for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin();
       gsfIter!=gsfElectrons->end(); gsfIter++){
	
	double deltaR = sqrt(pow((gsfIter->eta()-moIter->eta()),2) + pow((gsfIter->phi()-moIter->phi()),2));
	if ( deltaR < deltaR_ ){
	//if ( (genPc->pdg_id() == 11) && (gsfIter->charge() < 0.) || (genPc->pdg_id() == -11) &&
	//(gsfIter->charge() > 0.) ){
	  double tmpGsfRatio = gsfIter->p()/moIter->energy();
	  if ( fabs(tmpGsfRatio-1) < fabs(gsfOkRatio-1) ) {
	    gsfOkRatio = tmpGsfRatio;
	    bestGsfElectron=*gsfIter;
	    okGsfFound = true;
	  } 
	//} 
	} 
      } // loop over rec ele to look for the best one	

      // analysis when the matching object is matched by a rec electron
     if (okGsfFound){
	// electron related distributions
	h_ele_charge        -> Fill( bestGsfElectron.charge() );
	h_ele_vertexP       -> Fill( bestGsfElectron.p() );
	h_ele_vertexPt      -> Fill( bestGsfElectron.pt() );
	h_ele_vertexEta     -> Fill( bestGsfElectron.eta() );

	// generated distributions for matched electrons
	h_ele_matchingObjectPt_matched      -> Fill( moIter->energy()/cosh(moIter->eta()) );
        h_ele_matchingObjectPhi_matched   -> Fill( moIter->phi() );
	h_ele_matchingObjectEta_matched     -> Fill( moIter->eta() );
        h_ele_matchingObjectZ_matched   -> Fill( moIter->z() );
	h_ele_vertexPhi     -> Fill( bestGsfElectron.phi() );
	h_ele_vertexX     -> Fill( bestGsfElectron.vertex().x() );
	h_ele_vertexY     -> Fill( bestGsfElectron.vertex().y() );
	h_ele_vertexZ     -> Fill( bestGsfElectron.vertex().z() );

	// track related distributions
	h_ele_foundHits     -> Fill( bestGsfElectron.gsfTrack()->numberOfValidHits() );
	h_ele_chi2          -> Fill( bestGsfElectron.gsfTrack()->normalizedChi2() );  

        // from electron interface, hence using mode
	h_ele_PinMnPout_mode     -> Fill( bestGsfElectron.trackMomentumAtVtx().R() - bestGsfElectron.trackMomentumOut().R() );
	
	// match distributions 
	h_ele_HoE -> Fill(bestGsfElectron.hadronicOverEm());
	h_ele_EoP    -> Fill( bestGsfElectron.eSuperClusterOverP() );
	h_ele_EoPout -> Fill( bestGsfElectron.eSeedClusterOverPout() );
	h_ele_dEtaSc_propVtx -> Fill(bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
	h_ele_dPhiSc_propVtx -> Fill(bestGsfElectron.deltaPhiSuperClusterTrackAtVtx()); 
	h_ele_dPhiCl_propOut -> Fill(bestGsfElectron.deltaPhiSeedClusterTrackAtCalo()); 
	 
	//classes
	int eleClass = bestGsfElectron.classification();
	h_ele_classes ->Fill(eleClass);	

	eleClass = eleClass%100; // get rid of barrel/endcap distinction
        h_ele_eta->Fill(fabs(bestGsfElectron.eta()));
        if (eleClass == 0) {
	  h_ele_eta_golden ->Fill(fabs(bestGsfElectron.eta()));
	}
        if (eleClass == 30 || eleClass == 31 || eleClass == 32  || eleClass == 33 || eleClass == 34 ) {
	  h_ele_eta_shower ->Fill(fabs(bestGsfElectron.eta()));
	}

	//fbrem 

      } // gsf electron found

  } // loop overmatching object
  

}


