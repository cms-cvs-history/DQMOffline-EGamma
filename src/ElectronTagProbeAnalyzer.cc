
#include "DQMOffline/EGamma/interface/ElectronTagProbeAnalyzer.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "TMath.h"

#include <iostream>

using namespace reco ;

ElectronTagProbeAnalyzer::ElectronTagProbeAnalyzer( const edm::ParameterSet & conf )
 : ElectronDqmAnalyzerBase(conf)
 {
  // general, collections
  Selection_ = conf.getParameter<int>("Selection");
  electronCollection_=conf.getParameter<edm::InputTag>("ElectronCollection");
  matchingObjectCollection_ = conf.getParameter<edm::InputTag>("MatchingObjectCollection");
  trackCollection_=conf.getParameter<edm::InputTag>("TrackCollection");
  vertexCollection_=conf.getParameter<edm::InputTag>("VertexCollection");
  gsftrackCollection_=conf.getParameter<edm::InputTag>("GsfTrackCollection");
  readAOD_ = conf.getParameter<bool>("ReadAOD");

//  // matching
//  matchingCondition_ = conf.getParameter<std::string>("MatchingCondition");
//  assert (matchingCondition_=="Cone") ;
//  maxPtMatchingObject_ = conf.getParameter<double>("MaxPtMatchingObject");
//  maxAbsEtaMatchingObject_ = conf.getParameter<double>("MaxAbsEtaMatchingObject");
//  deltaR_ = conf.getParameter<double>("DeltaR");

  // tag and probe
  massLow_ = conf.getParameter< double >("MassLow");
  massHigh_ = conf.getParameter< double >("MassHigh");
  TPchecksign_ = conf.getParameter<bool>("TpCheckSign");
  TAGcheckclass_ = conf.getParameter<bool>("TagCheckClass");
  PROBEetcut_ = conf.getParameter<bool>("ProbeEtCut");
  PROBEcheckclass_ = conf.getParameter<bool>("ProbeCheckClass");

  // electron selection
  minEt_ = conf.getParameter<double>("MinEt");
  minPt_ = conf.getParameter<double>("MinPt");
  maxAbsEta_ = conf.getParameter<double>("MaxAbsEta");
  isEB_ = conf.getParameter<bool>("SelectEb");
  isEE_ = conf.getParameter<bool>("SelectEe");
  isNotEBEEGap_ = conf.getParameter<bool>("SelectNotEbEeGap");
  isEcalDriven_ = conf.getParameter<bool>("SelectEcalDriven");
  isTrackerDriven_ = conf.getParameter<bool>("SelectTrackerDriven");
  eOverPMinBarrel_ = conf.getParameter<double>("MinEopBarrel");
  eOverPMaxBarrel_ = conf.getParameter<double>("MaxEopBarrel");
  eOverPMinEndcaps_ = conf.getParameter<double>("MinEopEndcaps");
  eOverPMaxEndcaps_ = conf.getParameter<double>("MaxEopEndcaps");
  dEtaMinBarrel_ = conf.getParameter<double>("MinDetaBarrel");
  dEtaMaxBarrel_ = conf.getParameter<double>("MaxDetaBarrel");
  dEtaMinEndcaps_ = conf.getParameter<double>("MinDetaEndcaps");
  dEtaMaxEndcaps_ = conf.getParameter<double>("MaxDetaEndcaps");
  dPhiMinBarrel_ = conf.getParameter<double>("MinDphiBarrel");
  dPhiMaxBarrel_ = conf.getParameter<double>("MaxDphiBarrel");
  dPhiMinEndcaps_ = conf.getParameter<double>("MinDphiEndcaps");
  dPhiMaxEndcaps_ = conf.getParameter<double>("MaxDphiEndcaps");
  sigIetaIetaMinBarrel_ = conf.getParameter<double>("MinSigIetaIetaBarrel");
  sigIetaIetaMaxBarrel_ = conf.getParameter<double>("MaxSigIetaIetaBarrel");
  sigIetaIetaMinEndcaps_ = conf.getParameter<double>("MinSigIetaIetaEndcaps");
  sigIetaIetaMaxEndcaps_ = conf.getParameter<double>("MaxSigIetaIetaEndcaps");
  hadronicOverEmMaxBarrel_ = conf.getParameter<double>("MaxHoeBarrel");
  hadronicOverEmMaxEndcaps_ = conf.getParameter<double>("MaxHoeEndcaps");
  mvaMin_ = conf.getParameter<double>("MinMva");
  tipMaxBarrel_ = conf.getParameter<double>("MaxTipBarrel");
  tipMaxEndcaps_ = conf.getParameter<double>("MaxTipEndcaps");
  tkIso03Max_ = conf.getParameter<double>("MaxTkIso03");
  hcalIso03Depth1MaxBarrel_ = conf.getParameter<double>("MaxHcalIso03Depth1Barrel");
  hcalIso03Depth1MaxEndcaps_ = conf.getParameter<double>("MaxHcalIso03Depth1Endcaps");
  hcalIso03Depth2MaxEndcaps_ = conf.getParameter<double>("MaxHcalIso03Depth2Endcaps");
  ecalIso03MaxBarrel_ = conf.getParameter<double>("MaxEcalIso03Barrel");
  ecalIso03MaxEndcaps_ = conf.getParameter<double>("MaxEcalIso03Endcaps");

  // for trigger
  triggerResults_ = conf.getParameter<edm::InputTag>("TriggerResults");
//  HLTPathsByName_= conf.getParameter<std::vector<std::string > >("HltPaths");
//  HLTPathsByIndex_.resize(HLTPathsByName_.size());

  // histos limits and binning
  nbineta=conf.getParameter<int>("NbinEta");
  nbineta2D=conf.getParameter<int>("NbinEta2D");
  etamin=conf.getParameter<double>("EtaMin");
  etamax=conf.getParameter<double>("EtaMax");
  //
  nbinphi=conf.getParameter<int>("NbinPhi");
  nbinphi2D=conf.getParameter<int>("NbinPhi2D");
  phimin=conf.getParameter<double>("PhiMin");
  phimax=conf.getParameter<double>("PhiMax");
  //
  nbinpt=conf.getParameter<int>("NbinPt");
  nbinpteff=conf.getParameter<int>("NbinPtEff");
  nbinpt2D=conf.getParameter<int>("NbinPt2D");
  ptmax=conf.getParameter<double>("PtMax");
  //
  nbinp=conf.getParameter<int>("NbinP");
  nbinp2D=conf.getParameter<int>("NbinP2D");
  pmax=conf.getParameter<double>("PMax");
  //
  nbineop=conf.getParameter<int>("NbinEop");
  nbineop2D=conf.getParameter<int>("NbinEop2D");
  eopmax=conf.getParameter<double>("EopMax");
  eopmaxsht=conf.getParameter<double>("EopMaxSht");
  //
  nbindeta=conf.getParameter<int>("NbinDeta");
  detamin=conf.getParameter<double>("DetaMin");
  detamax=conf.getParameter<double>("DetaMax");
  //
  nbindphi=conf.getParameter<int>("NbinDphi");
  dphimin=conf.getParameter<double>("DphiMin");
  dphimax=conf.getParameter<double>("DphiMax");
  //
  nbindetamatch=conf.getParameter<int>("NbinDetaMatch");
  nbindetamatch2D=conf.getParameter<int>("NbinDetaMatch2D");
  detamatchmin=conf.getParameter<double>("DetaMatchMin");
  detamatchmax=conf.getParameter<double>("DetaMatchMax");
  //
  nbindphimatch=conf.getParameter<int>("NbinDphiMatch");
  nbindphimatch2D=conf.getParameter<int>("NbinDphiMatch2D");
  dphimatchmin=conf.getParameter<double>("DphiMatchMin");
  dphimatchmax=conf.getParameter<double>("DphiMatchMax");
  //
  nbinfhits=conf.getParameter<int>("NbinFhits");
  fhitsmax=conf.getParameter<double>("FhitsMax");
  //
  nbinlhits=conf.getParameter<int>("NbinLhits");
  lhitsmax=conf.getParameter<double>("LhitsMax");
  //
  nbinxyz=conf.getParameter<int>("NbinXyz");
  nbinxyz2D=conf.getParameter<int>("NbinXyz2D");
  //
  nbinpoptrue= conf.getParameter<int>("NbinPopTrue");
  poptruemin=conf.getParameter<double>("PopTrueMin");
  poptruemax=conf.getParameter<double>("PopTrueMax");
  //
  nbinmee= conf.getParameter<int>("NbinMee");
  meemin=conf.getParameter<double>("MeeMin");
  meemax=conf.getParameter<double>("MeeMax");
  //
  nbinhoe= conf.getParameter<int>("NbinHoe");
  hoemin=conf.getParameter<double>("HoeMin");
  hoemax=conf.getParameter<double>("HoeMax");
 }

