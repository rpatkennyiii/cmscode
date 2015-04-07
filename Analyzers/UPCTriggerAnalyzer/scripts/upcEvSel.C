#include "TCut.h"
#include "TString.h"
#include "/home/mojoe137/lib/root/macros/UPCScripts/upcGenPlots.C"
#include "/home/mojoe137/lib/root/macros/ZDCScripts/zdcEnergy.C"

#ifndef __CINT__ 
  float ptBinz[]={0.0,.05,.1,.15,.2,.25,.3,.35,.4,.45,.5,.55,.6,.65,.7,.75,.8,.85,.9,.95,1.0,1.25,1.5,2.0,2.5};
  float rapBinz[]={-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,1.7,1.8,1.9,2.0,2.1,2.2,2.3};
  
  TCut genEvSel("L1_SingleMuOpen_BptxAND&&HLT_HIZeroBiasPixel_SingleTrack_v1");
  
  TCut beamHaloVeto("!(L1Tech_BSC_halo_beam1_inner.v0||L1Tech_BSC_halo_beam1_outer.v0||L1Tech_BSC_halo_beam2_inner.v0||L1Tech_BSC_halo_beam2_outer.v0)");
  TCut vertCut("!hiSelectedVertexTree.isFake && abs(hiSelectedVertexTree.pVz) <= 25 && sqrt(pow(hiSelectedVertexTree.pVx,2)+pow(hiSelectedVertexTree.pVy,2)) <= 2 && hiSelectedVertexTree.nTracks >= 2");
  TCut vertLooseCut("!hiSelectedVertexTree.isFake && abs(hiSelectedVertexTree.pVz) <= 25 && sqrt(pow(hiSelectedVertexTree.pVx,2)+pow(hiSelectedVertexTree.pVy,2)) <= 2");
  TCut vertLooseCutPP("!offlinePrimaryVerticesTree.isFake[0] && abs(offlinePrimaryVerticesTree.z[0]) <= 25 && sqrt(pow(offlinePrimaryVerticesTree.x[0],2)+pow(offlinePrimaryVerticesTree.y[0],2)) <= 2");
  TCut hfCoincVeto("!(Sum$((CaloEta>=3)&&(CaloEnergy>3))>=3&&Sum$((CaloEta<=-3)&&(CaloEnergy>3))>=3)");
  TCut hfCoincTightVeto("!(Sum$((CaloEta>=3)&&(CaloEnergy>=3))>0&&Sum$((CaloEta<=-3)&&(CaloEnergy>3))>0)");
  TCut hfSigVeto("!(Sum$((CaloEta>=3)&&(CaloEnergy>3))>=3||Sum$((CaloEta<=-3)&&(CaloEnergy>3))>=3)");
  //	TCut minBiasVeto="!L1_HcalHfCoincPmORBscMinBiasThresh1_BptxAND_instance1";
  TCut minBiasVeto="!L1_BscMinBiasThreshold2_BptxAND";
  TCut cShapeCut="Accept";
  
  TCut candRapidityCut="abs(abs(cand_rapidity)-2)<=0.4";
  TCut candRapidityCut2="abs(abs(cand_rapidity)-2)<=0.3";
  TCut candRapidityCut3="abs(abs(cand_rapidity)-2.1)<=0.2";
  TCut candRapidityCut4="abs(abs(cand_rapidity)-2.2)<=0.10";
  TCut candRapidityCut5="abs(abs(cand_rapidity)-2.05)<=0.25";
  TCut candCoherent="cand_pt<0.200";
  TCut candCoherentOld="cand_pt<0.150";
  TCut candInCoherent="cand_pt>0.200&&cand_pt<1.5";
  TCut candInCoherent2="cand_pt>0.200&&cand_pt<1.0";
  
  TCut muonCutsTracker("nTrackerMuon==2&&qoverpTracker[0]/qoverpTracker[1]<0");
  TCut muonCutsGlobal("nGlobalMuon==2&&qoverpGlobal[0]/qoverpGlobal[1]<0");
  TCut muonCutsCand("cand_charge[0]==0&&acceptedCand==1");
  TCut muonCutsTrack("hiSelectedTracksTree.nTracks==2&&hiSelectedTracksTree.qoverp[0]/hiSelectedTracksTree.qoverp[1]<0");
  TCut muonDaughterCuts("(abs(abs(muon1_rapidity)-2.25)<0.15&&abs(muon1_pt-1.55)<0.15)&&(abs(abs(muon2_rapidity)-2.25)<0.15&&abs(muon2_pt-1.55)<0.15)");
  TCut muonDaughterOrCuts("(abs(abs(muon1_rapidity)-2.25)<0.15&&abs(muon1_pt-1.55)<0.15)||(abs(abs(muon2_rapidity)-2.25)<0.15&&abs(muon2_pt-1.55)<0.15)");
  
  TCut accRapTracker("abs("+recoPseudoRapidity("Tracker[1]")+")<2.5&&1.4<abs("+recoPseudoRapidity("Tracker[1]")+")&&abs("+recoPseudoRapidity("Tracker[0]")+")<2.5&&1.8<abs("+recoPseudoRapidity("Tracker[0]")+")");
  TCut accRapTracks("abs("+recoPseudoRapidity("[1]")+")<2.5&&1.4<abs("+recoPseudoRapidity("[1]")+")&&abs("+recoPseudoRapidity("[0]")+")<2.5&&1.8<abs("+recoPseudoRapidity("[0]")+")");
  TCut jPsiTracker(recoInvMass()+">2.900&&"+recoInvMass()+"<3.25");
  TCut jPsiTracks(recoInvMass("")+">2.900&&"+recoInvMass("")+"<3.25");
  TCut jPsiCand("cand_mass>=2.6&&cand_mass<=3.4");
  TCut jPsiCand2("cand_mass>=2.8&&cand_mass<=3.3");
  TCut jPsiCandPAS("cand_mass>=2.6&&cand_mass<=3.5");
  TCut jPsiCandRapidityPAS("abs(abs(cand_rapidity)-2.05)<0.25");
  TCut gammaCand("cand_mass>=4.0&&cand_mass<=8.0");
  TCut sideBandCand("cand_mass>=2.0&&cand_mass<=2.5");
  TCut sideBandCand2("(cand_mass>=2.6&&cand_mass<=2.8)||(cand_mass>=3.3&&cand_mass<=3.5)");
  TCut jPsiGen("("+genInvMass()+">2.6)&&("+genInvMass()+")<3.5");
  TCut gammaGen("("+genInvMass()+">4)&&("+genInvMass()+")<8");
  
  TCut hfMaxCut="Max$(CaloEnergy*(abs(CaloEta)>3))<5";
  
  //TCut ebCut="Max$(CaloEmEnergy*(CaloIsEB))<=1.28";
  //TCut eeCut="Max$(CaloEmEnergy*(CaloIsEE))<=2.97";
  //TCut hbCut="Max$(CaloHadEnergy*(CaloIsHB))<=1.44";
  //TCut heCut="Max$(CaloHadEnergy*(CaloIsHE))<=2.48";
  //TCut hfCut="Max$(CaloHadEnergy*(CaloIsHF))<=3.62";
  
  TCut ebCut="MaxEBEnergy<=0.75";
  TCut eeCut="MaxEEEnergy<=2.75";
  TCut hbCut="MaxHBEnergy<=1.25";
  TCut heCut="MaxHEEnergy<=2.25";
  TCut hfCut="MaxHFEnergy<=4.25";
  
  TCut ebNoOLCut="Max$(CaloEmEnergy*(CaloIsEB&&!CaloIsEE))<=1.28";
  TCut eeNoOLCut="Max$(CaloEmEnergy*(CaloIsEE&&!CaloIsEB))<=2.97";
  TCut hbNoOLCut="Max$(CaloHadEnergy*(CaloIsHB&&!CaloIsHE))<=1.44";
  TCut heNoOLCut="Max$(CaloHadEnergy*(CaloIsHE&&!CaloIsHF&&!CaloIsHB))<=2.48";
  TCut hfNoOLCut="Max$(CaloHadEnergy*(CaloIsHF&&!CaloIsHE))<=3.62";
  
  TCut ebENoOLCut="Max$(CaloEnergy*(CaloIsEB&&!CaloIsEE))<=1.25";
  TCut eeENoOLCut="Max$(CaloEnergy*(CaloIsEE&&!CaloIsEB))<=2.75";
  TCut hbENoOLCut="Max$(CaloEnergy*(CaloIsHB&&!CaloIsHE))<=1.25";
  TCut heENoOLCut="Max$(CaloEnergy*(CaloIsHE&&!CaloIsHF&&!CaloIsHB))<=2.25";
  TCut hfENoOLCut="Max$(CaloEnergy*(CaloIsHF&&!CaloIsHE))<=4.25";
  
  TCut ebRecHitCut="EBEnergyMax<=(0.45)";
  TCut eeRecHitCut="EEEnergyMax<=(2.95)";
  TCut hbRecHitCut="HBEnergyMax<=(1.25)";
  TCut heRecHitCut="HEEnergyMax<=(1.75)";
  TCut hfRecHitCut="HFEnergyMax<=(3.85)";
  
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
 //1n with 2 sigma cut 
  TCut zdc1nuPos=TString("("+zdcChargeNoiseSub("pos")+")>(1558-2*454)").Data();//from zero bias fits
  TCut zdc1nuNeg=TString("("+zdcChargeNoiseSub()+")>(2831.7-2*660.7)").Data();//mean of 1n - 2 sigma1n
  TCut zdc1nNeg45=TString(zdcSignalThreshold45("neg","2")).Data();
  TCut zdc1nPos45=TString(zdcSignalThreshold45("pos","2")).Data();
  TCut zdc1nNeg45New=TString(zdcSignalThreshold45New("neg","2")).Data();
  TCut zdc1nPos45New=TString(zdcSignalThreshold45New("pos","2")).Data();
  TCut zdc1nNeg45EMb=TString("("+zdcChargeNoiseSub5EMBoost()+")>(2537.2-2*589.9)").Data();//mean of 1n - 2 sigma1n
  TCut zdc1nPos45EMb=TString("("+zdcChargeNoiseSub5EMBoost("pos")+")>(1692-2*577.4)").Data();//from zero bias fits
