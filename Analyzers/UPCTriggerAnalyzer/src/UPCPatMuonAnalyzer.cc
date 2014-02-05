#include "Analyzers/UPCTriggerAnalyzer/interface/UPCPatMuonAnalyzer.h"
#include <iostream>

using namespace edm;

UPCPatMuonAnalyzer::UPCPatMuonAnalyzer(const edm::ParameterSet& iConfig):muonLabel(iConfig.getParameter<InputTag>("muonLabel")){}

UPCPatMuonAnalyzer::~UPCPatMuonAnalyzer(){}

void UPCPatMuonAnalyzer::beginJob(){
	mFileServer->file().cd();

	isPat=((muonLabel.label()).substr(0,3)=="pat");

	MuonTree = new TTree(TString(muonLabel.label())+"Tree",TString(muonLabel.label())+"Tree");
	MuonTree->Branch("nGlobalMuon",&nGlobalMuon,"nGlobalMuon/i");
	MuonTree->Branch("nTrackerMuon",&nTrackerMuon,"nTrackerMuon/i");
	MuonTree->Branch("nStandAloneMuon",&nStandAloneMuon,"nStandAloneMuon/i");
	MuonTree->Branch("nCaloMuon",&nCaloMuon,"nCaloMuon/i");

	MuonTree->Branch("CaloMuonEnergy",&CaloMuonEnergy,"CaloMuonEnergy[nCaloMuon]/F");

	MuonTree->Branch("ndofTracker",ndofTracker,"ndofTracker[nTrackerMuon]/i");
	MuonTree->Branch("numberOfValidTrackerHitsTracker",numberOfValidTrackerHitsTracker,"numberOfValidTrackerHitsTracker[nTrackerMuon]/i");
	MuonTree->Branch("pixelLayersWithMeasurementTracker",pixelLayersWithMeasurementTracker,"pixelLayersWithMeasurementTracker[nTrackerMuon]/i");
	MuonTree->Branch("trackerLayersWithMeasurementTracker",trackerLayersWithMeasurementTracker,"trackerLayersWithMeasurementTracker[nTrackerMuon]/i");
	MuonTree->Branch("chi2Tracker",chi2Tracker,"chi2Tracker[nTrackerMuon]/F");
	MuonTree->Branch("isGoodMuonTracker",isGoodMuonTracker,"isGoodMuonTracker[nTrackerMuon]/O");
	
	MuonTree->Branch("pTracker",pTracker,"pTracker[nTrackerMuon]/F");
	MuonTree->Branch("qoverpTracker",qoverpTracker,"qoverpTracker[nTrackerMuon]/F");
	MuonTree->Branch("lambdaTracker",lambdaTracker,"lambdaTracker[nTrackerMuon]/F");
	MuonTree->Branch("phiTracker",phiTracker,"phiTracker[nTrackerMuon]/F");
	MuonTree->Branch("varQoverpTracker",varQoverpTracker,"varQoverpTracker[nTrackerMuon]/F");
	MuonTree->Branch("varLambdaTracker",varLambdaTracker,"varLambdaTracker[nTrackerMuon]/F");
	MuonTree->Branch("varPhiTracker",varPhiTracker,"varPhiTracker[nTrackerMuon]/F");
	MuonTree->Branch("covarQoverpLambdaTracker",covarQoverpLambdaTracker,"covarQoverpLambdaTracker[nTrackerMuon]/F");
	MuonTree->Branch("covarQoverpPhiTracker",covarQoverpPhiTracker,"covarQoverpPhiTracker[nTrackerMuon]/F");
	MuonTree->Branch("covarLambdaPhiTracker",covarLambdaPhiTracker,"covarLambdaPhiTracker[nTrackerMuon]/F");
	MuonTree->Branch("xTracker",xTracker,"xTracker[nTrackerMuon]/F");
	MuonTree->Branch("yTracker",yTracker,"yTracker[nTrackerMuon]/F");
	MuonTree->Branch("zTracker",zTracker,"zTracker[nTrackerMuon]/F");

	MuonTree->Branch("ndofGlobal",ndofGlobal,"ndofGlobal[nGlobalMuon]/i");
	MuonTree->Branch("numberOfValidTrackerHitsGlobal",numberOfValidTrackerHitsGlobal,"numberOfValidTrackerHitsGlobal[nGlobalMuon]/i");
	MuonTree->Branch("pixelLayersWithMeasurementGlobal",pixelLayersWithMeasurementGlobal,"pixelLayersWithMeasurementGlobal[nGlobalMuon]/i");
	MuonTree->Branch("trackerLayersWithMeasurementGlobal",trackerLayersWithMeasurementGlobal,"trackerLayersWithMeasurementGlobal[nGlobalMuon]/i");
	MuonTree->Branch("chi2Global",chi2Global,"chi2Global[nGlobalMuon]/F");
	MuonTree->Branch("isGoodMuonGlobal",isGoodMuonGlobal,"isGoodMuonGlobal[nGlobalMuon]/O");

	MuonTree->Branch("pGlobal",pGlobal,"pGlobal[nGlobalMuon]/F");
	MuonTree->Branch("qoverpGlobal",qoverpGlobal,"qoverpGlobal[nGlobalMuon]/F");
	MuonTree->Branch("lambdaGlobal",lambdaGlobal,"lambdaGlobal[nGlobalMuon]/F");
	MuonTree->Branch("phiGlobal",phiGlobal,"phiGlobal[nGlobalMuon]/F");
	MuonTree->Branch("varQoverpGlobal",varQoverpGlobal,"varQoverpGlobal[nGlobalMuon]/F");
	MuonTree->Branch("varLambdaGlobal",varLambdaGlobal,"varLambdaGlobal[nGlobalMuon]/F");
	MuonTree->Branch("varPhiGlobal",varPhiGlobal,"varPhiGlobal[nGlobalMuon]/F");
	MuonTree->Branch("covarQoverpLambdaGlobal",covarQoverpLambdaGlobal,"covarQoverpLambdaGlobal[nGlobalMuon]/F");
	MuonTree->Branch("covarQoverpPhiGlobal",covarQoverpPhiGlobal,"covarQoverpPhiGlobal[nGlobalMuon]/F");
	MuonTree->Branch("covarLambdaPhiGlobal",covarLambdaPhiGlobal,"covarLambdaPhiGlobal[nGlobalMuon]/F");
	MuonTree->Branch("xGlobal",xGlobal,"xGlobal[nGlobalMuon]/F");
	MuonTree->Branch("yGlobal",yGlobal,"yGlobal[nGlobalMuon]/F");
	MuonTree->Branch("zGlobal",zGlobal,"zGlobal[nGlobalMuon]/F");

	MuonTree->Branch("ndofStandAlone",ndofStandAlone,"ndofStandAlone[nStandAloneMuon]/i");
	MuonTree->Branch("numberOfValidTrackerHitsStandAlone",numberOfValidTrackerHitsStandAlone,"numberOfValidTrackerHitsStandAlone[nStandAloneMuon]/i");
	MuonTree->Branch("pixelLayersWithMeasurementStandAlone",pixelLayersWithMeasurementStandAlone,"pixelLayersWithMeasurementStandAlone[nStandAloneMuon]/i");
	MuonTree->Branch("trackerLayersWithMeasurementStandAlone",trackerLayersWithMeasurementStandAlone,"trackerLayersWithMeasurementStandAlone[nStandAloneMuon]/i");
	MuonTree->Branch("chi2StandAlone",chi2StandAlone,"chi2StandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("isGoodMuonStandAlone",isGoodMuonStandAlone,"isGoodMuonStandAlone[nStandAloneMuon]/O");

	MuonTree->Branch("pStandAlone",pStandAlone,"pStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("qoverpStandAlone",qoverpStandAlone,"qoverpStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("lambdaStandAlone",lambdaStandAlone,"lambdaStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("phiStandAlone",phiStandAlone,"phiStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("varQoverpStandAlone",varQoverpStandAlone,"varQoverpStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("varLambdaStandAlone",varLambdaStandAlone,"varLambdaStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("varPhiStandAlone",varPhiStandAlone,"varPhiStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("covarQoverpLambdaStandAlone",covarQoverpLambdaStandAlone,"covarQoverpLambdaStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("covarQoverpPhiStandAlone",covarQoverpPhiStandAlone,"covarQoverpPhiStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("covarLambdaPhiStandAlone",covarLambdaPhiStandAlone,"covarLambdaPhiStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("xStandAlone",xStandAlone,"xStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("yStandAlone",yStandAlone,"yStandAlone[nStandAloneMuon]/F");
	MuonTree->Branch("zStandAlone",zStandAlone,"zStandAlone[nStandAloneMuon]/F");

	if(isPat){
		MuonTree->Branch("l1DeltaRGlobal",l1DeltaRGlobal,"l1DeltaRGlobal[nGlobalMuon]/F");
		MuonTree->Branch("l1QualityGlobal",l1QualityGlobal,"l1QualityGlobal[nGlobalMuon]/i");
		MuonTree->Branch("l1DeltaRTracker",l1DeltaRTracker,"l1DeltaRTracker[nTrackerMuon]/F");
		MuonTree->Branch("l1QualityTracker",l1QualityTracker,"l1QualityTracker[nTrackerMuon]/i");
		MuonTree->Branch("l1DeltaRStandAlone",l1DeltaRStandAlone,"l1DeltaRStandAlone[nStandAloneMuon]/F");
		MuonTree->Branch("l1QualityStandAlone",l1QualityStandAlone,"l1QualityStandAlone[nStandAloneMuon]/i");
	}
}

void UPCPatMuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<vector<pat::Muon> > muons;
	iEvent.getByLabel(muonLabel,muons);

	nGlobalMuon=0;
	nTrackerMuon=0;
	nStandAloneMuon=0;
	nCaloMuon=0;
	
	if(!muons.failedToGet()){
		for(unsigned int i=0; i<(*muons).size();i++){
			bool isGood=muon::isGoodMuon((*muons)[i],muon::TMOneStationTight);
			float deltaR(0);
			int quality(0);

			if(isPat){ 
				const pat::Muon muon1 = (*muons)[i];
				deltaR=muon1.userFloat("muonL1Info:deltaR");
				quality=muon1.userInt("muonL1Info:quality");
			}

			if(((*muons)[i]).isGlobalMuon()&&nGlobalMuon<100){
				isGoodMuonGlobal[nGlobalMuon]=isGood;
				l1DeltaRGlobal[nGlobalMuon]=deltaR;
				l1QualityGlobal[nGlobalMuon]=quality;

				getTrack(((*muons)[i].globalTrack()).get(),ndofGlobal[nGlobalMuon],chi2Global[nGlobalMuon],numberOfValidTrackerHitsGlobal[nGlobalMuon],pixelLayersWithMeasurementGlobal[nGlobalMuon],trackerLayersWithMeasurementGlobal[nGlobalMuon],xGlobal[nGlobalMuon],yGlobal[nGlobalMuon],zGlobal[nGlobalMuon],pGlobal[nGlobalMuon],qoverpGlobal[nGlobalMuon],lambdaGlobal[nGlobalMuon],phiGlobal[nGlobalMuon],varQoverpGlobal[nGlobalMuon],varLambdaGlobal[nGlobalMuon],varPhiGlobal[nGlobalMuon],
                                           covarQoverpLambdaGlobal[nGlobalMuon],covarQoverpPhiGlobal[nGlobalMuon],covarLambdaPhiGlobal[nGlobalMuon]);
				nGlobalMuon++;
			}
			if(((*muons)[i]).isTrackerMuon()&&nTrackerMuon<100){
				isGoodMuonTracker[nTrackerMuon]=isGood;
				l1DeltaRTracker[nTrackerMuon]=deltaR;
				l1QualityTracker[nTrackerMuon]=quality;
		
				getTrack(((*muons)[i].track()).get(),ndofTracker[nTrackerMuon],chi2Tracker[nTrackerMuon],numberOfValidTrackerHitsTracker[nTrackerMuon],pixelLayersWithMeasurementTracker[nTrackerMuon],trackerLayersWithMeasurementTracker[nTrackerMuon],xTracker[nTrackerMuon],yTracker[nTrackerMuon],zTracker[nTrackerMuon],pTracker[nTrackerMuon],qoverpTracker[nTrackerMuon],lambdaTracker[nTrackerMuon],phiTracker[nTrackerMuon],varQoverpTracker[nTrackerMuon],varLambdaTracker[nTrackerMuon],varPhiTracker[nTrackerMuon],
                                           covarQoverpLambdaTracker[nTrackerMuon],covarQoverpPhiTracker[nTrackerMuon],covarLambdaPhiTracker[nTrackerMuon]);
				nTrackerMuon++;
			}
			if(((*muons)[i]).isStandAloneMuon()&&nStandAloneMuon<100){
				isGoodMuonStandAlone[nStandAloneMuon]=isGood;
				l1DeltaRStandAlone[nStandAloneMuon]=deltaR;
				l1QualityStandAlone[nStandAloneMuon]=quality;

				getTrack(((*muons)[i].standAloneMuon()).get(),ndofStandAlone[nStandAloneMuon],chi2StandAlone[nStandAloneMuon],numberOfValidTrackerHitsStandAlone[nStandAloneMuon],pixelLayersWithMeasurementStandAlone[nStandAloneMuon],trackerLayersWithMeasurementStandAlone[nStandAloneMuon],xStandAlone[nStandAloneMuon],yStandAlone[nStandAloneMuon],zStandAlone[nStandAloneMuon],pStandAlone[nStandAloneMuon],qoverpStandAlone[nStandAloneMuon],lambdaStandAlone[nStandAloneMuon],phiStandAlone[nStandAloneMuon],varQoverpStandAlone[nStandAloneMuon],varLambdaStandAlone[nStandAloneMuon],varPhiStandAlone[nStandAloneMuon],
                                           covarQoverpLambdaStandAlone[nStandAloneMuon],covarQoverpPhiStandAlone[nStandAloneMuon],covarLambdaPhiStandAlone[nStandAloneMuon]);
				nStandAloneMuon++;
			}
			if(((*muons)[i]).isCaloMuon()){
				nCaloMuon++;
				CaloMuonEnergy=((*muons)[i].calEnergy()).tower;
			}
		}
	}

	MuonTree->Fill();
}

void UPCPatMuonAnalyzer::getTrack(const Track *trax, unsigned int &ndof, float &chi2, unsigned int &vHits, unsigned int &pixHits, unsigned int &traxHits,
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
	vHits=trax->hitPattern().numberOfValidTrackerHits();
	pixHits=trax->hitPattern().pixelLayersWithMeasurement();
	traxHits=trax->hitPattern().trackerLayersWithMeasurement();
}
