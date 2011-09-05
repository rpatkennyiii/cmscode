{
	gROOT->ProcessLine(".L TrackTree.C+");
	TFile fileOpp("SkimOppositeChargeTree.root");
	TFile fileSame("SkimSameChargeTree.root");
	TTree *oppCharge = fileOpp.Get("TrackTree");
	TTree *sameCharge = fileSame.Get("TrackTree");
	
	TrackTree *oppTT=new TrackTree(oppCharge,"OppChargeLorentzEVec.root",0.000511);
	oppTT.Loop();
	TrackTree *oppTT=new TrackTree(oppCharge,"OppChargeLorentzMuVec.root",0.1057);
	oppTT.Loop();
	TrackTree *oppTT=new TrackTree(sameCharge,"SameChargeLorentzEVec.root",0.000511);
	oppTT.Loop();
	TrackTree *oppTT=new TrackTree(sameCharge,"SameChargeLorentzMuVec.root",0.1057);
	oppTT.Loop();

	TFile fileOppE4vec("OppChargeLorentzEVec.root");
	TFile fileOppMu4vec("OppChargeLorentzMuVec.root");
	TFile fileSameE4vec("SameChargeLorentzEVec.root");
	TFile fileSameMu4vec("SameChargeLorentzMuVec.root");

	TTree *oppChargeE4vec = fileOppE4vec.Get("LorentzTree");
	TTree *oppChargeMu4vec = fileOppMu4vec.Get("LorentzTree");
	TTree *sameChargeE4vec = fileSameE4vec.Get("LorentzTree");
	TTree *sameChargeMu4vec = fileSameMu4vec.Get("LorentzTree");

	TCanvas *tcan = new TCanvas("plots","plots",1200,700);
	tcan->Divide(2,2);
	
	tcan->cd(1);	
	oppChargeE4vec->Process("LorentzTree.C","eePairsOppQInvMass");
	tcan->cd(2);	
	oppChargeMu4vec->Process("LorentzTree.C","muPairsOppQInvMass");
	tcan->cd(3);	
	sameChargeE4vec->Process("LorentzTree.C","eePairsSameQInvMass");
	tcan->cd(4);	
	sameChargeMu4vec->Process("LorentzTree.C","muPairsSameQInvMass");

	tcan->SaveAs("~/Desktop/InvMass.png");
}
