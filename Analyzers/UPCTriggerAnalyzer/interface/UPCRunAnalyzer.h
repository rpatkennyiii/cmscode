#ifndef UPCRUNANALYZER_H
#define UPCRUNANALYZER_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"
#include "TVectorT.h"
#include "TList.h"

using namespace std;

class UPCRunAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCRunAnalyzer(const edm::ParameterSet&);
	~UPCRunAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	edm::Service<TFileService> mFileServer;

	int RunData[4];

	TTree* RunTree;
};
DEFINE_FWK_MODULE(UPCRunAnalyzer);
#endif
