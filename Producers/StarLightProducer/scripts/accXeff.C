#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TEventList.h"
#include "TCut.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TEventList.h"
#include "TCut.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include <iostream>
#include <string>
#include "math.h"

TString genRapidity(TString number=""){return(
	TString("0.5*log(")
	+"(sqrt(pow(mass"+number+",2)+pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"+pz"+number+")/"
	+"(sqrt(pow(mass"+number+",2)+pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"-pz"+number+"))");
}

TString genPt(TString number=""){return("sqrt(pow(px"+number+",2)+pow(py"+number+",2))");}


void accXeff(){
	//What File?
	TFile *f = new TFile("MC/reco_cuts/totals/jpsi_total.root");
	TFile file1("exJpsiHist.root", "RECREATE");
	//Tracks?
	TCut twoTracks = "nTracks==2";
	TCut moreTracks = "nTracks>2";
	TCut tracks = twoTracks;
	//Mass?
	TCut jpsi_mass =  "cand_mass>2.8 && cand_mass<3.4";
	TCut upsilon_mass = "cand_mass>9.2 && cand_mass<9.8";
	TCut psi2s_mass = "cand_mass>3.4 && cand_mass<4.0";
	TCut mass = jpsi_mass;
	//Histograms?
	Float_t Bin = 500;
	Float_t Max  = 10;
  	Float_t Min  = 0;

//Make the trees.
  	TTree *vert=(TTree *)f->Get("offlinePrimaryVerticesTree");
	TTree *muon=(TTree *)f->Get("upcPatMuonsWithTriggerTree");
	TTree *l1=(TTree *)f->Get("L1BitTree");
	TTree *cs=(TTree *)f->Get("ClusterShapeTree");
	TTree *calo=(TTree *)f->Get("CaloTree");
	TTree *maxcalo=(TTree *)f->Get("MaxCaloTree");
	TTree *zdc=(TTree *)f->Get("ZDCDigiTree");
	TTree *track=(TTree *)f->Get("generalTracksTree");
	TTree *cand=(TTree *)f->Get("onia2MuMuPatDiTrksTree");
	TTree *pix=(TTree *)f->Get("pixelTracksTree");
	TTree *cent=(TTree *)f->Get("PixelHitsCentralityTree");
	TTree *gen=(TTree *)f->Get("genTree");
//Friend the trees.
	cand->AddFriend(l1);
	cand->AddFriend(maxcalo);
	cand->AddFriend(vert);
	cand->AddFriend(muon);
	cand->AddFriend(gen);

	gen->AddFriend(cand);
	gen->AddFriend(l1);
	gen->AddFriend(maxcalo);
	gen->AddFriend(vert);
	gen->AddFriend(muon);

	TCut genRapBoundary = "(gen_rapidity>1.8 && gen_rapidity<2.3) || (gen_rapidity<-1.8 && gen_rapidity>-2.3)";
	TCut candRapBoundary = "(cand_rapidity>1.8 && cand_rapidity<2.3) || (cand_rapidity<-1.8 && cand_rapidity>-2.3)";
	//TCut runs = "Run >= 211313 && Run <= 211631";
	TCut goodMuon = "muon1_isGoodMuon && muon2_isGoodMuon";
	TCut trackerMuon = "muon1_Tracker && muon2_Tracker";
	TCut pixLayers = "muon1_pixelLayersWithMeasurement>0 && muon2_pixelLayersWithMeasurement>0";
	TCut tracLayers = "muon1_trackerLayersWithMeasurement>5 && muon2_trackerLayersWithMeasurement>5";
	TCut highPurity = "muon1_isHighPurity && muon2_isHighPurity";
	TCut numMuon = "nMuon==2";
	TCut vertex1 = "(muon1_dz-offlinePrimaryVerticesTree.z[0])<20 && muon1_dB<0.3";
	TCut vertex2 = "(muon2_dz-offlinePrimaryVerticesTree.z[0])<20 && muon2_dB<0.3";
	TCut maxHFEta = "(MaxHFEta>0 && MaxHFEnergy<2.0)||(MaxHFEta<0 && MaxHFEnergy<1.5)"; 
	TCut trigger = "HLT_PAUpcSingleMuOpenFull_TrackVeto7_v1";
	//TCut trigger = "muon1_pass || muon2_pass";
	TCut L1Quality = "muon1_l1Quality || muon2_l1Quality";
	TCut L1Trigger = "L1_SingleMuOpen";
	TCut singlePt ="muon1_pt>.2 && muon2_pt>.2";
	TCut dimuonPt ="cand_pt<1.5";

	TString genIndex="[4]";
  	gen->SetAlias("gen_rapidity", genRapidity(genIndex));
  	gen->SetAlias("gen_pt", genPt(genIndex));


  	TH1F *hist[14];
	for(int k=0; k< 14; k++){
		char *histname = new char[10];
		sprintf(histname, "hist[%d]",k);
		hist[k]=new TH1F(histname,"",Bin,Min,Max);
	}


  	TCut all = genRapBoundary;
  	cand->Draw("gen_pt>>hist[0]",all);	
	Float_t num = gen->GetEntries(all);
	Float_t den = gen->GetEntries(all);
	cout<<"0 Gen level: "<<gen->GetEntries(all)<<"  ("<<num/den<<")  "<<endl;

	all = all + candRapBoundary;
	cand->Draw("cand_pt>>hist[1]",all);	
	num = hist[1]->GetEntries();
	cout<<"1 candRapBoundary level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + goodMuon;
	cand->Draw("cand_pt>>hist[2]",all);	
	num = hist[2]->GetEntries();
	cout<<"2 goodMuon level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + trackerMuon;
	cand->Draw("cand_pt>>hist[3]",all);	
	num = hist[3]->GetEntries();
	cout<<"3 trackerMuon level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + tracLayers;
	cand->Draw("cand_pt>>hist[4]",all);	
	num = hist[4]->GetEntries();
	cout<<"6 tracLayers level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + pixLayers;
	cand->Draw("cand_pt>>hist[5]",all);	
	num = hist[5]->GetEntries();
	cout<<"5 pixLayers level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + highPurity;
	cand->Draw("cand_pt>>hist[6]",all);	
	num = hist[6]->GetEntries();
	cout<<"6 highPurity level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + vertex1 + vertex2;
	cand->Draw("cand_pt>>hist[7]",all);	
	num = hist[7]->GetEntries();
	cout<<"7 vertices level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + maxHFEta;
	cand->Draw("cand_pt>>hist[8]",all);	
	num = hist[8]->GetEntries();
	cout<<"8 maxHFEta level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + singlePt;
	cand->Draw("cand_pt>>hist[9]",all);	
	num = hist[9]->GetEntries();
	cout<<"9 singlePt level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + dimuonPt;
	cand->Draw("cand_pt>>hist[10]",all);	
	num = hist[10]->GetEntries();
	cout<<"10 dimuon level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + L1Quality;
	cand->Draw("cand_pt>>hist[11]",all);	
	num = hist[11]->GetEntries();
	cout<<"11 L1Quality level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + L1Trigger;
	cand->Draw("cand_pt>>hist[12]",all);	
	num = hist[12]->GetEntries();
	cout<<"12 L1 tirgger level: "<<num<<"  ("<<num/den<<")  "<<endl;

	all = all + trigger;
	cand->Draw("cand_pt>>hist[13]",all);	
	num = hist[13]->GetEntries();
	cout<<"13 trigger level: "<<num<<"  ("<<num/den<<")  "<<endl;

	hist[13]->Write();
	file1.Save();

}