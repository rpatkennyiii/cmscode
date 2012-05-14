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
#include "DataFormats/Math/interface/deltaPhi.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//Root Classes
#include "TTree.h"
#include "TFile.h"

using namespace reco;
using namespace std;

class UPCPatCandidateAnalyzer : public edm::EDAnalyzer {
   public:
      explicit UPCPatCandidateAnalyzer(const edm::ParameterSet&);
      ~UPCPatCandidateAnalyzer();

   private:
      virtual void beginJob();
      virtual void analyze(const edm::Event&, const edm::EventSetup&);

      edm::Service<TFileService> mFileServer;
      edm::Handle<pat::CompositeCandidateCollection> collDiMuon;
      edm::InputTag _patDiMuon;
      string _hltTrigger;

      int acceptedCand;
      int Reco_QQ_size;       // Number of reconstructed Onia 
      float Reco_QQ_pt[100000];
      float Reco_QQ_eta[100000];
      float Reco_QQ_phi[100000];
      float Reco_QQ_mass[100000];
      int Reco_QQ_charge[100000];
      float Reco_QQ_rapidity[100000];
      float Reco_QQ_p[100000];
      float Reco_QQ_energy[100000];
      float Reco_QQ_et[100000];
      float Reco_QQ_px[100000];
      float Reco_QQ_py[100000];
      float Reco_QQ_pz[100000];
      float Reco_QQ_theta[100000];
      bool pass1[100000];
      bool pass2[100000];
      
      //daughter Pat muons from the composite candidate

      float Reco_QQ_mupl_pt[100000];
      float Reco_QQ_mupl_eta[100000];
      float Reco_QQ_mupl_phi[100000];
      float Reco_QQ_mupl_mass[100000];
      int Reco_QQ_mupl_charge[100000];
      int Reco_QQ_mupl_StandAlone[100000];
      int Reco_QQ_mupl_Global[100000];
      int Reco_QQ_mupl_Tracker[100000];
      float Reco_QQ_mupl_rapidity[100000];

      float Reco_QQ_mumi_pt[100000];
      float Reco_QQ_mumi_eta[100000];
      float Reco_QQ_mumi_phi[100000];
      float Reco_QQ_mumi_mass[100000];
      int Reco_QQ_mumi_charge[100000];
      int Reco_QQ_mumi_StandAlone[100000];
      int Reco_QQ_mumi_Global[100000];
      int Reco_QQ_mumi_Tracker[100000];
      float Reco_QQ_mumi_rapidity[100000];

      float dPhi[100000];

      TTree* candTree; 
};
DEFINE_FWK_MODULE(UPCPatCandidateAnalyzer);
#endif
