// -*- C++ -*-
//
// Package:    SecMarZDCAnalyzer
// Class:      SecMarZDCAnalyzer
// 
/**\class SecMarZDCAnalyzer SecMarZDCAnalyzer.cc Analyzers/SecMarZDCAnalyzer/src/SecMarZDCAnalyzer.cc

Description: <one line class summary>

	Implementation:
<Notes on implementation>
*/
//
// Original Author:  Taylan Yetkin
//         Created:  Sat Nov  7 23:04:14 CET 2009
// $Id: SecMarZDCAnalyzer.h,v 1.1.1.1 2010/01/21 00:44:19 ssen Exp $
//
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
// needed due to CMSSW_3_5_3 compile fail:
#include "TCanvas.h"


class TH1F;
class TH2F;

class SecMarZDCAnalyzer : public edm::EDAnalyzer {
public:
	explicit SecMarZDCAnalyzer(const edm::ParameterSet&);
	~SecMarZDCAnalyzer();


private:
	virtual void beginJob() ;
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
	void htmlOutput(void);
	void ZDCHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);

	void histoHTML(TH1F * hist, const char *xlab, const char *ylab, int width, std::ofstream & htmlFile, std::string htmlDir, bool log=false);
	void histoHTML2(TH2F * hist, const char *xlab, const char *ylab, int width, std::ofstream & htmlFile, std::string htmlDir, bool color = false, bool log=false);
	std::string getIMG(TH1F* hist, int size, std::string htmlDir, const char* xlab, const char* ylab, bool log=false);
	std::string getIMG2(TH2F* hist, int size, std::string htmlDir, const char* xlab, const char* ylab,bool color=false, bool log=false);
	void parseString(std::string& title);
	void cleanString(std::string& title);

private:


	std::string baseHtmlDir_;
	std::string externalFile1;
	std::string externalFile2;
	long runBegin,lumibegin,lumiend,evtNo;
	int run, event, lumi;
	std::string startTime;
	bool htmlPrint;
	bool doZDCHTML;

	int Runno;
	// edm::ESHandle<HcalDbService> conditions;
	edm::Service<TFileService> mFileServer;


	double EMP[5]; 
	double HadP[4]; 
	double DigiEMP[5]; 
	double EMM[5]; 
	double HadM[4]; 
	double DigiEMM[5]; 
	double C_Had;
	double C_EM;
	double alpha;
    double EMP_TotE;
    double HadP_TotE;
    double Digi_EMP_TotE;
    double EMM_TotE;
    double HadM_TotE;
    double Digi_EMM_TotE;
    
        TH1F* h_bunchXing;
        TH1F* h_lumiBlock;
	TH1F* h_ZDCP_EM_Pulse[5];
	TH1F* h_ZDCP_HAD_Pulse[4];
	TH1F* h_ZDCP_EM_Charge[5];
	TH1F* h_ZDCP_EM_RecHitEnergy[5];
	TH1F* h_ZDCP_EM_RecHitTiming[5];
	TH1F* h_ZDCP_HAD_Charge[4];
	TH1F* h_ZDCP_HAD_RecHitEnergy[4];
	TH1F* h_ZDCP_HAD_RecHitTiming[4];
	TH1F* h_ZDCP_EM_TSMean[5];
	TH1F* h_ZDCP_HAD_TSMean[4];
	TH1F* h_ZDCP_EM_RecHitEnergyAve;
	TH1F* h_ZDCP_HAD_RecHitEnergyAve;
	TH1F* h_ZDCP_EM_RecHitTimingAve;
	TH1F* h_ZDCP_HAD_RecHitTimingAve;
	TH1F* h_ZDCP_EMHAD_ChargeAve;
	TH1F* h_ZDCP_EMHAD_TSMeanAve;
	TH1F* h_ZDCP_EM_TotE;
	TH1F* h_ZDCP_Had_TotE;
	TH1F* h_ZDCP_TotE_HadronLike;
	TH1F* h_ZDCP_TotE;

	TH1F* h_ZDCM_EM_Pulse[5];
	TH1F* h_ZDCM_HAD_Pulse[4];
	TH1F* h_ZDCM_EM_Charge[5];
	TH1F* h_ZDCM_EM_RecHitEnergy[5];
	TH1F* h_ZDCM_EM_RecHitTiming[5];
	TH1F* h_ZDCM_HAD_Charge[4];
	TH1F* h_ZDCM_HAD_RecHitEnergy[4];
	TH1F* h_ZDCM_HAD_RecHitTiming[4];
	TH1F* h_ZDCM_EM_TSMean[5];
	TH1F* h_ZDCM_HAD_TSMean[4];
	TH1F* h_ZDCM_EM_RecHitEnergyAve;
	TH1F* h_ZDCM_HAD_RecHitEnergyAve;
	TH1F* h_ZDCM_EM_RecHitTimingAve;
	TH1F* h_ZDCM_HAD_RecHitTimingAve;
	TH1F* h_ZDCM_EMHAD_ChargeAve;
	TH1F* h_ZDCM_EMHAD_TSMeanAve;
	TH1F* h_ZDCM_EM_TotE;
	TH1F* h_ZDCM_Had_TotE;
	TH1F* h_ZDCM_TotE_HadronLike;
	TH1F* h_ZDCM_TotE;	
};
DEFINE_FWK_MODULE(SecMarZDCAnalyzer);
