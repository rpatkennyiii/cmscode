#ifndef UPCTRIGGERANALYZER_H
#define UPCTRIGGERANALYZER_H

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

class UPCTriggerAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCTriggerAnalyzer(const edm::ParameterSet&);
	~UPCTriggerAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	edm::Service<TFileService> mFileServer;
	CentralityProvider *CentProv;
	vector<reco::Muon> Muonz;

	int nGPTracks, nSelTracks;
	int RunData[4];
	int centi;	
	double cent[2];
	float VertData[6];
	vector<int> ndofGP,ndofSel;
	vector<double> chi2GP,xGP, yGP, zGP, 
	   pGP, qoverpGP, lambdaGP, phiGP,
	   varQoverpGP, varLambdaGP, varPhiGP,
	   covarQoverpLambdaGP,covarQoverpPhiGP,covarLambdaPhiGP;
	vector<double> chi2Sel,xSel, ySel, zSel, 
	   pSel, qoverpSel, lambdaSel, phiSel,
	   varQoverpSel, varLambdaSel, varPhiSel,
	   covarQoverpLambdaSel,covarQoverpPhiSel,covarLambdaPhiSel;

//	TTree* MuonTree;
	TTree* TrakTree;
	TTree* CenTree;
	TTree* RunTree;
	TTree* VertTree;
};
DEFINE_FWK_MODULE(UPCTriggerAnalyzer);
#endif
