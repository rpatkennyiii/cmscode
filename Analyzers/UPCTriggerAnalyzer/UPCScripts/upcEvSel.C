#include "/net/hisrv0001/home/mojoe137/scratchSpace/CMSSW_4_4_3/src/Analyzers/UPCTriggerAnalyzer/UPCScripts/upcGenPlots.C"
#include "/net/hisrv0001/home/mojoe137/scratchSpace/CMSSW_4_4_3/src/Analyzers/ZDCAnalyzer/ZDCScripts/zdcEnergy.C"
#include "TCut.h"
#include "TString.h"

float ptBinz[]={0.0,.05,.1,.15,.2,.25,.3,.35,.4,.45,.5,.55,.6,.65,.7,.75,.8,.85,.9,.95,1.0,1.25,1.5,2.0,2.5};
float rapBinz[]={-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,1.7,1.8,1.9,2.0,2.1,2.2,2.3};

TCut genEvSel("L1_SingleMuOpen_BptxAND&&HLT_HIZeroBiasPixel_SingleTrack_v1");

TCut beamHaloVeto("!(L1Tech_BSC_halo_beam1_inner.v0||L1Tech_BSC_halo_beam1_outer.v0||L1Tech_BSC_halo_beam2_inner.v0||L1Tech_BSC_halo_beam2_outer.v0)");
TCut vertCut("!hiSelectedVertexTree.isFake && abs(hiSelectedVertexTree.pVz) <= 25 && sqrt(pow(hiSelectedVertexTree.pVx,2)+pow(hiSelectedVertexTree.pVy,2)) <= 2 && hiSelectedVertexTree.nTracks >= 2");
TCut vertLooseCut("!hiSelectedVertexTree.isFake && abs(hiSelectedVertexTree.pVz) <= 25 && sqrt(pow(hiSelectedVertexTree.pVx,2)+pow(hiSelectedVertexTree.pVy,2)) <= 2");
TCut vertLooseCutPP("!offlinePrimaryVerticesWithBSTree.isFake && abs(offlinePrimaryVerticesWithBSTree.z) <= 25 && sqrt(pow(offlinePrimaryVerticesWithBSTree.x,2)+pow(offlinePrimaryVerticesWithBSTree.y,2)) <= 2");
TCut hfCoincVeto("!(Sum$((CaloEta>=3)&&(CaloEnergy>3))>=3&&Sum$((CaloEta<=-3)&&(CaloEnergy>3))>=3)");
TCut hfCoincTightVeto("!(Sum$((CaloEta>=3)&&(CaloEnergy>=3))>0&&Sum$((CaloEta<=-3)&&(CaloEnergy>3))>0)");
TCut hfSigVeto("!(Sum$((CaloEta>=3)&&(CaloEnergy>3))>=3||Sum$((CaloEta<=-3)&&(CaloEnergy>3))>=3)");
TCut minBiasVeto="!L1_HcalHfCoincPmORBscMinBiasThresh1_BptxAND_instance1";
TCut cShapeCut="Accept";

TCut muonCutsTracker("nTrackerMuon==2&&qoverpTracker[0]/qoverpTracker[1]<0");
TCut muonCutsGlobal("nGlobalMuon==2&&qoverpGlobal[0]/qoverpGlobal[1]<0");
TCut muonCutsCand("cand_charge[0]==0&&acceptedCand==1");
TCut muonCutsTrack("hiSelectedTracksTree.nTracks==2&&hiSelectedTracksTree.qoverp[0]/hiSelectedTracksTree.qoverp[1]<0");

TCut accRapTracker("abs("+recoPseudoRapidity("Tracker[1]")+")<2.5&&1.4<abs("+recoPseudoRapidity("Tracker[1]")+")&&abs("+recoPseudoRapidity("Tracker[0]")+")<2.5&&1.8<abs("+recoPseudoRapidity("Tracker[0]")+")");
TCut accRapTracks("abs("+recoPseudoRapidity("[1]")+")<2.5&&1.4<abs("+recoPseudoRapidity("[1]")+")&&abs("+recoPseudoRapidity("[0]")+")<2.5&&1.8<abs("+recoPseudoRapidity("[0]")+")");
TCut jPsiTracker(recoInvMass()+">2.900&&"+recoInvMass()+"<3.25");
TCut jPsiTracks(recoInvMass("")+">2.900&&"+recoInvMass("")+"<3.25");
TCut jPsiCand("cand_mass>=2.6&&cand_mass<=3.4");
TCut sideBandCand("cand_mass>=2.0&&cand_mass<=2.5");
TCut jPsiGen("("+genInvMass()+">2.6)&&("+genInvMass()+")<3.4");

TCut hfMaxCut="Max$(CaloEnergy*(abs(CaloEta)>3))<5";

//TCut ebCut="Max$(CaloEmEnergy*(CaloIsEB))<=1.28";
//TCut eeCut="Max$(CaloEmEnergy*(CaloIsEE))<=2.97";
//TCut hbCut="Max$(CaloHadEnergy*(CaloIsHB))<=1.44";
//TCut heCut="Max$(CaloHadEnergy*(CaloIsHE))<=2.48";
//TCut hfCut="Max$(CaloHadEnergy*(CaloIsHF))<=3.62";

