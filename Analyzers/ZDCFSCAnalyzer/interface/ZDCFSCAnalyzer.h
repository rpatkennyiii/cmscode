#ifndef ZDCFSCANALYZER_H
#define ZDCFSCANALYZER_H

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

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

class ZDCFSCAnalyzer : public edm::EDAnalyzer {
public:
	explicit ZDCFSCAnalyzer(const edm::ParameterSet&);
	~ZDCFSCAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
	long runBegin,lumibegin,lumiend,evtNo;
	int run, event, lumi;
	std::string startTime;
	std::string *BranchNames;
        int DigiDataADC[340];
        float DigiDatafC[340];
        float RecData[34];
	int BeamData[2];

	int Runno;
	edm::Service<TFileService> mFileServer;
	
	TTree* ZDCDigiTree;
	TTree* FSCDigiTree;
	TTree* BeamTree;
};
DEFINE_FWK_MODULE(ZDCFSCAnalyzer);

#endif