//1n with 3 sigma cut 
  TCut zdc1nuPos3Sigma=TString("("+zdcChargeNoiseSub("pos")+")>(1558-3*454)").Data();//from zero bias fits
  TCut zdc1nuNeg3Sigma=TString("("+zdcChargeNoiseSub()+")>(2831.7-3*660.7)").Data();//mean of 1n - 2 sigma1n
  TCut zdc1nNeg453Sigma=TString(zdcSignalThreshold45("neg","3")).Data();
  TCut zdc1nPos453Sigma=TString(zdcSignalThreshold45("pos","3")).Data();
  TCut zdc1nNeg45New3Sigma=TString(zdcSignalThreshold45New("neg","3")).Data();
  TCut zdc1nPos45New3Sigma=TString(zdcSignalThreshold45New("pos","3")).Data();
  TCut zdc1nNeg45EMb3Sigma=TString("("+zdcChargeNoiseSub5EMBoost()+")>(2537.2-3*589.9)").Data();//mean of 1n - 2 sigma1n
  TCut zdc1nPos45EMb3Sigma=TString("("+zdcChargeNoiseSub5EMBoost("pos")+")>(1692-3*577.4)").Data();//from zero bias fits
//1n with 1 sigma cut 
  TCut zdc1nuPos1Sigma=TString("("+zdcChargeNoiseSub("pos")+")>(1558-3*454)").Data();//from zero bias fits
  TCut zdc1nuNeg1Sigma=TString("("+zdcChargeNoiseSub()+")>(2831.7-3*660.7)").Data();//mean of 1n - 2 sigma1n
  TCut zdc1nNeg451Sigma=TString(zdcSignalThreshold45("neg","1")).Data();
  TCut zdc1nPos451Sigma=TString(zdcSignalThreshold45("pos","1")).Data();
  TCut zdc1nNeg45New1Sigma=TString(zdcSignalThreshold45New("neg","1")).Data();
  TCut zdc1nPos45New1Sigma=TString(zdcSignalThreshold45New("pos","1")).Data();
  TCut zdc1nNeg45EMb1Sigma=TString("("+zdcChargeNoiseSub5EMBoost()+")>(2537.2-3*589.9)").Data();//mean of 1n - 2 sigma1n
  TCut zdc1nPos45EMb1Sigma=TString("("+zdcChargeNoiseSub5EMBoost("pos")+")>(1692-3*577.4)").Data();//from zero bias fits