TCut ebCut="MaxEBEnergy<=0.71021";
TCut eeCut="MaxEEEnergy<=2.81025";
TCut hbCut="MaxHBEnergy<=1.22913";
TCut heCut="MaxHEEnergy<=2.11142";
TCut hfCut="MaxHFEnergy<=3.92785";

TCut ebNoOLCut="Max$(CaloEmEnergy*(CaloIsEB&&!CaloIsEE))<=1.28";
TCut eeNoOLCut="Max$(CaloEmEnergy*(CaloIsEE&&!CaloIsEB))<=2.97";
TCut hbNoOLCut="Max$(CaloHadEnergy*(CaloIsHB&&!CaloIsHE))<=1.44";
TCut heNoOLCut="Max$(CaloHadEnergy*(CaloIsHE&&!CaloIsHF&&!CaloIsHB))<=2.48";
TCut hfNoOLCut="Max$(CaloHadEnergy*(CaloIsHF&&!CaloIsHE))<=3.62";

//TCut ebENoOLCut="Max$(CaloEnergy*(CaloIsEB&&!CaloIsEE))<=1.28";
//TCut eeENoOLCut="Max$(CaloEnergy*(CaloIsEE&&!CaloIsEB))<=2.97";
//TCut hbENoOLCut="Max$(CaloEnergy*(CaloIsHB&&!CaloIsHE))<=1.44";
//TCut heENoOLCut="Max$(CaloEnergy*(CaloIsHE&&!CaloIsHF&&!CaloIsHB))<=2.48";
//TCut hfENoOLCut="Max$(CaloEnergy*(CaloIsHF&&!CaloIsHE))<=3.62";

TCut ebENoOLCut="Max$(CaloEnergy*(CaloIsEB&&!CaloIsEE))<=1.0325";
TCut eeENoOLCut="Max$(CaloEnergy*(CaloIsEE&&!CaloIsEB))<=2.4818";
TCut hbENoOLCut="Max$(CaloEnergy*(CaloIsHB&&!CaloIsHE))<=1.305";
TCut heENoOLCut="Max$(CaloEnergy*(CaloIsHE&&!CaloIsHF&&!CaloIsHB))<=2.3254";
TCut hfENoOLCut="Max$(CaloEnergy*(CaloIsHF&&!CaloIsHE))<=4.316";

//TCut ebRecHitCut="EBEnergyMax<=(0.2375+2.0*0.06294)";
//TCut eeRecHitCut="EEEnergyMax<=(1.204+2.0*0.4373 )";
//TCut hbRecHitCut="HBEnergyMax<=(0.9091+2.0*0.1722)";
//TCut heRecHitCut="HEEnergyMax<=(1.224+2.0*0.396)";
//TCut hfRecHitCut="HFEnergyMax<=(2.694+2.0*3.206)";

TCut hfRecHitCut="HFEnergyMax<=(3.95)";
TCut hbRecHitCut="HBEnergyMax<=(1.25)";
TCut heRecHitCut="HEEnergyMax<=(1.75)";
TCut ebRecHitCut="EBEnergyMax<=(0.45)";
TCut eeRecHitCut="EEEnergyMax<=(2.95)";

TCut ebRecHitPlusCut="EBEnergyMaxPlus<=(0.3152)";
TCut ebRecHitMinusCut="EBEnergyMaxMinus<=(0.32146)";
TCut eeRecHitPlusCut="EEEnergyMaxPlus<=(0.5702)";
TCut eeRecHitMinusCut="EEEnergyMaxMinus<=(0.5106)";
TCut hbRecHitPlusCut="HBEnergyMaxPlus<=(1.1255)";
TCut hbRecHitMinusCut="HBEnergyMaxMinus<=(1.153)";
TCut heRecHitPlusCut="HEEnergyMaxPlus<=(1.6696)";
TCut heRecHitMinusCut="HEEnergyMaxMinus<=(1.7894)";
TCut hfRecHitPlusCut="HFEnergyMaxPlus<=(7.043)";
TCut hfRecHitMinusCut="HFEnergyMaxMinus<=(6.637)";

TCut centralRecHitCut="(Sum$(Energy*(abs(Eta)<2.5))+Sum$(EBEnergy*(abs(EBEta)<2.5)))<215.0";
TCut forwardRecHitCut="(Sum$(HFEnergy*(abs(HFEta)>3)))<57.0";
TCut nTrackCut="hiSelectedTracksTree.nTracks==2";
TCut nTrackCutPP="generalTracksTree.nTracks==2";
TCut exPixCut="hiPixelTracksTree.nTracks==2";
TCut exCaloNoOLCut=ebNoOLCut&&eeNoOLCut&&hbNoOLCut&&heNoOLCut&&hfNoOLCut;
TCut exCaloENoOLCut=ebENoOLCut&&eeENoOLCut&&hbENoOLCut&&heENoOLCut&&hfENoOLCut;
TCut exCaloCut=ebCut&&eeCut&&hbCut&&heCut&&hfCut;
TCut exRecHitCut=ebRecHitCut&&eeRecHitCut&&hbRecHitCut&&heRecHitCut&&hfRecHitCut;
TCut exRecHitAsymCut=ebRecHitPlusCut&&eeRecHitPlusCut&&hbRecHitPlusCut&&heRecHitPlusCut&&hfRecHitPlusCut&&
		 ebRecHitMinusCut&&eeRecHitMinusCut&&hbRecHitMinusCut&&heRecHitMinusCut&&hfRecHitMinusCut;

