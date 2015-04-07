#include "TFile.h"
#include "TTree.h"

TTree *upcSetDiTrkTree(TString file="~/data/pbpb2011/upcPPReRecoDiTrk.root", bool pp=1, bool rec=1, bool hlt=0, bool both=1, bool oldmu=false){
	TFile *_fileData = new TFile(file);
	TTree *vert=(TTree *)_fileData->Get(pp?"offlinePrimaryVerticesTree":"hiSelectedVertexTree");
	TTree *muon=(TTree *)_fileData->Get(oldmu?"MuonTree":"upcPatMuonsWithTriggerTree");
	TTree *l1=(TTree *)_fileData->Get(hlt?"hltbitanalysis/HltTree":"L1BitTree");
	TTree *cs=(TTree *)_fileData->Get("ClusterShapeTree");
	TTree *calo=(TTree *)_fileData->Get("CaloTree");
	TTree *maxcalo=(TTree *)_fileData->Get("MaxCaloTree");
	TTree *zdc=(TTree *)_fileData->Get("ZDCDigiTree");
	TTree *track=(TTree *)_fileData->Get(pp?"generalTracksTree":"hiSelectedTracksTree");
	TTree *cand=(TTree *)_fileData->Get(both?"onia2MuMuPatDiTrksTree":"UPCPatCandidateTree");
	TTree *pix=(TTree *)_fileData->Get(false?"pixelTracksTree":"hiPixelTracksTree");
	TTree *cent=(TTree *)_fileData->Get("PixelHitsCentralityTree");
	TTree *gen=(TTree *)_fileData->Get("genTree");
	TTree *rechbhe=(TTree *)_fileData->Get(rec?"HBHERecHitTree":"");
	TTree *rechf=(TTree *)_fileData->Get(rec?"HFRecHitTree":"");
	TTree *receb=(TTree *)_fileData->Get(rec?"EBRecHitTree":"");
	TTree *recee=(TTree *)_fileData->Get(rec?"EERecHitTree":"");
  TTree *castor=(TTree *)_fileData->Get("CastorRecTree");

	cand->AddFriend(l1);
	cand->AddFriend(calo);
	cand->AddFriend(maxcalo);
	cand->AddFriend(cs);
	cand->AddFriend(vert);
	cand->AddFriend(track);
	cand->AddFriend(muon);
	cand->AddFriend(pix);
	cand->AddFriend(cent);
	cand->AddFriend(zdc);
	cand->AddFriend(gen);
	cand->AddFriend(rechbhe);
	cand->AddFriend(rechf);
	cand->AddFriend(receb);
	cand->AddFriend(recee);
  cand->AddFriend(castor);
	
	return(cand);
}
