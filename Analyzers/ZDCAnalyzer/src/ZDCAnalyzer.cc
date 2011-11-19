#include "Analyzers/ZDCAnalyzer/interface/ZDCAnalyzer.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include <iostream>

//static const float HFQIEConst = 4.0;
//static const float HFQIEConst = 2.6;
//static const float EMGain  = 0.025514;
//static const float HADGain = 0.782828;

using namespace edm;
using namespace std;

ZDCAnalyzer::ZDCAnalyzer(const ParameterSet& iConfig) :
l1GtRR_(iConfig.getParameter<edm::InputTag>("l1GtRR"))
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
   using namespace edm;
	++evtNo;
	time_t a = (iEvent.time().value()) >> 32; // store event info
	event = iEvent.id().event();
	run = iEvent.id().run();
	lumi = iEvent.luminosityBlock();

     centrality_ = new CentralityProvider(iSetup);  
      centrality_->newEvent(iEvent,iSetup); // make sure you do this first in every event
	if (runBegin < 0) {         // parameters for the first event
		startTime = ctime(&a);
		lumibegin = lumiend = lumi;
		runBegin = iEvent.id().run();
		Runno=iEvent.id().run();
	}
	if (lumi < lumibegin)lumibegin = lumi;
	if (lumi > lumiend)lumiend = lumi;

    SumHF=0.;

      bin = centrality_->getBin();

    hiNpix = centrality_->raw()->multiplicityPixel();
    hiNpixelTracks = centrality_->raw()->NpixelTracks();
    hiNtracks = centrality_->raw()->Ntracks();
    hiNtracksPtCut = centrality_->raw()->NtracksPtCut();
    hiNtracksEtaCut = centrality_->raw()->NtracksEtaCut();
    hiNtracksEtaPtCut = centrality_->raw()->NtracksEtaPtCut();

    hiHF = centrality_->raw()->EtHFtowerSum();   
    hiHFplus = centrality_->raw()->EtHFtowerSumPlus();
    hiHFminus = centrality_->raw()->EtHFtowerSumMinus();
    hiHFhit = centrality_->raw()->EtHFhitSum();
    hiHFhitPlus = centrality_->raw()->EtHFhitSumPlus();
    hiHFhitMinus = centrality_->raw()->EtHFhitSumMinus();

    hiZDC = centrality_->raw()->zdcSum();
    hiZDCplus = centrality_->raw()->zdcSumPlus();
    hiZDCminus = centrality_->raw()->zdcSumMinus();
   
	BeamData[0]=iEvent.bunchCrossing();
	BeamData[1]=lumi;
	BeamData[2]=run;
	BeamData[3]=event;
    BeamData[4]=bin;
    SumHF=hiHF;
    
	BeamTree->Fill();
// from UserCode/pkenny/Analyzers/L1BitAnalyzer
   RunData[0]=iEvent.bunchCrossing();
   RunData[1]=iEvent.id().luminosityBlock();
   RunData[2]=iEvent.id().event();
   RunData[3]=iEvent.id().run();

   m_l1GtUtils.retrieveL1EventSetup(iSetup);
   edm::ESHandle<L1GtTriggerMenu> menuRcd;
   iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
   const L1GtTriggerMenu* menu = menuRcd.product();
    edm::Handle<L1GlobalTriggerReadoutRecord> l1GtRR; 
   iEvent.getByLabel(l1GtRR_, l1GtRR);

   if (l1GtRR.isValid()) {
       const DecisionWord&  dWord = l1GtRR->decisionWord();
       nBits=0;
       for (CItAlgo l1Trig = menu->gtAlgorithmAliasMap().begin(); l1Trig != menu->gtAlgorithmAliasMap().end(); ++l1Trig) {
	  int itrig = (l1Trig->second).algoBitNumber();
	  string trigName = string( (l1Trig->second).algoName() );
	  if(firstEv){
	     L1BitTree->Branch(trigName.c_str(),&L1Bits[nBits],(trigName+"/O").c_str());
	  }
	  L1Bits[nBits] = dWord.at(itrig);
	  nBits++;
       }
       firstEv=false;
   }
   L1BitTree->Fill();