TCut sameZdcLut="Run>=182052&&Run!=182785&&Run!=182324";
TCut sameZdcGain="Run>=181910&&Run!=182785&&Run!=182324";
TCut sameZdcThres="Run>=182395&&Run!=182785&&Run!=182324";
TCut zdc1nuPos("("+zdcChargeNoiseSub("pos")+")>(1558-2*454)");//from zero bias fits
TCut zdc1nuNeg("("+zdcChargeNoiseSub()+")>(2831.7-2*660.7)");//mean of 1n - 2 sigma1n
TCut zdc4nuThreshPos("("+zdcChargeNoiseSub("pos")+")<9792");//4*mean1n+2*4*sigma1n
TCut zdc4nuThreshNeg("("+zdcChargeNoiseSub()+")<16604");
TCut zdcTs4Pos("("+hdTimedIn("pos")+")==15");
TCut zdcTs4Neg("("+hdTimedIn("neg")+")==15");
TCut zdcTs5Pos("("+hdTimedIn("pos","[5]")+")==15");
TCut zdcTs5Neg("("+hdTimedIn("neg","[5]")+")==15");
TCut zdcLooseTs4Pos("((("+hdTimedIn("pos","[4]")+")==3)||(("+hdTimedIn("pos","[4]")+")==7)||(("+hdTimedIn("pos","[4]")+")==11)||(("+hdTimedIn("pos","[4]")+")==15))");
TCut zdcLooseTs4Neg("((("+hdTimedIn("neg","[4]")+")==3)||(("+hdTimedIn("neg","[4]")+")==7)||(("+hdTimedIn("neg","[4]")+")==11)||(("+hdTimedIn("neg","[4]")+")==15))");
TCut zdcLooseTs5Pos("((("+hdTimedIn("pos","[5]")+")==3)||(("+hdTimedIn("pos","[5]")+")==7)||(("+hdTimedIn("pos","[5]")+")==11)||(("+hdTimedIn("pos","[5]")+")==15))");
TCut zdcLooseTs5Neg("((("+hdTimedIn("neg","[5]")+")==3)||(("+hdTimedIn("neg","[5]")+")==7)||(("+hdTimedIn("neg","[5]")+")==11)||(("+hdTimedIn("neg","[5]")+")==15))");
TCut zdcXOr=(zdc1nuPos||zdc1nuNeg)&&!(zdc1nuPos&&zdc1nuNeg);
TCut zdcOr=(zdc1nuPos||zdc1nuNeg);
TCut zdcAdd=(zdc1nuPos&&zdc1nuNeg);

TCut innerTrackHitsCut="muon1_numberOfValidHits[0]>10&&muon2_numberOfValidHits[0]>10";
TCut pixelLayersCut="muon1_pixelLayersWithMeasurement[0]>0&&muon2_pixelLayersWithMeasurement[0]>0";
TCut normalizedX2Cut="muon1_normalizedChi2[0]<4.&&muon2_normalizedChi2[0]<4.";
TCut dBCut="muon1_dB[0]<3.&&muon2_dB[0]<3.";
TCut dZCut="abs(muon1_dz[0])<15.&&abs(muon2_dz[0])<15.";
TCut vProb="cand_vProb[0]>0.01";
TCut trkArbit="muon1_trkArbit[0]&&muon2_trkArbit[0]";
TCut muonQualityCuts=innerTrackHitsCut&&pixelLayersCut&&normalizedX2Cut&&dBCut&&trkArbit&&dZCut&&vProb;

TCut evSel=nTrackCut&&beamHaloVeto&&vertLooseCut&&cShapeCut&&exRecHitCut&&zdcXOr;
TCut evSelPreCut=nTrackCut&&muonCutsCand&&"(pass1||pass2)"&&muonQualityCuts&&exCaloENoOLCut&&zdcXOr;
TCut evSelNoHFVeto=nTrackCut&&beamHaloVeto&&vertLooseCut&&cShapeCut;
TCut evSelPP=nTrackCutPP&&vertLooseCutPP&&cShapeCut&&hfCoincVeto;
TCut evSelwMuQ=muonCutsCand&&"(pass1[0]||pass2[0])"&&muonQualityCuts&&evSel;
TCut evSelNoHFVetowMuQ=muonCutsCand&&"(pass1||pass2)"&&muonQualityCuts&&evSelNoHFVeto;
TCut evSelwMuQPP=muonCutsCand&&"(pass1||pass2)"&&muonQualityCuts&&evSelPP;
