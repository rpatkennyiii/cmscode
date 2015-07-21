#include "Analyzers/UPCTriggerAnalyzer/interface/UPCGenParticleAnalyzer.h"

using namespace edm;

UPCGenParticleAnalyzer::UPCGenParticleAnalyzer(const edm::ParameterSet& iConfig):genParticleCollection(iConfig.getParameter<string>("genParticleCollection")){}

UPCGenParticleAnalyzer::~UPCGenParticleAnalyzer(){}

void UPCGenParticleAnalyzer::beginJob(){
	mFileServer->file().cd();
	
	string tName(genParticleCollection+"Tree");
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
        GenPartTree->Branch("polHXCosTheta",&polCosTheta,"polHXCosTheta");
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
	
	double e1=sqrt(px[4]*px[4]+py[4]*py[4]+pz[4]*pz[4]+mass[4]*mass[4]);
	double e2=sqrt(px[5]*px[5]+py[5]*py[5]+pz[5]*pz[5]+mass[5]*mass[5]);

	TLorentzVector muPlus,muMinus; 
	if(charge[4] >= charge[5]){ 
	 muPlus=TLorentzVector(px[4],py[4],pz[4],e1);
	 muMinus=TLorentzVector(px[5],py[5],pz[5],e2);
	}else{
	 muPlus=TLorentzVector(px[5],py[5],pz[5],e2);
	 muMinus=TLorentzVector(px[4],py[4],pz[4],e1);
	}
	 
	polCosTheta=calcPolHXCosTheta(muPlus,muMinus);

	GenPartTree->SetBranchAddress("nParticles",&nParticles);
	GenPartTree->SetBranchAddress("x",&x[0]);
	GenPartTree->SetBranchAddress("y",&y[0]);
	GenPartTree->SetBranchAddress("z",&z[0]);
	GenPartTree->SetBranchAddress("px",&px[0]);
	GenPartTree->SetBranchAddress("py",&py[0]);
	GenPartTree->SetBranchAddress("pz",&pz[0]);
	GenPartTree->SetBranchAddress("charge",&charge[0]);
	GenPartTree->SetBranchAddress("mass",&mass[0]);
	GenPartTree->SetBranchAddress("polHXCosTheta",&polCosTheta);
	
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

float UPCGenParticleAnalyzer::calcPolHXCosTheta(TLorentzVector muPlus_LAB,
	     TLorentzVector muMinus_LAB){
	TLorentzVector jPsi=muPlus_LAB+muMinus_LAB; 

	TVector3 jPsiHat=jPsi.Vect().Unit(); 
	TVector3 boost=-jPsi.BoostVector(); 

	muPlus_LAB.Boost(boost); 
	TVector3 muPlusHat=muPlus_LAB.Vect().Unit(); 
	 
	return(jPsiHat.Dot(muPlusHat));
}
