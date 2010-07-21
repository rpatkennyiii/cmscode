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
// $Id: SecMarZDCAnalyzer.cc,v 1.1.1.1 2010/01/21 00:44:19 ssen Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/time.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Analyzers/SecMarZDCAnalyzer/interface/SecMarZDCAnalyzer.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"

static const float HFQIEConst = 2.6;

using namespace edm;
using namespace std;

SecMarZDCAnalyzer::SecMarZDCAnalyzer(const edm::ParameterSet& iConfig){
        Runno = iConfig.getUntrackedParameter < int >("RunNo", 1);
	htmlPrint = iConfig.getUntrackedParameter < bool > ("printHTML", false);
	doZDCHTML = iConfig.getUntrackedParameter < bool > ("printZDCHTML", false);

	// base Html output directory
	baseHtmlDir_ = iConfig.getUntrackedParameter < string > ("baseHtmlDir", "");
	if (baseHtmlDir_.size() != 0)
		cout << "-->HTML output will go to baseHtmlDir = '" << baseHtmlDir_ << "'" << endl;
	else
		cout << "-->HTML output is disabled" << endl;

	runBegin = -1;
	evtNo = 0;
	lumibegin = 0;
	lumiend = 0;
	startTime = "Not Avaliable";
	// root style
	gStyle->Reset("Default");
	gStyle->SetOptFit(1);
	gStyle->SetCanvasColor(0);
	gStyle->SetPadColor(0);
	gStyle->SetFillColor(0);
	gStyle->SetTitleFillColor(10);
	// gStyle->SetOptStat(0);
	gStyle->SetOptStat("ouemr");
	gStyle->SetPalette(1);
}


SecMarZDCAnalyzer::~SecMarZDCAnalyzer(){
}

void SecMarZDCAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	++evtNo;
	time_t a = (iEvent.time().value()) >> 32;
	event = iEvent.id().event();
	run = iEvent.id().run();

	lumi = iEvent.luminosityBlock();

	if (runBegin < 0) {         // parameters for the first event
		startTime = ctime(&a);
		lumibegin = lumiend = lumi;
		runBegin = iEvent.id().run();
	}
	run = iEvent.id().run();//needed in the tree

	if (lumi < lumibegin)lumibegin = lumi;
	if (lumi > lumiend)lumiend = lumi;

	h_lumiBlock->Fill(iEvent.luminosityBlock());
	h_bunchXing->Fill(iEvent.bunchCrossing());

	edm::Handle<ZDCDigiCollection> zdc_digi_h;
	iEvent.getByType(zdc_digi_h);
	const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;

	for (int iz=0;iz<5;iz++){
		EMP[iz]=0.0; 
		DigiEMP[iz]=0.0; 
		EMM[iz]=0.0; 
		DigiEMM[iz]=0.0; 
        }
	for (int iy=0;iy<4;iy++){
		HadM[iy]=0.0; 
		HadP[iy]=0.0; 
    	}
    	EMP_TotE=0.0;
    	HadP_TotE=0.0;
    	Digi_EMP_TotE=0.0;
    	EMM_TotE=0.0;
    	HadM_TotE=0.0;
    	Digi_EMM_TotE=0.0;
    
	if(zdc_digi){
		for (ZDCDigiCollection::const_iterator j=zdc_digi->begin();j!=zdc_digi->end(); ++j) {
			const ZDCDataFrame digi = (const ZDCDataFrame)(*j);		
			int iSide      = digi.id().zside();
			int iSection   = digi.id().section();
			int iChannel   = digi.id().channel();
			double fSum = 0.;		
			double fData[10] = {-999.};
			int fTS = digi.size();
			for (int i = 0; i < fTS; ++i) {
				fData[i] = HFQIEConst*digi[i].nominal_fC();
			}
			        
			double fTSMean = getTime(fData, fSum);

			if(iSection==1){
                            if (iSide ==1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCP_EM_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCP_EM_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCP_EM_TSMean[iChannel-1]->Fill(fTSMean);
			    }
                            if (iSide == -1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCM_EM_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCM_EM_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCM_EM_TSMean[iChannel-1]->Fill(fTSMean);
			    }
			}
			if(iSection==2){
                            if (iSide ==1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCP_HAD_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCP_HAD_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCP_HAD_TSMean[iChannel-1]->Fill(fTSMean);
			    }
                            if (iSide == -1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCM_HAD_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCM_HAD_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCM_HAD_TSMean[iChannel-1]->Fill(fTSMean);
			    }
			}				
		}
	}

	// ZDC RecHits here.
	Handle <ZDCRecHitCollection> zdc_recHits_h;
	//iEvent.getByLabel("zdcreco", zdc_recHits_h);
	iEvent.getByType(zdc_recHits_h);
	const ZDCRecHitCollection *zdc_recHits = zdc_recHits_h.failedToGet()? 0 : &*zdc_recHits_h;

	if (zdc_recHits) { // object is available
    		std::cout << "ZDC RECHITS here" << iEvent.id() << std::endl;
		for (ZDCRecHitCollection::const_iterator zhit = zdc_recHits->begin(); zhit != zdc_recHits->end(); zhit++){		
			 int iSide      = (zhit->id()).zside();
			 int iSection   = (zhit->id()).section();
			 int iChannel   = (zhit->id()).channel();
			 //std::cout << "RecHitEnergy  " << zhit->energy() << "  RecHitTime  " << zhit->time() << std::endl;
			 std::cout << "iSection  " << iSection << "  iSide  "  << iSide << "  iChannel  " <<  iChannel 
			           <<  "  RecHitEnergy  " << zhit->energy() << "  RecHitTime  " << zhit->time() << std::endl;
			 if(iSection==1){
			     	if (iSide ==1 ){
				h_ZDCP_EM_RecHitEnergy[iChannel-1]->Fill(zhit->energy());
				h_ZDCP_EM_RecHitTiming[iChannel-1]->Fill(zhit->time());
				EMP[iChannel-1]=EMP[iChannel-1]+zhit->energy();
				DigiEMP[iChannel-1]=DigiEMP[iChannel-1]/C_EM + zhit->energy();
				}
				if (iSide == -1 ){
				h_ZDCM_EM_RecHitEnergy[iChannel-1]->Fill(zhit->energy());
				h_ZDCM_EM_RecHitTiming[iChannel-1]->Fill(zhit->time());
				EMM[iChannel-1]=EMM[iChannel-1]+zhit->energy();
				DigiEMM[iChannel-1]=DigiEMM[iChannel-1]/C_EM + zhit->energy();
				}
			 }
			 if(iSection==2){
			     	if (iSide ==1 ){
				h_ZDCP_HAD_RecHitEnergy[iChannel-1]->Fill(zhit->energy());
				h_ZDCP_HAD_RecHitTiming[iChannel-1]->Fill(zhit->time());
				HadP[iChannel-1] = HadP[iChannel-1] + zhit->energy();
				}
				if (iSide == -1 ){
				h_ZDCM_HAD_RecHitEnergy[iChannel-1]->Fill(zhit->energy());
				h_ZDCM_HAD_RecHitTiming[iChannel-1]->Fill(zhit->time());
				HadM[iChannel-1] = HadM[iChannel-1] + zhit->energy();
				}
			}
		}
    	}
	for (int jz=0;jz<5;jz++){
		EMP_TotE=EMP_TotE+EMP[jz];
		EMM_TotE=EMM_TotE+EMM[jz];
		Digi_EMP_TotE=Digi_EMP_TotE+DigiEMP[jz];
		Digi_EMM_TotE=Digi_EMM_TotE+DigiEMM[jz];
    }
	for (int jy=0;jy<4;jy++){
		HadP_TotE=HadP_TotE+HadP[jy];
		HadM_TotE=HadM_TotE+HadM[jy];
    }
    h_ZDCP_EM_TotE->Fill(EMP_TotE);
    h_ZDCM_EM_TotE->Fill(EMM_TotE);
    h_ZDCP_Had_TotE->Fill(HadP_TotE);
    h_ZDCM_Had_TotE->Fill(HadM_TotE);
    h_ZDCP_TotE_HadronLike->Fill(alpha*Digi_EMP_TotE*C_Had + HadP_TotE);
    h_ZDCM_TotE_HadronLike->Fill(alpha*Digi_EMM_TotE*C_Had + HadM_TotE);
    h_ZDCP_TotE->Fill(EMP_TotE+HadP_TotE);
    h_ZDCM_TotE->Fill(EMM_TotE+HadM_TotE);
	
}

