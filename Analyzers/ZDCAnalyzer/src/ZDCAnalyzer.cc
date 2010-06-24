#include "Analyzers/ZDCAnalyzer/interface/ZDCAnalyzer.h"

static const float HFQIEConst = 2.6;
static const float EMGain  = 0.025514;
static const float HADGain = 0.782828;

using namespace edm;
using namespace std;

ZDCAnalyzer::ZDCAnalyzer(const edm::ParameterSet& iConfig){
        Runno = iConfig.getUntrackedParameter < int >("RunNo", 1);

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


ZDCAnalyzer::~ZDCAnalyzer(){
}


void ZDCAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	++evtNo;
	time_t a = (iEvent.time().value()) >> 32; // store event info
	event = iEvent.id().event();
	run = iEvent.id().run();
	lumi = iEvent.luminosityBlock();

	if (runBegin < 0) {         // parameters for the first event
		startTime = ctime(&a);
		lumibegin = lumiend = lumi;
		runBegin = iEvent.id().run();
	}

	if (lumi < lumibegin)lumibegin = lumi;
	if (lumi > lumiend)lumiend = lumi;

	h_lumiBlock->Fill(iEvent.luminosityBlock());
	h_bunchXing->Fill(iEvent.bunchCrossing());

	edm::Handle<ZDCDigiCollection> zdc_digi_h;
	iEvent.getByType(zdc_digi_h);
	const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;

	if(zdc_digi){
		double QEM = 0.;
		double QEMR = 0.;
		double QHAD = 0.;
		double QHAD1 = 0.;
		double QHAD2 = 0.;
		double QHADR = 0.;
		
		for(int i=0; i<180; i++){data[i]=0;}

		for (ZDCDigiCollection::const_iterator j=zdc_digi->begin();j!=zdc_digi->end(); ++j) {
			const ZDCDataFrame digi = (const ZDCDataFrame)(*j);		
			int iSide      = digi.id().zside();
			int iSection   = digi.id().section();
			int iChannel   = digi.id().channel();
			int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

			double fSum = 0.;		
			double fData[10] = {-999.};
			int fTS = digi.size();
			for (int i = 0; i < fTS; ++i) {
				fData[i] = HFQIEConst*digi[i].nominal_fC();
				data[i+chid*10] = fData[i];
			}
			        
			double fTSMean = getTime(fData, fSum);

			if(iSection==1){
                            if (iSide ==1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCP_EM_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCP_EM_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCP_EM_TSMean[iChannel-1]->Fill(fTSMean);
			    h_ZDCP_EM_RecHitEnergy[iChannel-1]->Fill(fSum*EMGain);
			    QEM += fSum;
			    QEMR += fSum*EMGain;
			    }
                            if (iSide == -1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCM_EM_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCM_EM_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCM_EM_TSMean[iChannel-1]->Fill(fTSMean);
			    h_ZDCM_EM_RecHitEnergy[iChannel-1]->Fill(fSum*EMGain);
			    }
			}
			if(iSection==2){
                            if (iSide ==1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCP_HAD_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCP_HAD_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCP_HAD_TSMean[iChannel-1]->Fill(fTSMean);
			    h_ZDCP_HAD_RecHitEnergy[iChannel-1]->Fill(fSum*HADGain);
			    QHAD += fSum;
			    if (iChannel == 1 || iChannel ==2) QHAD1 += fSum;
			    if (iChannel == 3 || iChannel ==4) QHAD2 += fSum;
			    QHADR += fSum*HADGain;
			    }
                            if (iSide == -1 ){ 
			    for(int i = 0; i < fTS; ++i){h_ZDCM_HAD_Pulse[iChannel-1]->Fill(i,fData[i]);}
			    h_ZDCM_HAD_Charge[iChannel-1]->Fill(fSum);
			    h_ZDCM_HAD_TSMean[iChannel-1]->Fill(fTSMean);
			    h_ZDCM_HAD_RecHitEnergy[iChannel-1]->Fill(fSum*HADGain);
			    }
			}				
		}
		
		ZDCTree->Fill();
		h_ZDCP_Charge_Correlation->Fill(QEM, QHAD);
		h_ZDCP_Charge_CorrelationHAD->Fill(QHAD1, QHAD2);
		h_ZDCP_Charge_Correlation2->Fill(QEM/5, QHAD/4);
		h_ZDCP_RecHit_Correlation->Fill(QEMR/5, QHADR/4);
	}
}

