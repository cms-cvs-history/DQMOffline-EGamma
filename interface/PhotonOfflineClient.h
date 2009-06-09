#ifndef PhotonOfflineClient_H
#define PhotonOfflineClient_H

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
//
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

// EgammaCoreTools
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"


#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TVector3.h"
#include "TProfile.h"
//


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//

#include <vector>

/** \class PhotonOfflineClient
 **  
 **
 **  $Id: PhotonOfflineClient
 **  $Date: 2009/05/13 14:48:22 $ 
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


class PhotonOfflineClient : public edm::EDAnalyzer
{

 public:
   
  //
  explicit PhotonOfflineClient( const edm::ParameterSet& pset ) ;
  virtual ~PhotonOfflineClient();
                                   
      
  virtual void analyze(const edm::Event&, const edm::EventSetup&  ) ;
  virtual void beginJob( const edm::EventSetup& ) ;
  virtual void endJob() ;
  virtual void endLuminosityBlock( const edm::LuminosityBlock& , const edm::EventSetup& ) ;
 private:
  //

  MonitorElement*  h_filters_;
  MonitorElement* p_efficiencyVsEtaLoose_;
  MonitorElement* p_efficiencyVsEtLoose_;
  MonitorElement* p_efficiencyVsEtaTight_;
  MonitorElement* p_efficiencyVsEtTight_;
  MonitorElement* p_efficiencyVsEtaHLT_;
  MonitorElement* p_efficiencyVsEtHLT_;
  std::vector<MonitorElement*> p_convFractionVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_convFractionVsEta_;
  std::vector<MonitorElement*> p_convFractionVsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_convFractionVsEt_;


  MonitorElement* p_vertexReconstructionEfficiencyVsEta_;




  void doProfileX(TH2 * th2, MonitorElement* me);
  void doProfileX(MonitorElement * th2m, MonitorElement* me);
  void dividePlots(MonitorElement* dividend, MonitorElement* numerator, MonitorElement* denominator);
  void dividePlots(MonitorElement* dividend, MonitorElement* numerator, double denominator); 
      

  DQMStore *dbe_;
  int verbosity_;

  edm::ParameterSet parameters_;

  double cutStep_;
  int numberOfSteps_;
  double etMin_;
  double etMax_;
  int etBin_;
  double etaMin_;
  double etaMax_;
  int etaBin_;
  bool standAlone_;
  std::string outputFileName_;

  std::stringstream currentFolder_;
   
};





#endif
