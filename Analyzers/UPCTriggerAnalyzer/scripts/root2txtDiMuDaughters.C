#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include <fstream>

using namespace std;

int root2txtDiMuDaughters( TString fileName="small.root",  TString outFileName="ralVectorsDiMu.txt"){
  TFile *_file0 = new TFile(fileName);
  TTree *t0 = (TTree*)_file0->Get("onia2MuMuPatDiTrksTree");
  TTree *vert=(TTree *)_file0->Get("offlinePrimaryVerticesTree");

  t0->AddFriend(vert);

  TCut dZCutSoftMu1PP="abs(muon1_dz-offlinePrimaryVerticesTree.z[0])<20.";
  TCut dZCutSoftMu2PP="abs(muon2_dz-offlinePrimaryVerticesTree.z[0])<20.";
  TCut dBCutSoftMu1="muon1_dB<0.3";
  TCut dBCutSoftMu2="muon2_dB<0.3";

  TCut goodCut="muon1_isGoodMuon&&muon2_isGoodMuon";
  TCut trackerMuCut="muon1_Tracker&&muon2_Tracker";
  TCut pixLayCut="muon1_pixelLayersWithMeasurement>0&&muon2_pixelLayersWithMeasurement>0";
  TCut trxLayCut="muon1_trackerLayersWithMeasurement>5&&muon2_trackerLayersWithMeasurement>5";
  TCut hiPureCut="muon1_isHighPurity&&muon2_isHighPurity";
  TCut promptCut=dZCutSoftMu1PP&&dZCutSoftMu2PP&&dBCutSoftMu1&&dBCutSoftMu2;
  TCut l1MatchCut="muon1_l1Quality>0||muon2_l1Quality>0";
  TCut hltMatchCut="muon1_pass||muon2_pass";

  TCut allCuts=goodCut&&trackerMuCut&&pixLayCut&&trxLayCut&&hiPureCut&&promptCut&&l1MatchCut&&hltMatchCut;

  int n=t0->Draw("muon1_pt*cos(muon1_phi):muon1_pt*sin(muon1_phi):muon1_pt*sinh(muon1_eta):muon2_pt*cos(muon2_phi):muon2_pt*sin(muon2_phi):muon2_pt*sinh(muon2_eta):muon1_charge",allCuts,"paragoff");
  double *mu1Px=t0->GetVal(0), *mu1Py=t0->GetVal(1), *mu1Pz=t0->GetVal(2),
    *mu2Px=t0->GetVal(3), *mu2Py=t0->GetVal(4), *mu2Pz=t0->GetVal(5),
    *mu1Charge=t0->GetVal(6);

  ofstream outFile;
  outFile.open(outFileName);

  for(int i=0; i<n; i++){
    if(mu1Charge[i]>0){ 
      outFile << mu1Px[i] << '\t' << mu1Py[i] << '\t' << mu1Pz[i] << '\t'
              << mu2Px[i] << '\t' << mu2Py[i] << '\t' << mu2Pz[i] << endl;
    } else {
      outFile << mu2Px[i] << '\t' << mu2Py[i] << '\t' << mu2Pz[i] << '\t'
              << mu1Px[i] << '\t' << mu1Py[i] << '\t' << mu1Pz[i] << endl;
    }
  }

  return(0);
}
