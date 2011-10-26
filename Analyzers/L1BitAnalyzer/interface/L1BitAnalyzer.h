#ifndef L1BITANALYZER_H
#define L1BITANALYZER_H

#include <string>

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

class L1BitAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1BitAnalyzer(edm::ParameterSet const& conf);
  ~L1BitAnalyzer();
  virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
  virtual void beginRun(const edm::Run& , const edm::EventSetup& );
  virtual void endJob();

private:
  edm::Service<TFileService> mFileServer;
  TTree *L1BitTree;
  unsigned int RunData[4], nBits;
  bool L1Bits[400], firstEv;
  edm::InputTag l1GtRR_;
  L1GtUtils m_l1GtUtils;
};
DEFINE_FWK_MODULE(L1BitAnalyzer);

#endif