//Xn > 1n with 2 sigma cut 
  TCut zdcGt1nuNeg=TString("("+zdcChargeNoiseSub()+")>(2831.7+2*660.7)").Data();//mean of 1n - 2 sigma1n
  TCut zdcGt1nuPos=TString("("+zdcChargeNoiseSub("pos")+")>(1558+2*454)").Data();//from zero bias fits
  TCut zdcGt1nNeg45=TString(zdcSignalThreshold45("neg","-2")).Data();
  TCut zdcGt1nPos45=TString(zdcSignalThreshold45("pos","-2")).Data();
  TCut zdcGt1nNeg45New=TString(zdcSignalThreshold45New("neg","-2")).Data();
  TCut zdcGt1nPos45New=TString(zdcSignalThreshold45New("pos","-2")).Data();
  TCut zdcGt1nNeg45EMb=TString("("+zdcChargeNoiseSub5EMBoost()+")>(2537.2+2*589.9)").Data();//mean of 1n - 2 sigma1n
  TCut zdcGt1nPos45EMb=TString("("+zdcChargeNoiseSub5EMBoost("pos")+")>(1692+2*577.4)").Data();//from zero bias fits
//Xn > 1n with 3 sigma cut 
  TCut zdcGt1nuNeg3Sigma=TString("("+zdcChargeNoiseSub()+")>(2831.7+3*660.7)").Data();//mean of 1n - 2 sigma1n
  TCut zdcGt1nuPos3Sigma=TString("("+zdcChargeNoiseSub("pos")+")>(1558+3*454)").Data();//from zero bias fits
  TCut zdcGt1nNeg453Sigma=TString(zdcSignalThreshold45("neg","-3")).Data();
  TCut zdcGt1nPos453Sigma=TString(zdcSignalThreshold45("pos","-3")).Data();
  TCut zdcGt1nNeg45New3Sigma=TString(zdcSignalThreshold45New("neg","-3")).Data();
  TCut zdcGt1nPos45New3Sigma=TString(zdcSignalThreshold45New("pos","-3")).Data();
  TCut zdcGt1nNeg45EMb3Sigma=TString("("+zdcChargeNoiseSub5EMBoost()+")>(2537.2+3*589.9)").Data();//mean of 1n - 2 sigma1n
  TCut zdcGt1nPos45EMb3Sigma=TString("("+zdcChargeNoiseSub5EMBoost("pos")+")>(1692+3*577.4)").Data();//from zero bias fits
