#ifndef UPCCASTORANALYZER_H
#define UPCCASTORANALYZER_H

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

class UPCCastorAnalyzer : public edm::EDAnalyzer {
public:
	explicit UPCCastorAnalyzer(const edm::ParameterSet&);
	~UPCCastorAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
	unsigned int nbOfCASTORRecHits, CastorRecHitModule[2240], CastorRecHitSector[2240];
        float CastorRecHitData[2240];

	edm::Service<TFileService> mFileServer;
	
	TTree* CastorRecHitTree;
};
DEFINE_FWK_MODULE(UPCCastorAnalyzer);

#endif
