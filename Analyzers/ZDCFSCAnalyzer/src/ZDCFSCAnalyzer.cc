#include "Analyzers/ZDCFSCAnalyzer/interface/ZDCFSCAnalyzer.h"
#include <iostream>

//static const float HFQIEConst = 4.0;
//static const float HFQIEConst = 2.6;
//static const float EMGain  = 0.025514;
//static const float HADGain = 0.782828;

using namespace edm;
using namespace std;

ZDCFSCAnalyzer::ZDCFSCAnalyzer(const ParameterSet& iConfig)
{
	runBegin = -1;
	evtNo = 0;
	lumibegin = 0;
	lumiend = 0;
	startTime = "Not Avaliable";
}


ZDCFSCAnalyzer::~ZDCFSCAnalyzer(){}


void ZDCFSCAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
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
	ESHandle<HcalDbService> conditions;
	iEvent.getByType(zdc_digi_h);
	iEvent.getByType(zdc_recHits_h);
	const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;
	const ZDCRecHitCollection *zdc_recHits = zdc_recHits_h.failedToGet()? 0 : &*zdc_recHits_h;

	iSetup.get<HcalDbRecord>().get(conditions);

	if(zdc_digi){
		for(int i=0; i<340; i++){DigiDatafC[i]=0;DigiDataADC[i]=0;}

		for (ZDCDigiCollection::const_iterator j=zdc_digi->begin();j!=zdc_digi->end();j++){

			const ZDCDataFrame digi = (const ZDCDataFrame)(*j);		
			int iSide      = 1+2*(12-digi.elecId().spigot());
			int iSection   = digi.id().section();
			int iChannel   = digi.id().channel();
			bool isFsc=(digi[0].fiber()<2||digi[0].fiberChan()<6);

			int fscCh = isFsc?(9+(iSide+1)/2*17+(digi[0].fiber()-4)*3+digi[0].fiberChan()):0;
			int chid = iSection?((iSection-1)*5+(iSide+1)/2*17+(iChannel-1)):fscCh;
			
			const HcalQIEShape* qieshape=conditions->getHcalShape();
			const HcalQIECoder* qiecoder=iSection?conditions->getHcalCoder(digi.id()):0;
			CaloSamples caldigi;

			if(iSection){ 
				HcalCoderDb coder(*qiecoder,*qieshape);
				coder.adc2fC(digi,caldigi);
			}
	
			int fTS = digi.size();
			
			if(iSection||isFsc){
			   for (int i = 0; i < fTS; ++i) {
				   DigiDatafC[i+chid*10] = iSection?caldigi[i]:digi[i].nominal_fC();
				   DigiDataADC[i+chid*10] = digi[i].adc();
			   }
			}
		}
		ZDCDigiTree->Fill();
		FSCDigiTree->Fill();
	}

}


void ZDCFSCAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
		
	string bnames[] = {"negEM1","negEM2","negEM3","negEM4","negEM5",
		     	   "negHD1","negHD2","negHD3","negHD4",
			   "negFSC1","negFSC2","negFSC3","negFSC4",
			   "negFSC5","negFSC6","negFSC7","negFSC8",
			   "posEM1","posEM2","posEM3","posEM4","posEM5",
			   "posHD1","posHD2","posHD3","posHD4",
			   "posFSC1","posFSC2","posFSC3","posFSC4",
			   "posFSC5","posFSC6","posFSC7","posFSC8"};
	BranchNames=bnames;
   	ZDCDigiTree = new TTree("ZDCDigiTree","ZDC Digi Tree");
   	FSCDigiTree = new TTree("FSCDigiTree","ZDC Digi Tree");
	BeamTree = new TTree("BeamTree","Beam Tree");

	BeamTree->Branch("BunchXing",&BeamData[0],"BunchXing/I");
	BeamTree->Branch("LumiBlock",&BeamData[1],"LumiBlock/I");

	for(int i=0; i<34; i++){
		if(i<9||(i>16&&i<26)){
		   ZDCDigiTree->Branch((bnames[i]+"fC").c_str(),&DigiDatafC[i*10],(bnames[i]+"cFtsz[10]/F").c_str());
		   ZDCDigiTree->Branch((bnames[i]+"ADC").c_str(),&DigiDataADC[i*10],(bnames[i]+"ADCtsz[10]/I").c_str());
		}else{
		   FSCDigiTree->Branch((bnames[i]+"fC").c_str(),&DigiDatafC[i*10],(bnames[i]+"cFtsz[10]/F").c_str());
		   FSCDigiTree->Branch((bnames[i]+"ADC").c_str(),&DigiDataADC[i*10],(bnames[i]+"ADCtsz[10]/I").c_str());
		}
	}	
}
