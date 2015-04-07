#include "TChain.h"
#include "TString.h"

TChain *upcSetChain(TString file="ForwardGroup/UPC/data/UPCOniaCutTree.root", bool pp=0, bool rec=1, bool hlt=0, bool mc=1, bool both=1, bool cT=0){
	TChain *vert=new TChain(pp?"offlinePrimaryVerticesWithBSTree":"hiSelectedVertexTree");
	TChain *muon=new TChain("MuonTree");
	TChain *l1=new TChain(hlt?"hltbitanalysis/HltTree":"L1BitTree");
	TChain *cs=new TChain("ClusterShapeTree");
	TChain *calo=new TChain(cT?"CaloTree":0);
	TChain *zdc=new TChain("ZDCDigiTree");
	TChain *track=new TChain(pp?"generalTracksTree":"hiSelectedTracksTree");
	TChain *cand=new TChain(both?"onia2MuMuPatTraTraTree":"UPCPatCandidateTree");
	TChain *candG=new TChain(both?"onia2MuMuPatGlbGlbTree":0);
	TChain *pix=new TChain(pp?"pixelTracksTree":"hiPixelTracksTree");
	TChain *cent=new TChain((pp||mc)?0:"PixelHitsCentralityTree");
	TChain *gen=new TChain(mc?(pp?"genParticlesTree":"hiGenParticlesTree"):0);
	TChain *rechbhe=new TChain(rec?"HBHERecHitTree":"");
	TChain *rechf=new TChain(rec?"HFRecHitTree":"");
	TChain *receb=new TChain(rec?"EBRecHitTree":"");
	TChain *recee=new TChain(rec?"EERecHitTree":"");

	vert->Add(file);
	muon->Add(file);
	l1->Add(file);
	cs->Add(file);
	calo->Add(file);
	zdc->Add(file);
	track->Add(file);
	cand->Add(file);
	candG->Add(file);
	pix->Add(file);
	cent->Add(file);
	gen->Add(file);
	rechbhe->Add(file);
	rechf->Add(file);
	receb->Add(file);
	recee->Add(file);

	cand->AddFriend(l1);
	cand->AddFriend(cs);
	cand->AddFriend(vert);
	cand->AddFriend(track);
	cand->AddFriend(muon);
	cand->AddFriend(pix);
	cand->AddFriend(zdc);
	if(both) cand->AddFriend(candG);
	if(cT) cand->AddFriend(calo);
	if(!(pp|mc)) cand->AddFriend(cent);
	if(mc) cand->AddFriend(gen);
	if(rec) cand->AddFriend(rechbhe);
	if(rec) cand->AddFriend(rechf);
	if(rec) cand->AddFriend(receb);
	if(rec) cand->AddFriend(recee);
	
	return(cand);
}
