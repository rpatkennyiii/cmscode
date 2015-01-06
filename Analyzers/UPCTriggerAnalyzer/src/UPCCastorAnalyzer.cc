#include "Analyzers/UPCTriggerAnalyzer/interface/UPCCastorAnalyzer.h"

using namespace edm;
using namespace std;

UPCCastorAnalyzer::UPCCastorAnalyzer(const ParameterSet& iConfig){}

UPCCastorAnalyzer::~UPCCastorAnalyzer(){}

void UPCCastorAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	Handle<CastorRecHitCollection> castor_recHits_h;
	iEvent.getByType(castor_recHits_h);
	const CastorRecHitCollection *castor_recHits = castor_recHits_h.failedToGet()? 0 : &*castor_recHits_h;

	nbOfCASTORRecHits=0;

	 if(castor_recHits){

		for (CastorRecHitCollection::const_iterator chit=castor_recHits->begin();chit!=castor_recHits->end();chit++){		
			CastorRecHitModule[nbOfCASTORRecHits] = (unsigned int) ((chit->id()).module());
			CastorRecHitSector[nbOfCASTORRecHits] = (unsigned int) ((chit->id()).sector());
			CastorRecHitData[nbOfCASTORRecHits] = chit->energy();
        		nbOfCASTORRecHits++;
		}
		
		CastorRecHitTree->Fill();
	}
}


void UPCCastorAnalyzer::beginJob(){
	mFileServer->file().cd();
		
  CastorRecHitTree = new TTree("CastorRecTree","CastorRecTree");

	CastorRecHitTree->Branch("nCastorRecHits",&nbOfCASTORRecHits,"nCastorRecHits/i");
	CastorRecHitTree->Branch("CastorRecHitModule",CastorRecHitModule,"CastorRecHitModule[nCastorRecHits]/i");
	CastorRecHitTree->Branch("CastorRecHitSector",CastorRecHitSector,"CastorRecHitSector[nCastorRecHits]/i");
	CastorRecHitTree->Branch("CastorRecHitData",CastorRecHitData,"CastorRecHitData[nCastorRecHits]/F");
}
