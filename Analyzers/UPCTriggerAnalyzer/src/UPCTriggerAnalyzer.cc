#include "Analyzers/UPCTriggerAnalyzer/interface/UPCTriggerAnalyzer.h"

using namespace edm;

UPCTriggerAnalyzer::UPCTriggerAnalyzer(const edm::ParameterSet& iConfig){}

UPCTriggerAnalyzer::~UPCTriggerAnalyzer(){}

void UPCTriggerAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
	
	TrakTree = new TTree("TrackTree","TrackTree");
	CenTree = new TTree("CentralityTree","CentralityTree");
	RunTree = new TTree("RunTree","RunTree");

	TrakTree->Branch("nGloPrimTracks",&nGPTracks,"nGloPrimTracks/I");
	TrakTree->Branch("pxGloPrim",pxGP,"pxGloPrim[nGloPrimTracks]/F");
	TrakTree->Branch("pyGloPrim",pyGP,"pyGloPrim[nGloPrimTracks]/F");
	TrakTree->Branch("pzGloPrim",pzGP,"pzGloPrim[nGloPrimTracks]/F");
	TrakTree->Branch("xGloPrim",xGP,"xGloPrim[nGloPrimTracks]/F");
	TrakTree->Branch("yGloPrim",yGP,"yGloPrim[nGloPrimTracks]/F");
	TrakTree->Branch("zGloPrim",zGP,"zGloPrim[nGloPrimTracks]/F");
	
	TrakTree->Branch("nSelTracks",&nSelTracks,"nSelTracks/I");
	TrakTree->Branch("pxSel",pxSel,"pxSel[nSelTracks]/F");
	TrakTree->Branch("pySel",pySel,"pySel[nSelTracks]/F");
	TrakTree->Branch("pzSel",pzSel,"pzSel[nSelTracks]/F");
	TrakTree->Branch("xSel",xSel,"xSel[nSelTracks]/F");
	TrakTree->Branch("ySel",ySel,"ySel[nSelTracks]/F");
	TrakTree->Branch("zSel",zSel,"zSel[nSelTracks]/F");

	CenTree->Branch("CentralityNpart",&cent[0],"NpartMean/F");
	CenTree->Branch("CentralityValue",&cent[1],"centralityValue/F");
	CenTree->Branch("CentralityBin",&centi,"Bin/I");

	RunTree->Branch("BunchXing",&RunData[0],"BunchXing/I");
	RunTree->Branch("LumiBlock",&RunData[1],"LumiBlock/I");
	RunTree->Branch("Event",&RunData[2],"Event/I");
	RunTree->Branch("Run",&RunData[3],"Run/I");
}

void UPCTriggerAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	CentProv = new CentralityProvider(iSetup);
	CentProv->newEvent(iEvent,iSetup);
	
	RunData[0]=iEvent.bunchCrossing();
	RunData[1]=iEvent.id().luminosityBlock();
	RunData[2]=iEvent.id().event();
	RunData[3]=iEvent.id().run();

	cent[0]=CentProv->NpartMean();
	cent[1]=CentProv->centralityValue();
	centi=CentProv->getBin();

	Handle<TrackCollection> hiGloPrimTrax;
	Handle<TrackCollection> hiSelTrax;

	iEvent.getByLabel("hiGlobalPrimTracks",hiGloPrimTrax);
	iEvent.getByLabel("hiSelectedTracks",hiSelTrax);

	x1.clear(); y1.clear(); z1.clear(); 
	px1.clear(); py1.clear(); pz1.clear(); 
	x2.clear(); y2.clear(); z2.clear(); 
	px2.clear(); py2.clear(); pz2.clear();

	if(!hiGloPrimTrax.failedToGet()){
		GetTracks(hiGloPrimTrax);
		for(int i=0;i<int(x2.size());i++){
			x1.push_back(x2[i]);
			y1.push_back(y2[i]);
			z1.push_back(z2[i]);
			px1.push_back(px2[i]);
			py1.push_back(py2[i]);
			pz1.push_back(pz2[i]);
		}

	}
	if(!hiSelTrax.failedToGet()){GetTracks(hiSelTrax);}

	nGPTracks=x1.size(); nSelTracks=x2.size();
	xGP=&(x1[0]);yGP=&(y1[0]);zGP=&(z1[0]);
	xSel=&(x2[0]);ySel=&(y2[0]);zSel=&(z2[0]);
	pxGP=&(px1[0]);pyGP=&(py1[0]);pzGP=&(pz1[0]);
	pxSel=&(px2[0]);pySel=&(py2[0]);pzSel=&(pz2[0]);

	TrakTree->Fill();
	CenTree->Fill();
	RunTree->Fill();
}

void UPCTriggerAnalyzer::GetTracks(Handle<TrackCollection> TrackCol){
	for(TrackCollection::const_iterator trax=(&*TrackCol)->begin();
	    trax!=(&*TrackCol)->end();
	    trax++){
		x2.push_back(trax->vx());
		y2.push_back(trax->vy());
		z2.push_back(trax->vz());
		px2.push_back(trax->px());
		py2.push_back(trax->py());
		pz2.push_back(trax->pz());
	}
}

