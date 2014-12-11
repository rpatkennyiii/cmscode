#ifndef UPCPATCANDIDATEANALYZER_H
#define UPCPATCANDIDATEANALYZER_H

#include <string>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/Math/interface/deltaPhi.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TRotation.h"
#include "TVector3.h"

using namespace reco;
using namespace std;

class UPCPatCandidateAnalyzer : public edm::EDAnalyzer {
   public:
      explicit UPCPatCandidateAnalyzer(const edm::ParameterSet&);
      ~UPCPatCandidateAnalyzer();

   private:
      virtual void beginJob();
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      float* calcPol(TLorentzVector,TLorentzVector);

      edm::Service<TFileService> mFileServer;
      edm::Handle<pat::CompositeCandidateCollection> collDiMuon;
      edm::InputTag _patDiMuon;
      string _hltTrigger;

      int acceptedCand;
      int cand_size;       // Number of reconstructed Onia 
      float cand_pt[100000];
      float cand_eta[100000];
      float cand_phi[100000];
      float cand_mass[100000];
      int cand_charge[100000];
      float cand_rapidity[100000];
      float cand_p[100000];
      float cand_energy[100000];
      float cand_et[100000];
      float cand_px[100000];
      float cand_py[100000];
      float cand_pz[100000];
      float cand_theta[100000];
      float cand_PolHXphi[100000];
      float cand_PolHXtheta[100000];
      
      //daughter Pat muons from the composite candidate

      float muon1_pt[100000];
      float muon1_eta[100000];
      float muon1_phi[100000];
      float muon1_mass[100000];
      int muon1_charge[100000];
      int muon1_StandAlone[100000];
      int muon1_Global[100000];
      int muon1_Tracker[100000];
      int muon1_Calo[100000];
      int muon1_l1Quality[100000];
      float muon1_l1DeltaR[100000];
      float muon1_rapidity[100000];
      float muon1_caloMuonCompatibility[100000];
      float muon1_caloMuonTower[100000];
      float muon1_caloMuonTowerS9[100000];
      float muon1_caloMuonEm[100000];
      float muon1_caloMuonEmS9[100000];
      float muon1_caloMuonEmS25[100000];
      float muon1_caloMuonEmMax[100000];
      float muon1_caloMuonHad[100000];
      float muon1_caloMuonHadS9[100000];
      float muon1_caloMuonHadMax[100000];
      float muon1_numberOfValidHits[100000];
      float muon1_pixelLayersWithMeasurement[100000];
      float muon1_trackerLayersWithMeasurement[100000];
      float muon1_normalizedChi2[100000];
      float muon1_dz[100000];
      float muon1_dB[100000];
      float muon1_numberOfMatchedStations[100000];
      float muon1_ndof[100000];
      bool muon1_trkArbit[100000];
      bool muon1_isGoodMuon[100000];
      bool muon1_isHighPurity[100000];
      bool muon1_pass[100000];

      float muon2_pt[100000];
      float muon2_eta[100000];
      float muon2_phi[100000];
      float muon2_mass[100000];
      int muon2_charge[100000];
      int muon2_StandAlone[100000];
      int muon2_Global[100000];
      int muon2_Tracker[100000];
      int muon2_Calo[100000];
      int muon2_l1Quality[100000];
      float muon2_l1DeltaR[100000];
      float muon2_rapidity[100000];
      float muon2_caloMuonCompatibility[100000];
      float muon2_caloMuonTower[100000];
      float muon2_caloMuonTowerS9[100000];
      float muon2_caloMuonEm[100000];
      float muon2_caloMuonEmS9[100000];
      float muon2_caloMuonEmS25[100000];
      float muon2_caloMuonEmMax[100000];
      float muon2_caloMuonHad[100000];
      float muon2_caloMuonHadS9[100000];
      float muon2_caloMuonHadMax[100000];
      float muon2_numberOfValidHits[100000];
      float muon2_pixelLayersWithMeasurement[100000];
      float muon2_trackerLayersWithMeasurement[100000];
      float muon2_normalizedChi2[100000];
      float muon2_dz[100000];
      float muon2_dB[100000];
      float muon2_numberOfMatchedStations[100000];
      float muon2_ndof[100000];
      bool muon2_trkArbit[100000];
      bool muon2_isGoodMuon[100000];
      bool muon2_isHighPurity[100000];
      bool muon2_pass[100000];

      float cand_dPhi[100000];
      float cand_vProb[100000];

      TTree* candTree; 
};
DEFINE_FWK_MODULE(UPCPatCandidateAnalyzer);
#endif
