#ifndef CALOENERGYANALYZER_H
#define CALOENERGYANALYZER_H

// system include files
#include <vector>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"
#include "TVectorT.h"

using namespace reco;
using namespace std;

class CaloEnergyAnalyzer : public edm::EDAnalyzer {
public:
	explicit CaloEnergyAnalyzer(const edm::ParameterSet&);
	~CaloEnergyAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	bool etaBinSize, HFCorrection, realHFbins,noiseCut;
	int RunData[4];
	int CaloSize;
	float *CaloEt;
	float *CaloEnergy;
	float *CaloEta;
	float *CaloPhi;
	float CalodEtdEta[82];
	float cent[2];
	int centi;	
	
	float etabin[83];
	float HFcor[11];
	
//	const ZDCDigiCollection *ZDCDigiz;
//	const ZDCRecHitCollection *ZDCRecHitz;
//	const HBHERecHitCollection *HBHERecHitz;
//	const HFRecHitCollection *HFRecHitz;
//	const CastorRecHitCollection *CastorRecHitz;
//	const CaloTowerCollection *CaloTowerz;
	CentralityProvider *CentProv;
	
	edm::Service<TFileService> mFileServer;
	
//	TTree* ZDCTree;
	TTree* RunTree;
	TTree* CenTree;
//	TTree* ForwardTree;
	TTree* CaloTree;
};
DEFINE_FWK_MODULE(CaloEnergyAnalyzer);

#endif
