#include "Analyzers/UPCTriggerAnalyzer/interface/UPCRunAnalyzer.h"

using namespace edm;

UPCRunAnalyzer::UPCRunAnalyzer(const edm::ParameterSet& iConfig){}

UPCRunAnalyzer::~UPCRunAnalyzer(){}

void UPCRunAnalyzer::beginJob(){
	mFileServer->file().cd();
	
	RunTree = new TTree("RunTree","RunTree");
	RunTree->Branch("BunchXing",&RunData[0],"BunchXing/I");
	RunTree->Branch("LumiBlock",&RunData[1],"LumiBlock/I");
	RunTree->Branch("Event",&RunData[2],"Event/I");
	RunTree->Branch("Run",&RunData[3],"Run/I");
}

void UPCRunAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	RunData[0]=iEvent.bunchCrossing();
	RunData[1]=iEvent.id().luminosityBlock();
	RunData[2]=iEvent.id().event();
	RunData[3]=iEvent.id().run();

	RunTree->Fill();
}
