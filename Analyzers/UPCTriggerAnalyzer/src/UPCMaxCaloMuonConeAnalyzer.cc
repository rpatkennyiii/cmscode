#ifndef UPCMAXCALCONEANALYZER_H
#define UPCMAXCALCONEANALYZER_H

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

class UPCMaxCalConeAnalyzer : public edm::EDAnalyzer {
public:
	explicit UPCMaxCalConeAnalyzer(const edm::ParameterSet&);
	~UPCMaxCalConeAnalyzer();

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
DEFINE_FWK_MODULE(UPCMaxCalConeAnalyzer);


using namespace edm;
using namespace std;

UPCMaxCalConeAnalyzer::UPCMaxCalConeAnalyzer(const ParameterSet& iConfig){}

UPCMaxCalConeAnalyzer::~UPCMaxCalConeAnalyzer(){}

void UPCMaxCalConeAnalyzer::beginJob(){
	mFileServer->file().cd();

	CaloTree = new TTree("MaxCaloTree","MaxCaloTree");
		
	CaloTree->Branch("MaxEBEnergy",&MaxEBEnergy,"MaxEBEnergy/F");
	CaloTree->Branch("MaxEEEnergy",&MaxEEEnergy,"MaxEEEnergy/F");
	CaloTree->Branch("MaxHBEnergy",&MaxHBEnergy,"MaxHBEnergy/F");
	CaloTree->Branch("MaxHEEnergy",&MaxHEEnergy,"MaxHEEnergy/F");
	CaloTree->Branch("MaxHFEnergy",&MaxHFEnergy,"MaxHFEnergy/F");
	CaloTree->Branch("MaxEBEta",&MaxEBEta,"MaxEBEta/F");
	CaloTree->Branch("MaxEEEta",&MaxEEEta,"MaxEEEta/F");
	CaloTree->Branch("MaxHBEta",&MaxHBEta,"MaxHBEta/F");
	CaloTree->Branch("MaxHEEta",&MaxHEEta,"MaxHEEta/F");
	CaloTree->Branch("MaxHFEta",&MaxHFEta,"MaxHFEta/F");
	CaloTree->Branch("MaxEBPhi",&MaxEBPhi,"MaxEBPhi/F");
	CaloTree->Branch("MaxEEPhi",&MaxEEPhi,"MaxEEPhi/F");
	CaloTree->Branch("MaxHBPhi",&MaxHBPhi,"MaxHBPhi/F");
	CaloTree->Branch("MaxHEPhi",&MaxHEPhi,"MaxHEPhi/F");
	CaloTree->Branch("MaxHFPhi",&MaxHFPhi,"MaxHFPhi/F");

	CaloTree->Branch("SumEBEnergy",&SumEBEnergy,"SumEBEnergy/F");
	CaloTree->Branch("SumEEEnergy",&SumEEEnergy,"SumEEEnergy/F");
	CaloTree->Branch("SumHBEnergy",&SumHBEnergy,"SumHBEnergy/F");
	CaloTree->Branch("SumHEEnergy",&SumHEEnergy,"SumHEEnergy/F");
	CaloTree->Branch("SumHFEnergy",&SumHFEnergy,"SumHFEnergy/F");
	CaloTree->Branch("AverageEBEta",&AverageEBEta,"AverageEBEta/F");
	CaloTree->Branch("AverageEEEta",&AverageEEEta,"AverageEEEta/F");
	CaloTree->Branch("AverageHBEta",&AverageHBEta,"AverageHBEta/F");
	CaloTree->Branch("AverageHEEta",&AverageHEEta,"AverageHEEta/F");
	CaloTree->Branch("AverageHFEta",&AverageHFEta,"AverageHFEta/F");
	CaloTree->Branch("AverageEBPhi",&AverageEBPhi,"AverageEBPhi/F");
	CaloTree->Branch("AverageEEPhi",&AverageEEPhi,"AverageEEPhi/F");
	CaloTree->Branch("AverageHBPhi",&AverageHBPhi,"AverageHBPhi/F");
	CaloTree->Branch("AverageHEPhi",&AverageHEPhi,"AverageHEPhi/F");
	CaloTree->Branch("AverageHFPhi",&AverageHFPhi,"AverageHFPhi/F");
}


void UPCMaxCalConeAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	Handle<CaloTowerCollection> calotower;
	iEvent.getByType(calotower);

	MaxEBEnergy=0;
	MaxEEEnergy=0;
	MaxHBEnergy=0;
	MaxHEEnergy=0;
	MaxHFEnergy=0;
	MaxEBEta=0;
	MaxEEEta=0;
	MaxHBEta=0;
	MaxHEEta=0;
	MaxHFEta=0;
	MaxEBPhi=0;
	MaxEEPhi=0;
	MaxHBPhi=0;
	MaxHEPhi=0;
	MaxHFPhi=0;

	SumEBEnergy=0;
	SumEEEnergy=0;
	SumHBEnergy=0;
	SumHEEnergy=0;
	SumHFEnergy=0;
	float SumEBEta=0;
	float SumEEEta=0;
	float SumHBEta=0;
	float SumHEEta=0;
	float SumHFEta=0;
	float SumEBPhi=0;
	float SumEEPhi=0;
	float SumHBPhi=0;
	float SumHEPhi=0;
	float SumHFPhi=0;

