#ifndef UPCTRIGGERANALYZER_H
#define UPCTRIGGERANALYZER_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"
#include "TVectorT.h"

using namespace reco;
using namespace std;

class UPCTriggerAnalyzer : public edm::EDAnalyzer{
public:
	explicit UPCTriggerAnalyzer(const edm::ParameterSet&);
	~UPCTriggerAnalyzer();
private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	void GetTracks(edm::Handle<TrackCollection>);

	edm::Service<TFileService> mFileServer;
	CentralityProvider *CentProv;

	int nGPTracks, nSelTracks;
	int RunData[4];
	int centi;	
	double cent[2];
	vector<double> x1,y1,z1,px1,py1,pz1;
	vector<double> x2,y2,z2,px2,py2,pz2;
	double *pxGP, *pyGP, *pzGP, *xGP, *yGP, *zGP,
	      *pxSel, *pySel, *pzSel, *xSel, *ySel, *zSel;

	TTree* TrakTree;
	TTree* CenTree;
	TTree* RunTree;
};
DEFINE_FWK_MODULE(UPCTriggerAnalyzer);

#endif
