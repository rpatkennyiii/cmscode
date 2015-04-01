#include "Analyzers/ZDCAnalyzer/interface/ZDCAnalyzer.h"

//static const float HFQIEConst = 4.0;
//static const float HFQIEConst = 2.6;
//static const float EMGain  = 0.025514;
//static const float HADGain = 0.782828;

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
	BeamData[0]=iEvent.bunchCrossing();
	BeamData[1]=iEvent.id().luminosityBlock();
	BeamData[2]=iEvent.run();
	BeamData[3]=iEvent.id().event();

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
		for(int i=0; i<180; i++){DigiDatafC[i]=0;DigiDataADC[i]=0;}

		for (ZDCDigiCollection::const_iterator j=zdc_digi->begin();j!=zdc_digi->end();j++){
			const ZDCDataFrame digi = (const ZDCDataFrame)(*j);		
			int iSide      = digi.id().zside();
			int iSection   = digi.id().section();
			int iChannel   = digi.id().channel();
			int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

			const HcalQIEShape* qieshape=conditions->getHcalShape();
			const HcalQIECoder* qiecoder=conditions->getHcalCoder(digi.id());
			CaloSamples caldigi;
			HcalCoderDb coder(*qiecoder,*qieshape);
			
		        coder.adc2fC(digi,caldigi);
	
			int fTS = digi.size();
			for (int i = 0; i < fTS; ++i) {
				DigiDatafC[i+chid*10] = caldigi[i];
				DigiDataADC[i+chid*10] = digi[i].adc();
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
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
		
	string bnames[] = {"negEM1","negEM2","negEM3","negEM4","negEM5",
		     	   "negHD1","negHD2","negHD3","negHD4",
			   "posEM1","posEM2","posEM3","posEM4","posEM5",
			   "posHD1","posHD2","posHD3","posHD4"};
	BranchNames=bnames;
   	ZDCDigiTree = new TTree("ZDCDigiTree","ZDC Digi Tree");
   	ZDCRecoTree = new TTree("ZDCRecTree","ZDC Rec Tree");
	BeamTree = new TTree("BeamTree","Beam Tree");

	BeamTree->Branch("BunchXing",&BeamData[0],"BunchXing/I");
	BeamTree->Branch("LumiBlock",&BeamData[1],"LumiBlock/I");
	BeamTree->Branch("Run",&BeamData[2],"Run/I");
	BeamTree->Branch("Event",&BeamData[3],"Event/I");

	for(int i=0; i<18; i++){
		ZDCDigiTree->Branch((bnames[i]+"fC").c_str(),&DigiDatafC[i*10],(bnames[i]+"cFtsz[10]/F").c_str());
		ZDCDigiTree->Branch((bnames[i]+"ADC").c_str(),&DigiDataADC[i*10],(bnames[i]+"ADCtsz[10]/I").c_str());
		ZDCRecoTree->Branch((bnames[i]+"energy").c_str(),&RecData[i],(bnames[i]+"energy/F").c_str());
		ZDCRecoTree->Branch((bnames[i]+"timing").c_str(),&RecData[i+18],(bnames[i]+"timing/F").c_str());
	}	
}
