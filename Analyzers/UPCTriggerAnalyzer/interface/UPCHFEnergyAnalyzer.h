#ifndef CALOENERGYANALYZER_H
#define CALOENERGYANALYZER_H

// system include files
#include <vector>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "FWCore/Framework/interface/EventSetup.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

using namespace reco;
using namespace std;

class UPCHFEnergyAnalyzer : public edm::EDAnalyzer {
public:
	explicit UPCHFEnergyAnalyzer(const edm::ParameterSet&);
	~UPCHFEnergyAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	int CaloSize;
	float CaloEta[1000];
	float CaloPhi[1000];
	float CaloEnergy[1000];
	
	edm::Service<TFileService> mFileServer;
	TTree* CaloTree;
};
DEFINE_FWK_MODULE(UPCHFEnergyAnalyzer);

#endif
