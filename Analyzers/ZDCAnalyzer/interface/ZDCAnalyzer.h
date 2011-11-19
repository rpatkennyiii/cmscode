#ifndef ZDCANALYZER_H
#define ZDCANALYZER_H

// system include files
#include <string>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// L1 bit analyzer
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

class ZDCAnalyzer : public edm::EDAnalyzer {
public:
	explicit ZDCAnalyzer(const edm::ParameterSet&);
	~ZDCAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
	long runBegin,lumibegin,lumiend,evtNo;
	int run, event, lumi;
	std::string startTime;
	std::string *BranchNames;
        int DigiDataADC[180];
        float DigiDatafC[180];
        float RecData[36];
        float RecDataLowGain[18];
	int BeamData[6];
	double SumHF;
   int bin;
   int hiNpix, hiNpixelTracks, hiNtracks, hiNtracksPtCut, hiNtracksEtaCut, hiNtracksEtaPtCut;
   double hiHF, hiHFplus, hiHFminus, hiHFhit, hiHFhitPlus, hiHFhitMinus, hiEB, hiET, hiEE, hiEEplus, hiEEminus, hiZDC, hiZDCplus, hiZDCminus;

	int Runno;
	edm::Service<TFileService> mFileServer;
	
   CentralityProvider * centrality_;
	
	TTree* ZDCDigiTree;
	TTree* ZDCRecoTree;
	TTree* BeamTree;
// from UserCode/pkenny/Analyzers/L1BitAnalyzer
  TTree *L1BitTree;
  unsigned int RunData[4], nBits;
  bool L1Bits[400], firstEv;
  edm::InputTag l1GtRR_;
  L1GtUtils m_l1GtUtils;

};
DEFINE_FWK_MODULE(ZDCAnalyzer);

#endif
