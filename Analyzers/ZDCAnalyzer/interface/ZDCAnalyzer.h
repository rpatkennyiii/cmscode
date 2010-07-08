#ifndef ZDCANALYZER_H
#define ZDCANALYZER_H

// system include files
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sys/time.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/DetId/interface/DetId.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"

class ZDCAnalyzer : public edm::EDAnalyzer {
public:
	explicit ZDCAnalyzer(const edm::ParameterSet&);
	~ZDCAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	TH2F *book2DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
		int fNbinsX, double fXmin, double fXmax, int fNbinsY, double fYmin, double fYmax) const; 
	TH1F *book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
		int fNbins, double fXmin, double fXmax) const;
private:
	long runBegin,lumibegin,lumiend,evtNo;
	int run, event, lumi;
	std::string startTime;
	std::string *BranchNames;
        double data[180];

	int Runno;
	edm::Service<TFileService> mFileServer;

        TH1F* h_bunchXing;
        TH1F* h_lumiBlock;
	TTree* ZDCTree;
};
DEFINE_FWK_MODULE(ZDCAnalyzer);

#endif
