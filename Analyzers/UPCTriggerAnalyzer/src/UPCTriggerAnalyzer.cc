#include "Analyzers/UPCTriggerAnalyzer/interface/UPCTriggerAnalyzer.h"
#include <iostream>

using namespace edm;

void GetTracks(Handle<TrackCollection>, vector<double>&, vector<double>&, 
               vector<double>&, vector<double>&, vector<double>&, vector<double>&);

UPCTriggerAnalyzer::UPCTriggerAnalyzer(const edm::ParameterSet& iConfig){}

UPCTriggerAnalyzer::~UPCTriggerAnalyzer(){}

void UPCTriggerAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
	
	TrakTree = new TTree("TrackTree","TrackTree");
	CenTree = new TTree("CentralityTree","CentralityTree");
	RunTree = new TTree("RunTree","RunTree");

	TrakTree->Branch("nGloPrimTracks",&nGPTracks,"nGloPrimTracks/I");
	TrakTree->Branch("pxGloPrim",&pxGP[0],"pxGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("pyGloPrim",&pyGP[0],"pyGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("pzGloPrim",&pzGP[0],"pzGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("xGloPrim",&xGP[0],"xGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("yGloPrim",&yGP[0],"yGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("zGloPrim",&zGP[0],"zGloPrim[nGloPrimTracks]/D");
	
	TrakTree->Branch("nSelTracks",&nSelTracks,"nSelTracks/I");
	TrakTree->Branch("pxSel",&pxSel[0],"pxSel[nSelTracks]/D");
	TrakTree->Branch("pySel",&pySel[0],"pySel[nSelTracks]/D");
	TrakTree->Branch("pzSel",&pzSel[0],"pzSel[nSelTracks]/D");
	TrakTree->Branch("xSel",&xSel[0],"xSel[nSelTracks]/D");
	TrakTree->Branch("ySel",&ySel[0],"ySel[nSelTracks]/D");
	TrakTree->Branch("zSel",&zSel[0],"zSel[nSelTracks]/D");

	CenTree->Branch("CentralityNpart",&cent[0],"NpartMean/D");
	CenTree->Branch("CentralityValue",&cent[1],"centralityValue/D");
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

	xGP.clear(); yGP.clear(); zGP.clear(); 
	pxGP.clear(); pyGP.clear(); pzGP.clear(); 
	xSel.clear(); ySel.clear(); zSel.clear(); 
	pxSel.clear(); pySel.clear(); pzSel.clear();

	if(!hiGloPrimTrax.failedToGet()){GetTracks(hiGloPrimTrax,xGP,yGP,zGP,pxGP,pyGP,pzGP);}
	if(!hiSelTrax.failedToGet()){GetTracks(hiSelTrax,xSel,ySel,zSel,pxSel,pySel,pzSel);}

	nGPTracks=xGP.size(); nSelTracks=xSel.size();

	TrakTree->SetBranchAddress("pxGloPrim",&pxGP[0]);
	TrakTree->SetBranchAddress("pyGloPrim",&pyGP[0]);
	TrakTree->SetBranchAddress("pzGloPrim",&pzGP[0]);
	TrakTree->SetBranchAddress("xGloPrim",&xGP[0]);
	TrakTree->SetBranchAddress("yGloPrim",&yGP[0]);
	TrakTree->SetBranchAddress("zGloPrim",&zGP[0]);
	
	TrakTree->SetBranchAddress("pxSel",&pxSel[0]);
	TrakTree->SetBranchAddress("pySel",&pySel[0]);
	TrakTree->SetBranchAddress("pzSel",&pzSel[0]);
	TrakTree->SetBranchAddress("xSel",&xSel[0]);
	TrakTree->SetBranchAddress("ySel",&ySel[0]);
	TrakTree->SetBranchAddress("zSel",&zSel[0]);

	TrakTree->Fill();
	CenTree->Fill();
	RunTree->Fill();
}

void GetTracks(Handle<TrackCollection> TrackCol, 
	vector<double> &x, vector<double> &y, vector<double> &z,
	vector<double> &px, vector<double> &py, vector<double> &pz){
	for(TrackCollection::const_iterator trax=(&*TrackCol)->begin();
	    trax!=(&*TrackCol)->end();
	    trax++){
		x.push_back(trax->vx());
		y.push_back(trax->vy());
		z.push_back(trax->vz());
		px.push_back(trax->px());
		py.push_back(trax->py());
		pz.push_back(trax->pz());
	}
}

