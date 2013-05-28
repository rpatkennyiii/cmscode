#include "Analyzers/UPCTriggerAnalyzer/interface/UPCCalEnergyAnalyzer.h"

using namespace edm;
using namespace std;

UPCCalEnergyAnalyzer::UPCCalEnergyAnalyzer(const ParameterSet& iConfig){}

UPCCalEnergyAnalyzer::~UPCCalEnergyAnalyzer(){}

void UPCCalEnergyAnalyzer::beginJob(){
	mFileServer->file().cd();

	CaloTree = new TTree("CaloTree","CaloTree");
		
	CaloTree->Branch("CaloSize",&CaloSize);
	CaloTree->Branch("CaloEnergy",CaloEnergy,"CaloEnergy[CaloSize]/F");
	CaloTree->Branch("CaloHadEnergy",CaloHadEnergy,"CaloEnergyHad[CaloSize]/F");
	CaloTree->Branch("CaloEmEnergy",CaloEmEnergy,"CaloEnergyEm[CaloSize]/F");
	CaloTree->Branch("CaloEta",CaloEta,"CaloEta[CaloSize]/F");
	CaloTree->Branch("CaloPhi",CaloPhi,"CaloPhi[CaloSize]/F");
	CaloTree->Branch("CaloIsHF",CaloIsHF,"CaloIsHF[CaloSize]/O");
	CaloTree->Branch("CaloIsHB",CaloIsHB,"CaloIsHB[CaloSize]/O");
	CaloTree->Branch("CaloIsHE",CaloIsHE,"CaloIsHE[CaloSize]/O");
	CaloTree->Branch("CaloIsEE",CaloIsEE,"CaloIsEE[CaloSize]/O");
	CaloTree->Branch("CaloIsEB",CaloIsEB,"CaloIsEB[CaloSize]/O");
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

			CaloIsHF[CaloSize]=0;
			CaloIsHB[CaloSize]=0;
			CaloIsHE[CaloSize]=0;
			CaloIsEE[CaloSize]=0;
			CaloIsEB[CaloSize]=0;

			vector<DetId> detIds=calt->constituents();	
			for(unsigned i=0; i < detIds.size(); i++){
				HcalDetId hcalDetId((detIds[i].det()==DetId::Hcal)?HcalDetId(detIds[i]):HcalDetId()); 

				CaloIsHF[CaloSize]=(CaloIsHF[CaloSize]||
						(hcalDetId.subdet()==HcalForward&&detIds[i].det()==DetId::Hcal))?1:0;
				CaloIsHB[CaloSize]=(CaloIsHB[CaloSize]||
						(hcalDetId.subdet()==HcalBarrel&&detIds[i].det()==DetId::Hcal))?1:0;
				CaloIsHE[CaloSize]=(CaloIsHE[CaloSize]||
						(hcalDetId.subdet()==HcalEndcap&&detIds[i].det()==DetId::Hcal))?1:0;
				CaloIsEB[CaloSize]=(CaloIsEB[CaloSize]||
						(EcalSubdetector(detIds[i].subdetId())==EcalBarrel&&detIds[i].det()==DetId::Ecal))?1:0;
				CaloIsEE[CaloSize]=(CaloIsEE[CaloSize]||
						(EcalSubdetector(detIds[i].subdetId())==EcalEndcap&&detIds[i].det()==DetId::Ecal))?1:0;
			}

			CaloSize++;
		}
	}

	CaloTree->Fill();
}
