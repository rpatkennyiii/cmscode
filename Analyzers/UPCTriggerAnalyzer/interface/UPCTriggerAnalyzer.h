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

	edm::Service<TFileService> mFileServer;
	CentralityProvider *CentProv;

	int nGPTracks, nSelTracks;
	int RunData[4];
	int centi;	
	double cent[2];
	vector<double> xGP,yGP,zGP,pxGP,pyGP,pzGP,
		       xSel,ySel,zSel,pxSel,pySel,pzSel;

	TTree* TrakTree;
	TTree* CenTree;
	TTree* RunTree;
};
DEFINE_FWK_MODULE(UPCTriggerAnalyzer);

#endif
