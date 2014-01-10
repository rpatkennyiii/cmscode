#ifndef UPCVERTEXANALYZER_H
#define UPCVERTEXANALYZER_H

#include <string>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

using namespace reco;
using namespace std;

class UPCVertexAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCVertexAnalyzer(const edm::ParameterSet&);
	~UPCVertexAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	void getVertexData(const VertexCollection*, float*);

	edm::Service<TFileService> mFileServer;
	
	string vertexCName;
	float VertData[6];
	bool isFake;
	unsigned int nTracks;
	TTree* VertTree;
};
DEFINE_FWK_MODULE(UPCVertexAnalyzer);
#endif
