#include "Analyzers/UPCTriggerAnalyzer/interface/UPCPatCandidateAnalyzer.h"

using namespace edm;

UPCPatCandidateAnalyzer::UPCPatCandidateAnalyzer(const edm::ParameterSet& iConfig):_patDiMuon(iConfig.getParameter<InputTag>("patDiMuon")),
	_hltTrigger(iConfig.getParameter<string>("hltTrigger")){}

UPCPatCandidateAnalyzer::~UPCPatCandidateAnalyzer(){}

void UPCPatCandidateAnalyzer::beginJob(){
  mFileServer->file().cd();

  candTree = new TTree(TString(_patDiMuon.label())+"Tree",TString(_patDiMuon.label())+"Tree");

  candTree->Branch("acceptedCand",&acceptedCand,"acceptedCand/I");
  candTree->Branch("cand_pt", cand_pt, "cand_pt[acceptedCand]/F");
  candTree->Branch("cand_eta", cand_eta, "cand_eta[acceptedCand]/F");
  candTree->Branch("cand_phi", cand_phi, "cand_phi[acceptedCand]/F");
  candTree->Branch("cand_mass", cand_mass, "cand_mass[acceptedCand]/F");
  candTree->Branch("cand_charge", cand_charge, "cand_charge[acceptedCand]/I");
  candTree->Branch("cand_rapidity", cand_rapidity, "cand_rapidity[acceptedCand]/F");
  candTree->Branch("cand_p", cand_p, "cand_p[acceptedCand]/F");
  candTree->Branch("cand_energy", cand_energy, "cand_energy[acceptedCand]/F");
  candTree->Branch("cand_et", cand_et, "cand_et[acceptedCand]/F");
  candTree->Branch("cand_px", cand_px, "cand_px[acceptedCand]/F");
  candTree->Branch("cand_py", cand_py, "cand_py[acceptedCand]/F");
  candTree->Branch("cand_pz", cand_pz, "cand_pz[acceptedCand]/F");
  candTree->Branch("cand_theta", cand_theta, "cand_theta[acceptedCand]/F");
  candTree->Branch("cand_vProb", cand_vProb, "cand_vProb[acceptedCand]/F");
  candTree->Branch("cand_dPhi", cand_dPhi, "cand_dPhi[acceptedCand]/F");
  candTree->Branch("cand_PolHXphi", cand_PolHXphi, "cand_PolHXphi[acceptedCand]/F");
  candTree->Branch("cand_PolHXtheta", cand_PolHXtheta, "cand_PolHXtheta[acceptedCand]/F");

  candTree->Branch("muon1_pt", muon1_pt, "muon1_pt[acceptedCand]/F");
  candTree->Branch("muon1_eta", muon1_eta, "muon1_eta[acceptedCand]/F");
  candTree->Branch("muon1_phi", muon1_phi, "muon1_phi[acceptedCand]/F");
  candTree->Branch("muon1_mass", muon1_mass, "muon1_mass[acceptedCand]/F");
  candTree->Branch("muon1_charge", muon1_charge, "muon1_charge[acceptedCand]/I");
  candTree->Branch("muon1_StandAlone", muon1_StandAlone, "muon1_StandAlone[acceptedCand]/I");
  candTree->Branch("muon1_Global", muon1_Global, "muon1_Global[acceptedCand]/I");
  candTree->Branch("muon1_Tracker", muon1_Tracker, "muon1_Tracker[acceptedCand]/I");
  candTree->Branch("muon1_rapidity", muon1_rapidity, "muon1_rapidity[acceptedCand]/F");
  candTree->Branch("muon1_numberOfValidHits",muon1_numberOfValidHits,"muon1_numberOfValidHits[acceptedCand]/F");
  candTree->Branch("muon1_pixelLayersWithMeasurement",muon1_pixelLayersWithMeasurement,"muon1_pixelLayersWithMeasurement[acceptedCand]/F");
  candTree->Branch("muon1_trackerLayersWithMeasurement",muon1_trackerLayersWithMeasurement,"muon1_trackerLayersWithMeasurement[acceptedCand]/F");
  candTree->Branch("muon1_normalizedChi2",muon1_normalizedChi2,"muon1_normalizedChi2[acceptedCand]/F");
  candTree->Branch("muon1_dz",muon1_dz,"muon1_dz[acceptedCand]/F");
  candTree->Branch("muon1_dB",muon1_dB,"muon1_dB[acceptedCand]/F");
  candTree->Branch("muon1_numberOfMatchedStations",muon1_numberOfMatchedStations,"muon1_numberOfMatchedStations[acceptedCand]/F");
  candTree->Branch("muon1_ndof",muon1_ndof,"muon1_ndof[acceptedCand]/F");
  candTree->Branch("muon1_trkArbit",muon1_trkArbit,"muon1_trkArbit[acceptedCand]/O");
  candTree->Branch("muon1_isGoodMuon",muon1_isGoodMuon,"muon1_isGoodMuon[acceptedCand]/O");
  candTree->Branch("muon1_l1DeltaR",muon1_l1DeltaR,"muon1_l1DeltaR[acceptedCand]/F");
  candTree->Branch("muon1_l1Quality",muon1_l1Quality,"muon1_l1Quality[acceptedCand]/I");
  candTree->Branch("muon1_pass",muon1_pass,"muon1_pass[acceptedCand]/O");

  candTree->Branch("muon2_pt", muon2_pt, "muon2_pt[acceptedCand]/F");
  candTree->Branch("muon2_eta", muon2_eta, "muon2_eta[acceptedCand]/F");
  candTree->Branch("muon2_phi", muon2_phi, "muon2_phi[acceptedCand]/F");
  candTree->Branch("muon2_mass", muon2_mass, "muon2_mass[acceptedCand]/F");
  candTree->Branch("muon2_charge", muon2_charge, "muon2_charge[acceptedCand]/I");
  candTree->Branch("muon2_StandAlone", muon2_StandAlone, "muon2_StandAlone[acceptedCand]/I");
  candTree->Branch("muon2_Global", muon2_Global, "muon2_Global[acceptedCand]/I");
  candTree->Branch("muon2_Tracker", muon2_Tracker, "muon2_Tracker[acceptedCand]/I");
  candTree->Branch("muon2_rapidity", muon2_rapidity, "muon2_rapidity[acceptedCand]/F");
  candTree->Branch("muon2_numberOfValidHits",muon2_numberOfValidHits,"muon2_numberOfValidHits[acceptedCand]/F");
  candTree->Branch("muon2_pixelLayersWithMeasurement",muon2_pixelLayersWithMeasurement,"muon2_pixelLayersWithMeasurement[acceptedCand]/F");
  candTree->Branch("muon2_trackerLayersWithMeasurement",muon2_trackerLayersWithMeasurement,"muon2_trackerLayersWithMeasurement[acceptedCand]/F");
  candTree->Branch("muon2_normalizedChi2",muon2_normalizedChi2,"muon2_normalizedChi2[acceptedCand]/F");
  candTree->Branch("muon2_dz",muon2_dz,"muon2_dz[acceptedCand]/F");
  candTree->Branch("muon2_dB",muon2_dB,"muon2_dB[acceptedCand]/F");
  candTree->Branch("muon2_numberOfMatchedStations",muon2_numberOfMatchedStations,"muon2_numberOfMatchedStations[acceptedCand]/F");
  candTree->Branch("muon2_ndof",muon2_ndof,"muon2_ndof[acceptedCand]/F");
  candTree->Branch("muon2_trkArbit",muon2_trkArbit,"muon2_trkArbit[acceptedCand]/O");
  candTree->Branch("muon2_isGoodMuon",muon2_isGoodMuon,"muon2_isGoodMuon[acceptedCand]/O");
  candTree->Branch("muon2_l1DeltaR",muon2_l1DeltaR,"muon2_l1DeltaR[acceptedCand]/F");
  candTree->Branch("muon2_l1Quality",muon2_l1Quality,"muon2_l1Quality[acceptedCand]/I");
  candTree->Branch("muon2_pass",muon2_pass,"muon2_pass[acceptedCand]/O");
}

void UPCPatCandidateAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  cand_size=0;
  acceptedCand=0;

  iEvent.getByLabel(_patDiMuon,collDiMuon); 

  if (collDiMuon.isValid()) {

    for(std::vector<pat::CompositeCandidate>::const_iterator it=collDiMuon->begin();
	it!=collDiMuon->end() && cand_size<=1000; ++it) {
      
      const pat::CompositeCandidate* cand = &(*it);
      
      const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
      const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));

      const pat::TriggerObjectStandAloneCollection mu1HLTMatchesPath = muon1->triggerObjectMatchesByPath(_hltTrigger.c_str());
      const pat::TriggerObjectStandAloneCollection mu2HLTMatchesPath = muon2->triggerObjectMatchesByPath(_hltTrigger.c_str());

      muon1_pass[cand_size] = false;
      muon2_pass[cand_size] = false;

      TLorentzVector muPlus,muMinus; 
      if(muon1->charge() >= muon2->charge()){ 
	 muPlus=TLorentzVector(muon1->p4().x(),muon1->p4().y(),muon1->p4().z(),muon1->p4().t());
	 muMinus=TLorentzVector(muon2->p4().x(),muon2->p4().y(),muon2->p4().z(),muon2->p4().t());
      }else{
	 muPlus=TLorentzVector(muon2->p4().x(),muon2->p4().y(),muon2->p4().z(),muon2->p4().t());
	 muMinus=TLorentzVector(muon1->p4().x(),muon1->p4().y(),muon1->p4().z(),muon1->p4().t());
      }
	 
      float *phiTheta=calcPol(muPlus,muMinus);

      cand_pt[cand_size]=cand->pt();
      cand_eta[cand_size] = cand->eta();
      cand_phi[cand_size] = cand->phi();
      cand_mass[cand_size] = cand->mass();
      cand_charge[cand_size] = cand->charge();
      cand_rapidity[cand_size] = cand->rapidity();
      cand_p[cand_size] = cand->p();
      cand_energy[cand_size] = cand->energy();
      cand_et[cand_size] = cand->et();
      cand_px[cand_size] = cand->px();
      cand_py[cand_size] = cand->py();
      cand_pz[cand_size] = cand->pz();
      cand_theta[cand_size] = cand->theta();
      cand_vProb[cand_size] = cand->userFloat("vProb");
      cand_dPhi[cand_size] =  deltaPhi(muon1_phi[cand_size],muon2_phi[cand_size]);
      cand_PolHXphi[cand_size] = phiTheta[0];
      cand_PolHXtheta[cand_size] = phiTheta[1];
      
      muon1_pt[cand_size]=muon1->pt();
      muon1_eta[cand_size] = muon1->eta();
      muon1_phi[cand_size] = muon1->phi();
      muon1_mass[cand_size] = muon1->mass();
      muon1_charge[cand_size] = muon1->charge();
      muon1_StandAlone[cand_size] = muon1->isStandAloneMuon();
      muon1_Global[cand_size] = muon1->isGlobalMuon();
      muon1_Tracker[cand_size] = muon1->isTrackerMuon();
      muon1_rapidity[cand_size] = muon1->rapidity();
      muon1_numberOfValidHits[cand_size]=muon1->innerTrack()->numberOfValidHits();
      muon1_pixelLayersWithMeasurement[cand_size]=muon1->innerTrack()->hitPattern().pixelLayersWithMeasurement();
      muon1_trackerLayersWithMeasurement[cand_size]=muon1->innerTrack()->hitPattern().trackerLayersWithMeasurement();
      muon1_normalizedChi2[cand_size]=muon1->innerTrack()->normalizedChi2();
      muon1_dz[cand_size]=muon1->innerTrack()->dz();
      muon1_dB[cand_size]=muon1->dB();
      muon1_numberOfMatchedStations[cand_size]=muon1->numberOfMatchedStations();
      muon1_ndof[cand_size]=muon1->innerTrack()->ndof();
      muon1_trkArbit[cand_size]=bool(muon1->muonID("TrackerMuonArbitrated"));
      muon1_isGoodMuon[cand_size]=bool(muon::isGoodMuon((*muon1),muon::TMOneStationTight));
      muon1_l1DeltaR[cand_size]=muon1->userFloat("muonL1Info:deltaR");
      muon1_l1Quality[cand_size]=muon1->userInt("muonL1Info:quality");
      muon1_pass[cand_size] = bool(mu1HLTMatchesPath.size());

      muon2_pt[cand_size]=muon2->pt();
      muon2_eta[cand_size] = muon2->eta();
      muon2_phi[cand_size] = muon2->phi();
      muon2_mass[cand_size] = muon2->mass();
      muon2_charge[cand_size] = muon2->charge();
      muon2_StandAlone[cand_size] = muon2->isStandAloneMuon();
      muon2_Global[cand_size] = muon2->isGlobalMuon();
      muon2_Tracker[cand_size] = muon2->isTrackerMuon();
      muon2_rapidity[cand_size] = muon2->rapidity();
      muon2_numberOfValidHits[cand_size]=muon2->innerTrack()->numberOfValidHits();
      muon2_pixelLayersWithMeasurement[cand_size]=muon2->innerTrack()->hitPattern().pixelLayersWithMeasurement();
      muon2_trackerLayersWithMeasurement[cand_size]=muon2->innerTrack()->hitPattern().trackerLayersWithMeasurement();
      muon2_normalizedChi2[cand_size]=muon2->innerTrack()->normalizedChi2();
      muon2_dz[cand_size]=muon2->innerTrack()->dz();
      muon2_dB[cand_size]=muon2->dB();
      muon2_numberOfMatchedStations[cand_size]=muon2->numberOfMatchedStations();
      muon2_ndof[cand_size]=muon2->innerTrack()->ndof();
      muon2_trkArbit[cand_size]=bool(muon2->muonID("TrackerMuonArbitrated"));
      muon2_isGoodMuon[cand_size]=bool(muon::isGoodMuon((*muon2),muon::TMOneStationTight));
      muon2_l1DeltaR[cand_size]=muon2->userFloat("muonL1Info:deltaR");
      muon2_l1Quality[cand_size]=muon2->userInt("muonL1Info:quality");
      muon2_pass[cand_size] = bool(mu2HLTMatchesPath.size());

      cand_size++;
    }
  }
  
  acceptedCand=cand_size;
  candTree->Fill();
}