void SecMarZDCAnalyzer::beginJob(){
	TFileDirectory ZDCDir = mFileServer->mkdir("ZDC");
	char title[128];
	char name[128];
	C_Had = 0.782828;
	C_EM = 0.025514;
	alpha = 1.0;

	for(int i = 0; i < 4;++i){
		// pulse Plus Side	
		sprintf(title,"h_ZDCP_HADChan_%i_Pulse",i+1);
		sprintf(name,"ZDC HAD Section Pulse for channel %i",i+1);
		h_ZDCP_HAD_Pulse[i] = book1DHistogram(ZDCDir,title, name, 10, -0.5, 9.5);
		h_ZDCP_HAD_Pulse[i]->SetFillColor(kBlue);
		//integrated charge over 10 time samples		
		sprintf(title,"h_ZDCP_HADChan_%i_Charge",i+1);
		sprintf(name,"ZDC HAD Section Charge for channel %i",i+1);
		h_ZDCP_HAD_Charge[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000);
		h_ZDCP_HAD_Charge[i]->SetFillColor(kBlue);
		//charge weighted time slice		
		sprintf(title,"h_ZDCP_HADChan_%i_TSMean",i+1);
		sprintf(name,"ZDC HAD Section TSMean for channel %i",i+1);
		h_ZDCP_HAD_TSMean[i] = book1DHistogram(ZDCDir,title, name, 100, -0.5, 9.5);
		h_ZDCP_HAD_TSMean[i]->SetFillColor(kBlue);
		//RecHit for HAD sections
		sprintf(title,"h_ZDCP_HADChan_%i_RecHit_Energy",i+1);
		sprintf(name,"ZDC HAD Section Rechit Energy for channel %i",i+1);
		h_ZDCP_HAD_RecHitEnergy[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_HAD_RecHitEnergy[i]->SetFillColor(kRed);

		sprintf(title,"h_ZDCP_HADChan_%i_RecHit_Timing",i+1);
		sprintf(name,"ZDC HAD Section Rechit Timing for channel %i",i+1);
		h_ZDCP_HAD_RecHitTiming[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_HAD_RecHitTiming[i]->SetFillColor(kRed);

		// pulse Minus Side	
		sprintf(title,"h_ZDCM_HADChan_%i_Pulse",i+1);
		sprintf(name,"ZDC HAD Section Pulse for channel %i",i+1);
		h_ZDCM_HAD_Pulse[i] = book1DHistogram(ZDCDir,title, name, 10, -0.5, 9.5);
		h_ZDCM_HAD_Pulse[i]->SetFillColor(7);
		//integrated charge over 10 time samples		
		sprintf(title,"h_ZDCM_HADChan_%i_Charge",i+1);
		sprintf(name,"ZDC HAD Section Charge for channel %i",i+1);
		h_ZDCM_HAD_Charge[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000);
		h_ZDCM_HAD_Charge[i]->SetFillColor(7);
		//charge weighted time slice		
		sprintf(title,"h_ZDCM_HADChan_%i_TSMean",i+1);
		sprintf(name,"ZDC HAD Section TSMean for channel %i",i+1);
		h_ZDCM_HAD_TSMean[i] = book1DHistogram(ZDCDir,title, name, 100, -0.5, 9.5);
		h_ZDCM_HAD_TSMean[i]->SetFillColor(7);
		//RecHit for HAD sections
		sprintf(title,"h_ZDCM_HADChan_%i_RecHit_Energy",i+1);
		sprintf(name,"ZDC HAD Section Rechit Energy for channel %i",i+1);
		h_ZDCM_HAD_RecHitEnergy[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_HAD_RecHitEnergy[i]->SetFillColor(kRed);

		sprintf(title,"h_ZDCM_HADChan_%i_RecHit_Timing",i+1);
		sprintf(name,"ZDC HAD Section Rechit Timing for channel %i",i+1);
		h_ZDCM_HAD_RecHitTiming[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_HAD_RecHitTiming[i]->SetFillColor(kRed);
	}
	for(int i = 0; i < 5;++i){
		// pulse Plus Side 
		sprintf(title,"h_ZDCP_EMChan_%i_Pulse",i+1);
		sprintf(name,"ZDC EM Section Pulse for channel %i",i+1);
		h_ZDCP_EM_Pulse[i] = book1DHistogram(ZDCDir,title, name, 10, -0.5, 9.5);		
		h_ZDCP_EM_Pulse[i]->SetFillColor(kBlue);
		//integrated charge over 10 time samples
		sprintf(title,"h_ZDCP_EMChan_%i_Charge",i+1);
		sprintf(name,"ZDC EM Section Charge for channel %i",i+1);
		h_ZDCP_EM_Charge[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_EM_Charge[i]->SetFillColor(kBlue);
		//charge weighted time slice
		sprintf(title,"h_ZDCP_EMChan_%i_TSMean",i+1);
		sprintf(name,"ZDC EM Section TSMean for channel %i",i+1);
		h_ZDCP_EM_TSMean[i] = book1DHistogram(ZDCDir,title, name, 100, -0.5, 9.5);
		h_ZDCP_EM_TSMean[i]->SetFillColor(kBlue);
		//RecHit for EM sections
		sprintf(title,"h_ZDCP_EMChan_%i_RecHit_Energy",i+1);
		sprintf(name,"ZDC EM Section Rechit Energy for channel %i",i+1);
		h_ZDCP_EM_RecHitEnergy[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_EM_RecHitEnergy[i]->SetFillColor(kRed);

		sprintf(title,"h_ZDCP_EMChan_%i_RecHit_Timing",i+1);
		sprintf(name,"ZDC EM Section Rechit Timing for channel %i",i+1);
		h_ZDCP_EM_RecHitTiming[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_EM_RecHitTiming[i]->SetFillColor(kRed);

		// pulse Minus Side
		sprintf(title,"h_ZDCM_EMChan_%i_Pulse",i+1);
		sprintf(name,"ZDC EM Section Pulse for channel %i",i+1);
		h_ZDCM_EM_Pulse[i] = book1DHistogram(ZDCDir,title, name, 10, -0.5, 9.5);		
		h_ZDCM_EM_Pulse[i]->SetFillColor(7);
		//integrated charge over 10 time samples
		sprintf(title,"h_ZDCM_EMChan_%i_Charge",i+1);
		sprintf(name,"ZDC EM Section Charge for channel %i",i+1);
		h_ZDCM_EM_Charge[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_EM_Charge[i]->SetFillColor(7);
		//charge weighted time slice
		sprintf(title,"h_ZDCM_EMChan_%i_TSMean",i+1);
		sprintf(name,"ZDC EM Section TSMean for channel %i",i+1);
		h_ZDCM_EM_TSMean[i] = book1DHistogram(ZDCDir,title, name, 100, -0.5, 9.5);
		h_ZDCM_EM_TSMean[i]->SetFillColor(7);
		//RecHit for EM sections
		sprintf(title,"h_ZDCM_EMChan_%i_RecHit_Energy",i+1);
		sprintf(name,"ZDC EM Section Rechit Energy for channel %i",i+1);
		h_ZDCM_EM_RecHitEnergy[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_EM_RecHitEnergy[i]->SetFillColor(kRed);

		sprintf(title,"h_ZDCM_EMChan_%i_RecHit_Timing",i+1);
		sprintf(name,"ZDC EM Section Rechit Timing for channel %i",i+1);
		h_ZDCM_EM_RecHitTiming[i] = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_EM_RecHitTiming[i]->SetFillColor(kRed);
	}

	h_bunchXing = book1DHistogram(ZDCDir,"h_bunchXing", "BX distribution", 3565, -0.5, 3563.5);
	h_lumiBlock = book1DHistogram(ZDCDir,"h_lumiBlock", "Lumi Blocks", 300, -0.5, 295.5);
	h_bunchXing->SetFillColor(kBlue);
	h_lumiBlock->SetFillColor(kBlue);

	h_ZDCP_EM_RecHitEnergyAve = book1DHistogram(ZDCDir,"h_ZDCP_EM_RecHitEnergyAve", "ZDCP EM Section RecHitEnergyAve", 5, -0.5, 4.5);
	h_ZDCP_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(1,"+EM1");
	h_ZDCP_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(2,"+EM2");
	h_ZDCP_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(3,"+EM3");
	h_ZDCP_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(4,"+EM4");
	h_ZDCP_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(5,"+EM5");
	h_ZDCP_HAD_RecHitEnergyAve = book1DHistogram(ZDCDir,"h_ZDCP_HAD_RecHitEnergyAve", "ZDCP HAD Section RecHitEnergyAve", 4, -0.5, 3.5);
	h_ZDCP_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(1,"+HAD1");
	h_ZDCP_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(2,"+HAD2");
	h_ZDCP_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(3,"+HAD3");
	h_ZDCP_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(4,"+HAD4");

	h_ZDCP_EM_RecHitTimingAve = book1DHistogram(ZDCDir,"h_ZDCP_EM_RecHitTimingAve", "ZDCP EM Section RecHitTimingAve", 5, -0.5, 4.5);
	h_ZDCP_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(1,"+EM1");
	h_ZDCP_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(2,"+EM2");
	h_ZDCP_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(3,"+EM3");
	h_ZDCP_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(4,"+EM4");
	h_ZDCP_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(5,"+EM5");
	h_ZDCP_HAD_RecHitTimingAve = book1DHistogram(ZDCDir,"h_ZDCP_HAD_RecHitTimingAve", "ZDCP HAD Section RecHitTimingAve", 4, -0.5, 3.5);
	h_ZDCP_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(1,"+HAD1");
	h_ZDCP_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(2,"+HAD2");
	h_ZDCP_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(3,"+HAD3");
	h_ZDCP_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(4,"+HAD4");

	h_ZDCP_EMHAD_ChargeAve = book1DHistogram(ZDCDir,"h_ZDCP_EMHAD_ChargeAve", "ZDC Plus ChargeAve", 9, -0.5, 8.5);
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(1,"+EM1");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(2,"+EM2");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(3,"+EM3");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(4,"+EM4");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(5,"+EM5");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(6,"+HAD1");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(7,"+HAD2");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(8,"+HAD3");
	h_ZDCP_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(9,"+HAD4");

	//setZDClabels(h_ZDCP_EMHAD_TSMeanAve);
	h_ZDCP_EMHAD_TSMeanAve = book1DHistogram(ZDCDir,"h_ZDCP_EMHAD_TSMeanAve", "ZDC Plus Timing", 9, -0.5, 8.5);
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(1,"+EM1");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(2,"+EM2");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(3,"+EM3");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(4,"+EM4");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(5,"+EM5");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(6,"+HAD1");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(7,"+HAD2");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(8,"+HAD3");
	h_ZDCP_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(9,"+HAD4");

	h_ZDCP_EM_RecHitEnergyAve->Sumw2();
	h_ZDCP_HAD_RecHitEnergyAve->Sumw2();
	h_ZDCP_EM_RecHitTimingAve->Sumw2();
	h_ZDCP_HAD_RecHitTimingAve->Sumw2();
	h_ZDCP_EMHAD_ChargeAve->Sumw2();
	h_ZDCP_EMHAD_TSMeanAve->Sumw2();

	h_ZDCP_EM_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCP_HAD_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCP_EM_RecHitTimingAve->SetFillColor(kRed);
	h_ZDCP_HAD_RecHitTimingAve->SetFillColor(kRed);
	h_ZDCP_EMHAD_TSMeanAve->SetFillColor(kRed);


		sprintf(title,"h_ZDCP_EM_TotE");
		sprintf(name,"ZDC EM Section Total Rechit Energy (EM1+EM2+EM3+EM4+EM5), +ZDC");
		h_ZDCP_EM_TotE = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_EM_TotE->SetFillColor(kGreen);
		sprintf(title,"h_ZDCP_Had_TotE");
		sprintf(name,"ZDC Had Section Total Rechit Energy (Had1+Had2+Had3+Had4), +ZDC");
		h_ZDCP_Had_TotE = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_Had_TotE->SetFillColor(kGreen);
		sprintf(title,"h_ZDCP_TotE_HadronLike");
		sprintf(name,"ZDC Total Energy assuming Hadronic particle only (alpha*(EM1+EM2+EM3+EM4+EM5)+(Had1+Had2+Had3+Had4))*0.782828, +ZDC");
		h_ZDCP_TotE_HadronLike = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_TotE_HadronLike->SetFillColor(kGreen);	
		sprintf(title,"h_ZDCP_TotE");
		sprintf(name,"ZDC Total Rechit Energy (EM*0.025514, Had*0.782828), which assumes multiple particle types hit +ZDC");
		h_ZDCP_TotE = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCP_TotE->SetFillColor(kGreen);		
	h_ZDCP_EM_TotE->Sumw2();
	h_ZDCP_Had_TotE->Sumw2();
	h_ZDCP_TotE_HadronLike->Sumw2();
	h_ZDCP_TotE->Sumw2();		
	// Minus Side Histograms

	h_ZDCM_EM_RecHitEnergyAve = book1DHistogram(ZDCDir,"h_ZDCM_EM_RecHitEnergyAve", "ZDCM EM Section RecHitEnergyAve", 5, -0.5, 4.5);
	h_ZDCM_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(1,"-EM1");
	h_ZDCM_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(2,"-EM2");
	h_ZDCM_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(3,"-EM3");
	h_ZDCM_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(4,"-EM4");
	h_ZDCM_EM_RecHitEnergyAve->GetXaxis()->SetBinLabel(5,"-EM5");
	h_ZDCM_HAD_RecHitEnergyAve = book1DHistogram(ZDCDir,"h_ZDCM_HAD_RecHitEnergyAve", "ZDCM HAD Section RecHitEnergyAve", 4, -0.5, 3.5);
	h_ZDCM_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(1,"-HAD1");
	h_ZDCM_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(2,"-HAD2");
	h_ZDCM_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(3,"-HAD3");
	h_ZDCM_HAD_RecHitEnergyAve->GetXaxis()->SetBinLabel(4,"-HAD4");

	h_ZDCM_EM_RecHitTimingAve = book1DHistogram(ZDCDir,"h_ZDCM_EM_RecHitTimingAve", "ZDCM EM Section RecHitTimingAve", 5, -0.5, 4.5);
	h_ZDCM_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(1,"-EM1");
	h_ZDCM_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(2,"-EM2");
	h_ZDCM_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(3,"-EM3");
	h_ZDCM_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(4,"-EM4");
	h_ZDCM_EM_RecHitTimingAve->GetXaxis()->SetBinLabel(5,"-EM5");
	h_ZDCM_HAD_RecHitTimingAve = book1DHistogram(ZDCDir,"h_ZDCM_HAD_RecHitTimingAve", "ZDCM HAD Section RecHitTimingAve", 4, -0.5, 3.5);
	h_ZDCM_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(1,"-HAD1");
	h_ZDCM_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(2,"-HAD2");
	h_ZDCM_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(3,"-HAD3");
	h_ZDCM_HAD_RecHitTimingAve->GetXaxis()->SetBinLabel(4,"-HAD4");

	h_ZDCM_EMHAD_ChargeAve = book1DHistogram(ZDCDir,"h_ZDCM_EMHAD_ChargeAve", "ZDC Minus ChargeAve", 9, -0.5, 8.5);
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(1,"-EM1");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(2,"-EM2");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(3,"-EM3");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(4,"-EM4");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(5,"-EM5");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(6,"-HAD1");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(7,"-HAD2");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(8,"-HAD3");
	h_ZDCM_EMHAD_ChargeAve->GetXaxis()->SetBinLabel(9,"-HAD4");

	h_ZDCM_EMHAD_TSMeanAve = book1DHistogram(ZDCDir,"h_ZDCM_EMHAD_TSMeanAve", "ZDC Minus Timing", 9, -0.5, 8.5);
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(1,"-EM1");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(2,"-EM2");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(3,"-EM3");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(4,"-EM4");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(5,"-EM5");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(6,"-HAD1");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(7,"-HAD2");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(8,"-HAD3");
	h_ZDCM_EMHAD_TSMeanAve->GetXaxis()->SetBinLabel(9,"-HAD4");

	h_ZDCM_EM_RecHitEnergyAve->Sumw2();
	h_ZDCM_HAD_RecHitEnergyAve->Sumw2();
	h_ZDCM_EM_RecHitTimingAve->Sumw2();
	h_ZDCM_HAD_RecHitTimingAve->Sumw2();
	h_ZDCM_EMHAD_ChargeAve->Sumw2();
	h_ZDCM_EMHAD_TSMeanAve->Sumw2();
	h_ZDCM_EM_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCM_HAD_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCM_EM_RecHitTimingAve->SetFillColor(kRed);
	h_ZDCM_HAD_RecHitTimingAve->SetFillColor(kRed);
	h_ZDCM_EMHAD_TSMeanAve->SetFillColor(kRed);
	
		sprintf(title,"h_ZDCM_EM_TotE");
		sprintf(name,"ZDC EM Section Total Rechit Energy (EM1+EM2+EM3+EM4+EM5), -ZDC");
		h_ZDCM_EM_TotE = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_EM_TotE->SetFillColor(kGreen);
		sprintf(title,"h_ZDCM_Had_TotE");
		sprintf(name,"ZDC Had Section Total Rechit Energy (Had1+Had2+Had3+Had4), -ZDC");
		h_ZDCM_Had_TotE = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_Had_TotE->SetFillColor(kGreen);
		sprintf(title,"h_ZDCM_TotE_HadronLike");
		sprintf(name,"ZDC Total Energy assuming Hadronic particle only (alpha*(EM1+EM2+EM3+EM4+EM5)+(Had1+Had2+Had3+Had4))*0.782828, -ZDC");
		h_ZDCM_TotE_HadronLike = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_TotE_HadronLike->SetFillColor(kGreen);	
		sprintf(title,"h_ZDCM_TotE");
		sprintf(name,"ZDC Total Rechit Energy (EM*0.025514, Had*0.782828), which assumes multiple particle types hit -ZDC");
		h_ZDCM_TotE = book1DHistogram(ZDCDir,title, name, 100, -10., 1000.);		
		h_ZDCM_TotE->SetFillColor(kGreen);			
	h_ZDCM_EM_TotE->Sumw2();
	h_ZDCM_Had_TotE->Sumw2();
	h_ZDCM_TotE_HadronLike->Sumw2();
	h_ZDCM_TotE->Sumw2();

}

//exact copy from http://cmslxr.fnal.gov/lxr/source/DQM/HcalMonitorModule/src/HcalTimingMonitorModule.cc#100
bool SecMarZDCAnalyzer::isGood(double fData[10], double fCut, double fPercentage){
	bool dec = false;
	int n = 10;
	int ts_max=-1; //double max=-100;
	ts_max = getTSMax(fData,10);
	if(ts_max==0 || ts_max==(n-1)) return false;
	float sum = fData[ts_max-1] + fData[ts_max+1];
	//cout << "tsMax " << ts_max << " data[tsmax] " << mData[ts_max] << " sum " << sum << endl;
	if(fData[ts_max] > fCut && sum > (fData[ts_max]*fPercentage)) dec = true;
	return dec;
}


int SecMarZDCAnalyzer::getTSMax(double fData[10], int fTS)
{
	int ts_max = -100;
	double max = -9999.;
	for(int j = 0; j < fTS; ++j){
		if(max < fData[j]){
			max = fData[j];
			ts_max = j;
		}
	}
	return ts_max;
}
double SecMarZDCAnalyzer::getTime(double fData[10], double& fSum){
	int n = 10;
	int ts_max = getTSMax(fData,10);
	double Time=0,SumT=0;      //,MaxT=-10;
	if (ts_max>=0){
		Time = ts_max*fData[ts_max];
		SumT = fData[ts_max];
		if(ts_max>0){
			Time += (ts_max-1)*fData[ts_max-1];
			SumT += fData[ts_max-1];
		}
		if(ts_max<(n-1)){
			Time += (ts_max+1)*fData[ts_max+1];
			SumT += fData[ts_max+1];
		}
		Time=Time/SumT;
	}
	fSum = SumT;

	return Time;
}
TH1F *SecMarZDCAnalyzer::book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
int fNbins, double fXmin, double fXmax) const {
	char title[1024];
	sprintf(title, "%s [RUN:%i]", fTitle.c_str(), Runno);
	return fDir.make < TH1F > (fName.c_str(), title, fNbins, fXmin, fXmax);
}
TH2F *SecMarZDCAnalyzer::book2DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
int fNbinsX, double fXmin, double fXmax, int fNbinsY, double fYmin, double fYmax) const {
	char title[1024];
	sprintf(title, "%s [RUN:%i]", fTitle.c_str(), Runno);
	return fDir.make < TH2F > (fName.c_str(), title, fNbinsX, fXmin, fXmax, fNbinsY, fYmin, fYmax);
}

void SecMarZDCAnalyzer::endJob() {
	for(int i = 0; i < 5;++i){
		h_ZDCP_EMHAD_TSMeanAve->SetBinContent(i+1,h_ZDCP_EM_TSMean[i]->GetMean());
		h_ZDCP_EMHAD_TSMeanAve->SetBinError(i+1,h_ZDCP_EM_TSMean[i]->GetMeanError());
		h_ZDCP_EMHAD_ChargeAve->SetBinContent(i+1,h_ZDCP_EM_Charge[i]->GetMean());
		h_ZDCP_EMHAD_ChargeAve->SetBinError(i+1,h_ZDCP_EM_Charge[i]->GetMeanError());
		h_ZDCP_EM_RecHitEnergyAve->SetBinContent(i+1,h_ZDCP_EM_RecHitEnergy[i]->GetMean());
		h_ZDCP_EM_RecHitEnergyAve->SetBinError(i+1,h_ZDCP_EM_RecHitEnergy[i]->GetMeanError());
		h_ZDCP_EM_RecHitTimingAve->SetBinContent(i+1,h_ZDCP_EM_RecHitTiming[i]->GetMean());
		h_ZDCP_EM_RecHitTimingAve->SetBinError(i+1,h_ZDCP_EM_RecHitTiming[i]->GetMeanError());
		h_ZDCP_EM_Pulse[i]->Scale(10./h_ZDCP_EM_Pulse[i]->GetEntries());

		h_ZDCM_EMHAD_TSMeanAve->SetBinContent(i+1,h_ZDCM_EM_TSMean[i]->GetMean());
		h_ZDCM_EMHAD_TSMeanAve->SetBinError(i+1,h_ZDCM_EM_TSMean[i]->GetMeanError());
		h_ZDCM_EMHAD_ChargeAve->SetBinContent(i+1,h_ZDCM_EM_Charge[i]->GetMean());
		h_ZDCM_EMHAD_ChargeAve->SetBinError(i+1,h_ZDCM_EM_Charge[i]->GetMeanError());
		h_ZDCM_EM_RecHitEnergyAve->SetBinContent(i+1,h_ZDCM_EM_RecHitEnergy[i]->GetMean());
		h_ZDCM_EM_RecHitEnergyAve->SetBinError(i+1,h_ZDCM_EM_RecHitEnergy[i]->GetMeanError());
		h_ZDCM_EM_RecHitTimingAve->SetBinContent(i+1,h_ZDCM_EM_RecHitTiming[i]->GetMean());
		h_ZDCM_EM_RecHitTimingAve->SetBinError(i+1,h_ZDCM_EM_RecHitTiming[i]->GetMeanError());
		h_ZDCM_EM_Pulse[i]->Scale(10./h_ZDCM_EM_Pulse[i]->GetEntries());
	}
	for(int i = 0; i < 4;++i){
		h_ZDCP_EMHAD_TSMeanAve->SetBinContent(i+6,h_ZDCP_HAD_TSMean[i]->GetMean());
		h_ZDCP_EMHAD_TSMeanAve->SetBinError(i+6,h_ZDCP_HAD_TSMean[i]->GetMeanError());
		h_ZDCP_EMHAD_ChargeAve->SetBinContent(i+6,h_ZDCP_HAD_Charge[i]->GetMean());
		h_ZDCP_EMHAD_ChargeAve->SetBinError(i+6,h_ZDCP_HAD_Charge[i]->GetMeanError());
		h_ZDCP_HAD_RecHitEnergyAve->SetBinContent(i+1,h_ZDCP_HAD_RecHitEnergy[i]->GetMean());
		h_ZDCP_HAD_RecHitEnergyAve->SetBinError(i+1,h_ZDCP_HAD_RecHitEnergy[i]->GetMeanError());
		h_ZDCP_HAD_RecHitTimingAve->SetBinContent(i+1,h_ZDCP_HAD_RecHitTiming[i]->GetMean());
		h_ZDCP_HAD_RecHitTimingAve->SetBinError(i+1,h_ZDCP_HAD_RecHitTiming[i]->GetMeanError());
		h_ZDCP_HAD_Pulse[i]->Scale(10./h_ZDCP_HAD_Pulse[i]->GetEntries());

		h_ZDCM_EMHAD_TSMeanAve->SetBinContent(i+6,h_ZDCM_HAD_TSMean[i]->GetMean());
		h_ZDCM_EMHAD_TSMeanAve->SetBinError(i+6,h_ZDCM_HAD_TSMean[i]->GetMeanError());
		h_ZDCM_EMHAD_ChargeAve->SetBinContent(i+6,h_ZDCM_HAD_Charge[i]->GetMean());
		h_ZDCM_EMHAD_ChargeAve->SetBinError(i+6,h_ZDCM_HAD_Charge[i]->GetMeanError());
		h_ZDCM_HAD_RecHitEnergyAve->SetBinContent(i+1,h_ZDCM_HAD_RecHitEnergy[i]->GetMean());
		h_ZDCM_HAD_RecHitEnergyAve->SetBinError(i+1,h_ZDCM_HAD_RecHitEnergy[i]->GetMeanError());
		h_ZDCM_HAD_RecHitTimingAve->SetBinContent(i+1,h_ZDCM_HAD_RecHitTiming[i]->GetMean());
		h_ZDCM_HAD_RecHitTimingAve->SetBinError(i+1,h_ZDCM_HAD_RecHitTiming[i]->GetMeanError());
		h_ZDCM_HAD_Pulse[i]->Scale(10./h_ZDCM_HAD_Pulse[i]->GetEntries());
	}

	if (htmlPrint) htmlOutput();
	
}

void SecMarZDCAnalyzer::htmlOutput(void) 
{


	cout << "Preparing html output ..." << endl;

	char tmp[10];

	if (runBegin != -1)
		sprintf(tmp, "ZDCBeamSplash_R%09ld_L%ld_%ld", runBegin,lumibegin,lumiend);
	else
		sprintf(tmp, "ZDCBeamSplash_R%09d", 0);
	string htmlDir = baseHtmlDir_ + "/" + tmp + "/";

	system(("/bin/mkdir -p " + htmlDir).c_str());

	std::ofstream htmlFile;

	htmlFile.open((htmlDir + "index.html").c_str());

	// html page header
	htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
	htmlFile << "<html>  " << endl;
	htmlFile << "<head>  " << endl;
	htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
	htmlFile << " http-equiv=\"content-type\">  " << endl;
	htmlFile << "  <title>ZDC Beam Splash Analysis Output</title> " << endl;
	htmlFile << "</head>  " << endl;
	htmlFile << "<body>  " << endl;
	htmlFile << "<br>  " << endl;
	htmlFile << "<center><h1>ZDC Beam Splash Analysis Outputs</h1></center>" << endl;
	htmlFile << "<h2>Authors:&nbsp;(U.&nbsp;of&nbsp;Iowa)&nbsp;S.&nbsp;Sen,&nbsp;T.&nbsp;Yetkin</h2>" << endl;
	htmlFile << "<h2>Run Number:&nbsp;&nbsp;" << endl;
	htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

	htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
	htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

	htmlFile << "-" << endl; 
	htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

	htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
		startTime << "</span></h2> " << endl;
	htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;" << endl;
	htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2> " << endl;
	htmlFile << "<hr>" << endl;
	htmlFile << "<ul>" << endl;

	string htmlName;

	if (doZDCHTML) {
		htmlName = "ZDC.html";
		ZDCHTMLOutput(startTime, htmlDir, htmlName);
		htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
		htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">ZDC</a></td>" << endl;

	}
	htmlFile << "</tr></table>" << endl;
	htmlFile << "</ul>" << endl;

	// html page footer
	htmlFile << "</body> " << endl;
	htmlFile << "</html> " << endl;

	htmlFile.close();
	cout << "html output done..." << endl;
	return;
}

void SecMarZDCAnalyzer::ZDCHTMLOutput(string startTime, string htmlDir, string htmlName) {
	cout << "Preparing html output for " << htmlName << endl;

	ofstream htmlFile;

	htmlFile.open((htmlDir + htmlName).c_str());

	// html page header
	htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
	htmlFile << "<html>  " << endl;
	htmlFile << "<head>  " << endl;
	htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
	htmlFile << " http-equiv=\"content-type\">  " << endl;
	htmlFile << "  <title>ZDC Plots</title> " << endl;
	htmlFile << "</head>  " << endl;
	htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
	htmlFile << "<body>  " << endl;
	htmlFile << "<br>  " << endl;
	htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
	htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

	htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
	htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

	htmlFile << "-" << endl; 
	htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

	htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
		startTime << "</span></h2> " << endl;
	htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
	htmlFile << " style=\"color: rgb(0, 0, 153);\">ZDC</span></h2> " << endl;
	htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
	htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
	htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
	// begin table for histograms
	htmlFile << "<table width=100%  border=1><tr>" << endl;
	htmlFile << "</tr></table>" << endl;
	htmlFile << "<hr>" << endl;

	htmlFile << "<h2><strong>ZDC&nbsp;Histograms</strong></h2>" << endl;
	htmlFile << "<h3>" << endl;
	htmlFile << "<a href=\"#ZDC_Plots\">ZDC Plots </a></br>" << endl;
	htmlFile << "</h3>" << endl;
	htmlFile << "<hr>" << endl;

	
	string type = "ZDC";
	htmlFile << "<tr align=\"left\">" << endl;
	htmlFile << "<td>&nbsp;&nbsp;&nbsp;<a name=\"" << type << "_Plots\"><h3>" << type << "  Histograms</h3></td></tr>" << endl;

	htmlFile << "<tr align=\"left\">" << endl;
        histoHTML(h_bunchXing, "Bunch Xing", "Events", 92, htmlFile, htmlDir);
	histoHTML(h_lumiBlock, "Lumi Block", "Events", 92, htmlFile, htmlDir);
	histoHTML(h_ZDCP_EMHAD_ChargeAve, "ZDCP Channel id", "Ave. Charge (fC)", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EMHAD_TSMeanAve, "ZDCP Channel id", "TS_Mean", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitEnergyAve, "ZDCP EM Channel id", "Ave. Rechit Energy (GeV)", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitEnergyAve, "ZDCP HAD Channel id", "Ave. Rechit Energy (GeV)", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitTimingAve, "ZDCP EM Channel id", "Ave. Rechit Time", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitTimingAve, "ZDCP HAD Channel id", "Ave. Rechit Time", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Pulse[0], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Pulse[1], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Pulse[2], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Pulse[3], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Pulse[4], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Pulse[0], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Pulse[1], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Pulse[2], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Pulse[3], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Charge[0], "ZDCP EM Channel1 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Charge[1], "ZDCP EM Channel2 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Charge[2], "ZDCP EM Channel3 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Charge[3], "ZDCP EM Channel4 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_Charge[4], "ZDCP EM Channel5 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Charge[0], "ZDCP HAD Channel1 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Charge[1], "ZDCP HAD Channel2 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Charge[2], "ZDCP HAD Channel3 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_Charge[3], "ZDCP HAD Channel4 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitEnergy[0], "ZDCP EM Channel1 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitEnergy[1], "ZDCP EM Channel2 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitEnergy[2], "ZDCP EM Channel3 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitEnergy[3], "ZDCP EM Channel4 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitEnergy[4], "ZDCP EM Channel5 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitEnergy[0], "ZDCP HAD Channel1 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitEnergy[1], "ZDCP HAD Channel2 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitEnergy[2], "ZDCP HAD Channel3 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitEnergy[3], "ZDCP HAD Channel4 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitTiming[0], "ZDCP EM Channel1 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitTiming[1], "ZDCP EM Channel2 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitTiming[2], "ZDCP EM Channel3 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitTiming[3], "ZDCP EM Channel4 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_RecHitTiming[4], "ZDCP EM Channel5 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitTiming[0], "ZDCP HAD Channel1 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitTiming[1], "ZDCP HAD Channel2 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitTiming[2], "ZDCP HAD Channel3 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_RecHitTiming[3], "ZDCP HAD Channel4 RecHit Time", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_TSMean[0], "ZDCP EM Channel1 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_TSMean[1], "ZDCP EM Channel2 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_TSMean[2], "ZDCP EM Channel3 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_TSMean[3], "ZDCP EM Channel4 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_EM_TSMean[4], "ZDCP EM Channel5 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_TSMean[0], "ZDCP HAD Channel1 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_TSMean[1], "ZDCP HAD Channel2 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_TSMean[2], "ZDCP HAD Channel3 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCP_HAD_TSMean[3], "ZDCP HAD Channel4 TS Mean", "Events", 92, htmlFile, htmlDir,false);	

	histoHTML(h_ZDCM_EMHAD_ChargeAve, "ZDCM Channel id", "Ave. Charge (fC)", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EMHAD_TSMeanAve, "ZDCM Channel id", "TS_Mean", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitEnergyAve, "ZDCM EM Channel id", "Ave. Rechit Energy (GeV)", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitEnergyAve, "ZDCM HAD Channel id", "Ave. Rechit Energy (GeV)", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitTimingAve, "ZDCM EM Channel id", "Ave. Rechit Time", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitTimingAve, "ZDCM HAD Channel id", "Ave. Rechit Time", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Pulse[0], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Pulse[1], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Pulse[2], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Pulse[3], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Pulse[4], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Pulse[0], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Pulse[1], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Pulse[2], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Pulse[3], "Time Slice id", "Ave. Pulse Height", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Charge[0], "ZDCM EM Channel1 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Charge[1], "ZDCM EM Channel2 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Charge[2], "ZDCM EM Channel3 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Charge[3], "ZDCM EM Channel4 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_Charge[4], "ZDCM EM Channel5 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Charge[0], "ZDCM HAD Channel1 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Charge[1], "ZDCM HAD Channel2 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Charge[2], "ZDCM HAD Channel3 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_Charge[3], "ZDCM HAD Channel4 Charge (fC)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitEnergy[0], "ZDCM EM Channel1 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitEnergy[1], "ZDCM EM Channel2 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitEnergy[2], "ZDCM EM Channel3 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitEnergy[3], "ZDCM EM Channel4 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitEnergy[4], "ZDCM EM Channel5 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitEnergy[0], "ZDCM HAD Channel1 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitEnergy[1], "ZDCM HAD Channel2 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitEnergy[2], "ZDCM HAD Channel3 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitEnergy[3], "ZDCM HAD Channel4 RecHitEnergy (GeV)", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitTiming[0], "ZDCM EM Channel1 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitTiming[1], "ZDCM EM Channel2 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitTiming[2], "ZDCM EM Channel3 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitTiming[3], "ZDCM EM Channel4 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_RecHitTiming[4], "ZDCM EM Channel5 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitTiming[0], "ZDCM HAD Channel1 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitTiming[1], "ZDCM HAD Channel2 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitTiming[2], "ZDCM HAD Channel3 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_RecHitTiming[3], "ZDCM HAD Channel4 RecHitTime", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_TSMean[0], "ZDCM EM Channel1 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_TSMean[1], "ZDCM EM Channel2 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_TSMean[2], "ZDCM EM Channel3 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_TSMean[3], "ZDCM EM Channel4 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_EM_TSMean[4], "ZDCM EM Channel5 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_TSMean[0], "ZDCM HAD Channel1 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_TSMean[1], "ZDCM HAD Channel2 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_TSMean[2], "ZDCM HAD Channel3 TS Mean", "Events", 92, htmlFile, htmlDir,false);
	histoHTML(h_ZDCM_HAD_TSMean[3], "ZDCM HAD Channel4 TS Mean", "Events", 92, htmlFile, htmlDir,false);	
	htmlFile << "<hr>" << endl;
	histoHTML(h_ZDCP_EM_TotE, "ZDC EM Section Total Energy (GeV), +ZDC", "Events", 92, htmlFile, htmlDir,false);		
	histoHTML(h_ZDCP_Had_TotE, "ZDC Had Section Total Energy (GeV), +ZDC", "Events", 92, htmlFile, htmlDir,false);		
	histoHTML(h_ZDCP_TotE_HadronLike, "ZDC Total Energy Hadronic particle (GeV), +ZDC", "Events", 92, htmlFile, htmlDir,false);		
	histoHTML(h_ZDCP_TotE, "ZDC Total EM+Had Rechit Energy(GeV), +ZDC", "Events", 92, htmlFile, htmlDir,false);		
	histoHTML(h_ZDCM_EM_TotE, "ZDC EM Section Total Energy (GeV), -ZDC", "Events", 92, htmlFile, htmlDir,false);		
	histoHTML(h_ZDCM_Had_TotE, "ZDC Had Section Total Energy (GeV), -ZDC", "Events", 92, htmlFile, htmlDir,false);		
	histoHTML(h_ZDCM_TotE_HadronLike, "ZDC Total Energy Hadronic particle (GeV), -ZDC", "Events", 92, htmlFile, htmlDir,false);		
	histoHTML(h_ZDCM_TotE, "ZDC Total EM+Had Rechit Energy(GeV), -ZDC", "Events", 92, htmlFile, htmlDir,false);		
	
	htmlFile << "</tr>" << endl;
	htmlFile << "</table>" << endl;
	// end table
	htmlFile << "<br>" << endl;

	// html page footer
	htmlFile << "</body> " << endl;
	htmlFile << "</html> " << endl;
	htmlFile.close();
}
void SecMarZDCAnalyzer::histoHTML(TH1F * hist, const char *xlab, const char *ylab, int width, ofstream & htmlFile,
  string htmlDir, bool log) {

    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG(hist, 1, htmlDir, xlab, ylab,log);
        string imgName = "";

        imgName = getIMG(hist, 2, htmlDir, xlab, ylab, log);

        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}

void SecMarZDCAnalyzer::histoHTML2(TH2F * hist, const char *xlab, const char *ylab, int width,
  ofstream & htmlFile, string htmlDir, bool color, bool log) {
    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG2(hist, 1, htmlDir, xlab, ylab, color,log);
        string imgName = "";

        imgName = getIMG2(hist, 2, htmlDir, xlab, ylab, color,log);
        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}



string SecMarZDCAnalyzer::getIMG(TH1F * hist, int size, string htmlDir, const char *xlab, const char *ylab, bool log) {

    if (hist == NULL) {
        printf("getIMG:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetName();

    cleanString(name);
    char dest[512];

    if (runBegin > -1)
      sprintf(dest, "%s - Run %ld LS %ld-%ld", name.c_str(), runBegin, lumibegin, lumiend);
    else
        sprintf(dest, "%s", name.c_str());
    // set run generically hist->SetTitle(dest); 
    string title = dest;

    int xwid = 900;
    int ywid = 540;

    if (size == 1) {
        title = title + "_tmb";
        xwid = 600;
        ywid = 360;
    }
    TCanvas *can = new TCanvas(dest, dest, xwid, ywid);
    if(log){
        can->SetLogy();  
    }

    parseString(title);
    string outName = title + ".gif";
    string saveName = htmlDir + outName;

    hist->SetXTitle(xlab);
    hist->SetYTitle(ylab);
    //    if(name.find("h_max",0)!=string::npos){
    //    hist->Fit("landau");
    // }
    hist->Draw();

    can->SaveAs(saveName.c_str());
    delete can;

    return outName;
}

string SecMarZDCAnalyzer::getIMG2(TH2F * hist, int size, string htmlDir, const char *xlab, const char *ylab,
  bool color, bool log) {

    if (hist == NULL) {
        printf("getIMG2:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetName();

    cleanString(name);
    char dest[512];

    if (runBegin > -1)
      sprintf(dest, "%s - Run %ld LS %ld-%ld", name.c_str(), runBegin, lumibegin, lumiend);
    else
        sprintf(dest, "%s", name.c_str());
    // set run generically    hist->SetTitle(dest);
    string title = dest;

    int xwid = 900;
    int ywid = 540;

    if (size == 1) {
        title = title + "_tmb";
        xwid = 600;
        ywid = 360;
    }
    TCanvas *can = new TCanvas(dest, dest, xwid, ywid);

    parseString(title);
    string outName = title + ".gif";
    string saveName = htmlDir + outName;

    hist->SetXTitle(xlab);
    hist->SetYTitle(ylab);
    if (!color)
        hist->Draw();
    else {
        hist->SetStats(false);
        hist->Draw("COLZ");
    }
    can->SaveAs(saveName.c_str());
    delete can;

    return outName;
}
void SecMarZDCAnalyzer::cleanString(string & title) {

    for (unsigned int i = 0; i < title.size(); i++) {
        if (title.substr(i, 6) == " - Run") {
            title.replace(i, title.size() - i, "");
        }
        if (title.substr(i, 4) == "_Run") {
            title.replace(i, title.size() - i, "");
        }
        if (title.substr(i, 5) == "__Run") {
            title.replace(i, title.size() - i, "");
        }
    }
}

void SecMarZDCAnalyzer::parseString(string & title) {

    for (unsigned int i = 0; i < title.size(); i++) {
        if (title.substr(i, 1) == " ") {
            title.replace(i, 1, "_");
        }
        if (title.substr(i, 1) == "#") {
            title.replace(i, 1, "N");
        }
        if (title.substr(i, 1) == "-") {
            title.replace(i, 1, "_");
        }
        if (title.substr(i, 1) == "&") {
            title.replace(i, 1, "_and_");
        }
        if (title.substr(i, 1) == "(" || title.substr(i, 1) == ")") {
            title.replace(i, 1, "_");
        }
    }

    return;
}

