#include "Analyzers/UPCTriggerAnalyzer/interface/UPCGenParticleAnalyzer.h"

using namespace edm;

UPCGenParticleAnalyzer::UPCGenParticleAnalyzer(const edm::ParameterSet& iConfig):genParticleCollection(iConfig.getParameter<string>("genParticleCollection")),treeName(iConfig.getParameter<string>("treeName")){}

UPCGenParticleAnalyzer::~UPCGenParticleAnalyzer(){}

void UPCGenParticleAnalyzer::beginJob(){
	mFileServer->file().cd();
	
	string tName(treeName);
	GenPartTree = new TTree(tName.c_str(),tName.c_str());

	GenPartTree->Branch("nParticles",&nParticles,"nParticles/I");
	GenPartTree->Branch("x",&x[0],"x[nParticles]/D");
	GenPartTree->Branch("y",&y[0],"y[nParticles]/D");
	GenPartTree->Branch("z",&z[0],"z[nParticles]/D");
	GenPartTree->Branch("px",&px[0],"px[nParticles]/D");
	GenPartTree->Branch("py",&py[0],"py[nParticles]/D");
	GenPartTree->Branch("pz",&pz[0],"pz[nParticles]/D");
	GenPartTree->Branch("charge",&charge[0],"charge[nParticles]/D");
	GenPartTree->Branch("mass",&mass[0],"mass[nParticles]/D");
}

void UPCGenParticleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<vector<reco::GenParticle> > genPartz;
	iEvent.getByLabel(genParticleCollection.c_str(),genPartz);
	
	x.clear(); y.clear(); z.clear(); 
	px.clear(); py.clear(); pz.clear(); 
	charge.clear(); mass.clear();

	if(!genPartz.failedToGet()){getParticles(genPartz,x,y,z,
						px,py,pz,charge,mass);}
	nParticles=x.size();

	GenPartTree->SetBranchAddress("nParticles",&nParticles);
	GenPartTree->SetBranchAddress("x",&x[0]);
	GenPartTree->SetBranchAddress("y",&y[0]);
	GenPartTree->SetBranchAddress("z",&z[0]);
	GenPartTree->SetBranchAddress("px",&px[0]);
	GenPartTree->SetBranchAddress("py",&py[0]);
	GenPartTree->SetBranchAddress("pz",&pz[0]);
	GenPartTree->SetBranchAddress("charge",&charge[0]);
	GenPartTree->SetBranchAddress("mass",&mass[0]);
	
	GenPartTree->Fill();
}

void UPCGenParticleAnalyzer::getParticles(Handle<vector<reco::GenParticle> > ParticleCol,
	vector<double> &x, vector<double> &y, vector<double> &z,
	vector<double> &px, vector<double> &py, vector<double> &pz,
	vector<double> &charge, vector<double> &mass)
{
	for(vector<reco::GenParticle>::const_iterator trax=(&*ParticleCol)->begin();
	    trax!=(&*ParticleCol)->end();trax++){
		x.push_back(trax->vx());
		y.push_back(trax->vy());
		z.push_back(trax->vz());
		px.push_back(trax->px());
		py.push_back(trax->py());
		pz.push_back(trax->pz());
		charge.push_back(trax->charge());
		mass.push_back(trax->mass());
	 }  
}
