#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TEntryListArray.h"
#include "TEventList.h"
#include "TCut.h"
#include <iostream>
#include <vector>

using namespace std;

void setEventList( TTree *upc, TCut cut, TString newListName);
void setEntryListArray( TTree *upc, TCut cut, TString newListName);

void upcGetAccXEffList( float minRap=1.8, float maxRap=2.4, float minMass=2.6, float maxMass=3.5, TString fileName="data/pPb2013/MC/jpsi_total.root", bool mc=true, TString ppb=""){
  bool pp=1,  rec=1,  hlt=0,  both=1,  oldmu=false;
  TCut dZCutSoftMu1PP="abs(muon1_dz-offlinePrimaryVerticesTree.z[0])<20.";
  TCut dZCutSoftMu2PP="abs(muon2_dz-offlinePrimaryVerticesTree.z[0])<20.";
  TCut dBCutSoftMu1="muon1_dB<0.3";
  TCut dBCutSoftMu2="muon2_dB<0.3";

  vector<TCut> eCuts;
  vector<TCut> eArrayCuts;

  TCut pbpCut="Run>=211313&&Run<=211631";
  TCut ppbCut="Run>=210498&&Run<=211256";
  TCut configCut="";

  bool doConCut=false;
  if(ppb=="pPb"){
    configCut=ppbCut;
    doConCut=true;
  }
  if(ppb="Pbp"){
    configCut=pbpCut;
    doConCut=true;
  }

  TCut genCut=Form("Rap_dimu>%f&&Rap_dimu<%f",minRap,maxRap);
  TCut diMuMassCut=Form("cand_mass>%f&&cand_mass<%f",minMass,maxMass);
  TCut diMuRapCut=Form("cand_rapidity>%f&&cand_rapidity<%f",minRap,maxRap);
  TCut goodCut="muon1_isGoodMuon&&muon2_isGoodMuon";
  TCut trackerMuCut="muon1_Tracker&&muon2_Tracker";
  TCut pixLayCut="muon1_pixelLayersWithMeasurement>0&&muon2_pixelLayersWithMeasurement>0";
  TCut trxLayCut="muon1_trackerLayersWithMeasurement>5&&muon2_trackerLayersWithMeasurement>5";
  TCut hiPureCut="muon1_isHighPurity&&muon2_isHighPurity";
  TCut promptCut=dZCutSoftMu1PP&&dZCutSoftMu2PP&&dBCutSoftMu1&&dBCutSoftMu2;
  TCut hfVetoCut="((MaxHFEta>0 && MaxHFEnergy<2.0)||(MaxHFEta<0 && MaxHFEnergy<1.5))";
  TCut singleMuPtCut="muon1_pt>.2&&muon2_pt>.2";
  TCut diMuPtCut="cand_pt<1.5";
  TCut l1TrigCut="L1_SingleMuOpen";
  TCut l1MatchCut="muon1_l1Quality>0||muon2_l1Quality>0";
  TCut hltTrigCut="HLT_PAUpcSingleMuOpenFull_TrackVeto7_v1";
  TCut hltMatchCut="muon1_pass||muon2_pass";

  if(mc) eCuts.push_back(genCut);
  if(doConCut) eCuts.push_back(configCut);
  eCuts.push_back(hfVetoCut);
  if(mc) eCuts.push_back(l1TrigCut);
  if(mc) eCuts.push_back(hltTrigCut);

  eArrayCuts.push_back(goodCut);
  eArrayCuts.push_back(trackerMuCut);
  eArrayCuts.push_back(pixLayCut);
  eArrayCuts.push_back(trxLayCut);
  eArrayCuts.push_back(hiPureCut);
  eArrayCuts.push_back(promptCut);
  eArrayCuts.push_back(singleMuPtCut);
  eArrayCuts.push_back(diMuPtCut);
  eArrayCuts.push_back(diMuRapCut);
  eArrayCuts.push_back(diMuMassCut);
  eArrayCuts.push_back(l1MatchCut);
  eArrayCuts.push_back(hltMatchCut);

  TFile *_fileData = TFile::Open(fileName);
 	TTree *vert=(TTree *)_fileData->Get(pp?"offlinePrimaryVerticesTree":"hiSelectedVertexTree");
	TTree *muon=(TTree *)_fileData->Get(oldmu?"MuonTree":"upcPatMuonsWithTriggerTree");
	TTree *l1=(TTree *)_fileData->Get(hlt?"hltbitanalysis/HltTree":"L1BitTree");
	TTree *cs=(TTree *)_fileData->Get("ClusterShapeTree");
	TTree *calo=(TTree *)_fileData->Get("CaloTree");
	TTree *maxcalo=(TTree *)_fileData->Get("MaxCaloTree");
	TTree *zdc=(TTree *)_fileData->Get("ZDCDigiTree");
	TTree *track=(TTree *)_fileData->Get(pp?"generalTracksTree":"hiSelectedTracksTree");
	TTree *upc=(TTree *)_fileData->Get(both?"onia2MuMuPatDiTrksTree":"UPCPatCandidateTree");
	TTree *pix=(TTree *)_fileData->Get(false?"pixelTracksTree":"hiPixelTracksTree");
	TTree *cent=(TTree *)_fileData->Get("PixelHitsCentralityTree");
	TTree *gen=(TTree *)_fileData->Get("genTree");
	TTree *rechbhe=(TTree *)_fileData->Get(rec?"HBHERecHitTree":"");
	TTree *rechf=(TTree *)_fileData->Get(rec?"HFRecHitTree":"");
	TTree *receb=(TTree *)_fileData->Get(rec?"EBRecHitTree":"");
	TTree *recee=(TTree *)_fileData->Get(rec?"EERecHitTree":"");
  TTree *castor=(TTree *)_fileData->Get("CastorRecTree");

	upc->AddFriend(l1);
	upc->AddFriend(calo);
	upc->AddFriend(maxcalo);
	upc->AddFriend(cs);
	upc->AddFriend(vert);
	upc->AddFriend(track);
	upc->AddFriend(muon);
	upc->AddFriend(pix);
	upc->AddFriend(cent);
	upc->AddFriend(zdc);
	upc->AddFriend(gen);
	upc->AddFriend(rechbhe);
	upc->AddFriend(rechf);
	upc->AddFriend(receb);
	upc->AddFriend(recee);
  upc->AddFriend(castor);
	
  int dimuIndex=4;

  if(mc){
    upc->SetAlias("E_dimu",Form("sqrt(genTree.mass[%i]**2+genTree.px[%i]**2+genTree.py[%i]**2+genTree.pz[%i]**2)",dimuIndex,dimuIndex,dimuIndex,dimuIndex));
    upc->SetAlias("Pz_dimu",Form("genTree.pz[%i]",dimuIndex));
    upc->SetAlias("Px_dimu",Form("genTree.px[%i]",dimuIndex));
    upc->SetAlias("Py_dimu",Form("genTree.py[%i]",dimuIndex));
    upc->SetAlias("Pt_dimu","sqrt(Px_dimu**2+Py_dimu**2)");
    upc->SetAlias("Rap_dimu","log((E_dimu+Pz_dimu)/(E_dimu-Pz_dimu))/2");
  }

  for(unsigned int i=0; i<eCuts.size(); i++){
    eCuts[i].SetName(Form("cut%i",i));
    setEventList(upc, eCuts[i], eCuts[i].GetName());
  }

  TEventList *oldList=upc->GetEventList();
  eArrayCuts[0].SetName(Form("cutArray%i",0));
  TEntryListArray *newList = new TEntryListArray( eArrayCuts[0].GetName(), eArrayCuts[0].GetName() );
  TString newListName=eArrayCuts[0].GetName();
  int n=upc->Draw(">>+"+newListName,eArrayCuts[0],"entrylistarray");
  upc->SetEntryList(newList);
  cout << eArrayCuts[0].GetTitle() << ": " << upc->GetEntries("") << endl;
  oldList->Delete();

  for(unsigned int i=1; i<eArrayCuts.size(); i++){
    eArrayCuts[i].SetName(Form("cutArray%i",i));
    setEntryListArray(upc, eArrayCuts[i], eArrayCuts[i].GetName());
  }
}

void setEventList( TTree *upc, TCut cut, TString newListName){
  TEventList *oldList=upc->GetEventList();
  TEventList *newList = new TEventList( newListName, newListName );
  int n=upc->Draw(">>+"+newListName,cut,"eventlist");
  upc->SetEventList(newList);
  cout << cut.GetTitle() << ": " << upc->GetEntries("") << endl;
  if(oldList) oldList->Delete();
}
  
void setEntryListArray( TTree *upc, TCut cut, TString newListName){
  TEntryListArray *oldList=(TEntryListArray *)upc->GetEntryList();
  TEntryListArray *newList = new TEntryListArray( newListName, newListName );
  int n=upc->Draw(">>+"+newListName,cut,"entrylistarray");
  upc->SetEntryList(newList);
  cout << cut.GetTitle() << ": " << upc->GetEntries("") << endl;
  if(oldList) oldList->Delete();
}