void ZDCAnalyzer::beginJob(){
	TFileDirectory ZDCDir = mFileServer->mkdir("ZDC");
	string bnames[] = {"EM1","EM1","EM1","EM1","EM1",
		     	   "HM1","HM1","HM1","HM1",
			   "EP1","EP1","EP1","EP1","EP1",
			   "HP1","HP1","HP1","HP1"};
	BranchNames=bnames;
   	ZDCTree = new TTree("ZDCT","ZDC Tree");

//	for(int i=0; i<18; i++){ZDCTree->Branch(bnames[i].c_str(),&data[i*10],"tsz[10]/D");}
	for(int i=0; i<18; i++){ZDCTree->Branch(bnames[i].c_str(),&data[i*10],"ts1/D:ts2:ts3:ts4:ts5:ts6:ts7:ts8:ts9:ts10");}
 	
	char title[128];
	char name[128];
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

	h_ZDCP_Charge_Correlation = book2DHistogram(ZDCDir,"h_ZDCP_Charge_Correlation","ZDCP Charge Correlation",200,-10.,2000.,200,-10.,2000.);
	h_ZDCP_Charge_Correlation2 = book2DHistogram(ZDCDir,"h_ZDCP_Charge_Correlation2","ZDCP Charge Correlation2",100,-10.,1000.,100,-10.,1000.);
	h_ZDCP_RecHit_Correlation = book2DHistogram(ZDCDir,"h_ZDCP_RecHit_Correlation","ZDCP RecHit Correlation",600,0.,600.,100,0.,1000.);
	h_ZDCP_Charge_CorrelationHAD = book2DHistogram(ZDCDir,"h_ZDCP_Charge_CorrelationHAD","ZDCP Charge CorrelationHAD",100,-10.,1000.,100,-10.,1000.);

	h_ZDCP_EM_RecHitEnergyAve->Sumw2();
	h_ZDCP_HAD_RecHitEnergyAve->Sumw2();
	h_ZDCP_EMHAD_ChargeAve->Sumw2();
	h_ZDCP_EMHAD_TSMeanAve->Sumw2();

	h_ZDCP_EM_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCP_HAD_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCP_EMHAD_TSMeanAve->SetFillColor(kRed);
	h_ZDCP_Charge_Correlation->SetMarkerColor(kBlue);
	h_ZDCP_Charge_Correlation->SetMarkerStyle(kFullTriangleUp);
	h_ZDCP_Charge_Correlation2->SetMarkerColor(kRed);
	h_ZDCP_Charge_Correlation2->SetMarkerStyle(kFullTriangleUp);
	h_ZDCP_RecHit_Correlation->SetMarkerColor(kRed);
	h_ZDCP_RecHit_Correlation->SetMarkerStyle(kFullTriangleDown);
	h_ZDCP_Charge_CorrelationHAD->SetMarkerColor(kRed);
	h_ZDCP_Charge_CorrelationHAD->SetMarkerStyle(kFullTriangleUp);

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
	h_ZDCM_EMHAD_ChargeAve->Sumw2();
	h_ZDCM_EMHAD_TSMeanAve->Sumw2();
	h_ZDCM_EM_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCM_HAD_RecHitEnergyAve->SetFillColor(kRed);
	h_ZDCM_EMHAD_TSMeanAve->SetFillColor(kRed);
}

//exact copy from http://cmslxr.fnal.gov/lxr/source/DQM/HcalMonitorModule/src/HcalTimingMonitorModule.cc#100
bool ZDCAnalyzer::isGood(double fData[10], double fCut, double fPercentage){
	bool dec = false;
	int n = 10;
	int ts_max=-1;
	ts_max = getTSMax(fData,10);
	if(ts_max==0 || ts_max==(n-1)) return false;
	float sum = fData[ts_max-1] + fData[ts_max+1];
	//cout << "tsMax " << ts_max << " data[tsmax] " << mData[ts_max] << " sum " << sum << endl;
	if(fData[ts_max] > fCut && sum > (fData[ts_max]*fPercentage)) dec = true;
	return dec;
}


int ZDCAnalyzer::getTSMax(double fData[10], int fTS)
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


double ZDCAnalyzer::getTime(double fData[10], double& fSum){
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


TH1F *ZDCAnalyzer::book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
int fNbins, double fXmin, double fXmax) const {
	char title[1024];
	sprintf(title, "%s [RUN:%i]", fTitle.c_str(), Runno);
	return fDir.make < TH1F > (fName.c_str(), title, fNbins, fXmin, fXmax);
}


