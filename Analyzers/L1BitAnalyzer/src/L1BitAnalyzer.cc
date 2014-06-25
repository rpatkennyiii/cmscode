#include "Analyzers/L1BitAnalyzer/interface/L1BitAnalyzer.h"
#include <iostream>

using namespace std;

L1BitAnalyzer::L1BitAnalyzer(edm::ParameterSet const& conf):l1GtRR_(conf.getParameter<edm::InputTag>("l1GtRR")),
   hltresults_(conf.getParameter<edm::InputTag>("hltresults")),treeName(conf.getParameter<string>("treeName"))
{
   mFileServer->file().SetCompressionLevel(9);
   mFileServer->file().cd();

   L1BitTree = new TTree(treeName.c_str(),treeName.c_str());
   firstEv=true;

   L1BitTree->Branch("BunchXing",&RunData[0],"BunchXing/i");
   L1BitTree->Branch("LumiBlock",&RunData[1],"LumiBlock/i");
   L1BitTree->Branch("Event",&RunData[2],"Event/i");
   L1BitTree->Branch("Run",&RunData[3],"Run/i");
   L1BitTree->Branch("Time",&RunData[4],"Time/i");
}

L1BitAnalyzer::~L1BitAnalyzer(){}

void L1BitAnalyzer::beginRun(const edm::Run& e, const edm::EventSetup& iSetup){
   hltConfig_.init(e,iSetup,hltresults_.process(),firstEv);
   firstEv=true;
}

void L1BitAnalyzer::analyze(edm::Event const& e, edm::EventSetup const& iSetup){
   RunData[0]=e.bunchCrossing();
   RunData[1]=e.id().luminosityBlock();
   RunData[2]=e.id().event();
   RunData[3]=e.id().run();
   RunData[4]=e.time().unixTime();

   m_l1GtUtils.retrieveL1EventSetup(iSetup);
   edm::ESHandle<L1GtTriggerMenu> menuRcd;
   iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
   const L1GtTriggerMenu* menu = menuRcd.product();

   int iErrorCode = -1;
   L1GtUtils::TriggerCategory trigCategory = L1GtUtils::AlgorithmTrigger;
   const int pfSetIndexAlgorithmTrigger = m_l1GtUtils.prescaleFactorSetIndex(
             e, trigCategory, iErrorCode);

   edm::Handle<L1GlobalTriggerReadoutRecord> l1GtRR; 
   edm::Handle<edm::TriggerResults> hltresults;

   e.getByLabel(l1GtRR_, l1GtRR);
   e.getByLabel(hltresults_, hltresults);

   nBits=0;

   if (l1GtRR.isValid()) {
       const DecisionWord&  dWord = l1GtRR->decisionWord();
       const TechnicalTriggerWord&  ttdWord = l1GtRR->technicalTriggerWord();

       for (CItAlgo l1Trig = menu->gtAlgorithmAliasMap().begin(); l1Trig != menu->gtAlgorithmAliasMap().end(); ++l1Trig) {
	  int itrig = (l1Trig->second).algoBitNumber();
	  TString trigName = string( (l1Trig->second).algoName() );
	  if(firstEv){
	     L1BitTree->Branch(trigName,&L1Bits[nBits],(trigName+"/O"));
	     L1BitTree->Branch((trigName+"_Prescl"),&L1BitsPs[nBits],(trigName+"_Prescl/i"));
	  }
	  L1Bits[nBits] = dWord.at(itrig);
	  L1BitsPs[nBits] = m_l1GtUtils.prescaleFactor(e,trigName.Data(),iErrorCode);
	  if(L1BitTree->GetBranch(trigName)) L1BitTree->SetBranchAddress(trigName,&L1Bits[nBits]);
	  if(L1BitTree->GetBranch(trigName)) L1BitTree->SetBranchAddress(trigName+"_Prescl",&L1BitsPs[nBits]);
	  nBits++;
       }
       for (CItAlgo l1Trig = menu->gtTechnicalTriggerMap().begin(); l1Trig != menu->gtTechnicalTriggerMap().end(); ++l1Trig) {
	  int itrig = (l1Trig->second).algoBitNumber();
	  TString trigName = string( (l1Trig->second).algoName() );
	  if(firstEv){
	     L1BitTree->Branch(trigName,&L1Bits[nBits],(trigName+"/O"));
	     L1BitTree->Branch((trigName+"_Prescl"),&L1BitsPs[nBits],(trigName+"_Prescl/i"));
	  }
	  L1Bits[nBits] = ttdWord.at(itrig);
	  L1BitsPs[nBits] = m_l1GtUtils.prescaleFactor(e,trigName.Data(),iErrorCode);
	  if(L1BitTree->GetBranch(trigName)) L1BitTree->SetBranchAddress(trigName,&L1Bits[nBits]);
	  if(L1BitTree->GetBranch(trigName)) L1BitTree->SetBranchAddress(trigName+"_Prescl",&L1BitsPs[nBits]);
	  nBits++;
       }
   }

   if(hltresults.isValid()){
	edm::TriggerNames const& triggerNames = e.triggerNames(*hltresults);
        for (unsigned int itrig = 0; itrig != hltresults->size(); ++itrig) {
	      L1Bits[nBits]=0;
              TString trigName = triggerNames.triggerName(itrig);
	      if (firstEv){
                 L1BitTree->Branch(trigName,&L1Bits[nBits],trigName+"/O");
	         L1BitTree->Branch(trigName+"_Prescl",&L1BitsPs[nBits],trigName+"_Prescl/i");
              }
	      L1Bits[nBits] = hltresults->accept(itrig);
	      L1BitsPs[nBits] = hltConfig_.prescaleValue(e, iSetup, trigName.Data());
	      if(L1BitTree->GetBranch(trigName)) L1BitTree->SetBranchAddress(trigName,&L1Bits[nBits]);
	      if(L1BitTree->GetBranch(trigName)) L1BitTree->SetBranchAddress(trigName+"_Prescl",&L1BitsPs[nBits]);
	      nBits++;
         }
   }
   
   firstEv=false;
   L1BitTree->Fill();
}

void L1BitAnalyzer::endJob(){}
