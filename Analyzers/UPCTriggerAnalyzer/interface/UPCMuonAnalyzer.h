#ifndef UPCMUONANALYZER_H
#define UPCMUONANALYZER_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"
#include "TVectorT.h"
#include "TList.h"

using namespace reco;
using namespace std;

class UPCMuonAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCMuonAnalyzer(const edm::ParameterSet&);
	~UPCMuonAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	edm::Service<TFileService> mFileServer;
	vector<reco::Muon> Muonz;

	TTree* MuonTree;
};
DEFINE_FWK_MODULE(UPCMuonAnalyzer);
#endif
