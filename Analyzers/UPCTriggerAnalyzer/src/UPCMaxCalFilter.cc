#include <cmath>

// user include files
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/Framework/interface/EventSetup.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"

using namespace reco;
using namespace std;
using namespace edm;

class UPCMaxCalFilter : public EDFilter {
public:
	explicit UPCMaxCalFilter(const ParameterSet&);
	~UPCMaxCalFilter(){};

private:
	virtual void beginJob(){};
        virtual void endJob(){};
	virtual bool filter(edm::Event&, const edm::EventSetup&);

	double hfThreshold;	
	double heThreshold;	
	double hbThreshold;	
	double eeThreshold;	
	double ebThreshold;	
};

UPCMaxCalFilter::UPCMaxCalFilter(const ParameterSet& iConfig):hfThreshold(iConfig.getUntrackedParameter<double>("hfThreshold")),
	heThreshold(iConfig.getUntrackedParameter<double>("heThreshold")),
	hbThreshold(iConfig.getUntrackedParameter<double>("hbThreshold")),
	eeThreshold(iConfig.getUntrackedParameter<double>("eeThreshold")),
	ebThreshold(iConfig.getUntrackedParameter<double>("ebThreshold")){}

bool UPCMaxCalFilter::filter(Event& iEvent, const EventSetup& iSetup)
{
	Handle<CaloTowerCollection> calotower;
	iEvent.getByType(calotower);

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

				CaloIsHF=(CaloIsHF||(hcalDetId.subdet()==HcalForward&&detIds[i].det()==DetId::Hcal))?1:0;
				CaloIsHB=(CaloIsHB||(hcalDetId.subdet()==HcalBarrel&&detIds[i].det()==DetId::Hcal))?1:0;
				CaloIsHE=(CaloIsHE||(hcalDetId.subdet()==HcalEndcap&&detIds[i].det()==DetId::Hcal))?1:0;
				CaloIsEB=(CaloIsEB||(EcalSubdetector(detIds[i].subdetId())==EcalBarrel&&detIds[i].det()==DetId::Ecal))?1:0;
				CaloIsEE=(CaloIsEE||(EcalSubdetector(detIds[i].subdetId())==EcalEndcap&&detIds[i].det()==DetId::Ecal))?1:0;
			}

			if(CaloIsEB&&(calt->emEnergy()>ebThreshold))return 0;
			if(CaloIsEE&&(calt->emEnergy()>eeThreshold))return 0;
			if(CaloIsHB&&(calt->hadEnergy()>hbThreshold))return 0;
			if(CaloIsHE&&(calt->hadEnergy()>heThreshold))return 0;
			if(CaloIsHF&&(calt->hadEnergy()>hfThreshold))return 0;
		}

		return 1;
	}

	return 0;
}

DEFINE_FWK_MODULE(UPCMaxCalFilter);
