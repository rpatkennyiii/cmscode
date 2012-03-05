#include "Analyzers/L1BitAnalyzer/interface/L1BitAnalyzer.h"

using namespace std;

L1BitAnalyzer::L1BitAnalyzer(edm::ParameterSet const& conf):l1GtRR_(conf.getParameter<edm::InputTag>("l1GtRR")){
   mFileServer->file().SetCompressionLevel(9);
   mFileServer->file().cd();

   L1BitTree = new TTree("L1BitTree","L1BitTree");
   firstEv=true;

   L1BitTree->Branch("BunchXing",&RunData[0],"BunchXing/i");
   L1BitTree->Branch("LumiBlock",&RunData[1],"LumiBlock/i");
   L1BitTree->Branch("Event",&RunData[2],"Event/i");
   L1BitTree->Branch("Run",&RunData[3],"Run/i");
}

L1BitAnalyzer::~L1BitAnalyzer(){}

void L1BitAnalyzer::beginRun(const edm::Run& e, const edm::EventSetup& iSetup){}

void L1BitAnalyzer::analyze(edm::Event const& e, edm::EventSetup const& iSetup){
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
       const DecisionWord&  dWord = l1GtRR->decisionWord();
       const TechnicalTriggerWord&  ttdWord = l1GtRR->technicalTriggerWord();

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
       for (CItAlgo l1Trig = menu->gtTechnicalTriggerMap().begin(); l1Trig != menu->gtTechnicalTriggerMap().end(); ++l1Trig) {
	  int itrig = (l1Trig->second).algoBitNumber();
	  string trigName = string( (l1Trig->second).algoName() );
	  if(firstEv){
	     L1BitTree->Branch(trigName.c_str(),&L1Bits[nBits],(trigName+"/O").c_str());
	  }
	  L1Bits[nBits] = ttdWord.at(itrig);
	  nBits++;
       }
       firstEv=false;
   }
   L1BitTree->Fill();
}

void L1BitAnalyzer::endJob(){}
