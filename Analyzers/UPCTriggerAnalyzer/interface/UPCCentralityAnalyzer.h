#ifndef UPCCENTRALITYANALYZER_H
#define UPCCENTRALITYANALYZER_H

#include <string>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

using namespace reco;
using namespace std;

class UPCCentralityAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCCentralityAnalyzer(const edm::ParameterSet&);
	~UPCCentralityAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	edm::Service<TFileService> mFileServer;
	CentralityProvider *CentProv;

	int centi;	
	double cent[2];
	string centV;

	TTree* CenTree;
};
DEFINE_FWK_MODULE(UPCCentralityAnalyzer);
#endif
