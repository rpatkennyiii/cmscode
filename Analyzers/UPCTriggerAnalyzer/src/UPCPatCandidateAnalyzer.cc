#include "Analyzers/UPCTriggerAnalyzer/interface/UPCPatCandidateAnalyzer.h"

using namespace edm;

UPCPatCandidateAnalyzer::UPCPatCandidateAnalyzer(const edm::ParameterSet& iConfig):_patDiMuon(iConfig.getParameter<InputTag>("patDiMuon")),
	_hltTrigger(iConfig.getParameter<string>("hltTrigger")){}

UPCPatCandidateAnalyzer::~UPCPatCandidateAnalyzer(){}

void UPCPatCandidateAnalyzer::beginJob(){
  mFileServer->file().SetCompressionLevel(9);
  mFileServer->file().cd();

  candTree = new TTree("UPCPatCandidateTree","UPCPatCandidateTree");

  candTree->Branch("acceptedCand",&acceptedCand,"acceptedCand/I");
  candTree->Branch("Reco_QQ_pt", Reco_QQ_pt, "Reco_QQ_pt[acceptedCand]/F");
  candTree->Branch("Reco_QQ_eta", Reco_QQ_eta, "Reco_QQ_eta[acceptedCand]/F");
  candTree->Branch("Reco_QQ_phi", Reco_QQ_phi, "Reco_QQ_phi[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mass", Reco_QQ_mass, "Reco_QQ_mass[acceptedCand]/F");
  candTree->Branch("Reco_QQ_charge", Reco_QQ_charge, "Reco_QQ_charge[acceptedCand]/I");
  candTree->Branch("Reco_QQ_rapidity", Reco_QQ_rapidity, "Reco_QQ_rapidity[acceptedCand]/F");
  candTree->Branch("Reco_QQ_p", Reco_QQ_p, "Reco_QQ_p[acceptedCand]/F");
  candTree->Branch("Reco_QQ_energy", Reco_QQ_energy, "Reco_QQ_energy[acceptedCand]/F");
  candTree->Branch("Reco_QQ_et", Reco_QQ_et, "Reco_QQ_et[acceptedCand]/F");
  candTree->Branch("Reco_QQ_px", Reco_QQ_px, "Reco_QQ_px[acceptedCand]/F");
  candTree->Branch("Reco_QQ_py", Reco_QQ_py, "Reco_QQ_py[acceptedCand]/F");
  candTree->Branch("Reco_QQ_pz", Reco_QQ_pz, "Reco_QQ_pz[acceptedCand]/F");
  candTree->Branch("Reco_QQ_theta", Reco_QQ_theta, "Reco_QQ_theta[acceptedCand]/F");
  candTree->Branch("pass1",pass1,"pass1[acceptedCand]/O");
  candTree->Branch("pass2",pass2,"pass2[acceptedCand]/O");
  candTree->Branch("dPhi", dPhi, "dPhi[acceptedCand]/F");

  candTree->Branch("Reco_QQ_mupl_pt", Reco_QQ_mupl_pt, "Reco_QQ_mupl_pt[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mupl_eta", Reco_QQ_mupl_eta, "Reco_QQ_mupl_eta[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mupl_phi", Reco_QQ_mupl_phi, "Reco_QQ_mupl_phi[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mupl_mass", Reco_QQ_mupl_mass, "Reco_QQ_mupl_mass[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mupl_charge", Reco_QQ_mupl_charge, "Reco_QQ_mupl_charge[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mupl_StandAlone", Reco_QQ_mupl_StandAlone, "Reco_QQ_mupl_StandAlone[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mupl_Global", Reco_QQ_mupl_Global, "Reco_QQ_mupl_Global[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mupl_Tracker", Reco_QQ_mupl_Tracker, "Reco_QQ_mupl_Tracker[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mupl_rapidity", Reco_QQ_mupl_rapidity, "Reco_QQ_mupl_rapidity[acceptedCand]/F");

  candTree->Branch("Reco_QQ_mumi_pt", Reco_QQ_mumi_pt, "Reco_QQ_mumi_pt[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mumi_eta", Reco_QQ_mumi_eta, "Reco_QQ_mumi_eta[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mumi_phi", Reco_QQ_mumi_phi, "Reco_QQ_mumi_phi[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mumi_mass", Reco_QQ_mumi_mass, "Reco_QQ_mumi_mass[acceptedCand]/F");
  candTree->Branch("Reco_QQ_mumi_charge", Reco_QQ_mumi_charge, "Reco_QQ_mumi_charge[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mumi_StandAlone", Reco_QQ_mumi_StandAlone, "Reco_QQ_mumi_StandAlone[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mumi_Global", Reco_QQ_mumi_Global, "Reco_QQ_mumi_Global[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mumi_Tracker", Reco_QQ_mumi_Tracker, "Reco_QQ_mumi_Tracker[acceptedCand]/I");
  candTree->Branch("Reco_QQ_mumi_rapidity", Reco_QQ_mumi_rapidity, "Reco_QQ_mumi_rapidity[acceptedCand]/F");
}

void UPCPatCandidateAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  Reco_QQ_size=0;
  acceptedCand=0;
  iEvent.getByLabel(_patDiMuon,collDiMuon); 

  if (collDiMuon.isValid()) {

    for(std::vector<pat::CompositeCandidate>::const_iterator it=collDiMuon->begin();
	it!=collDiMuon->end(); ++it) {
      
      const pat::CompositeCandidate* cand = &(*it);
      
      const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
      const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));
      
      const pat::TriggerObjectStandAloneCollection mu1HLTMatchesPath = muon1->triggerObjectMatchesByPath(_hltTrigger.c_str());
      const pat::TriggerObjectStandAloneCollection mu2HLTMatchesPath = muon2->triggerObjectMatchesByPath(_hltTrigger.c_str());

      pass1[Reco_QQ_size] = false;
      pass2[Reco_QQ_size] = false;
      
      pass1[Reco_QQ_size] = bool(mu1HLTMatchesPath.size());
      pass2[Reco_QQ_size] = bool(mu2HLTMatchesPath.size());

      Reco_QQ_pt[Reco_QQ_size]=cand->pt();
      Reco_QQ_eta[Reco_QQ_size] = cand->eta();
      Reco_QQ_phi[Reco_QQ_size] = cand->phi();
      Reco_QQ_mass[Reco_QQ_size] = cand->mass();
      Reco_QQ_charge[Reco_QQ_size] = cand->charge();
      Reco_QQ_rapidity[Reco_QQ_size] = cand->rapidity();
      Reco_QQ_p[Reco_QQ_size] = cand->p();
      Reco_QQ_energy[Reco_QQ_size] = cand->energy();
      Reco_QQ_et[Reco_QQ_size] = cand->et();
      Reco_QQ_px[Reco_QQ_size] = cand->px();
      Reco_QQ_py[Reco_QQ_size] = cand->py();
      Reco_QQ_pz[Reco_QQ_size] = cand->pz();
      Reco_QQ_theta[Reco_QQ_size] = cand->theta();
      
      Reco_QQ_mupl_pt[Reco_QQ_size]=muon1->pt();
      Reco_QQ_mupl_eta[Reco_QQ_size] = muon1->eta();
      Reco_QQ_mupl_phi[Reco_QQ_size] = muon1->phi();
      Reco_QQ_mupl_mass[Reco_QQ_size] = muon1->mass();
      Reco_QQ_mupl_charge[Reco_QQ_size] = muon1->charge();
      Reco_QQ_mupl_StandAlone[Reco_QQ_size] = muon1->isStandAloneMuon();
      Reco_QQ_mupl_Global[Reco_QQ_size] = muon1->isGlobalMuon();
      Reco_QQ_mupl_Tracker[Reco_QQ_size] = muon1->isTrackerMuon();
      Reco_QQ_mupl_rapidity[Reco_QQ_size] = muon1->rapidity();

      Reco_QQ_mumi_pt[Reco_QQ_size]=muon2->pt();
      Reco_QQ_mumi_eta[Reco_QQ_size] = muon2->eta();
      Reco_QQ_mumi_phi[Reco_QQ_size] = muon2->phi();
      Reco_QQ_mumi_mass[Reco_QQ_size] = muon2->mass();
      Reco_QQ_mumi_charge[Reco_QQ_size] = muon2->charge();
      Reco_QQ_mumi_StandAlone[Reco_QQ_size] = muon2->isStandAloneMuon();
      Reco_QQ_mumi_Global[Reco_QQ_size] = muon2->isGlobalMuon();
      Reco_QQ_mumi_Tracker[Reco_QQ_size] = muon2->isTrackerMuon();
      Reco_QQ_mumi_rapidity[Reco_QQ_size] = muon2->rapidity();

      dPhi[Reco_QQ_size] =  deltaPhi(Reco_QQ_mupl_phi[Reco_QQ_size],Reco_QQ_mumi_phi[Reco_QQ_size]);

      Reco_QQ_size++;
    }
  }
  
  acceptedCand=Reco_QQ_size;
  candTree->Fill();
}