ElectronTagProbeAnalyzer::~ElectronTagProbeAnalyzer()
 {}

void ElectronTagProbeAnalyzer::book()
 {
  nEvents_ = 0 ;
  //nAfterTrigger_ = 0 ;

  // general
  //h2_ele_beamSpotXvsY = bookH2("h2_ele_beamSpotXvsY","beam spot x vs y",100,-1.,1.,100,-1.,1.) ;
  //py_ele_nElectronsVsLs= bookP1("py_ele_nElectronsVsLs","# gsf electrons vs LS",150,0.,150.,0.,20.) ;
  // py_ele_nClustersVsLs= bookP1("py_ele_nClustersVsLs","# clusters vs LS",150,0.,150.,0.,100.) ;
  ////py_ele_nGsfTracksVsLs= bookP1("py_ele_nGsfTracksVsLs","# gsf tracks vs LS",150,0.,150.,0.,20.) ;
  //py_ele_nTracksVsLs= bookP1("py_ele_nTracksVsLs","# tracks vs LS",150,0.,150.,0.,100.) ;
  //py_ele_nVerticesVsLs= bookP1("py_ele_nVerticesVsLs","# vertices vs LS",150,0.,150.,0.,10.) ;
  //h1_ele_triggers = bookH1("h1_ele_triggers","hlt triggers",128,0.,128.) ;

  // basic quantities
//  h1_ele_num_= bookH1("h1_ele_num","# rec electrons",20, 0.,20.,"N_{ele}");
//  h1_ele_vertexP = bookH1("h1_ele_vertexP",        "ele p at vertex",       nbinp,0.,pmax,"p_{vertex} (GeV/c)");
//  h1_ele_Et = bookH1("h1_ele_Et","ele SC transverse energy",  nbinpt,0.,ptmax,"E_{T} (GeV)");
//  h1_ele_vertexTIP = bookH1("h1_ele_vertexTIP","ele transverse impact parameter (wrt bs)",90,0.,0.15,"TIP (cm)");
//  h1_ele_charge = bookH1("h1_ele_charge","ele charge",5,-2.,2.,"charge");
  h1_ele_vertexPt_barrel = bookH1("h1_ele_vertexPt_barrel","ele transverse momentum in barrel",nbinpt,0.,ptmax,"p_{T vertex} (GeV/c)");
  h1_ele_vertexPt_endcaps = bookH1("h1_ele_vertexPt_endcaps","ele transverse momentum in endcaps",nbinpt,0.,ptmax,"p_{T vertex} (GeV/c)");
//  h1_ele_vertexEta = bookH1("h1_ele_vertexEta","ele momentum eta",nbineta,etamin,etamax,"#eta");
//  h1_ele_vertexPhi = bookH1("h1_ele_vertexPhi","ele  momentum #phi",nbinphi,phimin,phimax,"#phi (rad)");
  h2_ele_vertexEtaVsPhi = bookH2("h2_ele_vertexEtaVsPhi","ele momentum #eta vs #phi",nbinphi2D,phimin,phimax,nbineta2D,etamin,etamax,"#phi (rad)","#eta");
//  h1_ele_vertexX = bookH1("h1_ele_vertexX","ele vertex x",nbinxyz,-0.1,0.1,"x (cm)" );
//  h1_ele_vertexY = bookH1("h1_ele_vertexY","ele vertex y",nbinxyz,-0.1,0.1,"y (cm)" );
  h2_ele_vertexXvsY = bookH2("h2_ele_vertexXvsY","ele vertex x vs y",nbinxyz2D,-0.1,0.1,nbinxyz2D,-0.1,0.1,"y (cm)","x (cm)" );
  h1_ele_vertexZ = bookH1("h1_ele_vertexZ","ele vertex z",nbinxyz,-25, 25,"z (cm)" );

  // super-clusters
//  h1_scl_En = bookH1("h1_scl_energy","ele supercluster energy",nbinp,0.,pmax);
//  h1_scl_Eta = bookH1("h1_scl_eta","ele supercluster eta",nbineta,etamin,etamax);
//  h1_scl_Phi = bookH1("h1_scl_phi","ele supercluster phi",nbinphi,phimin,phimax);
  h1_scl_Et = bookH1("h1_scl_et","ele supercluster transverse energy",nbinpt,0.,ptmax);

  // electron track
//  h1_ele_ambiguousTracks = bookH1("h1_ele_ambiguousTracks","ele # ambiguous tracks",  5,0.,5.);
//  h2_ele_ambiguousTracksVsEta = bookH2("h2_ele_ambiguousTracksVsEta","ele # ambiguous tracks  vs eta",  nbineta2D,etamin,etamax,5,0.,5.);
//  h2_ele_ambiguousTracksVsPhi = bookH2("h2_ele_ambiguousTracksVsPhi","ele # ambiguous tracks  vs phi",  nbinphi2D,phimin,phimax,5,0.,5.);
//  h2_ele_ambiguousTracksVsPt = bookH2("h2_ele_ambiguousTracksVsPt","ele # ambiguous tracks vs pt",  nbinpt2D,0.,ptmax,5,0.,5.);
  h1_ele_chi2 = bookH1("h1_ele_chi2","ele track #chi^{2}",100,0.,15.,"#Chi^{2}");
  //py_ele_chi2VsEta = bookP1("py_ele_chi2VsEta","ele track <#chi^{2}> vs eta",nbineta2D,etamin,etamax,0.,15.);
  //py_ele_chi2VsPhi = bookP1("py_ele_chi2VsPhi","ele track <#chi^{2}> vs phi",nbinphi2D,phimin,phimax,0.,15.);
  //h2_ele_chi2VsPt = bookH2("h2_ele_chi2VsPt","ele track #chi^{2} vs pt",nbinpt2D,0.,ptmax,50,0.,15.);
  h1_ele_foundHits = bookH1("h1_ele_foundHits","ele track # found hits",nbinfhits,0.,fhitsmax,"N_{hits}");
  //py_ele_foundHitsVsEta = bookP1("py_ele_foundHitsVsEta","ele track <# found hits> vs eta",nbineta2D,etamin,etamax,0.,fhitsmax);
  //py_ele_foundHitsVsPhi = bookP1("py_ele_foundHitsVsPhi","ele track <# found hits> vs phi",nbinphi2D,phimin,phimax,0.,fhitsmax);
//  h2_ele_foundHitsVsPt = bookH2("h2_ele_foundHitsVsPt","ele track # found hits vs pt",nbinpt2D,0.,ptmax,nbinfhits,0.,fhitsmax);
  h1_ele_lostHits = bookH1("h1_ele_lostHits","ele track # lost hits",5,0.,5.,"N_{lost hits}");
  //py_ele_lostHitsVsEta = bookP1("py_ele_lostHitsVsEta","ele track <# lost hits> vs eta",nbineta2D,etamin,etamax,0.,lhitsmax);
  //py_ele_lostHitsVsPhi = bookP1("py_ele_lostHitsVsPhi","ele track <# lost hits> vs eta",nbinphi2D,phimin,phimax,0.,lhitsmax);
//  h2_ele_lostHitsVsPt = bookH2("h2_ele_lostHitsVsPt","ele track # lost hits vs eta",nbinpt2D,0.,ptmax,nbinlhits,0.,lhitsmax);

  // electron matching and ID
  //h_ele_EoPout = bookH1( "h_ele_EoPout","ele E/P_{out}",nbineop,0.,eopmax,"E_{seed}/P_{out}");
  //h_ele_dEtaCl_propOut = bookH1( "h_ele_dEtaCl_propOut","ele #eta_{cl} - #eta_{tr}, prop from outermost",nbindetamatch,detamatchmin,detamatchmax,"#eta_{seedcl} - #eta_{tr}");
  //h_ele_dPhiCl_propOut = bookH1( "h_ele_dPhiCl_propOut","ele #phi_{cl} - #phi_{tr}, prop from outermost",nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{seedcl} - #phi_{tr} (rad)");
  //h_ele_outerP = bookH1( "h_ele_outerP","ele track outer p, mean",nbinp,0.,pmax,"P_{out} (GeV/c)");
  //h_ele_outerP_mode = bookH1( "h_ele_outerP_mode","ele track outer p, mode",nbinp,0.,pmax,"P_{out} (GeV/c)");
  h1_ele_Eop_barrel = bookH1( "h1_ele_Eop_barrel","ele E/P_{vertex} in barrel",nbineop,0.,eopmax,"E/P_{vertex}");
  h1_ele_Eop_endcaps = bookH1( "h1_ele_Eop_endcaps","ele E/P_{vertex} in endcaps",nbineop,0.,eopmax,"E/P_{vertex}");
  //py_ele_EopVsPhi = bookP1("py_ele_EopVsPhi","ele <E/P_{vertex}> vs #phi",nbinphi2D,phimin,phimax,0.,eopmax,"#phi","E/P_{vertex}");
//  h2_ele_EopVsPt = bookH2("h2_ele_EopVsPt","ele E/P_{vertex} vs pt",nbinpt2D,0.,ptmax,nbineop,0.,eopmax,"E/P_{vertex}");
  h1_ele_EeleOPout_barrel = bookH1( "h1_ele_EeleOPout_barrel","ele E_{ele}/P_{out} in barrel",nbineop,0.,eopmax,"E_{ele}/P_{out}");
  h1_ele_EeleOPout_endcaps = bookH1( "h1_ele_EeleOPout_endcaps","ele E_{ele}/P_{out} in endcaps",nbineop,0.,eopmax,"E_{ele}/P_{out}");
//  h2_ele_EeleOPoutVsPhi = bookH2("h2_ele_EeleOPoutVsPhi","ele E_{ele}/P_{out} vs phi",nbinphi2D,phimin,phimax,nbineop,0.,eopmax,"E_{ele}/P_{out}");
//  h2_ele_EeleOPoutVsPt = bookH2("h2_ele_EeleOPoutVsPt","ele E_{ele}/P_{out} vs pt",nbinpt2D,0.,ptmax,nbineop,0.,eopmax,"E_{ele}/P_{out}");
  h1_ele_dEtaSc_propVtx_barrel = bookH1( "h1_ele_dEtaSc_propVtx_barrel","ele #eta_{sc} - #eta_{tr}, prop from vertex, in barrel",nbindetamatch,detamatchmin,detamatchmax,"#eta_{sc} - #eta_{tr}");
  h1_ele_dEtaSc_propVtx_endcaps = bookH1( "h1_ele_dEtaSc_propVtx_endcaps","ele #eta_{sc} - #eta_{tr}, prop from vertex, in endcaps",nbindetamatch,detamatchmin,detamatchmax,"#eta_{sc} - #eta_{tr}");
  // py_ele_dEtaSc_propVtxVsPhi = bookP1("py_ele_dEtaSc_propVtxVsPhi","ele <#eta_{sc} - #eta_{tr}, prop from vertex> vs #phi",nbinphi2D,phimin,phimax,detamatchmin,detamatchmax,"#phi","#eta_{sc} - #eta_{tr}");
//  h2_ele_dEtaSc_propVtxVsPt = bookH2("h2_ele_dEtaSc_propVtxVsPt","ele #eta_{sc} - #eta_{tr}, prop from vertex vs pt",nbinpt2D,0.,ptmax,nbindetamatch,detamatchmin,detamatchmax,"#eta_{sc} - #eta_{tr}");
  h1_ele_dEtaEleCl_propOut_barrel = bookH1( "h1_ele_dEtaEleCl_propOut_barrel","ele #eta_{EleCl} - #eta_{tr}, prop from outermost, in barrel",nbindetamatch,detamatchmin,detamatchmax,"#eta_{elecl} - #eta_{tr}");
  h1_ele_dEtaEleCl_propOut_endcaps = bookH1( "h1_ele_dEtaEleCl_propOut_endcaps","ele #eta_{EleCl} - #eta_{tr}, prop from outermost, in endcaps",nbindetamatch,detamatchmin,detamatchmax,"#eta_{elecl} - #eta_{tr}");
//  h2_ele_dEtaEleCl_propOutVsPhi = bookH2("h2_ele_dEtaEleCl_propOutVsPhi","ele #eta_{EleCl} - #eta_{tr}, prop from outermost vs phi",nbinphi2D,phimin,phimax,nbindetamatch,detamatchmin,detamatchmax,"#eta_{elecl} - #eta_{tr}");
//  h2_ele_dEtaEleCl_propOutVsPt = bookH2("h2_ele_dEtaEleCl_propOutVsPt","ele #eta_{EleCl} - #eta_{tr}, prop from outermost vs pt",nbinpt2D,0.,ptmax,nbindetamatch,detamatchmin,detamatchmax,"#eta_{elecl} - #eta_{tr}");
  h1_ele_dPhiSc_propVtx_barrel = bookH1( "h1_ele_dPhiSc_propVtx_barrel","ele #phi_{sc} - #phi_{tr}, prop from vertex, in barrel",nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{sc} - #phi_{tr} (rad)");
  h1_ele_dPhiSc_propVtx_endcaps = bookH1( "h1_ele_dPhiSc_propVtx_endcaps","ele #phi_{sc} - #phi_{tr}, prop from vertex, in endcaps",nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{sc} - #phi_{tr} (rad)");
  //py_ele_dPhiSc_propVtxVsPhi = bookP1("py_ele_dPhiSc_propVtxVsPhi","ele <#phi_{sc} - #phi_{tr}, prop from vertex> vs #phi",nbinphi2D,phimin,phimax,dphimatchmin,dphimatchmax,"#phi","#phi_{sc} - #phi_{tr} (rad)");
//  h2_ele_dPhiSc_propVtxVsPt = bookH2("h2_ele_dPhiSc_propVtxVsPt","ele #phi_{sc} - #phi_{tr}, prop from vertex vs pt",nbinpt2D,0.,ptmax,nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{sc} - #phi_{tr} (rad)");
  h1_ele_dPhiEleCl_propOut_barrel = bookH1( "h1_ele_dPhiEleCl_propOut_barrel","ele #phi_{EleCl} - #phi_{tr}, prop from outermost, in barrel",nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{elecl} - #phi_{tr} (rad)");
  h1_ele_dPhiEleCl_propOut_endcaps = bookH1( "h1_ele_dPhiEleCl_propOut_endcaps","ele #phi_{EleCl} - #phi_{tr}, prop from outermost, in endcaps",nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{elecl} - #phi_{tr} (rad)");
//  h2_ele_dPhiEleCl_propOutVsPhi = bookH2("h2_ele_dPhiEleCl_propOutVsPhi","ele #phi_{EleCl} - #phi_{tr}, prop from outermost vs phi",nbinphi2D,phimin,phimax,nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{elecl} - #phi_{tr} (rad)");
//  h2_ele_dPhiEleCl_propOutVsPt = bookH2("h2_ele_dPhiEleCl_propOutVsPt","ele #phi_{EleCl} - #phi_{tr}, prop from outermost vs pt",nbinpt2D,0.,ptmax,nbindphimatch,dphimatchmin,dphimatchmax,"#phi_{elecl} - #phi_{tr} (rad)");
  h1_ele_Hoe_barrel = bookH1("h1_ele_Hoe_barrel","ele hadronic energy / em energy, in barrel", nbinhoe, hoemin, hoemax,"H/E","Events","ELE_LOGY") ;
  h1_ele_Hoe_endcaps = bookH1("h1_ele_Hoe_endcaps","ele hadronic energy / em energy, in endcaps", nbinhoe, hoemin, hoemax,"H/E","Events","ELE_LOGY") ;
  //py_ele_HoeVsPhi = bookP1("py_ele_HoeVsPhi","ele <hadronic energy / em energy> vs #phi",nbinphi2D,phimin,phimax,hoemin,hoemax,"#phi","H/E","ELE_LOGY") ;
//  h2_ele_HoeVsPt = bookH2("h2_ele_HoeVsPt","ele hadronic energy / em energy vs pt",nbinpt2D,0.,ptmax,nbinhoe,hoemin,hoemax,"H/E","ELE_LOGY") ;
  h1_scl_SigEtaEta_barrel = bookH1("h1_scl_sigetaeta_barrel","ele supercluster sigma eta eta in barrel",100,0.,0.05);
  h1_scl_SigEtaEta_endcaps = bookH1("h1_scl_sigetaeta_endcaps","ele supercluster sigma eta eta in endcaps",100,0.,0.05);

  // fbrem
//  h1_ele_innerPt_mean = bookH1( "h1_ele_innerPt_mean","ele track inner p_{T}, mean",nbinpt,0.,ptmax,"P_{T in} (GeV/c)");
//  h1_ele_outerPt_mean = bookH1( "h1_ele_outerPt_mean","ele track outer p_{T}, mean",nbinpt,0.,ptmax,"P_{T out} (GeV/c)");
//  h1_ele_outerPt_mode = bookH1( "h1_ele_outerPt_mode","ele track outer p_{T}, mode",nbinpt,0.,ptmax,"P_{T out} (GeV/c)");
//  //h_ele_PinMnPout_mode = bookH1( "h1_ele_PinMnPout_mode","ele track inner p - outer p, mode"   ,nbinp,0.,100.);
//  h1_ele_PinMnPout = bookH1( "h1_ele_PinMnPout","ele track inner p - outer p, mean" ,nbinp,0.,200.,"P_{vertex} - P_{out} (GeV/c)");
//  h1_ele_PinMnPout_mode = bookH1( "h1_ele_PinMnPout_mode","ele track inner p - outer p, mode",nbinp,0.,100.,"P_{vertex} - P_{out}, mode (GeV/c)");
  h1_ele_fbrem = bookH1("h1_ele_fbrem","ele brem fraction",100,0.,1.,"P_{in} - P_{out} / P_{in}") ;
  // py_ele_fbremVsEta = bookP1("py_ele_fbremVsEta","ele <brem fraction> vs #eta",nbineta2D,etamin,etamax,0.,1.,"#eta","P_{in} - P_{out} / P_{in}") ;
  //py_ele_fbremVsPhi = bookP1("py_ele_fbremVsPhi","ele <brem fraction> vs #phi",nbinphi2D,phimin,phimax,0.,1.,"#phi","P_{in} - P_{out} / P_{in}") ;
//  h_ele_fbremVsPt = bookH2("h_ele_fbremVsPt","ele brem fraction vs pt",nbinpt2D,0.,ptmax,100,0.,1.,"P_{in} - P_{out} / P_{in}") ;
  h1_ele_classes = bookH1("h1_ele_classes","ele electron classes",150,0.0,150.);

  // pflow
  h1_ele_mva = bookH1( "h1_ele_mva","ele identification mva",100,-1.,1.);
  h1_ele_provenance = bookH1( "h1_ele_provenance","ele provenance",5,-2.,3.);

  // isolation
  h1_ele_tkSumPt_dr03 = bookH1("h1_ele_tkSumPt_dr03","tk isolation sum, dR=0.3",100,0.0,20.,"TkIsoSum, cone 0.3 (GeV/c)","Events","ELE_LOGY");
  h1_ele_ecalRecHitSumEt_dr03 = bookH1("h1_ele_ecalRecHitSumEt_dr03","ecal isolation sum, dR=0.3",100,0.0,20.,"EcalIsoSum, cone 0.3 (GeV)","Events","ELE_LOGY");
  h1_ele_hcalTowerSumEt_dr03 = bookH1("h1_ele_hcalTowerSumEt_dr03","hcal isolation sum, dR=0.3",100,0.0,20.,"HcalIsoSum, cone 0.3 (GeV)","Events","ELE_LOGY");
//  h1_ele_hcalDepth1TowerSumEt_dr03 = bookH1("h1_ele_hcalDepth1TowerSumEt_dr03","hcal depth1 isolation sum, dR=0.3",100,0.0,20.,"Hcal1IsoSum, cone 0.3 (GeV)","Events","ELE_LOGY");
//  h1_ele_hcalDepth2TowerSumEt_dr03 = bookH1("h1_ele_hcalDepth2TowerSumEt_dr03","hcal depth2 isolation sum, dR=0.3",100,0.0,20.,"Hcal2IsoSum, cone 0.3 (GeV)","Events","ELE_LOGY");
//  h1_ele_tkSumPt_dr04 = bookH1("h1_ele_tkSumPt_dr04","hcal isolation sum",100,0.0,20.,"TkIsoSum, cone 0.4 (GeV/c)","Events","ELE_LOGY");
//  h1_ele_ecalRecHitSumEt_dr04 = bookH1("h1_ele_ecalRecHitSumEt_dr04","ecal isolation sum, dR=0.4",100,0.0,20.,"EcalIsoSum, cone 0.4 (GeV)","Events","ELE_LOGY");
//  h1_ele_hcalTowerSumEt_dr04 = bookH1("h1_ele_hcalTowerSumEt_dr04","hcal isolation sum, dR=0.4",100,0.0,20.,"HcalIsoSum, cone 0.4 (GeV)","Events","ELE_LOGY");
////  h1_ele_hcalDepth1TowerSumEt_dr04 = bookH1("h1_ele_hcalDepth1TowerSumEt_dr04","hcal depth1 isolation sum, dR=0.4",100,0.0,20.,"Hcal1IsoSum, cone 0.4 (GeV)","Events","ELE_LOGY");
////  h1_ele_hcalDepth2TowerSumEt_dr04 = bookH1("h1_ele_hcalDepth2TowerSumEt_dr04","hcal depth2 isolation sum, dR=0.4",100,0.0,20.,"Hcal2IsoSum, cone 0.4 (GeV)","Events","ELE_LOGY");

  // di-electron mass
  h1_ele_mee_os = bookH1("h1_ele_mee_os","ele pairs invariant mass, opposite sign", nbinmee, meemin, meemax,"m_{e^{+}e^{-}} (GeV/c^{2})");
  h1_ele_mee = bookH1("h1_ele_mee","ele pairs invariant mass", nbinmee, meemin, meemax,"m_{ee} (GeV/c^{2})");



  //===========================
  // histos for matching and matched matched objects
  //===========================

  // matching object
  std::string matchingObjectType ;
  if (std::string::npos!=matchingObjectCollection_.label().find("SuperCluster",0))
   { matchingObjectType = "SC" ; }
  if (matchingObjectType=="")
   { edm::LogError("ElectronMcFakeValidator::beginJob")<<"Unknown matching object type !" ; }
  else
   { edm::LogInfo("ElectronMcFakeValidator::beginJob")<<"Matching object type: "<<matchingObjectType ; }
//  std::string htitle = "# "+matchingObjectType+"s", xtitle = "N_{"+matchingObjectType+"}" ;
//  h_matchingObject_Num = bookH1withSumw2("h_matchingObject_Num",htitle,nbinfhits,0.,fhitsmax,xtitle) ;

  // matching object distributions
  h1_matchingObject_Eta = bookH1withSumw2("h1_matchingObject_Eta",matchingObjectType+" #eta",nbineta,etamin,etamax,"#eta_{SC}");
//  h1_matchingObject_AbsEta = bookH1withSumw2("h1_matchingObject_AbsEta",matchingObjectType+" |#eta|",nbineta/2,0.,etamax,"|#eta|_{SC}");
//  h1_matchingObject_P = bookH1withSumw2("h1_matchingObject_P",matchingObjectType+" p",nbinp,0.,pmax,"E_{SC} (GeV)");
  h1_matchingObject_Pt = bookH1withSumw2("h1_matchingObject_Pt",matchingObjectType+" pt",nbinpteff,5.,ptmax);
  h1_matchingObject_Phi = bookH1withSumw2("h1_matchingObject_Phi",matchingObjectType+" phi",nbinphi,phimin,phimax);
  h1_matchingObject_Z = bookH1withSumw2("h1_matchingObject_Z",matchingObjectType+" z",nbinxyz,-25,25);

  h1_matchedObject_Eta = bookH1withSumw2("h1_matchedObject_Eta","Efficiency vs matching SC #eta",nbineta,etamin,etamax);
//  h1_matchedObject_AbsEta = bookH1withSumw2("h1_matchedObject_AbsEta","Efficiency vs matching SC |#eta|",nbineta/2,0.,2.5);
  h1_matchedObject_Pt = bookH1withSumw2("h1_matchedObject_Pt","Efficiency vs matching SC E_{T}",nbinpteff,5.,ptmax);
  h1_matchedObject_Phi = bookH1withSumw2("h1_matchedObject_Phi","Efficiency vs matching SC phi",nbinphi,phimin,phimax);
  h1_matchedObject_Z = bookH1withSumw2("h1_matchedObject_Z","Efficiency vs matching SC z",nbinxyz,-25,25);

//  // classes
//  h_matchedEle_eta = bookH1( "h_matchedEle_eta", "ele electron eta",  nbineta/2,0.0,etamax);
//  h_matchedEle_eta_golden = bookH1( "h_matchedEle_eta_golden", "ele electron eta golden",  nbineta/2,0.0,etamax);
//  h_matchedEle_eta_shower = bookH1( "h_matchedEle_eta_shower", "ele electron eta showering",  nbineta/2,0.0,etamax);
//  //h_matchedEle_eta_bbrem = bookH1( "h_matchedEle_eta_bbrem", "ele electron eta bbrem",  nbineta/2,0.0,etamax);
//  //h_matchedEle_eta_narrow = bookH1( "h_matchedEle_eta_narrow", "ele electron eta narrow",  nbineta/2,0.0,etamax);
//
 }

void ElectronTagProbeAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup & iSetup )
 {
  nEvents_++ ;
//  if (!trigger(iEvent)) return ;
//  nAfterTrigger_++ ;

//  edm::Handle<SuperClusterCollection> barrelSCs ;
//  iEvent.getByLabel("correctedHybridSuperClusters",barrelSCs) ;
//  edm::Handle<SuperClusterCollection> endcapsSCs ;
//  iEvent.getByLabel("correctedMulti5x5SuperClustersWithPreshower",endcapsSCs) ;
//  std::cout<<"[ElectronMcSignalValidator::analyze]"
//    <<"Event "<<iEvent.id()
//    <<" has "<<barrelSCs.product()->size()<<" barrel superclusters"
//    <<" and "<<endcapsSCs.product()->size()<<" endcaps superclusters" ;
//
  edm::Handle<GsfElectronCollection> gsfElectrons ;
  iEvent.getByLabel(electronCollection_,gsfElectrons) ;
  edm::Handle<reco::SuperClusterCollection> recoClusters ;
  iEvent.getByLabel(matchingObjectCollection_,recoClusters) ;
  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(trackCollection_,tracks);
  edm::Handle<reco::GsfTrackCollection> gsfTracks;
  iEvent.getByLabel(gsftrackCollection_,gsfTracks);
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vertexCollection_,vertices);
  edm::Handle<reco::BeamSpot> recoBeamSpotHandle ;
  iEvent.getByType(recoBeamSpotHandle) ;
  const BeamSpot bs = *recoBeamSpotHandle ;

  int ievt = iEvent.id().event();
  int irun = iEvent.id().run();
  int ils = iEvent.luminosityBlock();

  edm::LogInfo("ElectronMcSignalValidator::analyze")
    <<"Treating "<<gsfElectrons.product()->size()<<" electrons"
    <<" from event "<<ievt<<" in run "<<irun<<" and lumiblock "<<ils ;
  //h1_ele_num_->Fill((*gsfElectrons).size()) ;

  // general
  /*
  h2_ele_beamSpotXvsY->Fill(bs.position().x(),bs.position().y());
  py_ele_nElectronsVsLs->Fill(float(ils),(*gsfElectrons).size());
  py_ele_nClustersVsLs->Fill(float(ils),(*recoClusters).size());
  py_ele_nGsfTracksVsLs->Fill(float(ils),(*gsfTracks).size());
  py_ele_nTracksVsLs->Fill(float(ils),(*tracks).size());
  py_ele_nVerticesVsLs->Fill(float(ils),(*vertices).size());
  */

  //std::cout << "analysing new event " << std::endl;

  // selected rec electrons
  reco::GsfElectronCollection::const_iterator gsfIter ;
  for
   ( gsfIter=gsfElectrons->begin() ;
     gsfIter!=gsfElectrons->end();
     gsfIter++ )
   {
    // vertex TIP
    double vertexTIP =
     (gsfIter->vertex().x()-bs.position().x()) * (gsfIter->vertex().x()-bs.position().x()) +
     (gsfIter->vertex().y()-bs.position().y()) * (gsfIter->vertex().y()-bs.position().y()) ;
    vertexTIP = sqrt(vertexTIP) ;

    // select electrons
    if (!selected(gsfIter,vertexTIP)) continue ;

    reco::SuperClusterRef sclTagRef = gsfIter->superCluster() ;
    reco::SuperClusterCollection::const_iterator moIter ;
    for
     ( moIter=recoClusters->begin() ;
       moIter!=recoClusters->end() ;
       moIter++ )
     {
	    if (moIter->eta()==sclTagRef->eta()) continue ;

      /*
      if
        ( moIter->energy()/cosh(moIter->eta())>maxPtMatchingObject_ ||
          fabs(moIter->eta())> maxAbsEtaMatchingObject_ )
        { continue ; }
      */

      // Additional cuts on Tag

      // Additional cuts on Probe
      if(PROBEetcut_ && (moIter->energy()/cosh(moIter->eta())<minEt_)) continue ;

      float SCenergy = moIter->energy();
      math::XYZPoint caloposition = moIter->position();
      float theta = caloposition.Theta();
      math::XYZVector momentum;
      float SCmomentumX = SCenergy*sin(theta)*cos(moIter->phi());
      float SCmomentumY = SCenergy*sin(theta)*sin(moIter->phi());
      float SCmomentumZ = SCenergy*cos(theta);
      const reco::Particle::LorentzVector pSCprobecandidate(SCmomentumX,SCmomentumY,SCmomentumZ,SCenergy);

      math::XYZTLorentzVector p12 = (*gsfIter).p4()+ pSCprobecandidate;
      float mee2 = p12.Dot(p12);
      float invMass = sqrt(mee2);

      if( invMass < massLow_ || invMass > massHigh_ ) continue ;
      h1_ele_mee->Fill(invMass) ;

      h1_matchingObject_Eta->Fill( moIter->eta() );
      //h1_matchingObject_AbsEta->Fill( fabs(moIter->eta()) );
      //h1_matchingObject_P->Fill( moIter->energy() );
      h1_matchingObject_Pt->Fill( moIter->energy()/cosh(moIter->eta()) );
      h1_matchingObject_Phi->Fill( moIter->phi() );
      h1_matchingObject_Z->Fill(  moIter->z() );

      reco::GsfElectron bestGsfElectron;
      reco::SuperClusterRef sclProbeRef;
      bool okGsfFound = false;
      //double gsfOkRatio = 999999.;
      reco::GsfElectronCollection::const_iterator gsfIter2 ;
      for
       ( gsfIter2=gsfElectrons->begin();
         gsfIter2!=gsfElectrons->end() ;
         gsfIter2++ )
       {
	      // matching with ref
	      sclProbeRef = gsfIter2->superCluster();

	      if (sclProbeRef->eta() == moIter->eta())
         {
          //std::cout << "les deux ref SC sont egales : " << std::endl;
          bestGsfElectron=*gsfIter2;

          // opposite sign checking
          bool opsign = (gsfIter->charge()* bestGsfElectron.charge()<0.);
          if(TPchecksign_ && !opsign)  break;

          okGsfFound = true;
         } //fi on gsfEleSC.eta == probeSC.eta

//        // matching with cone
//        double dphi = gsfIter2->phi()-moIter->phi();
//        if (fabs(dphi)>CLHEP::pi)
//         dphi = dphi < 0? (CLHEP::twopi) + dphi : dphi - CLHEP::twopi;
//        double deltaR = sqrt(pow((moIter->eta()-gsfIter2->eta()),2) + pow(dphi,2));
//        if ( deltaR < deltaR_ )
//        {
//         double tmpGsfRatio = gsfIter2->p()/moIter->energy();
//         if ( fabs(tmpGsfRatio-1) < fabs(gsfOkRatio-1))
//          {
//           gsfOkRatio = tmpGsfRatio;
//           bestGsfElectron=*gsfIter2;
//           okGsfFound = true;
//          }
//        } // fi on deltaR

       }// end of loop on gsfEle to find the best one which matches with probe SC

      if (okGsfFound)
       {
        // fill matched histos for eff
        fillMatchedHistos(moIter,bestGsfElectron) ;

        // basic quantities
        if (gsfIter->isEB()) h1_ele_vertexPt_barrel->Fill( bestGsfElectron.pt() );
        if (gsfIter->isEE()) h1_ele_vertexPt_endcaps->Fill( bestGsfElectron.pt() );
        h2_ele_vertexEtaVsPhi->Fill( bestGsfElectron.phi(), bestGsfElectron.eta() );
        h2_ele_vertexXvsY->Fill( bestGsfElectron.vertex().y(), bestGsfElectron.vertex().x() );
        h1_ele_vertexZ->Fill( bestGsfElectron.vertex().z() );

        // supercluster related distributions
        reco::SuperClusterRef sclRef = bestGsfElectron.superCluster() ;
        double R=TMath::Sqrt(sclRef->x()*sclRef->x() + sclRef->y()*sclRef->y() +sclRef->z()*sclRef->z());
        double Rt=TMath::Sqrt(sclRef->x()*sclRef->x() + sclRef->y()*sclRef->y());
        h1_scl_Et->Fill(sclRef->energy()*(Rt/R));

        if (!readAOD_)
        { // track extra does not exist in AOD
         h1_ele_foundHits->Fill( bestGsfElectron.gsfTrack()->numberOfValidHits() );
         h1_ele_lostHits->Fill( bestGsfElectron.gsfTrack()->numberOfLostHits() );
         h1_ele_chi2->Fill( bestGsfElectron.gsfTrack()->normalizedChi2() );
        }

        // match distributions
        if (gsfIter->isEB())
        {
         h1_ele_Eop_barrel->Fill( bestGsfElectron.eSuperClusterOverP() );
         h1_ele_EeleOPout_barrel->Fill( bestGsfElectron.eEleClusterOverPout() );
         h1_ele_dEtaSc_propVtx_barrel->Fill(bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
         h1_ele_dEtaEleCl_propOut_barrel->Fill(bestGsfElectron.deltaEtaEleClusterTrackAtCalo());
         h1_ele_dPhiSc_propVtx_barrel->Fill(bestGsfElectron.deltaPhiSuperClusterTrackAtVtx());
         h1_ele_dPhiEleCl_propOut_barrel->Fill(bestGsfElectron.deltaPhiEleClusterTrackAtCalo());
         h1_ele_Hoe_barrel->Fill(bestGsfElectron.hadronicOverEm());
         h1_scl_SigEtaEta_barrel->Fill( bestGsfElectron.scSigmaEtaEta() );
        }
        if (gsfIter->isEE())
        {
         h1_ele_Eop_endcaps->Fill( bestGsfElectron.eSuperClusterOverP() );
         h1_ele_EeleOPout_endcaps->Fill( bestGsfElectron.eEleClusterOverPout() );
         h1_ele_dEtaSc_propVtx_endcaps->Fill(bestGsfElectron.deltaEtaSuperClusterTrackAtVtx());
         h1_ele_dEtaEleCl_propOut_endcaps->Fill(bestGsfElectron.deltaEtaEleClusterTrackAtCalo());
         h1_ele_dPhiSc_propVtx_endcaps->Fill(bestGsfElectron.deltaPhiSuperClusterTrackAtVtx());
         h1_ele_dPhiEleCl_propOut_endcaps->Fill(bestGsfElectron.deltaPhiEleClusterTrackAtCalo());
         h1_ele_Hoe_endcaps->Fill(bestGsfElectron.hadronicOverEm());
         h1_scl_SigEtaEta_endcaps->Fill( bestGsfElectron.scSigmaEtaEta() );
        }


        // fbrem
        h1_ele_fbrem->Fill(bestGsfElectron.fbrem()) ;
        h1_ele_classes->Fill(bestGsfElectron.classification());

        // pflow
        h1_ele_mva->Fill(bestGsfElectron.mva()) ;
        if (bestGsfElectron.ecalDrivenSeed()) h1_ele_provenance->Fill(1.) ;
        if (bestGsfElectron.trackerDrivenSeed()) h1_ele_provenance->Fill(-1.) ;
        if (bestGsfElectron.trackerDrivenSeed()||bestGsfElectron.ecalDrivenSeed()) h1_ele_provenance->Fill(0.);
        if (bestGsfElectron.trackerDrivenSeed()&&!bestGsfElectron.ecalDrivenSeed()) h1_ele_provenance->Fill(-2.);
        if (!bestGsfElectron.trackerDrivenSeed()&&bestGsfElectron.ecalDrivenSeed()) h1_ele_provenance->Fill(2.);

        // isolation
        h1_ele_tkSumPt_dr03->Fill(bestGsfElectron.dr03TkSumPt());
        h1_ele_ecalRecHitSumEt_dr03->Fill(bestGsfElectron.dr03EcalRecHitSumEt());
        h1_ele_hcalTowerSumEt_dr03->Fill(bestGsfElectron.dr03HcalTowerSumEt());

        // inv Mass with opposite sign
        if (((gsfIter->charge())*(bestGsfElectron.charge()))<0.)
        { h1_ele_mee_os->Fill(invMass) ; }

       }// fi on OkGsfFound

     } // end of loop on SC to find probe SC

   }// end of loop on Tag gsfEle

 }

float ElectronTagProbeAnalyzer::computeInvMass
 ( const reco::GsfElectron & e1,
   const reco::GsfElectron & e2 )
 {
  math::XYZTLorentzVector p12 = e1.p4()+e2.p4() ;
  float mee2 = p12.Dot(p12) ;
  float invMass = sqrt(mee2) ;
  return invMass ;
 }

void ElectronTagProbeAnalyzer::fillMatchedHistos
 ( const reco::SuperClusterCollection::const_iterator & moIter,
   const reco::GsfElectron & electron )
 {
  // generated distributions for matched electrons
  h1_matchedObject_Eta->Fill( moIter->eta() );
//  h1_matchedObject_AbsEta->Fill( fabs(moIter->eta()) );
  h1_matchedObject_Pt->Fill( moIter->energy()/cosh(moIter->eta()) );
  h1_matchedObject_Phi->Fill( moIter->phi() );
  h1_matchedObject_Z->Fill( moIter->z() );

  //classes
//  int eleClass = electron.classification() ;
//  h_ele_classes->Fill(eleClass) ;
//  h_matchedEle_eta->Fill(fabs(electron.eta()));
//  if (electron.classification() == GsfElectron::GOLDEN) h_matchedEle_eta_golden->Fill(fabs(electron.eta()));
//  if (electron.classification() == GsfElectron::SHOWERING) h_matchedEle_eta_shower->Fill(fabs(electron.eta()));
//  //if (electron.classification() == GsfElectron::BIGBREM) h_matchedEle_eta_bbrem->Fill(fabs(electron.eta()));
//  //if (electron.classification() == GsfElectron::OLDNARROW) h_matchedEle_eta_narrow->Fill(fabs(electron.eta()));
 }

//bool ElectronTagProbeAnalyzer::trigger( const edm::Event & e )
// {
//  // retreive TriggerResults from the event
//  edm::Handle<edm::TriggerResults> triggerResults ;
//  e.getByLabel(triggerResults_,triggerResults) ;
//
//  bool accept = false ;
//
//  if (triggerResults.isValid())
//   {
//    //std::cout << "TriggerResults found, number of HLT paths: " << triggerResults->size() << std::endl;
//    // get trigger names
//    edm::TriggerNames triggerNames_;
//    triggerNames_.init(*triggerResults) ;
////    if (nEvents_==1)
////     {
////      for (unsigned int i=0; i<triggerNames_.size(); i++)
////       { std::cout << "trigger path= " << triggerNames_.triggerName(i) << std::endl; }
////     }
//
//    unsigned int n = HLTPathsByName_.size() ;
//    for (unsigned int i=0; i!=n; i++)
//     {
//      HLTPathsByIndex_[i]=triggerNames_.triggerIndex(HLTPathsByName_[i]) ;
//     }
//
//    // empty input vectors (n==0) means any trigger paths
//    if (n==0)
//     {
//      n=triggerResults->size() ;
//      HLTPathsByName_.resize(n) ;
//      HLTPathsByIndex_.resize(n) ;
//      for ( unsigned int i=0 ; i!=n ; i++)
//       {
//        HLTPathsByName_[i]=triggerNames_.triggerName(i) ;
//        HLTPathsByIndex_[i]=i ;
//       }
//     }
//
////    if (nEvents_==1)
////     {
////      if (n>0)
////       {
////        std::cout << "HLT trigger paths requested: index, name and valididty:" << std::endl;
////        for (unsigned int i=0; i!=n; i++)
////         {
////          bool validity = HLTPathsByIndex_[i]<triggerResults->size();
////          std::cout
////            << " " << HLTPathsByIndex_[i]
////            << " " << HLTPathsByName_[i]
////            << " " << validity << std::endl;
////         }
////       }
////     }
//
//    // count number of requested HLT paths which have fired
//    unsigned int fired=0 ;
//    for ( unsigned int i=0 ; i!=n ; i++ )
//     {
//      if (HLTPathsByIndex_[i]<triggerResults->size())
//       {
//        if (triggerResults->accept(HLTPathsByIndex_[i]))
//         {
//          fired++ ;
//          h1_ele_triggers->Fill(float(HLTPathsByIndex_[i]));
//          //std::cout << "Fired HLT path= " << HLTPathsByName_[i] << std::endl ;
//          accept = true ;
//         }
//       }
//     }
//
//   }
//
//  return accept ;
// }

bool ElectronTagProbeAnalyzer::selected( const reco::GsfElectronCollection::const_iterator & gsfIter , double vertexTIP )
 {
  if ((Selection_>0)&&generalCut(gsfIter)) return false ;
  if ((Selection_>=1)&&etCut(gsfIter)) return false ;
  if ((Selection_>=2)&&isolationCut(gsfIter,vertexTIP)) return false ;
  if ((Selection_>=3)&&idCut(gsfIter)) return false ;
  return true ;
 }

bool ElectronTagProbeAnalyzer::generalCut( const reco::GsfElectronCollection::const_iterator & gsfIter)
 {
  if (fabs(gsfIter->eta())>maxAbsEta_) return true ;
  if (gsfIter->pt()<minPt_) return true ;

  if (gsfIter->isEB() && isEE_) return true ;
  if (gsfIter->isEE() && isEB_) return true ;
  if (gsfIter->isEBEEGap() && isNotEBEEGap_) return true ;

  if (gsfIter->ecalDrivenSeed() && isTrackerDriven_) return true ;
  if (gsfIter->trackerDrivenSeed() && isEcalDriven_) return true ;

  return false ;
 }

bool ElectronTagProbeAnalyzer::etCut( const reco::GsfElectronCollection::const_iterator & gsfIter )
 {
  if (gsfIter->superCluster()->energy()/cosh(gsfIter->superCluster()->eta())<minEt_) return true ;

  return false ;
 }

bool ElectronTagProbeAnalyzer::isolationCut( const reco::GsfElectronCollection::const_iterator & gsfIter, double vertexTIP )
 {
  if (gsfIter->isEB() && vertexTIP > tipMaxBarrel_) return true ;
  if (gsfIter->isEE() && vertexTIP > tipMaxEndcaps_) return true ;

  if (gsfIter->dr03TkSumPt() > tkIso03Max_) return true ;
  if (gsfIter->isEB() && gsfIter->dr03HcalDepth1TowerSumEt() > hcalIso03Depth1MaxBarrel_) return true ;
  if (gsfIter->isEE() && gsfIter->dr03HcalDepth1TowerSumEt() > hcalIso03Depth1MaxEndcaps_) return true ;
  if (gsfIter->isEE() && gsfIter->dr03HcalDepth2TowerSumEt() > hcalIso03Depth2MaxEndcaps_) return true ;
  if (gsfIter->isEB() && gsfIter->dr03EcalRecHitSumEt() > ecalIso03MaxBarrel_) return true ;
  if (gsfIter->isEE() && gsfIter->dr03EcalRecHitSumEt() > ecalIso03MaxEndcaps_) return true ;

  return false ;
 }

bool ElectronTagProbeAnalyzer::idCut( const reco::GsfElectronCollection::const_iterator & gsfIter )
 {
  if (gsfIter->isEB() && gsfIter->eSuperClusterOverP() < eOverPMinBarrel_) return true ;
  if (gsfIter->isEB() && gsfIter->eSuperClusterOverP() > eOverPMaxBarrel_) return true ;
  if (gsfIter->isEE() && gsfIter->eSuperClusterOverP() < eOverPMinEndcaps_) return true ;
  if (gsfIter->isEE() && gsfIter->eSuperClusterOverP() > eOverPMaxEndcaps_) return true ;
  if (gsfIter->isEB() && fabs(gsfIter->deltaEtaSuperClusterTrackAtVtx()) < dEtaMinBarrel_) return true ;
  if (gsfIter->isEB() && fabs(gsfIter->deltaEtaSuperClusterTrackAtVtx()) > dEtaMaxBarrel_) return true ;
  if (gsfIter->isEE() && fabs(gsfIter->deltaEtaSuperClusterTrackAtVtx()) < dEtaMinEndcaps_) return true ;
  if (gsfIter->isEE() && fabs(gsfIter->deltaEtaSuperClusterTrackAtVtx()) > dEtaMaxEndcaps_) return true ;
  if (gsfIter->isEB() && fabs(gsfIter->deltaPhiSuperClusterTrackAtVtx()) < dPhiMinBarrel_) return true ;
  if (gsfIter->isEB() && fabs(gsfIter->deltaPhiSuperClusterTrackAtVtx()) > dPhiMaxBarrel_) return true ;
  if (gsfIter->isEE() && fabs(gsfIter->deltaPhiSuperClusterTrackAtVtx()) < dPhiMinEndcaps_) return true ;
  if (gsfIter->isEE() && fabs(gsfIter->deltaPhiSuperClusterTrackAtVtx()) > dPhiMaxEndcaps_) return true ;
  if (gsfIter->isEB() && gsfIter->scSigmaIEtaIEta() < sigIetaIetaMinBarrel_) return true ;
  if (gsfIter->isEB() && gsfIter->scSigmaIEtaIEta() > sigIetaIetaMaxBarrel_) return true ;
  if (gsfIter->isEE() && gsfIter->scSigmaIEtaIEta() < sigIetaIetaMinEndcaps_) return true ;
  if (gsfIter->isEE() && gsfIter->scSigmaIEtaIEta() > sigIetaIetaMaxEndcaps_) return true ;
  if (gsfIter->isEB() && gsfIter->hadronicOverEm() > hadronicOverEmMaxBarrel_) return true ;
  if (gsfIter->isEE() && gsfIter->hadronicOverEm() > hadronicOverEmMaxEndcaps_) return true ;

  return false ;
 }


