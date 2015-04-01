#ifndef UPCECALCLUSTERANALYZER_H
#define UPCECALCLUSTERANALYZER_H

// system include files
#include <string>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "FWCore/Framework/interface/EventSetup.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

class UPCEcalClusterAnalyzer : public edm::EDAnalyzer {
public:
	explicit UPCEcalClusterAnalyzer(const edm::ParameterSet&);
	~UPCEcalClusterAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
	std::string EcalColName;
	unsigned int nEcalClst;
	float EcalClstEnergy[5000];
	float EcalClstX[5000];
	float EcalClstY[5000];
	float EcalClstZ[5000];
	float EcalClstPhiWidth[5000];
	float EcalClstEtaWidth[5000];

	edm::Service<TFileService> mFileServer;
	
	TTree* UPCEcalClstTree;
};
DEFINE_FWK_MODULE(UPCEcalClusterAnalyzer);

#endif
