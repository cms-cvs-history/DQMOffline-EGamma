#ifndef PhotonAnalyzer_H
#define PhotonAnalyzer_H

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
// DataFormats
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
/// EgammaCoreTools
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalEtaPhiRegion.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
// Geometry
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
//
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TVector3.h"
#include "TProfile.h"
//

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
//
//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//
#include <map>
#include <vector>

/** \class PhotonAnalyzer
 **  
 **
 **  $Id: PhotonAnalyzer
 **  $Date: 2008/09/30 19:51:05 $ 
 **  authors: 
 **   Nancy Marinelli, U. of Notre Dame, US  
 **   Jamie Antonelli, U. of Notre Dame, US
 **     
 ***/


// forward declarations
class TFile;
class TH1F;
class TH2F;
class TProfile;
class TTree;
class SimVertex;
class SimTrack;


class PhotonAnalyzer : public edm::EDAnalyzer
{

 public:
   
  //
  explicit PhotonAnalyzer( const edm::ParameterSet& ) ;
  virtual ~PhotonAnalyzer();
                                   
      
  virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
  virtual void beginJob( const edm::EventSetup& ) ;
  virtual void endJob() ;
 
 private:
  //

  float  phiNormalization( float& a);
  void makePizero(const edm::EventSetup& es, const edm::Handle<EcalRecHitCollection> eb, const edm::Handle<EcalRecHitCollection> ee ); 


  void doProfileX(TH2 * th2, MonitorElement* me);
  void doProfileX(MonitorElement * th2m, MonitorElement* me);
  void dividePlots(MonitorElement* dividend, MonitorElement* numerator, MonitorElement* denominator);
  void dividePlots(MonitorElement* dividend, MonitorElement* numerator, double denominator); 
      
      
  std::string fName_;
  DQMStore *dbe_;
  int verbosity_;

  int nEvt_;
  int nEntry_;

  unsigned int prescaleFactor_;


  edm::ParameterSet parameters_;
           
  std::string photonProducer_;       
  std::string photonCollection_;

  edm::InputTag barrelEcalHits_;
  edm::InputTag endcapEcalHits_;  

  edm::InputTag triggerSummary_;
  edm::InputTag triggerResultsHLT_;
  edm::InputTag triggerResultsFU_;

  HLTConfigProvider hltConfig_;


  double minPhoEtCut_;

  double cutStep_;
  int numberOfSteps_;

  bool useBinning_;
  bool useTriggerFiltering_;
  bool standAlone_;

  int isolationStrength_; 

  /// parameters needed for pizero finding
  double clusSeedThr_;
  int clusEtaSize_;
  int clusPhiSize_;

  double seleXtalMinEnergy_;

  bool ParameterLogWeighted_;
  double ParameterX0_;
  double ParameterT0_barl_;
  double ParameterW0_;

  double selePtGammaOne_;
  double selePtGammaTwo_;
  double selePtPi0_;
  double seleS4S9GammaOne_;
  double seleS4S9GammaTwo_;
  double selePi0BeltDR_;
  double selePi0BeltDeta_;
  double selePi0Iso_;
  double seleMinvMaxPi0_;
  double seleMinvMinPi0_;
 


  std::stringstream currentFolder_;
   
  MonitorElement*  h_triggers_;

  MonitorElement*  hMinvPi0EB_;
  MonitorElement*  hPt1Pi0EB_;
  MonitorElement*  hPt2Pi0EB_;
  MonitorElement*  hIsoPi0EB_;
  MonitorElement*  hPtPi0EB_;


  std::vector<MonitorElement*> h_nTrackIsolSolid_;
  std::vector<MonitorElement*> h_trackPtSumSolid_;
  std::vector<MonitorElement*> h_nTrackIsolHollow_;
  std::vector<MonitorElement*> h_trackPtSumHollow_;
  std::vector<MonitorElement*> h_ecalSum_;
  std::vector<MonitorElement*> h_hcalSum_;

