#include "Analyzers/ZDCAnalyzer/interface/ZDCAnalyzer.h"

static const float HFQIEConst = 4.0;
static const float EMGain  = 0.025514;
static const float HADGain = 0.782828;

using namespace edm;
using namespace std;

ZDCAnalyzer::ZDCAnalyzer(const edm::ParameterSet& iConfig){
	runBegin = -1;
	evtNo = 0;
	lumibegin = 0;
	lumiend = 0;
	startTime = "Not Avaliable";
}


ZDCAnalyzer::~ZDCAnalyzer(){}


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
		Runno=iEvent.id().run();
	}

	if (lumi < lumibegin)lumibegin = lumi;
	if (lumi > lumiend)lumiend = lumi;

	h_lumiBlock->Fill(iEvent.luminosityBlock());
	h_bunchXing->Fill(iEvent.bunchCrossing());

	edm::Handle<ZDCDigiCollection> zdc_digi_h;
	iEvent.getByType(zdc_digi_h);
	const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;

	if(zdc_digi){
		for(int i=0; i<180; i++){data[i]=0;}

		for (ZDCDigiCollection::const_iterator j=zdc_digi->begin();j!=zdc_digi->end(); ++j) {
			const ZDCDataFrame digi = (const ZDCDataFrame)(*j);		
			int iSide      = digi.id().zside();
			int iSection   = digi.id().section();
			int iChannel   = digi.id().channel();
			int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

			int fTS = digi.size();
			for (int i = 0; i < fTS; ++i) {
				data[i+chid*10] = HFQIEConst*digi[i].nominal_fC();
			}
		}
		
		ZDCTree->Fill();
	}
}

void ZDCAnalyzer::beginJob(){
	TFileDirectory ZDCDir = mFileServer->mkdir("ZDC");
	string bnames[] = {"EM1","EM2","EM3","EM4","EM5",
		     	   "HM1","HM2","HM3","HM4",
			   "EP1","EP2","EP3","EP4","EP5",
			   "HP1","HP2","HP3","HP4"};
	BranchNames=bnames;
   	ZDCTree = new TTree("ZDCTree","ZDC Tree");

	for(int i=0; i<18; i++){ZDCTree->Branch(bnames[i].c_str(),&data[i*10],(bnames[i]+"tsz[10]/D").c_str());}
//	for(int i=0; i<18; i++){ZDCTree->Branch(bnames[i].c_str(),&data[i*10],"ts1/D:ts2:ts3:ts4:ts5:ts6:ts7:ts8:ts9:ts10");}
 	
	h_bunchXing = book1DHistogram(ZDCDir,"h_bunchXing", "BX distribution", 3565, -0.5, 3563.5);
	h_lumiBlock = book1DHistogram(ZDCDir,"h_lumiBlock", "Lumi Blocks", 300, -0.5, 295.5);
	h_bunchXing->SetFillColor(kBlue);
	h_lumiBlock->SetFillColor(kBlue);
}

//exact copy from http://cmslxr.fnal.gov/lxr/source/DQM/HcalMonitorModule/src/HcalTimingMonitorModule.cc#100


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


void ZDCAnalyzer::endJob() {}

