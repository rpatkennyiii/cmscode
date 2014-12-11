#ifndef UPCTRACKANALYZER_H
#define UPCTRACKANALYZER_H

#include <string>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

using namespace reco;
using namespace std;

class UPCTrackAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCTrackAnalyzer(const edm::ParameterSet&);
	~UPCTrackAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void getTracks(edm::Handle<TrackCollection>, vector<int>&, vector<int>&, vector<double>&, vector<double>&, 
             vector<double>&, vector<double>&, vector<double>&, vector<double>&,
             vector<double>&, vector<double>&, vector<double>&, vector<double>&,
             vector<double>&, vector<double>&, vector<double>&, vector<double>&);

	edm::Service<TFileService> mFileServer;

	string trackCollection; 

	int nTracks;
	vector<int> ndof, isHighPurity;
	vector<double> chi2,x, y, z, 
	   p, qoverp, lambda, phi,
	   varQoverp, varLambda, varPhi,
	   covarQoverpLambda,covarQoverpPhi,covarLambdaPhi;

	TTree* TrakTree;
};
DEFINE_FWK_MODULE(UPCTrackAnalyzer);
#endif
