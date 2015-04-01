#include "Analyzers/FSCAnalyzer/interface/FSCAnalyzer.h"
#include <iostream>

using namespace std;

FSCAnalyzer::FSCAnalyzer(edm::ParameterSet const& conf):l1GtRR_(conf.getParameter<edm::InputTag>("l1GtRR")){
   mFileServer->file().SetCompressionLevel(9);
   mFileServer->file().cd();

   FSCTree = new TTree("FSCTree","FSCTree");

   FSCTree->Branch("nBits",&nBits,"nBits/i");
   FSCTree->Branch("ZeroBiasBit",&ZeroBiasBit,"ZeroBiasBit/O");
   FSCTree->Branch("BunchXing",&RunData[0],"BunchXing/i");
   FSCTree->Branch("LumiBlock",&RunData[1],"LumiBlock/i");
   FSCTree->Branch("Event",&RunData[2],"Event/i");
   FSCTree->Branch("Run",&RunData[3],"Run/i");

   FSCNames[0]="L1Tech_FSC_St1Sect45_upp.v0";
   FSCNames[1]="L1Tech_FSC_St1Sect45_down.v0";
   FSCNames[2]="L1Tech_FSC_St1Sect56_upp.v0";
   FSCNames[3]="L1Tech_FSC_St1Sect56_down.v0";
   FSCNames[4]="L1Tech_FSC_St2Sect45_upp.v0";
   FSCNames[5]="L1Tech_FSC_St2Sect45_down.v0";
   FSCNames[6]="L1Tech_FSC_St2Sect56_upp.v0";
   FSCNames[7]="L1Tech_FSC_St2Sect56_down.v0";
   FSCNames[8]="L1Tech_FSC_St3Sect45_uppLeft.v0";
   FSCNames[9]="L1Tech_FSC_St3Sect45_uppRight.v0";
   FSCNames[10]="L1Tech_FSC_St3Sect45_downRight.v0";
   FSCNames[11]="L1Tech_FSC_St3Sect45_downLeft.v0";
   FSCNames[12]="L1Tech_FSC_St3Sect56_uppRight.v0";
   FSCNames[13]="L1Tech_FSC_St3Sect56_downRight.v0";
   FSCNames[14]="L1Tech_FSC_St3Sect56_downLeft.v0";
   FSCNames[15]="L1Tech_FSC_St3Sect56_uppLeft.v0";

   for(int i=0;i<16;i++){FSCTree->Branch(FSCNames[i],&FSCBits[i],FSCNames[i]+"/O");}
}

FSCAnalyzer::~FSCAnalyzer(){}

void FSCAnalyzer::beginRun(const edm::Run& , const edm::EventSetup& ){}


void FSCAnalyzer::analyze(edm::Event const& e, edm::EventSetup const& iSetup){
   RunData[0]=e.bunchCrossing();
   RunData[1]=e.id().luminosityBlock();
   RunData[2]=e.id().event();
   RunData[3]=e.id().run();

   m_l1GtUtils.retrieveL1EventSetup(iSetup);
   edm::ESHandle<L1GtTriggerMenu> menuRcd;
   iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
   const L1GtTriggerMenu* menu = menuRcd.product();


   edm::Handle<L1GlobalTriggerReadoutRecord> l1GtRR; 
   e.getByLabel(l1GtRR_, l1GtRR);


   if (l1GtRR.isValid()) {

       nBits=0;
       const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = l1GtRR->technicalTriggerWord();
       const L1GtPsbWord psb = l1GtRR->gtPsbWord(0xbb0f, 0);
       unsigned int temp (( (psb.bData(1)) >> 8 ) & 0xff);

       for(int i=0;i<16; i++){
	   FSCBits[15-i]=false;
	   if(i>=8){FSCBits[15-i]=bool(temp/int(pow(2,15-i)));nBits++;} 
           temp%=int(pow(2,15-i));
       }	

       ZeroBiasBit=0;
       if(technicalTriggerWordBeforeMask.size()>0){
	  for (CItAlgo techTrig = menu->gtTechnicalTriggerMap().begin(); techTrig != menu->gtTechnicalTriggerMap().end(); ++techTrig) {
	     int itrig = (techTrig->second).algoBitNumber();
	     TString trigName = TString( (techTrig->second).algoName() );
	     
	     if("L1Tech_BPTX_plus_AND_minus.v0"==trigName){
		nBits++;
		ZeroBiasBit=bool(technicalTriggerWordBeforeMask.at(itrig));
	     }


	     for(int fsctrig=8; fsctrig < 16; fsctrig++){

		if(FSCNames[fsctrig]==trigName){
		   nBits++;
		   FSCBits[fsctrig] = bool(technicalTriggerWordBeforeMask.at(itrig));
		}else if(itrig == 0){
		   FSCBits[fsctrig]=false;
		}
	     }
	  }
       }
   }
   if(nBits>0){FSCTree->Fill();}
}


void FSCAnalyzer::endJob(){}
