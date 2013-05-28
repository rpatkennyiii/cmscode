#include "Analyzers/UPCTriggerAnalyzer/interface/UPCCastorAnalyzer.h"

using namespace edm;
using namespace std;

UPCCastorAnalyzer::UPCCastorAnalyzer(const ParameterSet& iConfig){}

UPCCastorAnalyzer::~UPCCastorAnalyzer(){}

void UPCCastorAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	Handle<CastorDigiCollection> castor_digi_h;
	Handle<CastorRecHitCollection> castor_recHits_h;
	iEvent.getByType(castor_digi_h);
	iEvent.getByType(castor_recHits_h);
	const CastorDigiCollection *castor_digi = castor_digi_h.failedToGet()? 0 : &*castor_digi_h;
	const CastorRecHitCollection *castor_recHits = castor_recHits_h.failedToGet()? 0 : &*castor_recHits_h;

	nbOfCASTORDigis=0;
	nbOfCASTORRecHits=0;

	if(castor_digi){
		for (CastorDigiCollection::const_iterator j=castor_digi->begin();j!=castor_digi->end();j++){
			const CastorDataFrame digi = (const CastorDataFrame)(*j);		
			CastorDigiModule[nbOfCASTORDigis] = (unsigned int) (digi.id()).module();
			CastorDigiSector[nbOfCASTORDigis] = (unsigned int) (digi.id()).sector();

			int fTS = digi.size();
			for (int i = 0; i < fTS; ++i) {
				CastorDigiDatafC[i+nbOfCASTORDigis*10] = (float) digi[i].nominal_fC();
				CastorDigiDataADC[i+nbOfCASTORDigis*10] = (unsigned int) (digi[i].adc());
			}
			nbOfCASTORDigis++;
		}
		
		CastorDigiTree->Fill();
	}

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
		
   	CastorDigiTree = new TTree("CastorDigiTree","CastorDigiTree");
   	CastorRecHitTree = new TTree("CastorRecTree","CastorRecTree");

	CastorDigiTree->Branch("nCastorDigis",&nbOfCASTORDigis,"nCastorDigis/i");
	CastorDigiTree->Branch("CastorDigiModule",CastorDigiModule,"CastorDigiModule[nCastorDigis]/i");
	CastorDigiTree->Branch("CastorDigiSector",CastorDigiSector,"CastorDigiSector[nCastorDigis]/i");
	CastorDigiTree->Branch("CastorDigiDataADC",CastorDigiDataADC,"CastorDigiDataADC[nCastorDigis]/i");
	CastorDigiTree->Branch("CastorDigiDatafC",CastorDigiDatafC,"CastorDigiDatafC[nCastorDigis]/F");

	CastorRecHitTree->Branch("nCastorRecHits",&nbOfCASTORRecHits,"nCastorRecHits/i");
	CastorRecHitTree->Branch("CastorRecHitModule",CastorRecHitModule,"CastorRecHitModule[nCastorRecHits]/i");
	CastorRecHitTree->Branch("CastorRecHitSector",CastorRecHitSector,"CastorRecHitSector[nCastorRecHits]/i");
	CastorRecHitTree->Branch("CastorRecHitData",CastorRecHitData,"CastorRecHitData[nCastorRecHits]/F");
}
