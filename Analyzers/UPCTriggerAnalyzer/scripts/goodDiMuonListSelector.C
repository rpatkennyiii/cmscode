#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TEventList.h"
#include "TCut.h"
#include <fstream>
#include <iostream>

using namespace std;

int goodDiMuonListSelector(TString fileName="ppPAT.root", TString cutFileName="cuts.txt", TString outFileName="small.root"){

  TFile *oldfile = new TFile(fileName);
  TTree *oldtree = (TTree*)oldfile->Get("onia2MuMuPatDiTrksTree");
  TTree *zdc=(TTree *)oldfile->Get("ZDCDigiTree");
  TTree *maxcalo=(TTree *)oldfile->Get("MaxCaloTree");
  TTree *track=(TTree *)oldfile->Get("generalTracksTree");
  TTree *vert=(TTree *)oldfile->Get("offlinePrimaryVerticesTree");
  TTree *cs=(TTree *)oldfile->Get("ClusterShapeTree");
  TTree *beam=(TTree *)oldfile->Get("BeamTree");
  TTree *mu=(TTree *)oldfile->Get("upcPatMuonsWithTriggerTree");

  TTree *cand=(TTree *)oldtree->Clone();

  oldtree->AddFriend(track);
  oldtree->AddFriend(vert);
  oldtree->AddFriend(maxcalo);
  oldtree->AddFriend(zdc);
  oldtree->AddFriend(beam);
  oldtree->AddFriend(mu);

  TCut allCuts="";

  ifstream cutstream(cutFileName);

  char tmpString[500];
  while( !cutstream.eof()&&cutstream.is_open() ){
    cutstream.getline(tmpString,500);
    TCut tmpCut=tmpString;
    allCuts=allCuts&&tmpCut;
  }

  cutstream.close();

  TEventList *allCutsList=new TEventList("allCutsList","allCutsList");
  oldtree->Draw(">>+allCutsList",allCuts,"eventlist");
  vert->SetEventList(allCutsList);
  cs->SetEventList(allCutsList);
  mu->SetEventList(allCutsList);
  track->SetEventList(allCutsList);
  cand->SetEventList(allCutsList);

  cout << allCutsList->GetN() << endl;

  TFile *newfile = new TFile(outFileName,"recreate");
  TTree *newverttree = vert->CopyTree("");
  TTree *newcstree = cs->CopyTree("");
  TTree *newmutree = mu->CopyTree("");
  TTree *newtracktree = track->CopyTree("");
  TTree *newtree = cand->CopyTree("");

  newfile->Write();

  delete oldfile;
  delete newfile;

  return(0);
}
