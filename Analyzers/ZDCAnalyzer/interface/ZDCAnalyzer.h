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
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"

class ZDCAnalyzer : public edm::EDAnalyzer {
public:
	explicit ZDCAnalyzer(const edm::ParameterSet&);
	~ZDCAnalyzer();

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();

        //void setZDClabels(MonitorElement * h);

	double getTime(double fData[10], double& fSum);
	int getTSMax(double fData[10], int fTS);
	bool isGood(double fData[10], double fCut, double fPercentage);
	TH2F *book2DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
		int fNbinsX, double fXmin, double fXmax, int fNbinsY, double fYmin, double fYmax) const; 
	TH1F *book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
		int fNbins, double fXmin, double fXmax) const;
private:
	std::string externalFile1;
	std::string externalFile2;
	long runBegin,lumibegin,lumiend,evtNo;
	int run, event, lumi;
	std::string startTime;

	int Runno;
	// edm::ESHandle<HcalDbService> conditions;
	edm::Service<TFileService> mFileServer;

        TH1F* h_bunchXing;
        TH1F* h_lumiBlock;
	TH1F* h_ZDCP_EM_Pulse[5];
	TH1F* h_ZDCP_HAD_Pulse[4];
	TH1F* h_ZDCP_EM_Charge[5];
	TH1F* h_ZDCP_EM_RecHitEnergy[5];
	TH1F* h_ZDCP_HAD_Charge[4];
	TH1F* h_ZDCP_HAD_RecHitEnergy[4];
	TH1F* h_ZDCP_EM_TSMean[5];
	TH1F* h_ZDCP_HAD_TSMean[4];
	TH1F* h_ZDCP_EM_RecHitEnergyAve;
	TH1F* h_ZDCP_HAD_RecHitEnergyAve;
	TH2F* h_ZDCP_Charge_Correlation;
	TH2F* h_ZDCP_Charge_CorrelationHAD;
	TH2F* h_ZDCP_Charge_Correlation2;
	TH2F* h_ZDCP_RecHit_Correlation;
	TH1F* h_ZDCP_EMHAD_ChargeAve;
	TH1F* h_ZDCP_EMHAD_TSMeanAve;

	TH1F* h_ZDCM_EM_Pulse[5];
	TH1F* h_ZDCM_HAD_Pulse[4];
	TH1F* h_ZDCM_EM_Charge[5];
	TH1F* h_ZDCM_EM_RecHitEnergy[5];
	TH1F* h_ZDCM_HAD_Charge[4];
	TH1F* h_ZDCM_HAD_RecHitEnergy[4];
	TH1F* h_ZDCM_EM_TSMean[5];
	TH1F* h_ZDCM_HAD_TSMean[4];
	TH1F* h_ZDCM_EM_RecHitEnergyAve;
	TH1F* h_ZDCM_HAD_RecHitEnergyAve;
	TH2F* h_ZDCM_RecHit_Correlation;
	TH1F* h_ZDCM_EMHAD_ChargeAve;
	TH1F* h_ZDCM_EMHAD_TSMeanAve;

};
DEFINE_FWK_MODULE(ZDCAnalyzer);

#endif
