#ifndef CALOENERGYANALYZER_H
#define CALOENERGYANALYZER_H

// system include files
#include <vector>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/Point3D.h"

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
	bool getVertexData(const VertexCollection*, float*);

	bool etaBinSize, HFCorrection, realHFbins,noiseCut;
	int RunData[4];
//	int CaloSize;
//	float *CaloEmEnergy;
//	float *CaloHadEnergy;
//	float *CaloEta;
//	float *CaloPhi;
	float CalodEtdEta[82];
	float VertData[6];
	float cent[2];
	int centi;	
	
	float etabin[83];
	float HFcor[11];
	
//	const CaloTowerCollection *CaloTowerz;
	CentralityProvider *CentProv;
	
	edm::Service<TFileService> mFileServer;
	
	TTree* RunTree;
	TTree* CenTree;
	TTree* CaloTree;
	TTree* VertTree;
};
DEFINE_FWK_MODULE(CaloEnergyAnalyzer);

#endif