float* UPCPatCandidateAnalyzer::calcPol(TLorentzVector muplus_LAB,
	     TLorentzVector muminus_LAB){
   const double pbeam = 3500.;
    // masses
   const double Mprot = 0.9382720;
   const double Ebeam = sqrt( pbeam*pbeam + Mprot*Mprot );
   const TLorentzVector beam1_LAB( 0., 0., pbeam, Ebeam );
   const TLorentzVector beam2_LAB( 0., 0., -pbeam, Ebeam );
   
   TLorentzVector qqbar_LAB = muplus_LAB + muminus_LAB;
   Double_t rapidity = qqbar_LAB.Rapidity();

   TVector3 LAB_to_QQBAR = -qqbar_LAB.BoostVector();

   TLorentzVector beam1_QQBAR = beam1_LAB;
   beam1_QQBAR.Boost( LAB_to_QQBAR );

   TLorentzVector beam2_QQBAR = beam2_LAB;
   beam2_QQBAR.Boost( LAB_to_QQBAR );

   TLorentzVector muplus_QQBAR = muplus_LAB;
   muplus_QQBAR.Boost( LAB_to_QQBAR );
   
   TVector3 beam1_direction     = beam1_QQBAR.Vect().Unit();
   TVector3 beam2_direction     = beam2_QQBAR.Vect().Unit();
   TVector3 qqbar_direction     = qqbar_LAB.Vect().Unit();
   TVector3 beam1_beam2_bisect  = ( beam1_direction - beam2_direction ).Unit();

   TVector3 Yaxis = ( beam1_direction.Cross( beam2_direction ) ).Unit();
   if ( rapidity < 0. ) Yaxis = -Yaxis; //H: added (5 Dec 2010)

   TVector3 newZaxis = qqbar_direction;
   TVector3 newYaxis = Yaxis;
   TVector3 newXaxis = newYaxis.Cross( newZaxis );

   TRotation rotation;
   rotation.RotateAxes( newXaxis, newYaxis, newZaxis );
   rotation.Invert();

   muplus_QQBAR.Transform( rotation );

   float *phiTheta=new float[2];

   phiTheta[0]=muplus_QQBAR.Phi();
   phiTheta[1]=muplus_QQBAR.Theta();

   return(phiTheta);
}
