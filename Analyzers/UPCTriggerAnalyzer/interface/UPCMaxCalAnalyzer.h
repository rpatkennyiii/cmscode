#ifndef UPCMAXCALANALYZER_H
#define UPCMAXCALANALYZER_H

// system include files
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

class UPCMaxCalAnalyzer : public edm::EDAnalyzer {
public:
	explicit UPCMaxCalAnalyzer(const edm::ParameterSet&);
	~UPCMaxCalAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	float MaxEBEnergy;
	float MaxEEEnergy;
	float MaxHBEnergy;
	float MaxHEEnergy;
	float MaxHFEnergy;
	float MaxEBEMEnergy;
	float MaxEEEMEnergy;
	float MaxHBEMEnergy;
	float MaxHEEMEnergy;
	float MaxHFEMEnergy;
	float MaxEBHadEnergy;
	float MaxEEHadEnergy;
	float MaxHBHadEnergy;
	float MaxHEHadEnergy;
	float MaxHFHadEnergy;
	float MaxEBEta;
	float MaxEEEta;
	float MaxHBEta;
	float MaxHEEta;
	float MaxHFEta;
	float MaxEBPhi;
	float MaxEEPhi;
	float MaxHBPhi;
	float MaxHEPhi;
	float MaxHFPhi;

	float SumEBEnergy;
	float SumEEEnergy;
	float SumHBEnergy;
	float SumHEEnergy;
	float SumHFEnergy;
	float AverageEBEta;
	float AverageEEEta;
	float AverageHBEta;
	float AverageHEEta;
	float AverageHFEta;
	float AverageEBPhi;
	float AverageEEPhi;
	float AverageHBPhi;
	float AverageHEPhi;
	float AverageHFPhi;
	
	edm::Service<TFileService> mFileServer;
	TTree* CaloTree;
};
DEFINE_FWK_MODULE(UPCMaxCalAnalyzer);

#endif
