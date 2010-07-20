#ifndef ZDCANALYZER_H
#define ZDCANALYZER_H

// system include files
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sys/time.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

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
	virtual void endJob();
private:
	long runBegin,lumibegin,lumiend,evtNo;
	int run, event, lumi;
	std::string startTime;
	std::string *BranchNames;
        double DigiData[180];
        double RecData[36];
	float BeamData[2];

	int Runno;
	edm::Service<TFileService> mFileServer;

	TTree* ZDCDigiTree;
	TTree* ZDCRecoTree;
	TTree* BeamTree;
};
DEFINE_FWK_MODULE(ZDCAnalyzer);

#endif