//
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

		for(int i=0; i<36; i++){RecData[i]=0.; }
		for(int i=0; i<18; i++){RecDataLowGain[i]=0.; }

		for (ZDCRecHitCollection::const_iterator zhit=zdc_recHits->begin();zhit!=zdc_recHits->end();zhit++){		
			int iSide      = (zhit->id()).zside();
			int iSection   = (zhit->id()).section();
			int iChannel   = (zhit->id()).channel();
			int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

			RecData[chid]=zhit->energy();
			RecDataLowGain[chid]=zhit->lowGainEnergy();
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
	BeamTree->Branch("CentralityBin",&BeamData[4],"CentralityBin/I");
	BeamTree->Branch("CentralityHF",&SumHF,"CentralityHF/D");
   BeamTree->Branch("bin",&bin,"bin/I");
   BeamTree->Branch("hiHF",&hiHF,"hiHF/F");
   BeamTree->Branch("hiHFplus",&hiHFplus,"hiHFplus/F");
   BeamTree->Branch("hiHFminus",&hiHFminus,"hiHFminus/F");
   BeamTree->Branch("hiZDC",&hiZDC,"hiZDC/F");
   BeamTree->Branch("hiZDCplus",&hiZDCplus,"hiZDCplus/F");
   BeamTree->Branch("hiZDCminus",&hiZDCminus,"hiZDCminus/F");
   
   BeamTree->Branch("hiHFhit",&hiHFhit,"hiHFhit/F");
   BeamTree->Branch("hiHFhitPlus",&hiHFhitPlus,"hiHFhitPlus/F");
   BeamTree->Branch("hiHFhitMinus",&hiHFhitMinus,"hiHFhitMinus/F");
   
   BeamTree->Branch("hiET",&hiET,"hiET/F");
   BeamTree->Branch("hiEE",&hiEE,"hiEE/F");
   BeamTree->Branch("hiEB",&hiEB,"hiEB/F");
   BeamTree->Branch("hiEEplus",&hiEEplus,"hiEEplus/F");
   BeamTree->Branch("hiEEminus",&hiEEminus,"hiEEminus/F");
   BeamTree->Branch("hiNpix",&hiNpix,"hiNpix/I");
   BeamTree->Branch("hiNpixelTracks",&hiNpixelTracks,"hiNpixelTracks/I");
   BeamTree->Branch("hiNtracks",&hiNtracks,"hiNtracks/I");
   BeamTree->Branch("hiNtracksPtCut",&hiNtracksPtCut,"hiNtracksPtCut/I");
   BeamTree->Branch("hiNtracksEtaCut",&hiNtracksEtaCut,"hiNtracksEtaCut/I");
   BeamTree->Branch("hiNtracksEtaPtCut",&hiNtracksEtaPtCut,"hiNtracksEtaPtCut/I");

	for(int i=0; i<18; i++){
		ZDCDigiTree->Branch((bnames[i]+"fC").c_str(),&DigiDatafC[i*10],(bnames[i]+"cFtsz[10]/F").c_str());
		ZDCDigiTree->Branch((bnames[i]+"ADC").c_str(),&DigiDataADC[i*10],(bnames[i]+"ADCtsz[10]/I").c_str());
//		ZDCDigiTree->Branch("CentralityBin",&BeamData[4],"CentralityBin/I");
//		ZDCDigiTree->Branch("CentralityHF",&SumHF,"CentralityHF/D");
		ZDCRecoTree->Branch((bnames[i]+"energy").c_str(),&RecData[i],(bnames[i]+"energy/F").c_str());
		ZDCRecoTree->Branch((bnames[i]+"lowGainEnergy").c_str(),&RecDataLowGain[i],(bnames[i]+"lowGainEnergy/F").c_str());
		ZDCRecoTree->Branch((bnames[i]+"timing").c_str(),&RecData[i+18],(bnames[i]+"timing/F").c_str());
//		ZDCRecoTree->Branch("CentralityBin",&BeamData[4],"CentralityBin/I");
//		ZDCRecoTree->Branch("CentralityHF",&SumHF,"CentralityHF/D");		
	}	
   L1BitTree = new TTree("L1BitTree","L1BitTree");
   firstEv=true;	
   L1BitTree->Branch("BunchXing",&RunData[0],"BunchXing/i");
   L1BitTree->Branch("LumiBlock",&RunData[1],"LumiBlock/i");
   L1BitTree->Branch("Event",&RunData[2],"Event/i");
   L1BitTree->Branch("Run",&RunData[3],"Run/i");	
}