TH2F *ZDCAnalyzer::book2DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
int fNbinsX, double fXmin, double fXmax, int fNbinsY, double fYmin, double fYmax) const {
	char title[1024];
	sprintf(title, "%s [RUN:%i]", fTitle.c_str(), Runno);
	return fDir.make < TH2F > (fName.c_str(), title, fNbinsX, fXmin, fXmax, fNbinsY, fYmin, fYmax);
}


void ZDCAnalyzer::endJob() {
	for(int i = 0; i < 5;++i){
		h_ZDCP_EMHAD_TSMeanAve->SetBinContent(i+1,h_ZDCP_EM_TSMean[i]->GetMean());
		h_ZDCP_EMHAD_TSMeanAve->SetBinError(i+1,h_ZDCP_EM_TSMean[i]->GetMeanError());
		h_ZDCP_EMHAD_ChargeAve->SetBinContent(i+1,h_ZDCP_EM_Charge[i]->GetMean());
		h_ZDCP_EMHAD_ChargeAve->SetBinError(i+1,h_ZDCP_EM_Charge[i]->GetMeanError());
		h_ZDCP_EM_RecHitEnergyAve->SetBinContent(i+1,h_ZDCP_EM_RecHitEnergy[i]->GetMean());
		h_ZDCP_EM_RecHitEnergyAve->SetBinError(i+1,h_ZDCP_EM_RecHitEnergy[i]->GetMeanError());
		h_ZDCP_EM_Pulse[i]->Scale(10./h_ZDCP_EM_Pulse[i]->GetEntries());

		h_ZDCM_EMHAD_TSMeanAve->SetBinContent(i+1,h_ZDCM_EM_TSMean[i]->GetMean());
		h_ZDCM_EMHAD_TSMeanAve->SetBinError(i+1,h_ZDCM_EM_TSMean[i]->GetMeanError());
		h_ZDCM_EMHAD_ChargeAve->SetBinContent(i+1,h_ZDCM_EM_Charge[i]->GetMean());
		h_ZDCM_EMHAD_ChargeAve->SetBinError(i+1,h_ZDCM_EM_Charge[i]->GetMeanError());
		h_ZDCM_EM_RecHitEnergyAve->SetBinContent(i+1,h_ZDCM_EM_RecHitEnergy[i]->GetMean());
		h_ZDCM_EM_RecHitEnergyAve->SetBinError(i+1,h_ZDCM_EM_RecHitEnergy[i]->GetMeanError());
		h_ZDCM_EM_Pulse[i]->Scale(10./h_ZDCM_EM_Pulse[i]->GetEntries());
	}
	for(int i = 0; i < 4;++i){
		h_ZDCP_EMHAD_TSMeanAve->SetBinContent(i+6,h_ZDCP_HAD_TSMean[i]->GetMean());
		h_ZDCP_EMHAD_TSMeanAve->SetBinError(i+6,h_ZDCP_HAD_TSMean[i]->GetMeanError());
		h_ZDCP_EMHAD_ChargeAve->SetBinContent(i+6,h_ZDCP_HAD_Charge[i]->GetMean());
		h_ZDCP_EMHAD_ChargeAve->SetBinError(i+6,h_ZDCP_HAD_Charge[i]->GetMeanError());
		h_ZDCP_HAD_RecHitEnergyAve->SetBinContent(i+1,h_ZDCP_HAD_RecHitEnergy[i]->GetMean());
		h_ZDCP_HAD_RecHitEnergyAve->SetBinError(i+1,h_ZDCP_HAD_RecHitEnergy[i]->GetMeanError());
		h_ZDCP_HAD_Pulse[i]->Scale(10./h_ZDCP_HAD_Pulse[i]->GetEntries());

		h_ZDCM_EMHAD_TSMeanAve->SetBinContent(i+6,h_ZDCM_HAD_TSMean[i]->GetMean());
		h_ZDCM_EMHAD_TSMeanAve->SetBinError(i+6,h_ZDCM_HAD_TSMean[i]->GetMeanError());
		h_ZDCM_EMHAD_ChargeAve->SetBinContent(i+6,h_ZDCM_HAD_Charge[i]->GetMean());
		h_ZDCM_EMHAD_ChargeAve->SetBinError(i+6,h_ZDCM_HAD_Charge[i]->GetMeanError());
		h_ZDCM_HAD_RecHitEnergyAve->SetBinContent(i+1,h_ZDCM_HAD_RecHitEnergy[i]->GetMean());
		h_ZDCM_HAD_RecHitEnergyAve->SetBinError(i+1,h_ZDCM_HAD_RecHitEnergy[i]->GetMeanError());
		h_ZDCM_HAD_Pulse[i]->Scale(10./h_ZDCM_HAD_Pulse[i]->GetEntries());
	}
}