  std::vector<MonitorElement*> p_nTrackIsolSolid_;
  std::vector<MonitorElement*> p_trackPtSumSolid_;
  std::vector<MonitorElement*> p_nTrackIsolHollow_;
  std::vector<MonitorElement*> p_trackPtSumHollow_;
  std::vector<MonitorElement*> p_ecalSum_;
  std::vector<MonitorElement*> p_hcalSum_;

  std::vector<MonitorElement*> p_efficiencyVsEta_;
  std::vector<MonitorElement*> p_efficiencyVsEt_;

  std::vector<MonitorElement*> p_convFractionVsEta_;
  std::vector<MonitorElement*> p_convFractionVsEt_;

  std::vector<MonitorElement*> h_phoE_part_;
  std::vector<std::vector<MonitorElement*> > h_phoE_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoE_;

  std::vector<MonitorElement*> h_phoEt_part_;
  std::vector<std::vector<MonitorElement*> > h_phoEt_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoEt_;

  std::vector<MonitorElement*> h_r9_part_;
  std::vector<std::vector<MonitorElement*> > h_r9_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_r9_;

  std::vector<MonitorElement*> h_hOverE_part_;
  std::vector<std::vector<MonitorElement*> > h_hOverE_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_hOverE_;

  std::vector<MonitorElement*> h_nPho_part_;
  std::vector<std::vector<MonitorElement*> > h_nPho_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_nPho_;

  std::vector<MonitorElement*> h_phoDistribution_part_;
  std::vector<std::vector<MonitorElement*> > h_phoDistribution_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoDistribution_;


  std::vector<MonitorElement*> h_phoConvEt_part_;
  std::vector<std::vector<MonitorElement*> > h_phoConvEt_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoConvEt_;

  std::vector<MonitorElement*> h_nConv_part_;
  std::vector<std::vector<MonitorElement*> > h_nConv_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_nConv_;

  std::vector<MonitorElement*> h_eOverPTracks_part_;
  std::vector<std::vector<MonitorElement*> > h_eOverPTracks_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_eOverPTracks_;

  std::vector<MonitorElement*> h_dCotTracks_part_;
  std::vector<std::vector<MonitorElement*> > h_dCotTracks_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dCotTracks_;

  std::vector<MonitorElement*> h_dPhiTracksAtVtx_part_;
  std::vector<std::vector<MonitorElement*> > h_dPhiTracksAtVtx_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dPhiTracksAtVtx_;

  std::vector<MonitorElement*> h_dPhiTracksAtEcal_part_;
  std::vector<std::vector<MonitorElement*> > h_dPhiTracksAtEcal_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dPhiTracksAtEcal_;

  std::vector<MonitorElement*> h_dEtaTracksAtEcal_part_;
  std::vector<std::vector<MonitorElement*> > h_dEtaTracksAtEcal_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dEtaTracksAtEcal_;


  std::vector<MonitorElement*> h_phoEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoEta_;
  std::vector<MonitorElement*> h_phoPhi_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoPhi_;

  std::vector<MonitorElement*> h_phoConvEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoConvEta_;
  std::vector<MonitorElement*> h_phoConvPhi_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoConvPhi_;

  std::vector<MonitorElement*> h_convVtxRvsZ_isol_;
  std::vector<std::vector<MonitorElement*> > h_convVtxRvsZ_;
  std::vector<MonitorElement*> h_convVtxRvsZLowEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_convVtxRvsZLowEta_;
  std::vector<MonitorElement*> h_convVtxRvsZHighEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_convVtxRvsZHighEta_;

  std::vector<MonitorElement*> h_r9VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > h_r9VsEt_;
  std::vector<MonitorElement*> p_r9VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_r9VsEt_;

  std::vector<MonitorElement*> h_tkChi2_isol_;
  std::vector<std::vector<MonitorElement*> > h_tkChi2_;

  std::vector<MonitorElement*> h_nHitsVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_nHitsVsEta_;
  std::vector<MonitorElement*> p_nHitsVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_nHitsVsEta_;

  //
  //
  


};


class ecalRecHitLess : public std::binary_function<EcalRecHit, EcalRecHit, bool> 
{
public:
  bool operator()(EcalRecHit x, EcalRecHit y) 
  { 
    return (x.energy() > y.energy()); 
  }
};


#endif
