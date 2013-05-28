#ifndef UPCMAXCALFILTER_H
#define UPCMAXCALFILTER_H

// system include files
#include <cmath>

// user include files
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "FWCore/Framework/interface/EventSetup.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"

using namespace reco;
using namespace std;

class UPCMaxCalFilter : public edm::EDFilter {
public:
	UPCMaxCalFilter(const edm::ParameterSet&);
	~UPCMaxCalFilter(){};
	bool filter(const edm::Event&, const edm::EventSetup&);

private:
	void beginJob(){};

	double hfThreshold;	
	double heThreshold;	
	double hbThreshold;	
	double eeThreshold;	
	double ebThreshold;	
};
DEFINE_FWK_MODULE(UPCMaxCalFilter);

#endif
