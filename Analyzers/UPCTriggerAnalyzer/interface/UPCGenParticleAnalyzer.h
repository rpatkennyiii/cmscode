#ifndef UPCGENPARTICLEANALYZER_H
#define UPCGENPARTICLEANALYZER_H

#include <string>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

using namespace reco;
using namespace std;

class UPCGenParticleAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCGenParticleAnalyzer(const edm::ParameterSet&);
	~UPCGenParticleAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void getParticles(edm::Handle<vector<reco::GenParticle> >, 
             vector<double>&, vector<double>&, vector<double>&, 
             vector<double>&, vector<double>&, vector<double>&,
             vector<double>&, vector<double>&);

	edm::Service<TFileService> mFileServer;

	string genParticleCollection; 

	int nParticles;
	vector<double>  x, y, z, 
			px, py, pz,
			mass, charge;
 
	TTree* GenPartTree;
};
DEFINE_FWK_MODULE(UPCGenParticleAnalyzer);
#endif
