#ifndef FSCANALYZER_H
#define FSCANALYZER_H

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "TTree.h"
#include "TFile.h"
#include "TString.h"

class FSCAnalyzer : public edm::EDAnalyzer {
public:
  explicit FSCAnalyzer(edm::ParameterSet const& conf);
  ~FSCAnalyzer();
  virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
  virtual void beginRun(const edm::Run& , const edm::EventSetup& );
  virtual void endJob();

private:
  edm::Service<TFileService> mFileServer;
  TTree *FSCTree;
  unsigned int RunData[4], nBits;
  bool FSCBits[16], ZeroBiasBit;
  TString FSCNames[16];
  edm::InputTag l1GtRR_;
  L1GtUtils m_l1GtUtils;
};
DEFINE_FWK_MODULE(FSCAnalyzer);

#endif
