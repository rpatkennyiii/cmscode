#include "Analyzers/UPCTriggerAnalyzer/interface/UPCEcalAnalyzer.h"

using namespace edm;
using namespace std;

UPCEcalAnalyzer::UPCEcalAnalyzer(const ParameterSet& iConfig):
	EcalColName(iConfig.getParameter<string>("ecalCollection")){}

UPCEcalAnalyzer::~UPCEcalAnalyzer(){}

void UPCEcalAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
	
	string EcalSecName = EcalColName.substr(11);
   	UPCEcalTree = new TTree(("UPC"+EcalSecName+"Tree").c_str(),("UPC"+EcalSecName+"Tree").c_str());
	UPCEcalTree->Branch(("n"+EcalSecName).c_str(),&nEcal,("n"+EcalSecName+"/i").c_str());
	UPCEcalTree->Branch((EcalSecName+"Energy").c_str(),EcalData,(EcalSecName+"Energy[nEcal]/F").c_str());
}

void UPCEcalAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	nEcal=0;	
	Handle<EcalRecHitCollection> ecal_recHits_h;
	iEvent.getByLabel(EcalColName.c_str(),ecal_recHits_h);
	const EcalRecHitCollection *ecal_recHits = ecal_recHits_h.failedToGet()? 0 : &*ecal_recHits_h;
	
	nEcal=ecal_recHits->size();
	if(ecal_recHits){
		for(unsigned int i=0; i<ecal_recHits->size();i++){
			EcalData[i]=(*ecal_recHits)[i].energy();
		}
	}

	UPCEcalTree->Fill();
}


