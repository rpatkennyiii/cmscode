#include "Analyzers/UPCTriggerAnalyzer/interface/UPCTriggerAnalyzer.h"
#include <iostream>

using namespace edm;

UPCTriggerAnalyzer::UPCTriggerAnalyzer(const edm::ParameterSet& iConfig){}

UPCTriggerAnalyzer::~UPCTriggerAnalyzer(){}

void UPCTriggerAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
	
	TrakTree = new TTree("TrackTree","TrackTree");
	CenTree = new TTree("CentralityTree","CentralityTree");
	RunTree = new TTree("RunTree","RunTree");
        VertTree = new TTree("VertexTree","VertexTree"); 
//	MuonTree = new TTree("MuonTree","MuonTree");

	TrakTree->Branch("nGloPrimTracks",&nGPTracks,"nGloPrimTracks/I");
	TrakTree->Branch("ndofGloPrim",&ndofGP,"ndofGloPrim[nGloPrimTracks]/I");
	TrakTree->Branch("chi2GloPrim",&chi2GP,"chi2GloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("pGloPrim",&pGP[0],"pGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("qoverpGloPrim",&qoverpGP[0],"qoverpGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("lambdaGloPrim",&lambdaGP[0],"lambdaGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("phiGloPrim",&phiGP[0],"phiGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("varQoverpGloPrim",&varQoverpGP[0],"varQoverpGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("varLambdaGloPrim",&varLambdaGP[0],"varLambdaGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("varPhiGloPrim",&varPhiGP[0],"varPhiGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("covarQoverpLambdaGloPrim",&covarQoverpLambdaGP[0],"covarQoverpLambdaGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("covarQoverpPhiGloPrim",&covarQoverpPhiGP[0],"covarQoverpPhiGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("covarLambdaPhiGloPrim",&covarLambdaPhiGP[0],"covarLambdaPhiGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("xGloPrim",&xGP[0],"xGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("yGloPrim",&yGP[0],"yGloPrim[nGloPrimTracks]/D");
	TrakTree->Branch("zGloPrim",&zGP[0],"zGloPrim[nGloPrimTracks]/D");
	
	TrakTree->Branch("nSelTracks",&nSelTracks,"nSelTracks/I");
	TrakTree->Branch("ndofSel",&ndofSel,"ndofSel[nSelTracks]/I");
	TrakTree->Branch("chi2Sel",&chi2Sel,"nchi2Sel[nSelTracks]/D");
	TrakTree->Branch("pSel",&pSel[0],"pxSel[nSelTracks]/D");
	TrakTree->Branch("qoverpSel",&qoverpSel[0],"qoverpSel[nSelTracks]/D");
	TrakTree->Branch("lambdaSel",&lambdaSel[0],"lambdaSel[nSelTracks]/D");
	TrakTree->Branch("phiSel",&phiSel[0],"phiSel[nSelTracks]/D");
	TrakTree->Branch("varQoverpSel",&varQoverpSel[0],"varQoverpSel[nSelTracks]/D");
	TrakTree->Branch("varLambdaSel",&varLambdaSel[0],"varLambdaSel[nSelTracks]/D");
	TrakTree->Branch("varPhiSel",&varPhiSel[0],"varPhiSel[nSelTracks]/D");
	TrakTree->Branch("covarQoverpLambdaSel",&covarQoverpLambdaSel[0],"covarQoverpLambdaSel[nSelTracks]/D");
	TrakTree->Branch("covarQoverpPhiSel",&covarQoverpPhiSel[0],"covarQoverpPhiSel[nSelTracks]/D");
	TrakTree->Branch("covarLambdaPhiSel",&covarLambdaPhiSel[0],"covarLambdaPhiSel[nSelTracks]/D");
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
	
	VertTree->Branch("VertSel",VertData,"pVx:pVxError:pVy:pVyError:pVz:pVzError");
//	MuonTree->Branch("Muons",&Muonz);
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
	Handle<VertexCollection> vertexCollection;
//	Handle<vector<reco::Muon> > muons;

	iEvent.getByLabel("hiGlobalPrimTracks",hiGloPrimTrax);
	iEvent.getByLabel("hiSelectedTracks",hiSelTrax);
	iEvent.getByLabel("hiSelectedVertex",vertexCollection);
//	iEvent.getByLabel("muons",muons);
	
	chi2GP.clear(); ndofGP.clear();
	xGP.clear(); yGP.clear(); zGP.clear(); 
	pGP.clear(); qoverpGP.clear(); lambdaGP.clear(); phiGP.clear();
	varQoverpGP.clear(); varLambdaGP.clear(); varPhiGP.clear();
	covarQoverpLambdaGP.clear();covarQoverpPhiGP.clear();covarLambdaPhiGP.clear();

	chi2Sel.clear(); ndofSel.clear();
	xSel.clear(); ySel.clear(); zSel.clear(); 
	pSel.clear(); qoverpSel.clear(); lambdaSel.clear(); phiSel.clear();
	varQoverpSel.clear(); varLambdaSel.clear(); varPhiSel.clear();
	covarQoverpLambdaSel.clear();covarQoverpPhiSel.clear();covarLambdaPhiSel.clear();
	
	const VertexCollection * vertices = vertexCollection.product();

//	Muonz.clear();
	if(!hiGloPrimTrax.failedToGet()){getTracks(hiGloPrimTrax,ndofGP,chi2GP,xGP,yGP,zGP,pGP,qoverpGP,lambdaGP,phiGP,varQoverpGP,varLambdaGP,varPhiGP,
                                                   covarQoverpLambdaGP,covarQoverpPhiGP,covarLambdaPhiGP);}
	
	if(!hiSelTrax.failedToGet()){getTracks(hiSelTrax,ndofSel,chi2Sel,xSel,ySel,zSel,pSel,qoverpSel,lambdaSel,phiSel,varQoverpSel,varLambdaSel,varPhiSel,
                                               covarQoverpLambdaSel,covarQoverpPhiSel,covarLambdaPhiSel);}
//	if(!muons.failedToGet()){
//		for(unsigned int i=0; i<(*muons).size();i++){Muonz.push_back((*muons)[i]);}
//	}

	if(getVertexData(vertices,VertData)){VertTree->Fill();}

	nGPTracks=xGP.size(); nSelTracks=xSel.size();

	TrakTree->SetBranchAddress("nGloPrimTracks",&nGPTracks);
	TrakTree->SetBranchAddress("ndofGloPrim",&ndofGP[0]);
	TrakTree->SetBranchAddress("chi2GloPrim",&chi2GP[0]);
	TrakTree->SetBranchAddress("pGloPrim",&pGP[0]);
	TrakTree->SetBranchAddress("qoverpGloPrim",&qoverpGP[0]);
	TrakTree->SetBranchAddress("lambdaGloPrim",&lambdaGP[0]);
	TrakTree->SetBranchAddress("phiGloPrim",&phiGP[0]);
	TrakTree->SetBranchAddress("varQoverpGloPrim",&varQoverpGP[0]);
	TrakTree->SetBranchAddress("varLambdaGloPrim",&varLambdaGP[0]);
	TrakTree->SetBranchAddress("varPhiGloPrim",&varPhiGP[0]);
	TrakTree->SetBranchAddress("covarQoverpLambdaGloPrim",&covarQoverpLambdaGP[0]);
	TrakTree->SetBranchAddress("covarQoverpPhiGloPrim",&covarQoverpPhiGP[0]);
	TrakTree->SetBranchAddress("covarLambdaPhiGloPrim",&covarLambdaPhiGP[0]);
	TrakTree->SetBranchAddress("xGloPrim",&xGP[0]);
	TrakTree->SetBranchAddress("yGloPrim",&yGP[0]);
	TrakTree->SetBranchAddress("zGloPrim",&zGP[0]);
	
	TrakTree->SetBranchAddress("nSelTracks",&nSelTracks);
	TrakTree->SetBranchAddress("ndofSel",&ndofSel[0]);
	TrakTree->SetBranchAddress("chi2Sel",&chi2Sel[0]);
	TrakTree->SetBranchAddress("pSel",&pSel[0]);
	TrakTree->SetBranchAddress("qoverpSel",&qoverpSel[0]);
	TrakTree->SetBranchAddress("lambdaSel",&lambdaSel[0]);
	TrakTree->SetBranchAddress("phiSel",&phiSel[0]);
	TrakTree->SetBranchAddress("varQoverpSel",&varQoverpSel[0]);
	TrakTree->SetBranchAddress("varLambdaSel",&varLambdaSel[0]);
	TrakTree->SetBranchAddress("varPhiSel",&varPhiSel[0]);
	TrakTree->SetBranchAddress("covarQoverpLambdaSel",&covarQoverpLambdaSel[0]);
	TrakTree->SetBranchAddress("covarQoverpPhiSel",&covarQoverpPhiSel[0]);
	TrakTree->SetBranchAddress("covarLambdaPhiSel",&covarLambdaPhiSel[0]);
	TrakTree->SetBranchAddress("xSel",&xSel[0]);
	TrakTree->SetBranchAddress("ySel",&ySel[0]);
	TrakTree->SetBranchAddress("zSel",&zSel[0]);

	TrakTree->Fill();
	CenTree->Fill();
	RunTree->Fill();
	VertTree->Fill();
//	MuonTree->Fill();
}

void UPCTriggerAnalyzer::getTracks(Handle<TrackCollection> TrackCol, vector<int> &ndof, vector<double> &chi2,
	vector<double> &x, vector<double> &y, vector<double> &z,
	vector<double> &p, vector<double> &qoverp, vector<double> &lambda, vector<double> &phi,
	vector<double> &varqoverp, vector<double> &varlambda, vector<double> &varphi,
	vector<double> &covarqoverplambda, vector<double> &covarqoverpphi, vector<double> &covarlambdaphi){
	for(TrackCollection::const_iterator trax=(&*TrackCol)->begin();
	    trax!=(&*TrackCol)->end();trax++){
		x.push_back(trax->vx());
		y.push_back(trax->vy());
		z.push_back(trax->vz());
		p.push_back(trax->p());
		qoverp.push_back(trax->qoverp());
		lambda.push_back(trax->lambda());
		phi.push_back(trax->phi());
		varqoverp.push_back(trax->covariance(0,0));
		varlambda.push_back(trax->covariance(1,1));
		varphi.push_back(trax->covariance(2,2));
		covarqoverplambda.push_back(trax->covariance(0,1));
		covarqoverpphi.push_back(trax->covariance(0,2));
		covarlambdaphi.push_back(trax->covariance(1,2));
		chi2.push_back(trax->chi2());
		ndof.push_back(trax->ndof());
	}
}

bool UPCTriggerAnalyzer::getVertexData(const VertexCollection *vert, float* vdata){
	if(vert->size()>0) {
		vdata[0]=vert->begin()->x();
		vdata[1]=vert->begin()->xError();
		vdata[2]=vert->begin()->y();
		vdata[3]=vert->begin()->yError();
		vdata[4]=vert->begin()->z();
		vdata[5]=vert->begin()->zError();
		return(vert->size());
	}else{return(-999.9);}
}