	if(!calotower.failedToGet()){
		for(CaloTowerCollection::const_iterator calt=(&*calotower)->begin();calt!=(&*calotower)->end();calt++){
			bool CaloIsHF=0;
			bool CaloIsHB=0;
			bool CaloIsHE=0;
			bool CaloIsEE=0;
			bool CaloIsEB=0;

			vector<DetId> detIds=calt->constituents();	
			for(unsigned i=0; i < detIds.size(); i++){
				HcalDetId hcalDetId((detIds[i].det()==DetId::Hcal)?HcalDetId(detIds[i]):HcalDetId()); 

				CaloIsEB=(CaloIsEB||(EcalSubdetector(detIds[i].subdetId())==EcalBarrel&&detIds[i].det()==DetId::Ecal));
				CaloIsEE=(CaloIsEE||(EcalSubdetector(detIds[i].subdetId())==EcalEndcap&&detIds[i].det()==DetId::Ecal));
				CaloIsHB=(CaloIsHB||hcalDetId.subdet()==HcalBarrel);
				CaloIsHE=(CaloIsHE||hcalDetId.subdet()==HcalEndcap);
				CaloIsHF=(CaloIsHF||hcalDetId.subdet()==HcalForward);
			}

			if(calt->emEnergy()>MaxEBEnergy&&CaloIsEB&&!CaloIsEE)MaxEBEta=calt->p4().eta();
			if(calt->emEnergy()>MaxEEEnergy&&CaloIsEE&&!CaloIsEB)MaxEEEta=calt->p4().eta();
			if(calt->energyInHB()>MaxHBEnergy)MaxHBEta=calt->p4().eta();
			if(calt->energyInHE()>MaxHEEnergy)MaxHEEta=calt->p4().eta();
			if(calt->energyInHF()>MaxHFEnergy)MaxHFEta=calt->p4().eta();

			if(calt->emEnergy()>MaxEBEnergy&&CaloIsEB&&!CaloIsEE)MaxEBPhi=calt->p4().phi();
			if(calt->emEnergy()>MaxEEEnergy&&CaloIsEE&&!CaloIsEB)MaxEEPhi=calt->p4().phi();
			if(calt->energyInHB()>MaxHBEnergy)MaxHBPhi=calt->p4().phi();
			if(calt->energyInHE()>MaxHEEnergy)MaxHEPhi=calt->p4().phi();
			if(calt->energyInHF()>MaxHFEnergy)MaxHFPhi=calt->p4().phi();

			if(calt->emEnergy()>MaxEBEnergy&&CaloIsEB&&!CaloIsEE)MaxEBEnergy=calt->emEnergy();
			if(calt->emEnergy()>MaxEEEnergy&&CaloIsEE&&!CaloIsEB)MaxEEEnergy=calt->emEnergy();
			if(calt->energyInHB()>MaxHBEnergy)MaxHBEnergy=calt->energyInHB();
			if(calt->energyInHE()>MaxHEEnergy)MaxHEEnergy=calt->energyInHE();
			if(calt->energyInHF()>MaxHFEnergy)MaxHFEnergy=calt->energyInHF();

			SumEBEnergy+=calt->emEnergy()*int(CaloIsEB);
			SumEEEnergy+=calt->emEnergy()*int(CaloIsEE);
			SumHBEnergy+=calt->energyInHB();
			SumHEEnergy+=calt->energyInHE();
			SumHFEnergy+=calt->energyInHF();

			SumEBEta+=calt->emEnergy()*int(CaloIsEB)*calt->p4().eta();
			SumEEEta+=calt->emEnergy()*int(CaloIsEE)*calt->p4().eta();
			SumHBEta+=calt->energyInHB()*int(CaloIsHB)*calt->p4().eta();
			SumHEEta+=calt->energyInHE()*int(CaloIsHE)*calt->p4().eta();
			SumHFEta+=calt->energyInHF()*int(CaloIsHF)*calt->p4().eta();

			SumEBPhi+=calt->emEnergy()*int(CaloIsEB)*calt->p4().phi();
			SumEEPhi+=calt->emEnergy()*int(CaloIsEE)*calt->p4().phi();
			SumHBPhi+=calt->energyInHB()*int(CaloIsHB)*calt->p4().phi();
			SumHEPhi+=calt->energyInHE()*int(CaloIsHE)*calt->p4().phi();
			SumHFPhi+=calt->energyInHF()*int(CaloIsHF)*calt->p4().phi();
		}
	}
	
	AverageEBEta=SumEBEta/SumEBEnergy;
	AverageEEEta=SumEEEta/SumEEEnergy;
	AverageHBEta=SumHBEta/SumHBEnergy;
	AverageHEEta=SumHEEta/SumHEEnergy;
	AverageHFEta=SumHFEta/SumHFEnergy;
	
	AverageEBPhi=SumEBPhi/SumEBEnergy;
	AverageEEPhi=SumEEPhi/SumEEEnergy;
	AverageHBPhi=SumHBPhi/SumHBEnergy;
	AverageHEPhi=SumHEPhi/SumHEEnergy;
	AverageHFPhi=SumHFPhi/SumHFEnergy;

	CaloTree->Fill();
}
#endif
