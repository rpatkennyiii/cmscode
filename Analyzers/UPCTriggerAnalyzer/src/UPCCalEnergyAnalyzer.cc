#include "Analyzers/UPCTriggerAnalyzer/interface/UPCCalEnergyAnalyzer.h"

using namespace edm;
using namespace std;

UPCCalEnergyAnalyzer::UPCCalEnergyAnalyzer(const ParameterSet& iConfig){}

UPCCalEnergyAnalyzer::~UPCCalEnergyAnalyzer(){}

void UPCCalEnergyAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();

	CaloTree = new TTree("CaloTree","CaloTree");
		
	CaloTree->Branch("CaloSize",&CaloSize);
	CaloTree->Branch("CaloEnergy",&CaloEnergy,"CaloEnergy[CaloSize]/F");
	CaloTree->Branch("CaloHadEnergy",&CaloHadEnergy,"CaloEnergyHad[CaloSize]/F");
	CaloTree->Branch("CaloEmEnergy",&CaloEmEnergy,"CaloEnergyEm[CaloSize]/F");
	CaloTree->Branch("CaloEta",&CaloEta,"CaloEta[CaloSize]/F");
	CaloTree->Branch("CaloPhi",&CaloPhi,"CaloPhi[CaloSize]/F");
}


void UPCCalEnergyAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	Handle<CaloTowerCollection> calotower;
	iEvent.getByType(calotower);

	if(!calotower.failedToGet()){
		CaloSize=0;
		for(CaloTowerCollection::const_iterator calt=(&*calotower)->begin();calt!=(&*calotower)->end();calt++){
			CaloEnergy[CaloSize]=calt->energy();
			CaloHadEnergy[CaloSize]=calt->hadEnergy();
			CaloEmEnergy[CaloSize]=calt->emEnergy();
			CaloEta[CaloSize]=calt->eta();
			CaloPhi[CaloSize]=calt->phi();
			CaloSize++;
		}
	}

	CaloTree->Fill();
}
