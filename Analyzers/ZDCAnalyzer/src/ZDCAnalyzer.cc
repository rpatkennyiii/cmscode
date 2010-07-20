#include "Analyzers/ZDCAnalyzer/interface/ZDCAnalyzer.h"

//static const float HFQIEConst = 4.0;
static const float HFQIEConst = 2.6;
static const float EMGain  = 0.025514;
static const float HADGain = 0.782828;

using namespace edm;
using namespace std;

ZDCAnalyzer::ZDCAnalyzer(const ParameterSet& iConfig)
{
	runBegin = -1;
	evtNo = 0;
	lumibegin = 0;
	lumiend = 0;
	startTime = "Not Avaliable";
}


ZDCAnalyzer::~ZDCAnalyzer(){}


void ZDCAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
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

	BeamData[0]=iEvent.bunchCrossing();
	BeamData[1]=lumi;

	BeamTree->Fill();

	Handle<ZDCDigiCollection> zdc_digi_h;
	Handle<ZDCRecHitCollection> zdc_recHits_h;
	iEvent.getByType(zdc_digi_h);
	iEvent.getByType(zdc_recHits_h);
	const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;
	const ZDCRecHitCollection *zdc_recHits = zdc_recHits_h.failedToGet()? 0 : &*zdc_recHits_h;

	if(zdc_digi){
		for(int i=0; i<180; i++){DigiData[i]=0;}

		for (ZDCDigiCollection::const_iterator j=zdc_digi->begin();j!=zdc_digi->end();j++){
			const ZDCDataFrame digi = (const ZDCDataFrame)(*j);		
			int iSide      = digi.id().zside();
			int iSection   = digi.id().section();
			int iChannel   = digi.id().channel();
			int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

			int fTS = digi.size();
			for (int i = 0; i < fTS; ++i) {
				DigiData[i+chid*10] = HFQIEConst*digi[i].nominal_fC();
			}
		}
		
		ZDCDigiTree->Fill();
	}

	if(zdc_recHits){

		for(int i=0; i<36; i++){RecData[i]=0;}

		for (ZDCRecHitCollection::const_iterator zhit=zdc_recHits->begin();zhit!=zdc_recHits->end();zhit++){		
			int iSide      = (zhit->id()).zside();
			int iSection   = (zhit->id()).section();
			int iChannel   = (zhit->id()).channel();
			int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

			RecData[chid]=zhit->energy();
			RecData[chid+18]=zhit->time();
		}
		
		ZDCRecoTree->Fill();
	}
}


void ZDCAnalyzer::beginJob(){
	mFileServer->cd();
		
	string bnames[] = {"negEM1","negEM2","negEM3","negEM4","negEM5",
		     	   "negHD1","negHD2","negHD3","negHD4",
			   "posEM1","posEM2","posEM3","posEM4","posEM5",
			   "posHD1","posHD2","posHD3","posHD4"};
	BranchNames=bnames;
   	ZDCDigiTree = new TTree("ZDCDigiTree","ZDC Digi Tree");
   	ZDCRecoTree = new TTree("ZDCRecTree","ZDC Rec Tree");
	BeamTree = new TTree("BeamTree","Beam Tree");

	BeamTree->Branch("BunchXing",&BeamData[0],"BunchXing/F");
	BeamTree->Branch("LumiBlock",&BeamData[1],"LumiBlock/F");

	for(int i=0; i<18; i++){
		ZDCDigiTree->Branch(bnames[i].c_str(),&DigiData[i*10],(bnames[i]+"tsz[10]/D").c_str());
		ZDCRecoTree->Branch((bnames[i]+"energy").c_str(),&RecData[i],(bnames[i]+"energy/D").c_str());
		ZDCRecoTree->Branch((bnames[i]+"timing").c_str(),&RecData[i+18],(bnames[i]+"timing/D").c_str());
	}	
}


void ZDCAnalyzer::endJob() {}