//Xn > 1n with 1 sigma cut 
  TCut zdcGt1nuNeg1Sigma=TString("("+zdcChargeNoiseSub()+")>(2831.7+1*660.7)").Data();//mean of 1n - 2 sigma1n
  TCut zdcGt1nuPos1Sigma=TString("("+zdcChargeNoiseSub("pos")+")>(1558+1*454)").Data();//from zero bias fits
  TCut zdcGt1nNeg451Sigma=TString(zdcSignalThreshold45("neg","-1")).Data();
  TCut zdcGt1nPos451Sigma=TString(zdcSignalThreshold45("pos","-1")).Data();
  TCut zdcGt1nNeg45New1Sigma=TString(zdcSignalThreshold45New("neg","-1")).Data();
  TCut zdcGt1nPos45New1Sigma=TString(zdcSignalThreshold45New("pos","-1")).Data();
  TCut zdcGt1nNeg45EMb1Sigma=TString("("+zdcChargeNoiseSub5EMBoost()+")>(2537.2+1*589.9)").Data();//mean of 1n - 2 sigma1n
  TCut zdcGt1nPos45EMb1Sigma=TString("("+zdcChargeNoiseSub5EMBoost("pos")+")>(1692+1*577.4)").Data();//from zero bias fits

  TCut zdc0nPos1Sig=TString(zdcNoiseThreshold( "pos","1")).Data();
  TCut zdc0nNeg1Sig=TString(zdcNoiseThreshold("neg","1")).Data();
  TCut zdc0nPos2Sig=TString(zdcNoiseThreshold("pos","2")).Data();
  TCut zdc0nNeg2Sig=TString(zdcNoiseThreshold("neg","2")).Data();
  TCut zdc0nPos3Sig=TString(zdcNoiseThreshold("pos","3")).Data();
  TCut zdc0nNeg3Sig=TString(zdcNoiseThreshold("neg","3")).Data();
  
  TCut zdc4nuThreshPos=TString("("+zdcChargeNoiseSub("pos")+")<9792").Data();//4*mean1n+2*4*sigma1n
  TCut zdc4nuThreshNeg=TString("("+zdcChargeNoiseSub()+")<16604").Data();
  
  TCut zdcTs4Pos=TString("("+hdTimedIn("pos")+")==15").Data();
  TCut zdcTs4Neg=TString("("+hdTimedIn("neg")+")==15").Data();
  TCut zdcTs5Pos=TString("("+hdTimedIn("pos","[5]")+")==15").Data();
  TCut zdcTs5Neg=TString("("+hdTimedIn("neg","[5]")+")==15").Data();
  
  TCut zdcLooseTs4Pos=TString("((("+hdTimedIn("pos","[4]")+")==3)||(("+hdTimedIn("pos","[4]")+")==7)||(("+hdTimedIn("pos","[4]")+")==11)||(("+hdTimedIn("pos","[4]")+")==15))").Data();
  TCut zdcLooseTs4Neg=TString("((("+hdTimedIn("neg","[4]")+")==3)||(("+hdTimedIn("neg","[4]")+")==7)||(("+hdTimedIn("neg","[4]")+")==11)||(("+hdTimedIn("neg","[4]")+")==15))").Data();
  TCut zdcLooseTs5Pos=TString("((("+hdTimedIn("pos","[5]")+")==3)||(("+hdTimedIn("pos","[5]")+")==7)||(("+hdTimedIn("pos","[5]")+")==11)||(("+hdTimedIn("pos","[5]")+")==15))").Data();
  TCut zdcLooseTs5Neg=TString("((("+hdTimedIn("neg","[5]")+")==3)||(("+hdTimedIn("neg","[5]")+")==7)||(("+hdTimedIn("neg","[5]")+")==11)||(("+hdTimedIn("neg","[5]")+")==15))").Data();
  
  TCut zdcXn0n=(zdc1nuPos||zdc1nuNeg)&&!(zdc1nuPos&&zdc1nuNeg);
  TCut zdcXn=(zdc1nuPos||zdc1nuNeg);
  TCut zdcXnXn=(zdc1nuPos&&zdc1nuNeg);
  TCut zdc1n0n=zdcXn0n&&!zdcGt1nuPos&&!zdcGt1nuNeg;
  TCut zdc1n1n=zdcXnXn&&!zdcGt1nuPos&&!zdcGt1nuNeg;
  
  TCut zdcXn0nPrime=(zdc1nPos45&&zdc0nNeg2Sig)||(zdc1nNeg45&&zdc0nPos2Sig);
  TCut zdcXnPrime=(zdc1nPos45||zdc1nNeg45);
  TCut zdcXnXnPrime=(zdc1nPos45)&&(zdc1nNeg45);
  TCut zdc1n0nPrime=zdcXn0nPrime&&!zdcGt1nPos45&&!zdcGt1nNeg45;
  TCut zdc1n1nPrime=zdcXnXnPrime&&!zdcGt1nPos45&&!zdcGt1nNeg45;
  
  TCut zdcXn0nPrimeNew=(zdc1nPos45New&&zdc0nNeg2Sig)||(zdc1nNeg45New&&zdc0nPos2Sig);
  TCut zdcXnPrimeNew=(zdc1nPos45New||zdc1nNeg45New);
  TCut zdcXnXnPrimeNew=(zdc1nPos45New)&&(zdc1nNeg45New);
  TCut zdc1n0nPrimeNew=zdcXn0nPrime&&!zdcGt1nPos45New&&!zdcGt1nNeg45New;
  TCut zdc1n1nPrimeNew=zdcXnXnPrime&&!zdcGt1nPos45New&&!zdcGt1nNeg45New;

  TCut zdcXn0nPrimeEMb=(zdc1nPos45EMb&&zdc0nNeg2Sig)||(zdc1nNeg45EMb&&zdc0nPos2Sig);
  TCut zdcXnPrimeEMb=(zdc1nPos45EMb||zdc1nNeg45EMb);
  TCut zdcXnXnPrimeEMb=(zdc1nPos45EMb)&&(zdc1nNeg45EMb);
  TCut zdc1n0nPrimeEMb=zdcXn0nPrime&&!zdcGt1nPos45EMb&&!zdcGt1nNeg45EMb;
  TCut zdc1n1nPrimeEMb=zdcXnXnPrime&&!zdcGt1nPos45EMb&&!zdcGt1nNeg45EMb;

  TCut zdcXn0nPrime3Sigma=(zdc1nPos453Sigma&&zdc0nNeg2Sig)||(zdc1nNeg453Sigma&&zdc0nPos2Sig);
  TCut zdcXnPrime3Sigma=(zdc1nPos453Sigma||zdc1nNeg453Sigma);
  TCut zdcXnXnPrime3Sigma=(zdc1nPos453Sigma)&&(zdc1nNeg453Sigma);
  TCut zdc1n0nPrime3Sigma=zdcXn0nPrime&&!zdcGt1nPos453Sigma&&!zdcGt1nNeg453Sigma;
  TCut zdc1n1nPrime3Sigma=zdcXnXnPrime&&!zdcGt1nPos453Sigma&&!zdcGt1nNeg453Sigma;

  TCut zdcXn0nPrime1Sigma=(zdc1nPos451Sigma&&zdc0nNeg2Sig)||(zdc1nNeg451Sigma&&zdc0nPos2Sig);
  TCut zdcXnPrime1Sigma=(zdc1nPos451Sigma||zdc1nNeg451Sigma);
  TCut zdcXnXnPrime1Sigma=(zdc1nPos451Sigma)&&(zdc1nNeg451Sigma);
  TCut zdc1n0nPrime1Sigma=zdcXn0nPrime&&!zdcGt1nPos451Sigma&&!zdcGt1nNeg451Sigma;
  TCut zdc1n1nPrime1Sigma=zdcXnXnPrime&&!zdcGt1nPos451Sigma&&!zdcGt1nNeg451Sigma;
  
  TCut zdcSameSide=("cand_rapidity<0"&&zdc1nuNeg)||("cand_rapidity>0"&&zdc1nuPos);
  TCut zdcOppSide=("cand_rapidity>0"&&zdc1nuNeg)||("cand_rapidity<0"&&zdc1nuPos);
  TCut zdcSameSidePrime=("cand_rapidity<0"&&zdc1nNeg45)||("cand_rapidity>0"&&zdc1nuPos);
  TCut zdcOppSidePrime=("cand_rapidity>0"&&zdc1nNeg45)||("cand_rapidity<0"&&zdc1nuPos);
  
  TCut goodMuonCutMu1="muon1_isGoodMuon";
  TCut goodMuonCutMu2="muon2_isGoodMuon";
  TCut innerTrackHitsCut="muon1_numberOfValidHits>10&&muon2_numberOfValidHits>10";
  TCut pixelLayersCut="muon1_pixelLayersWithMeasurement>0&&muon2_pixelLayersWithMeasurement>0";
  TCut normalizedX2Cut="muon1_normalizedChi2<4.&&muon2_normalizedChi2<4.";
  TCut pixelLayersCut2="muon1_pixelLayersWithMeasurement>1&&muon2_pixelLayersWithMeasurement>1";
  TCut normalizedX2Cut2="muon1_normalizedChi2<1.8&&muon2_normalizedChi2<1.8";
  TCut dBCut="muon1_dB<3.&&muon2_dB<3.";
  TCut dZCut2="abs(muon1_dz-hiSelectedVertexTree.pVz)<30.&&abs(muon2_dz-hiSelectedVertexTree.pVz)<30.";
  TCut dZCut2PP="abs(muon1_dz-offlinePrimaryVerticesTree.z[0])<30.&&abs(muon2_dz-offlinePrimaryVerticesTree.z[0])<30.";
  TCut dZCut="abs(muon1_dz)<15.&&abs(muon2_dz)<15.";
  TCut vProb="cand_vProb>0.01";
  TCut trkArbit="muon1_trkArbit&&muon2_trkArbit";
  TCut matchedMu1="muon1_l1Quality>=2&&muon1_l1DeltaR<=0.3";
  TCut matchedMu2="muon2_l1Quality>=2&&muon2_l1DeltaR<=0.3";
  // soft id
  TCut trackerLayersSoftMu1="muon1_trackerLayersWithMeasurement>5";
  TCut pixelLayersCutSoftMu1="muon1_pixelLayersWithMeasurement>0";
  TCut normalizedX2CutSoftMu1="muon1_normalizedChi2<3";
  TCut isHighPurityCutSoftMu1="muon1_isHighPurity";
  TCut isHighPurityCutSoftMu2="muon2_isHighPurity";
  TCut trackerLayersSoftMu2="muon2_trackerLayersWithMeasurement>5";
  TCut pixelLayersCutSoftMu2="muon2_pixelLayersWithMeasurement>0";
  TCut normalizedX2CutSoftMu2="muon2_normalizedChi2<3";
  TCut dBCutSoftMu1="muon1_dB<0.3";
  TCut dZCutSoftMu1="abs(muon1_dz-hiSelectedVertexTree.pVz)<20.";
  TCut dZCutSoftMu1PP="abs(muon1_dz-offlinePrimaryVerticesTree.z[0])<20.";
  TCut dBCutSoftMu2="muon2_dB<0.3";
  TCut dZCutSoftMu2="abs(muon2_dz-hiSelectedVertexTree.pVz)<20.";
  TCut dZCutSoftMu2PP="abs(muon2_dz-offlinePrimaryVerticesTree.z[0])<20.";
  TCut goodMuonCut="muon1_isGoodMuon&&muon2_isGoodMuon";
  TCut trackerLayersSoft=trackerLayersSoftMu2&&trackerLayersSoftMu1;
  TCut pixelLayersCutSoft=pixelLayersCutSoftMu2&&pixelLayersCutSoftMu1;
  TCut normalizedX2CutSoft=normalizedX2CutSoftMu2&&normalizedX2CutSoftMu1;
  TCut isHighPurityCutSoft=isHighPurityCutSoftMu2&&isHighPurityCutSoftMu1;
  TCut dBCutSoft=dBCutSoftMu2&&dBCutSoftMu1;
  TCut dZCutSoft=dZCutSoftMu2&&dZCutSoftMu1;
  TCut dZCutSoftPP=dZCutSoftMu2PP&&dZCutSoftMu1PP;
  // muon detectable and triggerable kenimatic cuts
  TCut muonDetCut="(muon1_pt>1.45||(abs(muon1_eta)>2.1&&muon1_pt>1.40))&&(muon2_pt>1.45||(abs(muon2_eta)>2.1&&muon2_pt>1.40))&&abs(abs(muon1_eta)-2)<0.4&&abs(abs(muon2_eta)-2)<0.4";
  TCut muon1DetCutLinear20="muon1_pt>(-0.15/0.1*(abs(muon1_eta)-1.6)+1.35)&&(abs(muon1_eta)<1.9||muon1_pt>0.90)&&(abs(muon1_eta)<2.2||muon1_pt>(0.15/0.1)*(abs(muon1_eta)-2.2)+0.9)&&abs(abs(muon1_eta)-2)<0.4";
  TCut muon2DetCutLinear20="muon2_pt>(-0.15/0.1*(abs(muon2_eta)-1.6)+1.35)&&(abs(muon2_eta)<1.9||muon2_pt>0.90)&&(abs(muon2_eta)<2.2||muon2_pt>(0.15/0.1)*(abs(muon2_eta)-2.2)+0.9)&&abs(abs(muon2_eta)-2)<0.4";
  TCut muon1DetCutFlat20="muon1_pt>0.90&&abs(abs(muon1_eta)-2.05)<0.25";
  TCut muon2DetCutFlat20="muon2_pt>0.90&&abs(abs(muon2_eta)-2.05)<0.25";
  TCut muon1DetCutStep20="(muon1_pt>1.20&&abs(abs(muon1_eta)-1.65)<0.05)||(muon1_pt>1.05&&abs(abs(muon1_eta)-1.75)<0.05)||(muon1_pt>0.90&&abs(abs(muon1_eta)-2.05)<0.25)||(muon1_pt>1.05&&abs(abs(muon1_eta)-2.35)<0.05)";
  TCut muon2DetCutStep20="(muon2_pt>1.20&&abs(abs(muon2_eta)-1.65)<0.05)||(muon2_pt>1.05&&abs(abs(muon2_eta)-1.75)<0.05)||(muon2_pt>0.90&&abs(abs(muon2_eta)-2.05)<0.25)||(muon2_pt>1.05&&abs(abs(muon2_eta)-2.35)<0.05)";
  TCut muon1DetCutStep20PP="(muon1_pt>0.155&&abs(abs(muon1_eta)-1.35)<0.05)||(muon1_pt>1.30&&abs(abs(muon1_eta)-1.45)<0.05)||(muon1_pt>1.2&&abs(abs(muon1_eta)-1.55)<0.05)||(muon1_pt>1.10&&abs(abs(muon1_eta)-1.65)<0.05)||(muon1_pt>1.0&&abs(abs(muon1_eta)-1.75)<0.05)||(muon1_pt>0.90&&abs(abs(muon1_eta)-1.85)<0.05)||(muon1_pt>0.85&&abs(abs(muon1_eta)-1.95)<0.05)||(muon1_pt>0.80&&abs(abs(muon1_eta)-2.15)<0.15)||(muon1_pt>0.9&&abs(abs(muon1_eta)-2.35)<0.05)";
  TCut muon2DetCutStep20PP="(muon2_pt>0.155&&abs(abs(muon2_eta)-1.35)<0.05)||(muon2_pt>1.30&&abs(abs(muon2_eta)-1.45)<0.05)||(muon2_pt>1.2&&abs(abs(muon2_eta)-1.55)<0.05)||(muon2_pt>1.10&&abs(abs(muon2_eta)-1.65)<0.05)||(muon2_pt>1.0&&abs(abs(muon2_eta)-1.75)<0.05)||(muon2_pt>0.90&&abs(abs(muon2_eta)-1.85)<0.05)||(muon2_pt>0.85&&abs(abs(muon2_eta)-1.95)<0.05)||(muon2_pt>0.80&&abs(abs(muon2_eta)-2.15)<0.15)||(muon2_pt>0.9&&abs(abs(muon2_eta)-2.35)<0.05)";
  TCut muon1TrigJPsiCoCut="((abs(abs(muon1_eta)-2.05))<0.05&&muon1_pt>1.5)||((abs(abs(muon1_eta)-2.20))<0.10&&muon1_pt>1.35)||((abs(abs(muon1_eta)-2.35))<0.05&&muon1_pt>1.2)";
  TCut muon2TrigJPsiCoCut="((abs(abs(muon2_eta)-2.05))<0.05&&muon2_pt>1.5)||((abs(abs(muon2_eta)-2.20))<0.10&&muon2_pt>1.35)||((abs(abs(muon2_eta)-2.35))<0.05&&muon2_pt>1.2)";
  TCut muon1TrigTnPCut="abs(abs(muon1_eta)-2.1)<0.3&&abs(muon1_pt-1.55)<0.25";
  TCut muon2TrigTnPCut="abs(abs(muon2_eta)-2.1)<0.3&&abs(muon2_pt-1.55)<0.25";
  TCut muon1TrigTnPCutPP="abs(abs(muon1_eta)-1.8)<0.6&&abs(muon1_pt-1.5)<0.3";
  TCut muon2TrigTnPCutPP="abs(abs(muon2_eta)-1.8)<0.6&&abs(muon2_pt-1.5)<0.3";
  TCut muon1TrigTnPAllCut="abs(abs(muon1_eta)-2.1)<0.3&&muon1_pt>1.3";
  TCut muon2TrigTnPAllCut="abs(abs(muon2_eta)-2.1)<0.3&&muon2_pt>1.3";
  TCut muon1TrigGammaCut="((abs(abs(muon1_eta)-1.65))<0.05&&muon1_pt>2.25)||((abs(abs(muon1_eta)-1.75))<0.05&&muon1_pt>2.1)||((abs(abs(muon1_eta)-1.85))<0.05&&muon1_pt>1.80)||((abs(abs(muon1_eta)-1.95))<0.05&&muon1_pt>1.65)||((abs(abs(muon1_eta)-2.05))<0.05&&muon1_pt>1.5)||((abs(abs(muon1_eta)-2.20))<0.10&&muon1_pt>1.35)||((abs(abs(muon1_eta)-2.35))<0.05&&muon1_pt>1.2)";
  TCut muon2TrigGammaCut="((abs(abs(muon2_eta)-1.65))<0.05&&muon2_pt>2.25)||((abs(abs(muon2_eta)-1.75))<0.05&&muon2_pt>2.1)||((abs(abs(muon2_eta)-1.85))<0.05&&muon2_pt>1.80)||((abs(abs(muon2_eta)-1.95))<0.05&&muon2_pt>1.65)||((abs(abs(muon2_eta)-2.05))<0.05&&muon2_pt>1.5)||((abs(abs(muon2_eta)-2.20))<0.10&&muon2_pt>1.35)||((abs(abs(muon2_eta)-2.35))<0.05&&muon2_pt>1.2)";
  TCut muonDetEtaCut="abs(abs(muon1_eta)-2)<0.4&&abs(abs(muon2_eta)-2)<0.4";
  //combos 
  TCut muonQualityCuts=innerTrackHitsCut&&pixelLayersCut&&normalizedX2Cut&&dBCut&&trkArbit&&dZCut&&vProb;
  TCut muonQualityCuts2=innerTrackHitsCut&&pixelLayersCut2&&normalizedX2Cut2&&dBCut&&dZCut2&&goodMuonCut&&vProb;
  TCut qualitySoftId=goodMuonCut&&trackerLayersSoft&&pixelLayersCutSoft&&normalizedX2CutSoft&&dZCutSoft&&dBCutSoft;
  TCut qualitySoftIdPP=goodMuonCut&&trackerLayersSoft&&pixelLayersCutSoft&&normalizedX2CutSoft&&dZCutSoftPP&&dBCutSoft;
  TCut qualitySoftIdPPb=goodMuonCut&&trackerLayersSoft&&pixelLayersCutSoft&&isHighPurityCutSoft&&dZCutSoftPP&&dBCutSoft;
  
  TCut trackerGoodMuon1Cut="isGoodMuonTracker";
  TCut trackerInnerTrackHitsCut="numberOfValidTrackerHitsTracker>10";
  TCut trackerInnerTrackHitsCutTight="numberOfValidTrackerHitsTracker>10";
  TCut trackerPixelLayersCut2="pixelLayersWithMeasurementTracker>1";
  TCut trackerNormalizedX2Cut2="chi2Tracker/ndofTracker<1.8";
  TCut trackerNormalizedX2CutTight="chi2Tracker/ndofTracker<1.4";
  TCut trackerDBCut="sqrt(pow(xTracker-hiSelectedVertexTree.pVx,2)+pow(yTracker-hiSelectedVertexTree.pVy,2))<3.";
  TCut trackerDZCut2="abs(zTracker-hiSelectedVertexTree.pVz)<30.";
  TCut trackerQualityCuts2=trackerInnerTrackHitsCut&&trackerPixelLayersCut2&&trackerNormalizedX2Cut2&&trackerDBCut&&trackerDZCut2&&trackerGoodMuon1Cut;
  TCut trackerQualityCutsTight=trackerInnerTrackHitsCutTight&&trackerPixelLayersCut2&&trackerNormalizedX2CutTight&&trackerDBCut&&trackerDZCut2&&trackerGoodMuon1Cut;
  //soft id
  TCut trackerGoodMuonCut="isGoodMuonTracker";
  TCut trackerTrackerLayers="trackerLayersWithMeasurementTracker>5";
  TCut trackerNormalizedX2CutSoft="chi2Tracker/ndofTracker<3";
  TCut trackerPixelLayersCutSoft="pixelLayersWithMeasurementTracker>0";
  TCut trackerDBCutSoft="sqrt(pow(xTracker-hiSelectedVertexTree.pVx,2)+pow(yTracker-hiSelectedVertexTree.pVy,2))<0.3";
  TCut trackerDZCutSoft="abs(zTracker-hiSelectedVertexTree.pVz)<20.";
  TCut GoodMuonCut="isGoodMuon";
  TCut TrackerLayers="trackerLayersWithMeasurement>5";
  TCut NormalizedX2CutSoft="chi2/ndof<3";
  TCut PixelLayersCutSoft="pixelLayersWithMeasurement>0";
  TCut DBCutSoft="sqrt(pow(x-offlinePrimaryVerticesTree.x[0],2)+pow(y-offlinePrimaryVerticesTree.y[0],2))<0.3";
  TCut DZCutSoft="abs(z-offlinePrimaryVerticesTree.z[0])<20.";
  TCut trackerQualitySoftId=trackerTrackerLayers&&trackerDZCutSoft&&trackerDBCutSoft&&trackerNormalizedX2CutSoft&&trackerPixelLayersCutSoft&&trackerGoodMuonCut;
  TCut QualitySoftId=TrackerLayers&&DZCutSoft&&DBCutSoft&&NormalizedX2CutSoft&&PixelLayersCutSoft&&GoodMuonCut;
  //trigger matching
  TCut trackerMatchedMu="l1QualityTracker>=2&&l1DeltaRTracker<=0.3";
  TCut trackerLeadingMu="(pTracker*cos(lambdaTracker)==Max$(pTracker*cos(lambdaTracker)))&&l1QualityTracker>=2&&l1DeltaRTracker<=0.3";
  TString softId=trackerQualitySoftId.GetTitle();
  TCut trackerLeadingQualityMu=TString("(pTracker*cos(lambdaTracker)*int("+softId+"))==Max$(pTracker*cos(lambdaTracker)*int("+softId+"))").Data();
  TCut trackerLeadingQualityMatchedMu=TString("(pTracker*cos(lambdaTracker)*int("+softId+"))==Max$(pTracker*cos(lambdaTracker)*int("+softId+"))&&l1QualityTracker>=2&&l1DeltaRTracker<=0.3").Data();
  TCut trackerLeadingQualityTriggerMu=TString("(pTracker*cos(lambdaTracker)*int("+softId+"))==Max$(pTracker*cos(lambdaTracker)*int("+softId+"))&&L1_SingleMuOpen_BptxAND").Data();
  
  TCut evSel=nTrackCut&&beamHaloVeto&&vertLooseCut&&cShapeCut&&exRecHitCut&&zdcXn0n;
  TCut evSelPreCut=nTrackCut&&muonCutsCand&&"(pass1||pass2)"&&muonQualityCuts&&exCaloENoOLCut&&zdcXn0n;
  TCut evSelNoHFVeto=nTrackCut&&beamHaloVeto&&vertLooseCut&&cShapeCut;
  TCut evSelPP=nTrackCutPP&&vertLooseCutPP&&cShapeCut&&hfCoincVeto;
  TCut evSelwMuQ=muonCutsCand&&"(pass1||pass2)"&&muonQualityCuts&&evSel;
  TCut evSelNoHFVetowMuQ=muonCutsCand&&"(pass1||pass2)"&&muonQualityCuts&&evSelNoHFVeto;
  TCut evSelwMuQPP=muonCutsCand&&"(pass1||pass2)"&&muonQualityCuts&&evSelPP;
