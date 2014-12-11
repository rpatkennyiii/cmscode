#include "Analyzers/UPCTriggerAnalyzer/interface/UPCTrackAnalyzer.h"

using namespace edm;

UPCTrackAnalyzer::UPCTrackAnalyzer(const edm::ParameterSet& iConfig):trackCollection(iConfig.getParameter<string>("trackCollection")){}

UPCTrackAnalyzer::~UPCTrackAnalyzer(){}

void UPCTrackAnalyzer::beginJob(){
	mFileServer->file().cd();
	
	string tName(trackCollection+"Tree");
	TrakTree = new TTree(tName.c_str(),tName.c_str());

	TrakTree->Branch("nTracks",&nTracks,"nTracks/I");
	TrakTree->Branch("ndof",&ndof,"ndof[nTracks]/I");
	TrakTree->Branch("isHighPurity",&isHighPurity,"isHighPurity[nTracks]/I");
	TrakTree->Branch("chi2",&chi2,"chi2[nTracks]/D");
	TrakTree->Branch("p",&p[0],"p[nTracks]/D");
	TrakTree->Branch("qoverp",&qoverp[0],"qoverp[nTracks]/D");
	TrakTree->Branch("lambda",&lambda[0],"lambda[nTracks]/D");
	TrakTree->Branch("phi",&phi[0],"phi[nTracks]/D");
	TrakTree->Branch("varQoverp",&varQoverp[0],"varQoverp[nTracks]/D");
	TrakTree->Branch("varLambda",&varLambda[0],"varLambda[nTracks]/D");
	TrakTree->Branch("varPhi",&varPhi[0],"varPhi[nTracks]/D");
	TrakTree->Branch("covarQoverpLambda",&covarQoverpLambda[0],"covarQoverpLambda[nTracks]/D");
	TrakTree->Branch("covarQoverpPhi",&covarQoverpPhi[0],"covarQoverpPhi[nTracks]/D");
	TrakTree->Branch("covarLambdaPhi",&covarLambdaPhi[0],"covarLambdaPhi[nTracks]/D");
	TrakTree->Branch("x",&x[0],"x[nTracks]/D");
	TrakTree->Branch("y",&y[0],"y[nTracks]/D");
	TrakTree->Branch("z",&z[0],"z[nTracks]/D");
}

void UPCTrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<TrackCollection> hiTrax;
	iEvent.getByLabel(trackCollection.c_str(),hiTrax);
	
	chi2.clear(); ndof.clear(); isHighPurity.clear();
	x.clear(); y.clear(); z.clear(); 
	p.clear(); qoverp.clear(); lambda.clear(); phi.clear();
	varQoverp.clear(); varLambda.clear(); varPhi.clear();
	covarQoverpLambda.clear();covarQoverpPhi.clear();covarLambdaPhi.clear();

	if(!hiTrax.failedToGet()){getTracks(hiTrax,ndof,isHighPurity,chi2,x,y,z,p,qoverp,lambda,phi,varQoverp,varLambda,varPhi,
                                           covarQoverpLambda,covarQoverpPhi,covarLambdaPhi);}

	nTracks=x.size();

	TrakTree->SetBranchAddress("nTracks",&nTracks);
	TrakTree->SetBranchAddress("ndof",&ndof[0]);
	TrakTree->SetBranchAddress("isHighPurity",&isHighPurity[0]);
	TrakTree->SetBranchAddress("chi2",&chi2[0]);
	TrakTree->SetBranchAddress("p",&p[0]);
	TrakTree->SetBranchAddress("qoverp",&qoverp[0]);
	TrakTree->SetBranchAddress("lambda",&lambda[0]);
	TrakTree->SetBranchAddress("phi",&phi[0]);
	TrakTree->SetBranchAddress("varQoverp",&varQoverp[0]);
	TrakTree->SetBranchAddress("varLambda",&varLambda[0]);
	TrakTree->SetBranchAddress("varPhi",&varPhi[0]);
	TrakTree->SetBranchAddress("covarQoverpLambda",&covarQoverpLambda[0]);
	TrakTree->SetBranchAddress("covarQoverpPhi",&covarQoverpPhi[0]);
	TrakTree->SetBranchAddress("covarLambdaPhi",&covarLambdaPhi[0]);
	TrakTree->SetBranchAddress("x",&x[0]);
	TrakTree->SetBranchAddress("y",&y[0]);
	TrakTree->SetBranchAddress("z",&z[0]);
	
	TrakTree->Fill();
}

void UPCTrackAnalyzer::getTracks(Handle<TrackCollection> TrackCol, vector<int> &ndof,vector<int> &isHighPurity, vector<double> &chi2,
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
		isHighPurity.push_back(trax->quality(reco::TrackBase::highPurity));
	}
}
