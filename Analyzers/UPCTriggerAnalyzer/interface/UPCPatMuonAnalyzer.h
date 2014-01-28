#ifndef UPCPATMUONANALYZER_H
#define UPCPATMUONANALYZER_H

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
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

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

class UPCPatMuonAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCPatMuonAnalyzer(const edm::ParameterSet&);
	~UPCPatMuonAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	void getTrack(const Track *trax,unsigned int &ndof, float &chi2,unsigned int &vHits, unsigned int &pixHits, unsigned int &traxHits,
	   float &x, float &y, float &z,
	   float &p, float &qoverp, float &lambda, float &phi,
	   float &varqoverp, float &varlambda, float &varphi,
	   float &covarqoverplambda, float &covarqoverpphi, float &covarlambdaphi);

	edm::InputTag muonLabel;
	edm::Service<TFileService> mFileServer;

	TTree* MuonTree;
	
	unsigned int
	nGlobalMuon,
	nTrackerMuon,
	nStandAloneMuon,
	nCaloMuon;
	
	unsigned int
	l1QualityTracker[100],
	l1QualityGlobal[100],
	l1QualityStandAlone[100],
	ndofTracker[100],
	ndofGlobal[100],
	ndofStandAlone[100],
	numberOfValidTrackerHitsTracker[100],
	numberOfValidTrackerHitsGlobal[100],
	numberOfValidTrackerHitsStandAlone[100],
	pixelLayersWithMeasurementTracker[100],
	pixelLayersWithMeasurementGlobal[100],
	pixelLayersWithMeasurementStandAlone[100],
	trackerLayersWithMeasurementTracker[100],
	trackerLayersWithMeasurementGlobal[100],
	trackerLayersWithMeasurementStandAlone[100];

	bool
	isPat,
	isGoodMuonTracker[100],
	isGoodMuonGlobal[100],
	isGoodMuonStandAlone[100];	
	
	float
	CaloMuonEnergy;

	float
	chi2Tracker[100],
	chi2Global[100],
	chi2StandAlone[100],
	l1DeltaRTracker[100],
	l1DeltaRGlobal[100],
	l1DeltaRStandAlone[100];
	
	float
	pTracker[100],
	qoverpTracker[100],
	lambdaTracker[100],
	phiTracker[100],
	varQoverpTracker[100],
	varLambdaTracker[100],
	varPhiTracker[100],
	covarQoverpLambdaTracker[100],
	covarQoverpPhiTracker[100],
	covarLambdaPhiTracker[100],
	xTracker[100],
	yTracker[100],
	zTracker[100];

	float
	pGlobal[100],
	qoverpGlobal[100],
	lambdaGlobal[100],
	phiGlobal[100],
	varQoverpGlobal[100],
	varLambdaGlobal[100],
	varPhiGlobal[100],
	covarQoverpLambdaGlobal[100],
	covarQoverpPhiGlobal[100],
	covarLambdaPhiGlobal[100],
	xGlobal[100],
	yGlobal[100],
	zGlobal[100];

	float
	pStandAlone[100],
	qoverpStandAlone[100],
	lambdaStandAlone[100],
	phiStandAlone[100],
	varQoverpStandAlone[100],
	varLambdaStandAlone[100],
	varPhiStandAlone[100],
	covarQoverpLambdaStandAlone[100],
	covarQoverpPhiStandAlone[100],
	covarLambdaPhiStandAlone[100],
	xStandAlone[100],
	yStandAlone[100],
	zStandAlone[100];
};
DEFINE_FWK_MODULE(UPCPatMuonAnalyzer);
#endif
