#include "Analyzers/CaloEnergyAnalyzer/interface/CaloEnergyAnalyzer.h"

using namespace edm;

CaloEnergyAnalyzer::CaloEnergyAnalyzer(const ParameterSet& iConfig){}

CaloEnergyAnalyzer::~CaloEnergyAnalyzer(){}

void CaloEnergyAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	CentProv = new CentralityProvider(iSetup);
	CentProv->newEvent(iEvent,iSetup);

	RunData[0]=iEvent.bunchCrossing();
	RunData[1]=iEvent.id().luminosityBlock();
	RunData[2]=iEvent.id().event();
	RunData[3]=iEvent.id().run();

	cent[0]=CentProv->NpartMean();
	cent[1]=CentProv->centralityValue();
	centi=CentProv->getBin();
	
	RunTree->Fill();
	CenTree->Fill();

//	Handle<ZDCDigiCollection> zdc_digi;
//	Handle<ZDCRecHitCollection> zdc_recHits;
//	Handle<HBHERecHitCollection> hbhe_recHits;
//	Handle<HFRecHitCollection> hf_recHits;
//	Handle<CastorRecHitCollection> castor_recHits;
//	Handle<CaloTowerCollection> calotower_h;
	Handle<CaloTowerCollection> calotower;

//	iEvent.getByType(zdc_digi);
//	iEvent.getByType(zdc_recHits);
//	iEvent.getByType(hbhe_recHits);
//	iEvent.getByType(hf_recHits);
//	iEvent.getByType(castor_recHits);
	iEvent.getByType(calotower);
	
	float etabin[83]={-5.19100,-4.88900,-4.71600,-4.53800,-4.36300,-4.19100,-4.01300
			,-3.83900,-3.66400,-3.48900,-3.31400,-3.13900,-3.00000,-2.96400
			,-2.65000,-2.50000,-2.32200,-2.17200,-2.04300,-1.93000,-1.83000
			,-1.74000,-1.65300,-1.56600,-1.47900,-1.39200,-1.30500,-1.21800
			,-1.13100,-1.04400,-0.95700,-0.87000,-0.78300,-0.69600,-0.60900
			,-0.52200,-0.43500,-0.34800,-0.26100,-0.17400,-0.08700,0.00000
			,0.08700,0.17400,0.26100,0.34800,0.43500,0.52200,0.60900,0.69600
			,0.78300,0.87000,0.95700,1.04400,1.13100,1.21800,1.30500,1.39200
			,1.47900,1.56600,1.65300,1.74000,1.83000,1.93000,2.04300,2.17200
			,2.32200,2.50000,2.65000,2.96400,3.00000,3.13900,3.31400,3.48900
			,3.66400,3.83900,4.01300,4.19100,4.36300,4.53800,4.71600,4.88900
			,5.19100};

	vector<float> cet;
	vector<float> ceta;
	vector<float> cphi;

	if(!calotower.failedToGet()){
		for(int j=0;j<82;j++){CalodEtdEta[j]=0;}
		int i=0;

		for(CaloTowerCollection::const_iterator calt=(&*calotower)->begin();calt!=(&*calotower)->end();calt++){
			cet.push_back(calt->et());
			ceta.push_back(calt->eta());
			cphi.push_back(calt->phi());
			
			for(int k=0;k<82;k++){
				if(calt->eta()>etabin[k]&&calt->eta()<=etabin[k+1]){
					CalodEtdEta[k]+=calt->et()/(etabin[k+1]-etabin[k]);
					k=82;
				}
			}

			i++;
		}
		CaloSize=cet.size();

		CaloTree->SetBranchAddress("CaloEt",&cet[0]);
		CaloTree->SetBranchAddress("CaloEta",&ceta[0]);
		CaloTree->SetBranchAddress("CaloPhi",&cphi[0]);

		CaloTree->Fill();
	}

//	ZDCDigiz=&*zdc_digi;
//	ZDCRecHitz=&*zdc_recHits;
//	HBHERecHitz=&*hbhe_recHits;
//	HFRecHitz=&*hf_recHits;
//	CastorRecHitz=&*castor_recHits;
//	CaloTowerz=&*calotower;

//	ZDCTree->Fill();
//	ForwardTree->Fill();
}


void CaloEnergyAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();

//   	ZDCTree = new TTree("ZDCTree","ZDC Tree");
	RunTree = new TTree("RunTree","Run Tree");
	CenTree = new TTree("CentTree","Centrality Tree");
//	ForwardTree = new TTree("ForwardTree","Forward Tree");
	CaloTree = new TTree("CaloTree","Calo Tree");
		
	RunTree->Branch("BunchXing",&RunData[0],"BunchXing/I");
	RunTree->Branch("LumiBlock",&RunData[1],"LumiBlock/I");
	RunTree->Branch("Event",&RunData[2],"Event/I");
	RunTree->Branch("Run",&RunData[3],"Run/I");

//	ZDCTree->Branch("ZDCDigi",&ZDCDigiz);
//	ZDCTree->Branch("ZDCRecHit",&ZDCRecHitz);
	
	CenTree->Branch("CentralityNpart",&cent[0],"NpartMean/F");
	CenTree->Branch("CentralityValue",&cent[1],"centralityValue/F");
	CenTree->Branch("CentralityBin",&centi,"Bin/I");

//	ForwardTree->Branch("HBHERecHit",&HBHERecHitz);
//	ForwardTree->Branch("HFRecHit",&HFRecHitz);
//	ForwardTree->Branch("CastorRecHit",&CastorRecHitz);

	CaloTree->Branch("CaloSize",&CaloSize);
	CaloTree->Branch("CaloEt",&CaloEt,"CaloEt[CaloSize]/F");
	CaloTree->Branch("CaloEta",&CaloEta,"CaloEta[CaloSize]/F");
	CaloTree->Branch("CaloPhi",&CaloPhi,"CaloPhi[CaloSize]/F");
	CaloTree->Branch("CalodEtdEta",&CalodEtdEta,"CalodEtdEta[82]/F");
}	
