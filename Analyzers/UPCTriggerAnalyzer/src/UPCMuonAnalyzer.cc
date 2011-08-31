#include "Analyzers/UPCTriggerAnalyzer/interface/UPCMuonAnalyzer.h"
#include <iostream>

using namespace edm;

UPCMuonAnalyzer::UPCMuonAnalyzer(const edm::ParameterSet& iConfig){}

UPCMuonAnalyzer::~UPCMuonAnalyzer(){}

void UPCMuonAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
	
	MuonTree = new TTree("MuonTree","MuonTree");
	MuonTree->Branch("Muons",&Muonz);
}

void UPCMuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<vector<reco::Muon> > muons;
	iEvent.getByLabel("muons",muons);
	
	Muonz.clear();

	if(!muons.failedToGet()){
		for(unsigned int i=0; i<(*muons).size();i++){Muonz.push_back((*muons)[i]);}
	}

	MuonTree->Fill();
}
