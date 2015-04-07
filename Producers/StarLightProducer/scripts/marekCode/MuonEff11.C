#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TEventList.h"
#include "tdrstyle.C"
#include "TCut.h"

 	  TString genIndex="[4]";

TString genPT(TString number=""){return("sqrt(pow(px"+number+",2)+pow(py"+number+",2))");}

TString genRapidity(TString number=""){return(
	TString("0.5*log(")
	+"(sqrt(pow(mass"+number+",2)+pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"+pz"+number+")/"
	+"(sqrt(pow(mass"+number+",2)+pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"-pz"+number+"))");
}




	/////////////////// THIS CODE REQUIRES APPROPRIATE FOLDER STRUCTURE FOR SAVED FILES: /////////////////

//	mkdir EfficiencyOutput
//	mkdir EfficiencyOutput/Aym
//	mkdir EfficiencyOutput/Sam
//	mkdir EfficiencyOutput/Sam2
//	mkdir EfficiencyOutput/DimuonAna


void MuonEff11(){ // cuts & method choice, terminal output, changes file names and saves to different folders



	////////////////////////////////////////////
	////	Customization area.		////
	////////////////////////////////////////////

	int WhatCuts=1; // change to 0 to use Sam cuts, 1 for Ayman, 2 for Sam second version of cuts
	bool TagAndProbe=1; // change to 0 for direct MC calculation

	bool AymBin = 0; // sets Ayman pT binning 10 bins 1.3 -- 1.8, see below
	bool onebin = 0; // one bin histos, produces efficiency terminal output, no plots saved
	bool DimuonAna = 1; // analysis for parent pt, only efficiency plots are relevant and saved (to DimuonAna folder). Additional DrawerDimuon code can be used to draw plots from root file

	//What File?
    TFile *f = new TFile("/media/marek/DATA/Jpsi_DATA/totals/jpsi_total.root");
//	/media/marek/DATA/Jpsi_DATA/totals/jpsi_total.root	/media/marek/DATA/Jpsi_DATA/pPb2013_data/UPCpPbDiTrkTree.root

	//Tracks?
    TCut twoTracks = "nTracks==2";
    TCut moreTracks = "nTracks>2";
    TCut Tracks = twoTracks;

	//Mass?
    TCut jpsi_mass =  "cand_mass>2.8 && cand_mass<3.4";
    TCut upsilon_mass = "cand_mass>9.2 && cand_mass<9.8";
    TCut psi2s_mass = "cand_mass>3.4 && cand_mass<4.0";
    TCut mass = jpsi_mass;
    TString particle = "jpsi"; // change according to mass cut for appropriate filename



	//filenames and folders
    TString title = "";
    TString folder = "";

	if (WhatCuts==1){
    folder = "Aym";
	if (TagAndProbe==1)
    title = "_"+particle+"_TagAndProbe_Aym";
	if (TagAndProbe==0)
    title = "_"+particle+"_Direct_Aym";
	}

	if (WhatCuts==0){
    folder = "Sam";
	if (TagAndProbe==1)
    title = "_"+particle+"_TagAndProbe_Sam";
	if (TagAndProbe==0)
    title = "_"+particle+"_Direct_Sam";
	}

	if (WhatCuts==2){
    folder = "Sam2";
	if (TagAndProbe==1)
    title = "_"+particle+"_TagAndProbe_Sam2";
	if (TagAndProbe==0)
    title = "_"+particle+"_Direct_Sam2";
	}

	if (DimuonAna==1)
    folder = "DimuonAna";





	//default pT binning
	int nPt = 30;
	float minPtBin =  0;
	float maxPtBin =  3;

	if (onebin==1){
    	nPt = 1;
    	minPtBin =  0;
    	maxPtBin =  3;
	}

	if (AymBin==1){
    	nPt = 10;
    	minPtBin =  1.2;
    	maxPtBin =  2.2;
	}


	////////////////////////////////////
	///	VARIABLES SETUP		///
	///////////////////////////////////

  TStyle *tdrStyle=setTDRStyle();
  tdrStyle->SetOptTitle(1);
  tdrStyle->SetOptStat(1);
  tdrStyle->SetPadRightMargin(0.19);
  tdrStyle->SetPadTopMargin(0.09);
  tdrStyle->SetPalette(1);
  TCanvas *c1=new TCanvas();


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



	/////////////////////////////////    CUTS      /////////////////////////////////////




	// Sam initial
	TCut SamCut = "";
	TCut genRapBoundary = "";
	TCut SamCut2 = "";

	// Aym initial
	TCut AymMu1 = "";
	TCut AymMu2 = "";




	///////////////////////////   Ayman cuts on muons  /////////////////////////////////////

	if (WhatCuts==1){


  TCut mu1eta =" (muon1_eta > 1.8 && muon1_eta < 2.5) || (muon1_eta < -1.8 && muon1_eta > -2.5) ";
  TCut mu2eta =" (muon2_eta > 1.8 && muon2_eta < 2.5) || (muon2_eta < -1.8 && muon2_eta > -2.5) ";


  TCut isGood1 = "muon1_isGoodMuon";
  TCut tracker1 = "muon1_Tracker";
  TCut trackerLayers1 = "muon1_trackerLayersWithMeasurement>5";
  TCut pixelLayers1 = "muon1_pixelLayersWithMeasurement>0";
  TCut Chi1 = "muon1_normalizedChi2<3";

  TCut isGood2 = "muon2_isGoodMuon";
  TCut tracker2 = "muon2_Tracker";
  TCut trackerLayers2 = "muon2_trackerLayersWithMeasurement>5";
  TCut pixelLayers2 = "muon2_pixelLayersWithMeasurement>0";
  TCut Chi2 = "muon2_normalizedChi2<3";

		AymMu1 = mu1eta+isGood1+tracker1+trackerLayers1+pixelLayers1+Chi1;
		AymMu2 = mu2eta+isGood2+tracker2+trackerLayers2+pixelLayers2+Chi2;

	}



	///////////////////////////  Sam cuts  ///////////////////////////////////

	if (WhatCuts==0){

  TCut candRapBoundary = "(cand_rapidity>1.8 && cand_rapidity<2.3) || (cand_rapidity>-2.3 && cand_rapidity<-1.8)";
  genRapBoundary =  "(gen_rapidity>1.8 && gen_rapidity<2.3) || (gen_rapidity>-2.3 && gen_rapidity<-1.8)";


  TCut runs = "Run >= 211313 && Run <= 211631";

  TCut tracker1 = "muon1_Tracker";
  TCut tracker2 = "muon2_Tracker";
  TCut goodMuon = "isGoodMuon";
  TCut trackerMuon = "isTrackerMuon";
  TCut pixLayers = "pixelLayersWithMeasurement>0";
  TCut tracLayers = "trackerLayersWithMeasurement>5";
  TCut highPurity = "isHighPurity";
  TCut numMuon = "nMuon==2";
  TCut vertex = "(z-offlinePrimaryVerticesTree.z[0])<20 && sqrt(pow(x-offlinePrimaryVerticesTree.x[0],2) + pow(y-offlinePrimaryVerticesTree.y[0],2))<0.3";
  TCut maxHFEta = "((MaxHFEta>0 && MaxHFEnergy<2.0)||(MaxHFEta<0 && MaxHFEnergy<1.5))"; 


  SamCut    = tracker1+tracker2+candRapBoundary+goodMuon+trackerMuon+pixLayers+tracLayers+highPurity+numMuon+vertex+maxHFEta;

	}






	///////////////////////////  Sam cuts Version 2 ///////////////////////////////////

	if (WhatCuts==2){

  	TCut genRapBoundary =  "(gen_rapidity>1.8 && gen_rapidity<2.3)||(gen_rapidity<-1.8 && gen_rapidity>-2.3)";
	TCut candRapBoundary = "(cand_rapidity>1.8 && cand_rapidity<2.3)||(cand_rapidity<-1.8 && cand_rapidity>-2.3)";


	TCut runs = "Run >= 211313 && Run <= 211631";
	TCut goodMuon = "muon1_isGoodMuon && muon2_isGoodMuon";
	TCut trackerMuon = "muon1_Tracker && muon2_Tracker";
	TCut pixLayers = "muon1_pixelLayersWithMeasurement>0 && muon2_pixelLayersWithMeasurement>0";
	TCut tracLayers = "muon1_trackerLayersWithMeasurement>5 && muon2_trackerLayersWithMeasurement>5";
	TCut highPurity = "muon1_isHighPurity && muon2_isHighPurity";
	TCut numMuon = "nMuon==2";
	TCut vertex1 = "(muon1_dz-offlinePrimaryVerticesTree.z[0])<20 && muon1_dB<0.3";
	TCut vertex2 = "(muon2_dz-offlinePrimaryVerticesTree.z[0])<20 && muon2_dB<0.3";
	TCut maxHFEta = "((MaxHFEta>0 && MaxHFEnergy<2.0)||(MaxHFEta<0 && MaxHFEnergy<1.5))"; 
	TCut L1Quality = "(muon1_l1Quality || muon2_l1Quality)";
	//TCut L1Trigger = "L1_SingleMuOpen"; // not used
	
	TCut singlePt ="(muon1_pt>.2 && muon2_pt>.2)";
	TCut dimuonPt ="cand_pt<1.5";


  SamCut2    = trackerMuon+candRapBoundary+goodMuon+trackerMuon+pixLayers+tracLayers+highPurity+numMuon+vertex1+vertex2+maxHFEta+L1Quality;

	}





  TCut m1trig    = "muon1_pass";
  TCut m2trig    = "muon2_pass";

  TCut genEvSel	= "HLT_PAUpcSingleMuOpenFull_TrackVeto7_v1 && (muon1_pass || muon2_pass)";






	/////////////////////////////////    CUTS  END    /////////////////////////////////////



	////////////////////////////////////
	///	HISTOS SETUP		///
	///////////////////////////////////


	//Set up the gen histogram.
 	TH1F *genPt=new TH1F("genPt","genPt",nPt,minPtBin,maxPtBin);

  	genPt->SetXTitle("p_{T}^{muon}");
	if (DimuonAna)
	genPt->SetXTitle("p_{T}^{dimuon}");


	//clone selected and trigger histograms from detectable histo
  	TH1F *detPt=		(TH1F *)genPt->Clone();
  	detPt->			SetNameTitle("detPt","detPt");

  	TH1F *selPt=		(TH1F *)genPt->Clone();
  	selPt->			SetNameTitle("selPt","selPt");

  	TH1F *selPt1=		(TH1F *)genPt->Clone();
  	selPt1->			SetNameTitle("selPt1","selPt1");

  	TH1F *selPt2=		(TH1F *)genPt->Clone();
  	selPt2->			SetNameTitle("selPt2","selPt2");

  	TH1F *selPtAll=		(TH1F *)genPt->Clone();
  	selPtAll->			SetNameTitle("selPtAll","selPtAll");

  	TH1F *trigPt=		(TH1F *)genPt->Clone();
  	trigPt->		SetNameTitle("trigPt","trigPt");

  	TH1F *trigPt1=		(TH1F *)genPt->Clone();
  	trigPt1->		SetNameTitle("trigPt1","trigPt1");

  	TH1F *trigPt2=		(TH1F *)genPt->Clone();
  	trigPt2->		SetNameTitle("trigPt2","trigPt2");

  	TH1F *trig2Pt1=		(TH1F *)genPt->Clone();
  	trig2Pt1->		SetNameTitle("trig2Pt1","trig2Pt1");

  	TH1F *trig1Pt2=		(TH1F *)genPt->Clone();
  	trig1Pt2->		SetNameTitle("trig1Pt2","trig1Pt2");

  	TH1F *trigPtAll=		(TH1F *)genPt->Clone();
  	trigPtAll->		SetNameTitle("trigPtAll","trigPtAll");

  	TH1F *trigNum=		(TH1F *)genPt->Clone();
  	trigNum->		SetNameTitle("trigNum","trigNum");

  	TH1F *trigDen=		(TH1F *)genPt->Clone();
  	trigDen->		SetNameTitle("trigDen","trigDen");

  	TH1F *trigPt1Both=		(TH1F *)genPt->Clone();
  	trigPt1Both->		SetNameTitle("trigPt1Both","trigPt1Both");

  	TH1F *trigPt2Both=		(TH1F *)genPt->Clone();
  	trigPt2Both->		SetNameTitle("trigPt2Both","trigPt2Both");


  	detPt->Sumw2();
  	selPt->Sumw2();
  	selPt1->Sumw2();
  	selPt2->Sumw2();
  	selPtAll->Sumw2();
  	trigPt1->Sumw2();
  	trigPt2->Sumw2();
  	trigPtAll->Sumw2();
  	trigNum->Sumw2();
  	trigDen->Sumw2();
  	trigPt1Both->Sumw2();
  	trigPt2Both->Sumw2();
  	genPt->Sumw2();

	// EFF vs pT and eta histogram binning

	float H2EtaBins = 40;
	float H2Eta1 = -4;
	float H2Eta2 = 4;
	float H2PtBins = 60;
	float H2Pt1 = 0;
	float H2Pt2 = 3;


	TH2F *EffVsEtaAndPt=new TH2F("EffVsEtaAndPt","EffVsEtaAndPt",H2EtaBins,H2Eta1,H2Eta2,H2PtBins,H2Pt1,H2Pt2);
	EffVsEtaAndPt->Sumw2();

	TH2F *EffVsEtaAndPt_N=new TH2F("EffVsEtaAndPt_N","EffVsEtaAndPt_N",H2EtaBins,H2Eta1,H2Eta2,H2PtBins,H2Pt1,H2Pt2);
	EffVsEtaAndPt_N->Sumw2();

	TH2F *EffVsEtaAndPt_D=new TH2F("EffVsEtaAndPt_D","EffVsEtaAndPt_D",H2EtaBins,H2Eta1,H2Eta2,H2PtBins,H2Pt1,H2Pt2);
	EffVsEtaAndPt_D->Sumw2();

	TH2F *EffVsEtaAndPt_N_1=new TH2F("EffVsEtaAndPt_N_1","EffVsEtaAndPt_N_1",H2EtaBins,H2Eta1,H2Eta2,H2PtBins,H2Pt1,H2Pt2);
	EffVsEtaAndPt_N_1->Sumw2();

	TH2F *EffVsEtaAndPt_D_1=new TH2F("EffVsEtaAndPt_D_1","EffVsEtaAndPt_D_1",H2EtaBins,H2Eta1,H2Eta2,H2PtBins,H2Pt1,H2Pt2);
	EffVsEtaAndPt_D_1->Sumw2();

	TH2F *EffVsEtaAndPt_N_2=new TH2F("EffVsEtaAndPt_N_2","EffVsEtaAndPt_N_2",H2EtaBins,H2Eta1,H2Eta2,H2PtBins,H2Pt1,H2Pt2);
	EffVsEtaAndPt_N_2->Sumw2();

	TH2F *EffVsEtaAndPt_D_2=new TH2F("EffVsEtaAndPt_D_2","EffVsEtaAndPt_D_2",H2EtaBins,H2Eta1,H2Eta2,H2PtBins,H2Pt1,H2Pt2);
	EffVsEtaAndPt_D_2->Sumw2();



	/////////////////////////////////    PROJECTIONS    /////////////////////////////////////



  	tdrStyle->SetPaintTextFormat("4.3f");


	//setup gen rapidity variable

        TString genPtString=genPT(genIndex);
  	gen->SetAlias("gen_pt", genPtString);
    	gen->SetAlias("gen_rapidity", genRapidity(genIndex));

	//create gen j/psi map

  	gen->Project("genPt","gen_pt",genRapBoundary);
  	genPt->SetTitle("GEN Candidates for "+title+" MC");

	//create detectable j/psi map

  	TEventList *detPtList=new TEventList("detPtList","detPtList");
  	cand->Project("detPtList","","","eventlist");
  	cand->SetEventList(detPtList);
  	cand->Project("detPt","cand_pt");

	//create selected j/psi map

  	TEventList *selPtList=new TEventList("selPtList","selPtList");
  	cand->Project("selPtList","",mass&&SamCut&&SamCut2&&AymMu1&&AymMu2,"eventlist"); // uses mass cut and Sam or Aym cut depending on "WhatCuts" (unused cut is set to "")
  	cand->SetEventList(selPtList);



	if (DimuonAna == 0){
  	cand->Project("selPt1","muon1_pt","");
  	cand->Project("selPt2","muon2_pt","");
  	selPtAll->Add( selPt1, selPt2);

  	cand->Project("trigPt1","muon1_pt",genEvSel&&m1trig);
  	cand->Project("trigPt2","muon2_pt",genEvSel&&m2trig);
  	trigPtAll->Add(trigPt1, trigPt2);
	}

	if (DimuonAna == 1){ // for dimuon analysis histos are filled cand_pt bins instead of muon pT
  	cand->Project("selPt1","cand_pt","");
  	cand->Project("selPt2","cand_pt","");
  	selPtAll->Add( selPt1, selPt2);

  	cand->Project("trigPt1","cand_pt",genEvSel&&m1trig);
  	cand->Project("trigPt2","cand_pt",genEvSel&&m2trig);
  	trigPtAll->Add(trigPt1, trigPt2);

	cand->Project("selPt","cand_pt","");
	}

  	TH1F *acc=(TH1F *)detPt->Clone();

  	TH1F *effTrigSel=(TH1F *)acc->Clone();
  	effTrigSel->SetTitle("UPC Trigger Efficiency (events number) for "+title);

  	TH1F *effTrig1=(TH1F *)acc->Clone();
  	effTrig1->SetTitle("1st Muon Trigger Efficiency for "+title);

  	TH1F *effTrig2=(TH1F *)acc->Clone();
  	effTrig2->SetTitle("2nd Muon Trigger Efficiency for "+title);

  	TH1F *effTrigAll=(TH1F *)acc->Clone();
  	effTrigAll->SetTitle("All Muons Trigger Efficiency for "+title);



	//////////////////////////	Tag And Probe	   //////////////////////////

	if (TagAndProbe==1){
	
  	cand->Project("EffVsEtaAndPt_N_1","muon1_pt : muon1_eta",genEvSel&&m1trig&&m2trig); // th2f
  	cand->Project("EffVsEtaAndPt_N_2","muon2_pt : muon2_eta",genEvSel&&m1trig&&m2trig); // th2f
  	cand->Project("EffVsEtaAndPt_D_1","muon1_pt : muon1_eta",genEvSel&&m2trig); // th2f
  	cand->Project("EffVsEtaAndPt_D_2","muon2_pt : muon2_eta",genEvSel&&m1trig); // th2f

  	EffVsEtaAndPt_N->Add( EffVsEtaAndPt_N_1, EffVsEtaAndPt_N_2);
  	EffVsEtaAndPt_D->Add( EffVsEtaAndPt_D_1, EffVsEtaAndPt_D_2);

  	EffVsEtaAndPt->Divide( EffVsEtaAndPt_N, EffVsEtaAndPt_D ,1,1,"B");


	if (DimuonAna == 1){ // for dimuon analysis histos are filled cand_pt bins instead of muon pT
  	cand->Project("trig2Pt1","cand_pt",genEvSel&&m2trig);
  	cand->Project("trig1Pt2","cand_pt",genEvSel&&m1trig);
  	cand->Project("trigPt1Both","cand_pt",genEvSel&&m1trig&&m2trig);
  	cand->Project("trigPt2Both","cand_pt",genEvSel&&m1trig&&m2trig);
	}

	if (DimuonAna == 0){
  	cand->Project("trig2Pt1","muon1_pt",genEvSel&&m2trig);
  	cand->Project("trig1Pt2","muon2_pt",genEvSel&&m1trig);
  	cand->Project("trigPt1Both","muon1_pt",genEvSel&&m1trig&&m2trig);
  	cand->Project("trigPt2Both","muon2_pt",genEvSel&&m1trig&&m2trig);
	}



  	trigNum->Add(trigPt1Both, trigPt2Both);
  	trigDen->Add(trig2Pt1, trig1Pt2);

  	effTrig1->Divide( trigPt1Both, trig2Pt1 ,1,1,"B");
  	effTrig2->Divide( trigPt2Both, trig1Pt2 ,1,1,"B");
  	effTrigAll->Divide( trigNum, trigDen,1,1,"B");

		} // tag and probe







	//////////////////////           Direct         ////////////////////////////

	if (TagAndProbe==0) {

	// direct MC


  	cand->Project("EffVsEtaAndPt_N_1","muon1_pt : muon1_eta",genEvSel&&m1trig); // th2f
  	cand->Project("EffVsEtaAndPt_N_2","muon2_pt : muon2_eta",genEvSel&&m2trig); // th2f
  	cand->Project("EffVsEtaAndPt_D_1","muon1_pt : muon1_eta",""); // th2f
  	cand->Project("EffVsEtaAndPt_D_2","muon2_pt : muon2_eta",""); // th2f

  	EffVsEtaAndPt_N->Add( EffVsEtaAndPt_N_1, EffVsEtaAndPt_N_2);
  	EffVsEtaAndPt_D->Add( EffVsEtaAndPt_D_1, EffVsEtaAndPt_D_2);

  	EffVsEtaAndPt->Divide( EffVsEtaAndPt_N, EffVsEtaAndPt_D ,1,1,"B");


  	trigPtAll->Add(trigPt1, trigPt2);
  	trigNum->Add(trigPt1Both, trigPt2Both);
  	trigDen->Add(trigPt1, trigPt2);

  	effTrig1->Divide( trigPt1, selPt1 ,1,1,"B");
  	effTrig2->Divide( trigPt2, selPt2 ,1,1,"B");
  	effTrigAll->Divide( trigPtAll, selPtAll,1,1,"B");

		} // direct





	/////////////////////////////////    PROJECTIONS  END  /////////////////////////////////////


	////////////////////////////////     TERMINAL OUTPUT   /////////////////////////////////////


float mt1 = effTrig1->GetBinContent(1);
float mt2 = effTrig2->GetBinContent(1);
float mtAll = effTrigAll->GetBinContent(1);
cand->Project("trigPt","cand_pt",""); // for getentries only
float NumSelEv = trigPt->GetEntries();
cand->Project("trigPt","cand_pt",genEvSel); // for getentries only
float NumTrigEv = trigPt->GetEntries();

		cout << endl << endl;

if (WhatCuts == 1) cout << "Ayman cuts, ";
if (WhatCuts == 0) cout << "Samuel cuts, ";
if (WhatCuts == 2) cout << "Samuel cuts 2, ";
if (TagAndProbe == 1) cout << "tag and probe" << endl << endl;
if (TagAndProbe == 0) cout << "direct" << endl << endl;



		cout << "Sel events	" << NumSelEv << endl;
		cout << "Trig events	" << NumTrigEv << endl;
		cout << "UPC trig eff	" << NumTrigEv/NumSelEv << endl;
		cout << "Trig 1		" << trigPt1->GetEntries() << endl;
		cout << "Trig 2		" << trigPt2->GetEntries() << endl;
		cout << "Trig 1 Both	" << trigPt1Both->GetEntries() << endl;
		cout << "Trig 2 Both	" << trigPt2Both->GetEntries() << endl;
		cout << endl << endl;

	if (onebin == 1){ // efficiency output for one bin histos (integrated)

		cout << "FROM 1 BIN HISTOS:"<< endl;
		cout << "Muon 1 eff	" << mt1 << endl;
		cout << "Muon 1 eff	" << mt2 << endl;
		cout << "Muon ave eff	" << mtAll << endl;
		cout << "UPC TRIG SEP MU EFF	" << 1-(1-mt1)*(1-mt2) << endl;
		cout << "UPC TRIG AVE MU EFF	" << 1-(1-mtAll)*(1-mtAll) << endl;
		cout << endl << endl;

		}



	////////////////////////////////  TERMINAL OUTPUT END  /////////////////////////////////////





	/////////////////////////////////       DRAWING      /////////////////////////////////////

	//set title
  	detPt->SetTitle("Detectable Candidates from "+title+" MC");
  	selPt1->SetTitle("1 Selected Candidates from "+title+" MC");
  	selPt2->SetTitle("2 Selected Candidates from "+title+" MC");
  	selPtAll->SetTitle("All Selected Candidates from "+title+" MC");
  	trigPt1->SetTitle("1st Muon Triggered from "+title+" MC");
  	trigPt2->SetTitle("2nd Muon Triggered from "+title+" MC");
  	trigPtAll->SetTitle("Triggered Muons from "+title+" MC");
  	trigPt1Both->SetTitle("Both Muons Triggered from "+title+" MC");

	// histos are saved or not depending on what analysis is done
	if (onebin == 0 ){
	if (DimuonAna == 0 ){
	//draw hisots
 	detPt->Draw();
  	//c1->SaveAs("EfficiencyOutput/"+folder+"/detAccReco"+title+".png");
  	selPt1->Draw();
  	//c1->SaveAs("EfficiencyOutput/"+folder+"/sel1"+title+".png");
  	selPt2->Draw();
  	//c1->SaveAs("EfficiencyOutput/"+folder+"/sel2"+title+".png");
  	selPtAll->Draw();
  	//c1->SaveAs("EfficiencyOutput/"+folder+"/selAll"+title+".png");
  	trigPt1->Draw();
  	c1->SaveAs("EfficiencyOutput/"+folder+"/trig1"+title+".png");
  	trigPt2->Draw();
  	c1->SaveAs("EfficiencyOutput/"+folder+"/trig2"+title+".png");
  	trigPtAll->Draw();
  	c1->SaveAs("EfficiencyOutput/"+folder+"/trigAll"+title+".png");

	} // dimuona analysis

	EffVsEtaAndPt->GetXaxis()->SetTitle("#eta^{muon}");
	EffVsEtaAndPt->GetYaxis()->SetTitle("p_{T}^{muon}");
	EffVsEtaAndPt->SetStats(0);
	EffVsEtaAndPt->Draw("colz");
	c1->SaveAs("EfficiencyOutput/"+folder+"/th2f"+title+".png");





	/////////////////////////////////       DIVISIONS      /////////////////////////////////////

  	



  	//acc->Divide( detPt, genPt,1,1,"B");
  	//acc->SetTitle("Candidate Acceptance for "+title);
  	//acc->Draw();
  	//acc->SetName("detAcc"+title);
  	//c1->SaveAs("EfficiencyOutput/"+folder+"/detAcc"+title+".png");


  	effTrig1->Draw();
  	effTrig1->SetName("effTrig");
  	c1->SaveAs("EfficiencyOutput/"+folder+"/effTrig1"+title+".png");

  	effTrig2->Draw();
  	effTrig2->SetName("effTrig2");
  	c1->SaveAs("EfficiencyOutput/"+folder+"/effTrig2"+title+".png");


  	effTrigAll->Draw();
  	effTrigAll->SetName("effTrigAll");
  	c1->SaveAs("EfficiencyOutput/"+folder+"/effTrigAll"+title+".png");

	} // onebin
  	



	/////////////////////////////////       DIMUON ANALYSIS      /////////////////////////////////////


  	TH1F *TrigEffUPC=(TH1F *)effTrigAll->Clone();
  	TH1F *TrigEffUPCsep=(TH1F *)effTrigAll->Clone();
	TrigEffUPC->SetXTitle("p_{T}^{dimuon}");
	TrigEffUPCsep->SetXTitle("p_{T}^{dimuon}");



	if (DimuonAna==1){

  	effTrigSel->Divide( trigPt, selPt ,1,1,"B");
  	effTrigSel->Draw();
  	effTrigSel->SetName("effTrigSel");
  	c1->SaveAs("EfficiencyOutput/"+folder+"/effTrigSel"+title+".png");

		Double_t cont;

	cout << endl << endl;

		for (Int_t nbin=1; nbin<=nPt; nbin++ )
		{
		cont=1-(1-effTrigAll->GetBinContent(nbin))*(1-effTrigAll->GetBinContent(nbin));
		
		TrigEffUPC->SetBinContent(nbin,cont);
		TrigEffUPC->SetBinError(nbin,2*(1-cont)*effTrigAll->GetBinError(nbin));
		//cout << cont << endl;
		}


	//cout << endl << endl;

	//TrigEffUPC->GetYaxis()->SetRangeUser(0,1);

	TrigEffUPC->SetTitle("UPC trig eff from ave muons "+title+" MC");
  	TrigEffUPC->Draw();
  	TrigEffUPC->SetName("TrigEffUPC");
  	c1->SaveAs("EfficiencyOutput/"+folder+"/TrigEffUPC"+title+".png");



		Double_t err;

	//cout << endl << endl;

		for (Int_t nbin=1; nbin<=nPt; nbin++ )
		{
		cont=1-(1-effTrig1->GetBinContent(nbin))*(1-effTrig2->GetBinContent(nbin));
		err=sqrt(pow((1-effTrig1->GetBinContent(nbin))*effTrig1->GetBinError(nbin),2)+pow((1-effTrig2->GetBinContent(nbin))*effTrig2->GetBinError(nbin),2));

		TrigEffUPCsep->SetBinContent(nbin,cont);
		TrigEffUPCsep->SetBinError(nbin,err);
		//cout << cont << endl;
		}

	//cout << endl << endl;

	//TrigEffUPCsep->GetYaxis()->SetRangeUser(0,1);

	TrigEffUPCsep->SetTitle("UPC trig eff from sep muons "+title+" MC");

  	TrigEffUPCsep->Draw();
  	TrigEffUPCsep->SetName("TrigEffUPCsep");
  	c1->SaveAs("EfficiencyOutput/"+folder+"/TrigEffUPCsep"+title+".png");


	} // DIMUON ANALYSIS



	///////////////////////////////     DIMUON ANALYSIS END   ///////////////////////////////////








	/////////////////////////////////       ROOT FILE      /////////////////////////////////////


  	TFile *outFile=new TFile("EfficiencyOutput/"+folder+"/output_file"+title+".root","RECREATE");
  	outFile->Add(genPt);
  	outFile->Add(detPt);
  	outFile->Add(selPt1);
  	outFile->Add(selPt2);
  	outFile->Add(selPtAll);
  	outFile->Add(trigPt1);
  	outFile->Add(trigPt2);
  	outFile->Add(trigPtAll);
  	outFile->Add(trigPt1Both);
  	outFile->Add(trigPt2Both);
	outFile->Add(effTrigAll);
  	outFile->Add(effTrig1);
  	outFile->Add(effTrig2);
	outFile->Add(effTrigSel);
  	outFile->Add(EffVsEtaAndPt);

	if (DimuonAna==1)
	{
  	outFile->Add(TrigEffUPCsep);
  	outFile->Add(TrigEffUPC);
	}

  	outFile->Write();
  	outFile->Close();
}





