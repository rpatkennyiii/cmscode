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
    CentralityProvider * centrality_;

	int Runno;
	edm::Service<TFileService> mFileServer;
	
	TTree* ZDCDigiTree;
	TTree* ZDCRecoTree;
	TTree* BeamTree;
};
DEFINE_FWK_MODULE(ZDCAnalyzer);

#endif
