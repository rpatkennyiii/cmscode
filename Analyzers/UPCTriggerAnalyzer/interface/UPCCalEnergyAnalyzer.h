#ifndef UPCCALENERGYANALYZER_H
#define UPCCALENERGYANALYZER_H

// system include files
#include <vector>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
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

class UPCCalEnergyAnalyzer : public edm::EDAnalyzer {
public:
	explicit UPCCalEnergyAnalyzer(const edm::ParameterSet&);
	~UPCCalEnergyAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	int CaloSize;
	float CaloEta[10000];
	float CaloPhi[10000];
	float CaloHadEnergy[10000];
	float CaloEmEnergy[10000];
	float CaloEnergy[10000];
	bool CaloIsHF[10000];
	bool CaloIsHB[10000];
	bool CaloIsHE[10000];
	bool CaloIsEB[10000];
	bool CaloIsEE[10000];
	
	edm::Service<TFileService> mFileServer;
	TTree* CaloTree;
};
DEFINE_FWK_MODULE(UPCCalEnergyAnalyzer);

#endif
