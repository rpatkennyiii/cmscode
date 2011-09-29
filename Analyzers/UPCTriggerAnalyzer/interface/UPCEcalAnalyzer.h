#ifndef UPCECALANALYZER_H
#define UPCECALANALYZER_H

// system include files
#include <string>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Framework/interface/EventSetup.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

class UPCEcalAnalyzer : public edm::EDAnalyzer {
public:
	explicit UPCEcalAnalyzer(const edm::ParameterSet&);
	~UPCEcalAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
	std::string EcalColName;
	unsigned int nEcal;
        float EcalData[137000];

	edm::Service<TFileService> mFileServer;
	
	TTree* UPCEcalTree;
};
DEFINE_FWK_MODULE(UPCEcalAnalyzer);

#endif