#else 
  float ptBinz[25];
  float rapBinz[14];
  
  TCut genEvSel;
  
  TCut beamHaloVeto;
  TCut vertCut;
  TCut vertLooseCut;
  TCut vertLooseCutPP;
  TCut hfCoincVeto;
  TCut hfCoincTightVeto;
  TCut hfSigVeto;
  //	TCut minBiasVeto;
  TCut minBiasVeto;
  TCut cShapeCut;
  
  TCut candRapidityCut;
  TCut candRapidityCut2;
  TCut candRapidityCut3;
  TCut candRapidityCut4;
  TCut candRapidityCut5;
  TCut candCoherent;
  TCut candCoherentOld;
  TCut candInCoherent;
  TCut candInCoherent2;
  
  TCut muonCutsTracker;
  TCut muonCutsGlobal;
  TCut muonCutsCand;
  TCut muonCutsTrack;
  TCut muonDaughterCuts;
  TCut muonDaughterOrCuts;
  
  TCut accRapTracker;
  TCut accRapTracks;
  TCut jPsiTracker;
  TCut jPsiTracks;
  TCut jPsiCand;
  TCut jPsiCand2;
  TCut jPsiCandPAS;
  TCut jPsiCandRapidityPAS;
  TCut gammaCand;
  TCut sideBandCand;
  TCut sideBandCand2;
  TCut jPsiGen;
  TCut gammaGen;
  
  TCut hfMaxCut;
  
  //TCut ebCut;
  //TCut eeCut;
  //TCut hbCut;
  //TCut heCut;
  //TCut hfCut;
  
  TCut ebCut;
  TCut eeCut;
  TCut hbCut;
  TCut heCut;
  TCut hfCut;
  
  TCut ebNoOLCut;
  TCut eeNoOLCut;
  TCut hbNoOLCut;
  TCut heNoOLCut;
  TCut hfNoOLCut;
  
  TCut ebENoOLCut;
  TCut eeENoOLCut;
  TCut hbENoOLCut;
  TCut heENoOLCut;
  TCut hfENoOLCut;
  
  TCut ebRecHitCut;
  TCut eeRecHitCut;
  TCut hbRecHitCut;
  TCut heRecHitCut;
  TCut hfRecHitCut;
  
  TCut ebRecHitPlusCut;
  TCut ebRecHitMinusCut;
  TCut eeRecHitPlusCut;
  TCut eeRecHitMinusCut;
  TCut hbRecHitPlusCut;
  TCut hbRecHitMinusCut;
  TCut heRecHitPlusCut;
  TCut heRecHitMinusCut;
  TCut hfRecHitPlusCut;
  TCut hfRecHitMinusCut;
  
  TCut centralRecHitCut;
  TCut forwardRecHitCut;
  TCut nTrackCut;
  TCut nTrackCutPP;
  TCut exPixCut;
  TCut exCaloNoOLCut;
  TCut exCaloENoOLCut;
  TCut exCaloCut;
  TCut exRecHitCut;
  TCut exRecHitAsymCut;
  
  TCut sameZdcLut;
  TCut sameZdcGain;
  TCut sameZdcThres;

  TCut zdc1nuPos;
  TCut zdc1nuNeg;
  TCut zdc1nNeg45;
  TCut zdc1nPos45;
  TCut zdc1nNeg45New;
  TCut zdc1nPos45New;
  TCut zdc1nNeg45EMb;
  TCut zdc1nPos45EMb;
  TCut zdc1nuPos3Sigma;
  TCut zdc1nuNeg3Sigma;
  TCut zdc1nNeg453Sigma;
  TCut zdc1nPos453Sigma;
  TCut zdc1nNeg45New3Sigma;
  TCut zdc1nPos45New3Sigma;
  TCut zdc1nNeg45EMb3Sigma;
  TCut zdc1nPos45EMb3Sigma;
  TCut zdc1nuPos1Sigma;
  TCut zdc1nuNeg1Sigma;
  TCut zdc1nNeg451Sigma;
  TCut zdc1nPos451Sigma;
  TCut zdc1nNeg45New1Sigma;
  TCut zdc1nPos45New1Sigma;
  TCut zdc1nNeg45EMb1Sigma;
  TCut zdc1nPos45EMb1Sigma;

  TCut zdcGt1nuPos;
  TCut zdcGt1nuNeg;
  TCut zdcGt1nNeg45;
  TCut zdcGt1nPos45;
  TCut zdcGt1nNeg45New;
  TCut zdcGt1nPos45New;
  TCut zdcGt1nNeg45EMb;
  TCut zdcGt1nPos45EMb;
  TCut zdcGt1nuPos3Sigma;
  TCut zdcGt1nuNeg3Sigma;
  TCut zdcGt1nNeg453Sigma;
  TCut zdcGt1nPos453Sigma;
  TCut zdcGt1nNeg45New3Sigma;
  TCut zdcGt1nPos45New3Sigma;
  TCut zdcGt1nNeg45EMb3Sigma;
  TCut zdcGt1nPos45EMb3Sigma;
  TCut zdcGt1nuPos1Sigma;
  TCut zdcGt1nuNeg1Sigma;
  TCut zdcGt1nNeg451Sigma;
  TCut zdcGt1nPos451Sigma;
  TCut zdcGt1nNeg45New1Sigma;
  TCut zdcGt1nPos45New1Sigma;
  TCut zdcGt1nNeg45EMb1Sigma;
  TCut zdcGt1nPos45EMb1Sigma;

  TCut zdc0nPos1Sig;
  TCut zdc0nNeg1Sig;
  TCut zdc0nPos2Sig;
  TCut zdc0nNeg2Sig;
  TCut zdc0nPos3Sig;
  TCut zdc0nNeg3Sig;
  
  TCut zdc4nuThreshPos;
  TCut zdc4nuThreshNeg;
  
  TCut zdcTs4Pos;
  TCut zdcTs4Neg;
  TCut zdcTs5Pos;
  TCut zdcTs5Neg;
  
  TCut zdcLooseTs4Pos;
  TCut zdcLooseTs4Neg;
  TCut zdcLooseTs5Pos;
  TCut zdcLooseTs5Neg;
  
  TCut zdcXn0n;
  TCut zdcXn;
  TCut zdcXnXn;
  TCut zdc1n0n;
  TCut zdc1n1n;
  
  TCut zdcXn0nPrime;
  TCut zdcXnPrime;
  TCut zdcXnXnPrime;
  TCut zdc1n0nPrime;
  TCut zdc1n1nPrime;
  
  TCut zdcXn0nPrime3Sigma;
  TCut zdcXnPrime3Sigma;
  TCut zdcXnXnPrime3Sigma;
  TCut zdc1n0nPrime3Sigma;
  TCut zdc1n1nPrime3Sigma;

  TCut zdcXn0nPrime1Sigma;
  TCut zdcXnPrime1Sigma;
  TCut zdcXnXnPrime1Sigma;
  TCut zdc1n0nPrime1Sigma;
  TCut zdc1n1nPrime1Sigma;

  TCut zdcXn0nPrimeNew;
  TCut zdcXnPrimeNew;
  TCut zdcXnXnPrimeNew;
  TCut zdc1n0nPrimeNew;
  TCut zdc1n1nPrimeNew;

  TCut zdcXn0nPrimeEMb;
  TCut zdcXnPrimeEMb;
  TCut zdcXnXnPrimeEMb;
  TCut zdc1n0nPrimeEMb;
  TCut zdc1n1nPrimeEMb;

  TCut zdcSameSide;
  TCut zdcOppSide;
  TCut zdcSameSidePrime;
  TCut zdcOppSidePrime;
  
  TCut goodMuonCutMu1;
  TCut goodMuonCutMu2;
  TCut innerTrackHitsCut;
  TCut pixelLayersCut;
  TCut normalizedX2Cut;
  TCut pixelLayersCut2;
  TCut normalizedX2Cut2;
  TCut dBCut;
  TCut dZCut2;
  TCut dZCut2PP;
  TCut dZCut;
  TCut vProb;
  TCut trkArbit;
  TCut matchedMu1;
  TCut matchedMu2;
  // soft id
  TCut pixelLayersCutSoftMu1;
  TCut normalizedX2CutSoftMu1;
  TCut isHighPurityCutSoftMu1;
  TCut dBCutSoftMu1;
  TCut dZCutSoftMu1;
  TCut dZCutSoftMu1PP;
  TCut trackerLayersSoftMu1;
  TCut pixelLayersCutSoftMu2;
  TCut normalizedX2CutSoftMu2;
  TCut isHighPurityCutSoftMu2;
  TCut dBCutSoftMu2;
  TCut dZCutSoftMu2;
  TCut dZCutSoftMu2PP;
  TCut trackerLayersSoftMu2;
  TCut goodMuonCut;
  TCut trackerLayersSoft;
  TCut pixelLayersCutSoft;
  TCut normalizedX2CutSoft;
  TCut isHighPurityCutSoft;
  TCut dZCutSoft;
  TCut dBCutSoft;
  TCut dZCutSoftPP;
  // muon detectable and triggerable kenimatic cuts
  TCut muonDetCut;
  TCut muon1DetCutLinear20;
  TCut muon2DetCutLinear20;
  TCut muon1DetCutFlat20;
  TCut muon2DetCutFlat20;
  TCut muon1DetCutStep20;
  TCut muon2DetCutStep20;
  TCut muon1DetCutStep20PP;
  TCut muon2DetCutStep20PP;
  TCut muon1TrigJPsiCoCut;
  TCut muon2TrigJPsiCoCut;
  TCut muon1TrigTnPCut;
  TCut muon2TrigTnPCut;
  TCut muon1TrigTnPCutPP;
  TCut muon2TrigTnPCutPP;
  TCut muon1TrigTnPAllCut;
  TCut muon2TrigTnPAllCut;
  TCut muon1TrigGammaCut;
  TCut muon2TrigGammaCut;
  TCut muonDetEtaCut;
  //combos 
  TCut muonQualityCuts;
  TCut muonQualityCuts2;
  //TCut muonQualityCuts2PP;
  TCut qualitySoftId;
  TCut qualitySoftIdPP;
  TCut qualitySoftIdPPb;
  
  TCut trackerGoodMuon1Cut;
  TCut trackerInnerTrackHitsCut;
  TCut trackerInnerTrackHitsCutTight;
  TCut trackerPixelLayersCut2;
  TCut trackerNormalizedX2Cut2;
  TCut trackerNormalizedX2CutTight;
  TCut trackerDBCut;
  TCut trackerDZCut2;
  TCut trackerQualityCuts2;
  TCut trackerQualityCutsTight;
  //soft id
  TCut trackerGoodMuonCut;
  TCut trackerTrackerLayers;
  TCut trackerNormalizedX2CutSoft;
  TCut trackerPixelLayersCutSoft;
  TCut trackerDBCutSoft;
  TCut trackerDZCutSoft;
  TCut trackerQualitySoftId;
  TCut QualitySoftId;
  TCut GoodMuonCut;
  TCut TrackerLayers;
  TCut NormalizedX2CutSoft;
  TCut PixelLayersCutSoft;
  TCut DBCutSoft;
  TCut DZCutSoft;
  //trigger matching
  TCut trackerMatchedMu;
  TCut trackerLeadingMu;
  TString softId;
  TCut trackerLeadingQualityMu;
  TCut trackerLeadingQualityMatchedMu;
  TCut trackerLeadingQualityTriggerMu;
  
  TCut evSel;
  TCut evSelPreCut;
  TCut evSelNoHFVeto;
  TCut evSelPP;
  TCut evSelwMuQ;
  TCut evSelNoHFVetowMuQ;
  TCut evSelwMuQPP;
#endif
