#include "Analyzers/UPCTriggerAnalyzer/interface/UPCHFEnergyAnalyzer.h"

using namespace edm;
using namespace std;

UPCHFEnergyAnalyzer::UPCHFEnergyAnalyzer(const ParameterSet& iConfig){}

UPCHFEnergyAnalyzer::~UPCHFEnergyAnalyzer(){}

void UPCHFEnergyAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();

	CaloTree = new TTree("CaloTree","CaloTree");
		
	CaloTree->Branch("CaloSize",&CaloSize);
	CaloTree->Branch("CaloEnergy",&CaloEnergy,"CaloEnergy[CaloSize]/F");
	CaloTree->Branch("CaloEta",&CaloEta,"CaloEta[CaloSize]/F");
	CaloTree->Branch("CaloPhi",&CaloPhi,"CaloPhi[CaloSize]/F");
}


void UPCHFEnergyAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	Handle<CaloTowerCollection> calotower;
	iEvent.getByType(calotower);

	if(!calotower.failedToGet()){
		CaloSize=0;
		for(CaloTowerCollection::const_iterator calt=(&*calotower)->begin();calt!=(&*calotower)->end();calt++){
			if(abs(calt->eta())>=3){
				CaloEnergy[CaloSize]=calt->energy();
				CaloEta[CaloSize]=calt->eta();
				CaloPhi[CaloSize]=calt->phi();
				CaloSize++;
			}
		}
	}

	CaloTree->Fill();
}
