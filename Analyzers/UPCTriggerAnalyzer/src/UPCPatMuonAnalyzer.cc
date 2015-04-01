#ifndef UPCPATMUONANALYZER_H
#define UPCPATMUONANALYZER_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include <iostream>

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
	void getTrack(const Track *trax,unsigned int &ndof, bool &isHighPurity, float &chi2,unsigned int &vHits, unsigned int &pixHits, unsigned int &traxHits,
	   float &x, float &y, float &z,
	   float &p, float &qoverp, float &lambda, float &phi,
	   float &varqoverp, float &varlambda, float &varphi,
	   float &covarqoverplambda, float &covarqoverpphi, float &covarlambdaphi);

	edm::InputTag muonLabel;
	edm::Service<TFileService> mFileServer;

	TTree* MuonTree;

	bool	
	isGlobalMuon[100],
	isTrackerMuon[100],
	isStandAloneMuon[100],
	isCaloMuon[100],
	isHighPurity[100],
	isGoodMuon[100];
	
	unsigned int
	nMuon,
	l1Quality[100],
	ndof[100],
	numberOfValidTrackerHits[100],
	pixelLayersWithMeasurement[100],
	trackerLayersWithMeasurement[100];

	float
	caloMuonTower[100],
	caloMuonTowerS9[100],
	caloMuonEm[100],
	caloMuonEmS9[100],
	caloMuonEmS25[100],
	caloMuonEmMax[100],
	caloMuonHad[100],
	caloMuonHadS9[100],
	caloMuonHadMax[100],
	caloMuonCompatibility[100],
	chi2[100],
	l1DeltaR[100],
	p[100],
	qoverp[100],
	lambda[100],
	phi[100],
	varQoverp[100],
	varLambda[100],
	varPhi[100],
	covarQoverpLambda[100],
	covarQoverpPhi[100],
	covarLambdaPhi[100],
	x[100],
	y[100],
	z[100];
};
DEFINE_FWK_MODULE(UPCPatMuonAnalyzer);

using namespace edm;

UPCPatMuonAnalyzer::UPCPatMuonAnalyzer(const edm::ParameterSet& iConfig):muonLabel(iConfig.getParameter<InputTag>("muonLabel")){}

UPCPatMuonAnalyzer::~UPCPatMuonAnalyzer(){}

void UPCPatMuonAnalyzer::beginJob(){
	mFileServer->file().cd();

	MuonTree = new TTree(TString(muonLabel.label())+"Tree",TString(muonLabel.label())+"Tree");
	MuonTree->Branch("nMuon",&nMuon,"nMuon/i");

	MuonTree->Branch("isGlobalMuon",isGlobalMuon,"isGlobalMuon[nMuon]/O");
	MuonTree->Branch("isTrackerMuon",isTrackerMuon,"isTrackerMuon[nMuon]/O");
	MuonTree->Branch("isStandAloneMuon",isStandAloneMuon,"isStandAloneMuon[nMuon]/O");
	MuonTree->Branch("isCaloMuon",isCaloMuon,"isCaloMuon[nMuon]/O");
	MuonTree->Branch("isHighPurity",isHighPurity,"isHighPurity[nMuon]/O");

	MuonTree->Branch("caloMuonTower",caloMuonTower,"caloMuonTower[nMuon]/F");
	MuonTree->Branch("caloMuonTowerS9",caloMuonTowerS9,"caloMuonTowerS9[nMuon]/F");
	MuonTree->Branch("caloMuonEm",caloMuonEm,"caloMuonEm[nMuon]/F");
	MuonTree->Branch("caloMuonEmS9",caloMuonEmS9,"caloMuonEmS9[nMuon]/F");
	MuonTree->Branch("caloMuonEmS25",caloMuonEmS25,"caloMuonEmS25[nMuon]/F");
	MuonTree->Branch("caloMuonEmMax",caloMuonEmMax,"caloMuonEmMax[nMuon]/F");
	MuonTree->Branch("caloMuonHad",caloMuonHad,"caloMuonHad[nMuon]/F");
	MuonTree->Branch("caloMuonHadS9",caloMuonHadS9,"caloMuonHadS9[nMuon]/F");
	MuonTree->Branch("caloMuonHadMax",caloMuonHadMax,"caloMuonHadMax[nMuon]/F");
	MuonTree->Branch("caloMuonCompatibility",caloMuonCompatibility,"caloMuonCompatibility[nMuon]/F");

        MuonTree->Branch("ndof",ndof,"ndof[nMuon]/i");
	MuonTree->Branch("numberOfValidTrackerHits",numberOfValidTrackerHits,"numberOfValidTrackerHits[nMuon]/i");
	MuonTree->Branch("pixelLayersWithMeasurement",pixelLayersWithMeasurement,"pixelLayersWithMeasurement[nMuon]/i");
	MuonTree->Branch("trackerLayersWithMeasurement",trackerLayersWithMeasurement,"trackerLayersWithMeasurement[nMuon]/i");
	MuonTree->Branch("chi2",chi2,"chi2[nMuon]/F");
	MuonTree->Branch("isGoodMuon",isGoodMuon,"isGoodMuon[nMuon]/O");

	MuonTree->Branch("p",p,"p[nMuon]/F");
	MuonTree->Branch("qoverp",qoverp,"qoverp[nMuon]/F");
	MuonTree->Branch("lambda",lambda,"lambda[nMuon]/F");
	MuonTree->Branch("phi",phi,"phi[nMuon]/F");
	MuonTree->Branch("varQoverp",varQoverp,"varQoverp[nMuon]/F");
	MuonTree->Branch("varLambda",varLambda,"varLambda[nMuon]/F");
	MuonTree->Branch("varPhi",varPhi,"varPhi[nMuon]/F");
	MuonTree->Branch("covarQoverpLambda",covarQoverpLambda,"covarQoverpLambda[nMuon]/F");
	MuonTree->Branch("covarQoverpPhi",covarQoverpPhi,"covarQoverpPhi[nMuon]/F");
	MuonTree->Branch("covarLambdaPhi",covarLambdaPhi,"covarLambdaPhi[nMuon]/F");
	MuonTree->Branch("x",x,"x[nMuon]/F");
	MuonTree->Branch("y",y,"y[nMuon]/F");
	MuonTree->Branch("z",z,"z[nMuon]/F");

        MuonTree->Branch("l1DeltaR",l1DeltaR,"l1DeltaR[nMuon]/F");
        MuonTree->Branch("l1Quality",l1Quality,"l1Quality[nMuon]/i");
}

void UPCPatMuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<vector<pat::Muon> > muons;
	iEvent.getByLabel(muonLabel,muons);

	if(!muons.failedToGet()){
		nMuon=0;
		for(unsigned int i=0; i<(*muons).size() && i<100; i++){
		        isGlobalMuon[i]=(*muons)[i].isGlobalMuon();
		        isTrackerMuon[i]=(*muons)[i].isTrackerMuon();
		        isStandAloneMuon[i]=(*muons)[i].isStandAloneMuon();
			isGoodMuon[i]=muon::isGoodMuon((*muons)[i],muon::TMOneStationTight);

			MuonEnergy muE=(*muons)[i].calEnergy();
			
			caloMuonTower[i]=muE.tower;
			caloMuonTowerS9[i]=muE.towerS9;
			caloMuonEm[i]=muE.em;
			caloMuonEmS9[i]=muE.emS9;
			caloMuonEmS25[i]=muE.emS25;
			caloMuonEmMax[i]=muE.emMax;
			caloMuonHad[i]=muE.had;
			caloMuonHadS9[i]=muE.hadS9;
			caloMuonHadMax[i]=muE.hadMax;
		        caloMuonCompatibility[i]=(*muons)[i].caloCompatibility();

		        l1DeltaR[i]=(*muons)[i].userFloat("muonL1Info:deltaR");
		        l1Quality[i]=(*muons)[i].userInt("muonL1Info:quality");

			const Track *track;
			if (isStandAloneMuon[i] && !(isGlobalMuon[i])) track=((*muons)[i].standAloneMuon()).get();
			else  track=((*muons)[i].track()).get();
		        getTrack(track,ndof[nMuon], isHighPurity[nMuon], chi2[nMuon],numberOfValidTrackerHits[nMuon],pixelLayersWithMeasurement[nMuon],trackerLayersWithMeasurement[nMuon],x[nMuon],y[nMuon],z[nMuon],p[nMuon],qoverp[nMuon],lambda[nMuon],phi[nMuon],varQoverp[nMuon],varLambda[nMuon],varPhi[nMuon],
				  covarQoverpLambda[nMuon],covarQoverpPhi[nMuon],covarLambdaPhi[nMuon]);
			nMuon++;
		}
	}

	MuonTree->Fill();
}

void UPCPatMuonAnalyzer::getTrack(const Track *trax, unsigned int &ndof, bool &isHighPurity, float &chi2, unsigned int &vHits, unsigned int &pixHits, unsigned int &traxHits,
	float &x, float &y, float &z,
	float &p, float &qoverp, float &lambda, float &phi,
	float &varqoverp, float &varlambda, float &varphi,
	float &covarqoverplambda, float &covarqoverpphi, float &covarlambdaphi){
	x=trax->vx();
	y=trax->vy();
	z=trax->vz();
	p=trax->p();
	qoverp=trax->qoverp();
	lambda=trax->lambda();
	phi=trax->phi();
	varqoverp=trax->covariance(0,0);
	varlambda=trax->covariance(1,1);
	varphi=trax->covariance(2,2);
	covarqoverplambda=trax->covariance(0,1);
	covarqoverpphi=trax->covariance(0,2);
	covarlambdaphi=trax->covariance(1,2);
	chi2=trax->chi2();
	ndof=trax->ndof();
	isHighPurity=trax->quality(reco::TrackBase::highPurity);
	vHits=trax->hitPattern().numberOfValidTrackerHits();
	pixHits=trax->hitPattern().pixelLayersWithMeasurement();
	traxHits=trax->hitPattern().trackerLayersWithMeasurement();
}

#endif
