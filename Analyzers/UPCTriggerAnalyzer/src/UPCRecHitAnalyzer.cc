//---------------------------------------------------------------------------
// -*- C++ -*-
//
// Package:    UPCRecHitAnalyzer
// Class:      UPCRecHitAnalyzer
// 
// class UPCRecHitAnalyzer UPCRecHitAnalyzer.cc HcalNoise/UPCRecHitAnalyzer/src/UPCRecHitAnalyzer.cc
//
// Original Author: Pat Kenny, +1 785-218-5488,
// Base Code Author: Yi Chen,40 3-B12,+41227675736,
//         Created:  Tue Mar 5 09:55:72 CST 2013
//
//
//---------------------------------------------------------------------------
#include <memory>
#include <string>
#include <map>
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
#include "TTree.h"
#include "TFile.h"
//---------------------------------------------------------------------------
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"

#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

//---------------------------------------------------------------------------
class UPCRecHitAnalyzer;
//---------------------------------------------------------------------------
class UPCRecHitAnalyzer : public edm::EDAnalyzer
{
public:
   explicit UPCRecHitAnalyzer(const edm::ParameterSet&);
   ~UPCRecHitAnalyzer();

private:
   virtual void beginJob();
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   virtual void endJob();

   bool FillHBHEDigis;                  // Whether to store HBHE digi-level information or not
   bool FillHFDigis;                    // Whether to store HF digi-level information or not
   bool FillHBHE;                  // Whether to store HBHE digi-level information or not
   bool FillHF;                    // Whether to store HF digi-level information or not
   bool FillEB;                    // Whether to store EB digi-level information or not
   bool FillEE;                    // Whether to store EE digi-level information or not
   bool FillHBHESumOnly;                  // Whether to store HBHE digi-level information or not
   bool FillHFSumOnly;                    // Whether to store HF digi-level information or not
   bool FillEBSumOnly;                    // Whether to store EB digi-level information or not
   bool FillEESumOnly;                    // Whether to store EE digi-level information or not
   double TotalChargeThreshold;    // To avoid trees from overweight, only store digis above some threshold
   string sHBHERecHitCollection;   // Name of the HBHE rechit collection
   edm::Service<TFileService> FileService;

   // HBHE rechits and digis
   int PulseCount;
   double HBEnergySumPlus;
   double HBEnergySumMinus;
   double HBEnergyMaxPlus;
   double HBEnergyMaxMinus;
   double HBEtaMaxPlus;
   double HBEtaMaxMinus;
   double HBPhiMaxPlus;
   double HBPhiMaxMinus;

   double HEEnergySumPlus;
   double HEEnergySumMinus;
   double HEEnergyMaxPlus;
   double HEEnergyMaxMinus;
   double HEEtaMaxPlus;
   double HEEtaMaxMinus;
   double HEPhiMaxPlus;
   double HEPhiMaxMinus;

   double Charge[5184][10];
   double Pedestal[5184][10];
   double Energy[5184];
   float Eta[5184];
   float Phi[5184];
   int IEta[5184];
   int IPhi[5184];
   int Depth[5184];
   double RecHitTime[5184];
   uint32_t FlagWord[5184];
   uint32_t AuxWord[5184];

   // HF rechits and digis
   int HFPulseCount;
   double HFEnergySumPlus;
   double HFEnergySumMinus;
   double HFEnergyMaxPlus;
   double HFEnergyMaxMinus;
   double HFEtaMaxPlus;
   double HFPhiMaxPlus;
   double HFEtaMaxMinus;
   double HFPhiMaxMinus;
   double HFCharge[1726][10];
   double HFPedestal[1726][10];
   float HFEta[1726];
   float HFPhi[1726];
   int HFIEta[1726];
   int HFIPhi[1726];
   int HFDepth[1726];
   double HFEnergy[1726];

   // EB rechits and digis
   int EBPulseCount;
   double EBEnergySumPlus;
   double EBEnergySumMinus;
   double EBEnergyMaxPlus;
   double EBEnergyMaxMinus;
   double EBEtaMaxPlus;
   double EBPhiMaxPlus;
   double EBEtaMaxMinus;
   double EBPhiMaxMinus;
   float EBEta[60581];
   float EBPhi[60581];
   double EBEnergy[60581];

   // EE rechits and digis
   int EEPulseCount;
   double EEEnergySumPlus;
   double EEEnergySumMinus;
   double EEEnergyMaxPlus;
   double EEEnergyMaxMinus;
   double EEEtaMaxPlus;
   double EEPhiMaxPlus;
   double EEEtaMaxMinus;
   double EEPhiMaxMinus;
   float EEEta[14407];
   float EEPhi[14407];
   double EEEnergy[14407];

   TTree *HBHETree;
   TTree *HFTree;
   TTree *EBTree;
   TTree *EETree;

   const CaloGeometry *Geometry;

   void ClearVariables();
};
//---------------------------------------------------------------------------
UPCRecHitAnalyzer::UPCRecHitAnalyzer(const edm::ParameterSet& iConfig)
{
   // Get stuff and initialize here
   FillHBHEDigis = iConfig.getUntrackedParameter<bool>("FillHBHEDigis", false);
   FillHFDigis = iConfig.getUntrackedParameter<bool>("FillHFDigis", false);
   FillHBHE = iConfig.getUntrackedParameter<bool>("FillHBHE", true);
   FillHF = iConfig.getUntrackedParameter<bool>("FillHF", true);
   FillEB = iConfig.getUntrackedParameter<bool>("FillEB", false);
   FillEE = iConfig.getUntrackedParameter<bool>("FillEE", false);
   FillHBHESumOnly = iConfig.getUntrackedParameter<bool>("FillHBHESumOnly", false);
   FillHFSumOnly = iConfig.getUntrackedParameter<bool>("FillHFSumOnly", false);
   FillEBSumOnly = iConfig.getUntrackedParameter<bool>("FillEBSumOnly", false);
   FillEESumOnly = iConfig.getUntrackedParameter<bool>("FillEESumOnly", false);
   TotalChargeThreshold = iConfig.getUntrackedParameter<double>("TotalChargeThreshold", 10);

   sHBHERecHitCollection = iConfig.getUntrackedParameter<string>("HBHERecHits", "hbhereco");

}
//---------------------------------------------------------------------------
UPCRecHitAnalyzer::~UPCRecHitAnalyzer()
{
}
//---------------------------------------------------------------------------
void UPCRecHitAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   //using namespace reco;

   ClearVariables();

   // get stuff
   Handle<HBHERecHitCollection> hRecHits;
   iEvent.getByLabel(InputTag(sHBHERecHitCollection), hRecHits);

   Handle<HFRecHitCollection> hHFRecHits;
   iEvent.getByLabel(InputTag("hfreco"), hHFRecHits);

   Handle<HBHEDigiCollection> hHBHEDigis;
   if(FillHBHEDigis) iEvent.getByType(hHBHEDigis);

   Handle<HFDigiCollection> hHFDigis;
   if(FillHFDigis) iEvent.getByType(hHFDigis);

   Handle<EcalRecHitCollection> EBRecHits;
   iEvent.getByLabel(InputTag("ecalRecHit", "EcalRecHitsEB"), EBRecHits);

   Handle<EcalRecHitCollection> EERecHits;
   iEvent.getByLabel(InputTag("ecalRecHit", "EcalRecHitsEE"), EERecHits);

   ESHandle<HcalDbService> hConditions;
   iSetup.get<HcalDbRecord>().get(hConditions);

   ESHandle<CaloGeometry> hGeometry;
   iSetup.get<CaloGeometryRecord>().get(hGeometry);
   Geometry = hGeometry.product();

   // HBHE rechit maps - we want to link rechits and digis together
   map<HcalDetId, int> RecHitIndex;
   for(int i = 0; i < (int)hRecHits->size(); i++)
   {
      HcalDetId id = (*hRecHits)[i].id();
      RecHitIndex.insert(pair<HcalDetId, int>(id, i));
   }

   // HF rechit maps
   map<HcalDetId, int> HFRecHitIndex;
   for(int i = 0; i < (int)hHFRecHits->size(); i++)
   {
      HcalDetId id = (*hHFRecHits)[i].id();
      HFRecHitIndex.insert(pair<HcalDetId, int>(id, i));
   }

   HBHEDigiCollection::const_iterator iterHBHE;
   if(FillHBHEDigis) iterHBHE = hHBHEDigis->begin();

   // loop over digis
   for(int i = 0; i < (int)hRecHits->size(); i++)
   {
      HcalDetId id = (*hRecHits)[i].id();
     
      // First let's convert ADC to deposited charge
      const HcalCalibrations &Calibrations = hConditions->getHcalCalibrations(id);
      const HcalQIECoder *ChannelCoder = hConditions->getHcalCoder(id);
      const HcalQIEShape *Shape = hConditions->getHcalShape();
      HcalCoderDb Coder(*ChannelCoder, *Shape);
      CaloSamples Tool;
      if(FillHBHEDigis) Coder.adc2fC(*iterHBHE, Tool);

      // Total charge of the digi
      double TotalCharge = 0;
      for(int i = 0; i < FillHBHEDigis?(int)iterHBHE->size():0; i++)
         TotalCharge = TotalCharge + Tool[i] - Calibrations.pedestal(iterHBHE->sample(i).capid());

      // If total charge is smaller than threshold, don't store this rechit/digi into the tree
      if(TotalCharge < TotalChargeThreshold)
         continue;

      // Safety check - there are only 5184 channels in HBHE, but just in case...
      if(PulseCount >= 5184)
      {
         PulseCount = PulseCount + 1;
         continue;
      }

      // Fill things into the tree
      for(int i = 0; FillHBHEDigis?(int)iterHBHE->size():0; i++)
      {
         const HcalQIESample &QIE = iterHBHE->sample(i);

         Pedestal[PulseCount][i] = Calibrations.pedestal(QIE.capid());
         Charge[PulseCount][i] = Tool[i] - Pedestal[PulseCount][i];
      }

      IEta[PulseCount] = id.ieta();
      IPhi[PulseCount] = id.iphi();
      Eta[PulseCount] = Geometry->getPosition(id).eta();
      Phi[PulseCount] = Geometry->getPosition(id).phi();
      Depth[PulseCount] = id.depth();

      Energy[PulseCount] = (*hRecHits)[RecHitIndex[id]].energy();
      
      double eta=Eta[PulseCount], energy=Energy[PulseCount], phi=Phi[PulseCount];
      HBEnergySumPlus += energy*double(abs(eta)<=1.305&&eta>=0);
      HBEnergySumMinus += energy*double(abs(eta)<=1.305&&eta<0);
      HBEtaMaxPlus = (energy*double(abs(eta)<=1.305)>HBEnergyMaxPlus&&eta>=0)?eta:HBEtaMaxPlus;
      HBPhiMaxPlus = (energy*double(abs(eta)<=1.305)>HBEnergyMaxPlus&&eta>=0)?phi:HBPhiMaxPlus;
      HBEtaMaxMinus = (energy*double(abs(eta)<=1.305)>HBEnergyMaxMinus&&eta<0)?eta:HBEtaMaxMinus;
      HBPhiMaxMinus = (energy*double(abs(eta)<=1.305)>HBEnergyMaxMinus&&eta<0)?phi:HBPhiMaxMinus;
      HBEnergyMaxPlus = (energy*double(abs(eta)<=1.305)>HBEnergyMaxPlus&&eta>=0)?energy:HBEnergyMaxPlus;
      HBEnergyMaxMinus = (energy*double(abs(eta)<=1.305)>HBEnergyMaxMinus&&eta<0)?energy:HBEnergyMaxMinus;

      HEEnergySumPlus += energy*double(abs(eta)>=1.392&&eta>=0);
      HEEnergySumMinus += energy*double(abs(eta)>=1.392&&eta<0);
      HEEtaMaxPlus = (energy*double(abs(eta)>=1.392)>HEEnergyMaxPlus&&eta>=0)?eta:HEEtaMaxPlus;
      HEPhiMaxPlus = (energy*double(abs(eta)>=1.392)>HEEnergyMaxPlus&&eta>=0)?phi:HEPhiMaxPlus;
      HEEtaMaxMinus = (energy*double(abs(eta)>=1.392)>HEEnergyMaxMinus&&eta<0)?eta:HEEtaMaxMinus;
      HEPhiMaxMinus = (energy*double(abs(eta)>=1.392)>HEEnergyMaxMinus&&eta<0)?phi:HEPhiMaxMinus;
      HEEnergyMaxPlus = (energy*double(abs(eta)>=1.392)>HEEnergyMaxPlus&&eta>=0)?energy:HEEnergyMaxPlus;
      HEEnergyMaxMinus = (energy*double(abs(eta)>=1.392)>HEEnergyMaxMinus&&eta<0)?energy:HEEnergyMaxMinus;

      RecHitTime[PulseCount] = (*hRecHits)[RecHitIndex[id]].time();

      FlagWord[PulseCount] = (*hRecHits)[RecHitIndex[id]].flags();
      AuxWord[PulseCount] = (*hRecHits)[RecHitIndex[id]].aux();

      PulseCount = PulseCount + 1;
      if(FillHBHEDigis) iterHBHE++;
   }

   HFDigiCollection::const_iterator iterHF;
   if(FillHFDigis) iterHF = hHFDigis->begin();
   // Loop over HF digis
   for(int i = 0; i < (int)hHFRecHits->size(); i++)
   {
      HcalDetId id = (*hHFRecHits)[i].id();

      // ADC -> fC
      const HcalCalibrations &Calibrations = hConditions->getHcalCalibrations(id);
      const HcalQIECoder *ChannelCoder = hConditions->getHcalCoder(id);
      const HcalQIEShape *Shape = hConditions->getHcalShape();
      HcalCoderDb Coder(*ChannelCoder, *Shape);
      CaloSamples Tool;
      if(FillHFDigis) Coder.adc2fC(*iterHF, Tool);

      // Fill!
      for(int i = 0; i < FillHFDigis?iterHF->size():0; i++)
      {
         const HcalQIESample &QIE = iterHF->sample(i);

         HFPedestal[HFPulseCount][i] = Calibrations.pedestal(QIE.capid());
         HFCharge[HFPulseCount][i] = Tool[i] - HFPedestal[HFPulseCount][i];
      }

      HFIEta[HFPulseCount] = id.ieta();
      HFIPhi[HFPulseCount] = id.iphi();
      HFEta[HFPulseCount] = Geometry->getPosition(id).eta();
      HFPhi[HFPulseCount] = Geometry->getPosition(id).phi();
      HFDepth[HFPulseCount] = id.depth();

      HFEnergy[HFPulseCount] = (*hHFRecHits)[HFRecHitIndex[id]].energy();

      double eta=HFEta[HFPulseCount], energy=HFEnergy[HFPulseCount], phi=HFPhi[HFPulseCount];
      HFEnergySumPlus += energy*double(eta>=0)*double(abs(eta)>=3.139);
      HFEnergySumMinus += energy*double(eta<0)*double(abs(eta)>=3.139);
      HFEtaMaxPlus = (energy*double(abs(eta)>=3.139)>HFEnergyMaxPlus&&eta>=0)?eta:HFEtaMaxPlus;
      HFPhiMaxPlus = (energy*double(abs(eta)>=3.139)>HFEnergyMaxPlus&&eta>=0)?phi:HFPhiMaxPlus;
      HFEtaMaxMinus = (energy*double(abs(eta)>=3.139)>HFEnergyMaxMinus&&eta<0)?eta:HFEtaMaxMinus;
      HFPhiMaxMinus = (energy*double(abs(eta)>=3.139)>HFEnergyMaxMinus&&eta<0)?phi:HFPhiMaxMinus;
      HFEnergyMaxPlus = (energy*double(abs(eta)>=3.139)>HFEnergyMaxPlus&&eta>=0)?energy:HFEnergyMaxPlus;
      HFEnergyMaxMinus = (energy*double(abs(eta)>=3.139)>HFEnergyMaxMinus&&eta<0)?energy:HFEnergyMaxMinus;

      HFPulseCount = HFPulseCount + 1;
      if(FillHFDigis) iterHF++;
   }

// EB RecHits
   for(int i = 0; i < (int)(*EBRecHits).size(); i++)
   {
      EBEta[EBPulseCount] = Geometry->getPosition((*EBRecHits)[i].id()).eta();
      EBPhi[EBPulseCount] = Geometry->getPosition((*EBRecHits)[i].id()).phi();
      EBEnergy[EBPulseCount] = (*EBRecHits)[i].energy();
      EBEnergySumPlus += (*EBRecHits)[i].energy()*double(EBEta[EBPulseCount]>=0);
      EBEnergySumMinus += (*EBRecHits)[i].energy()*double(EBEta[EBPulseCount]<0);
      EBEtaMaxPlus = ((*EBRecHits)[i].energy()>EBEnergyMaxPlus&&EBEta[EBPulseCount]>=0)?EBEta[EBPulseCount]:EBEtaMaxPlus;
      EBPhiMaxPlus = ((*EBRecHits)[i].energy()>EBEnergyMaxPlus&&EBEta[EBPulseCount]>=0)?EBPhi[EBPulseCount]:EBPhiMaxPlus;
      EBEtaMaxMinus = ((*EBRecHits)[i].energy()>EBEnergyMaxMinus&&EBEta[EBPulseCount]<0)?EBEta[EBPulseCount]:EBEtaMaxMinus;
      EBPhiMaxMinus = ((*EBRecHits)[i].energy()>EBEnergyMaxMinus&&EBEta[EBPulseCount]<0)?EBPhi[EBPulseCount]:EBPhiMaxMinus;
      EBEnergyMaxPlus = ((*EBRecHits)[i].energy()>EBEnergyMaxPlus&&EBEta[EBPulseCount]>=0)?(*EBRecHits)[i].energy():EBEnergyMaxPlus;
      EBEnergyMaxMinus = ((*EBRecHits)[i].energy()>EBEnergyMaxMinus&&EBEta[EBPulseCount]<0)?(*EBRecHits)[i].energy():EBEnergyMaxMinus;
      EBPulseCount = EBPulseCount + 1;
   }

// EE RecHits
   for(int i = 0; i < (int)(*EERecHits).size(); i++)
   {
      EEEta[EEPulseCount] = Geometry->getPosition((*EERecHits)[i].id()).eta();
      EEPhi[EEPulseCount] = Geometry->getPosition((*EERecHits)[i].id()).phi();
      EEEnergy[EEPulseCount] = (*EERecHits)[i].energy();
      EEEnergySumPlus += (*EERecHits)[i].energy()*double(EEEta[EEPulseCount]>=0);
      EEEnergySumMinus += (*EERecHits)[i].energy()*double(EEEta[EEPulseCount]<0);
      EEEtaMaxPlus = ((*EERecHits)[i].energy()>EEEnergyMaxPlus&&EEEta[EEPulseCount]>=0)?EEEta[EEPulseCount]:EEEtaMaxPlus;
      EEPhiMaxPlus = ((*EERecHits)[i].energy()>EEEnergyMaxPlus&&EEEta[EEPulseCount]>=0)?EEPhi[EEPulseCount]:EEPhiMaxPlus;
      EEEtaMaxMinus = ((*EERecHits)[i].energy()>EEEnergyMaxMinus&&EEEta[EEPulseCount]<0)?EEEta[EEPulseCount]:EEEtaMaxMinus;
      EEPhiMaxMinus = ((*EERecHits)[i].energy()>EEEnergyMaxMinus&&EEEta[EEPulseCount]<0)?EEPhi[EEPulseCount]:EEPhiMaxMinus;
      EEEnergyMaxPlus = ((*EERecHits)[i].energy()>EEEnergyMaxPlus&&EEEta[EEPulseCount]>=0)?(*EERecHits)[i].energy():EEEnergyMaxPlus;
      EEEnergyMaxMinus = ((*EERecHits)[i].energy()>EEEnergyMaxMinus&&EEEta[EEPulseCount]<0)?(*EERecHits)[i].energy():EEEnergyMaxMinus;
      EEPulseCount = EEPulseCount + 1;
   }
   
// finally actually fill the tree
   if(FillHBHE) HBHETree->Fill();
   if(FillHF) HFTree->Fill();
   if(FillEB) EBTree->Fill();
   if(FillEE) EETree->Fill();
}
//---------------------------------------------------------------------------
void UPCRecHitAnalyzer::beginJob()
{
   // Make branches in the output trees
   FileService->file().cd();

   if(FillHBHE)
   {
      HBHETree = new TTree("HBHERecHitTree","HBHERecHitTree");
      HBHETree->Branch("PulseCount", &PulseCount, "PulseCount/I");
      if(FillHBHESumOnly){
	 HBHETree->Branch("HEEnergySumPlus", &HEEnergySumPlus, "HEEnergySumPlus/D");
	 HBHETree->Branch("HEEnergySumMinus", &HEEnergySumMinus, "HEEnergySumMinus/D");
	 HBHETree->Branch("HEEnergyMaxPlus", &HEEnergyMaxPlus, "HEEnergyMaxPlus/D");
	 HBHETree->Branch("HEEnergyMaxMinus", &HEEnergyMaxMinus, "HEEnergyMaxMinus/D");
	 HBHETree->Branch("HEEtaMaxPlus", &HEEtaMaxPlus, "HEEtaMaxPlus/D");
	 HBHETree->Branch("HEPhiMaxPlus", &HEPhiMaxPlus, "HEPhiMaxPlus/D");
	 HBHETree->Branch("HEEtaMaxMinus", &HEEtaMaxMinus, "HEEtaMaxMinus/D");
	 HBHETree->Branch("HEPhiMaxMinus", &HEPhiMaxMinus, "HEPhiMaxMinus/D");

	 HBHETree->Branch("HBEnergySumPlus", &HBEnergySumPlus, "HBEnergySumPlus/D");
	 HBHETree->Branch("HBEnergySumMinus", &HBEnergySumMinus, "HBEnergySumMinus/D");
	 HBHETree->Branch("HBEnergyMaxPlus", &HBEnergyMaxPlus, "HBEnergyMaxPlus/D");
	 HBHETree->Branch("HBEnergyMaxMinus", &HBEnergyMaxMinus, "HBEnergyMaxMinus/D");
	 HBHETree->Branch("HBEtaMaxPlus", &HBEtaMaxPlus, "HBEtaMaxPlus/D");
	 HBHETree->Branch("HBPhiMaxPlus", &HBPhiMaxPlus, "HBPhiMaxPlus/D");
	 HBHETree->Branch("HBEtaMaxMinus", &HBEtaMaxMinus, "HBEtaMaxMinus/D");
	 HBHETree->Branch("HBPhiMaxMinus", &HBPhiMaxMinus, "HBPhiMaxMinus/D");
      } else {
	 HBHETree->Branch("Energy", &Energy, "Energy[PulseCount]/D");
	 HBHETree->Branch("Eta", &Eta, "Eta[PulseCount]/F");
	 HBHETree->Branch("Phi", &Phi, "Phi[PulseCount]/F");
	 HBHETree->Branch("IEta", &IEta, "IEta[PulseCount]/I");
	 HBHETree->Branch("IPhi", &IPhi, "IPhi[PulseCount]/I");
	 HBHETree->Branch("Depth", &Depth, "Depth[PulseCount]/I");
	 HBHETree->Branch("RecHitTime", &RecHitTime, "RecHitTime[PulseCount]/D");
	 HBHETree->Branch("FlagWord", &FlagWord, "FlagWord[PulseCount]/i");
	 HBHETree->Branch("AuxWord", &AuxWord, "AuxWord[PulseCount]/i");

	 if(FillHBHEDigis){
		 HBHETree->Branch("Charge", &Charge, "Charge[PulseCount][10]/D");
		 HBHETree->Branch("Pedestal", &Pedestal, "Pedestal[PulseCount][10]/D");
	 }
      }
   }

   if(FillHF)
   {
      HFTree = new TTree("HFRecHitTree","HFRecHitTree");
      HFTree->Branch("HFPulseCount", &HFPulseCount, "HFPulseCount/I");
      if(FillHFSumOnly){
	 HFTree->Branch("HFEnergySumPlus", &HFEnergySumPlus, "HFEnergySumPlus/D");
	 HFTree->Branch("HFEnergySumMinus", &HFEnergySumMinus, "HFEnergySumMinus/D");
	 HFTree->Branch("HFEnergyMaxPlus", &HFEnergyMaxPlus, "HFEnergyMaxPlus/D");
	 HFTree->Branch("HFEnergyMaxMinus", &HFEnergyMaxMinus, "HFEnergyMaxMinus/D");
	 HFTree->Branch("HFEtaMaxPlus", &HFEtaMaxPlus, "HFEtaMaxPlus/D");
	 HFTree->Branch("HFPhiMaxPlus", &HFPhiMaxPlus, "HFPhiMaxPlus/D");
	 HFTree->Branch("HFEtaMaxMinus", &HFEtaMaxMinus, "HFEtaMaxMinus/D");
	 HFTree->Branch("HFPhiMaxMinus", &HFPhiMaxMinus, "HFPhiMaxMinus/D");
      } else {
	 HFTree->Branch("HFPhi", &HFPhi, "HFPhi[HFPulseCount]/F");
	 HFTree->Branch("HFEta", &HFEta, "HFEta[HFPulseCount]/F");
	 HFTree->Branch("HFIPhi", &HFIPhi, "HFIPhi[HFPulseCount]/I");
	 HFTree->Branch("HFIEta", &HFIEta, "HFIEta[HFPulseCount]/I");
	 HFTree->Branch("HFDepth", &HFDepth, "HFDepth[HFPulseCount]/I");
	 HFTree->Branch("HFEnergy", &HFEnergy, "HFEnergy[HFPulseCount]/D");

	 if(FillHFDigis){
	     HFTree->Branch("HFCharge", &HFCharge, "HFCharge[HFPulseCount][10]/D");
	     HFTree->Branch("HFPedestal", &HFPedestal, "HFPedestal[HFPulseCount][10]/D");
	 }
      }
   }

   if(FillEB)
   {
      EBTree = new TTree("EBRecHitTree","EBRecHitTree");
      EBTree->Branch("EBPulseCount", &EBPulseCount, "EBPulseCount/I");
      if(FillEBSumOnly){
	 EBTree->Branch("EBEnergySumPlus", &EBEnergySumPlus, "EBEnergySumPlus/D");
	 EBTree->Branch("EBEnergySumMinus", &EBEnergySumMinus, "EBEnergySumMinus/D");
	 EBTree->Branch("EBEnergyMaxPlus", &EBEnergyMaxPlus, "EBEnergyMaxPlus/D");
	 EBTree->Branch("EBEnergyMaxMinus", &EBEnergyMaxMinus, "EBEnergyMaxMinus/D");
	 EBTree->Branch("EBEtaMaxPlus", &EBEtaMaxPlus, "EBEtaMaxPlus/D");
	 EBTree->Branch("EBPhiMaxPlus", &EBPhiMaxPlus, "EBPhiMaxPlus/D");
	 EBTree->Branch("EBEtaMaxMinus", &EBEtaMaxMinus, "EBEtaMaxMinus/D");
	 EBTree->Branch("EBPhiMaxMinus", &EBPhiMaxMinus, "EBPhiMaxMinus/D");
      } else {
	 EBTree->Branch("EBPhi", &EBPhi, "EBPhi[EBPulseCount]/F");
	 EBTree->Branch("EBEta", &EBEta, "EBEta[EBPulseCount]/F");
	 EBTree->Branch("EBEnergy", &EBEnergy, "EBEnergy[EBPulseCount]/D");
      }
   }

   if(FillEE)
   {
      EETree = new TTree("EERecHitTree","EERecHitTree");
      EETree->Branch("EEPulseCount", &EEPulseCount, "EEPulseCount/I");
      if(FillEESumOnly){
	 EETree->Branch("EEEnergySumPlus", &EEEnergySumPlus, "EEEnergySumPlus/D");
	 EETree->Branch("EEEnergySumMinus", &EEEnergySumMinus, "EEEnergySumMinus/D");
	 EETree->Branch("EEEnergyMaxPlus", &EEEnergyMaxPlus, "EEEnergyMaxPlus/D");
	 EETree->Branch("EEEnergyMaxMinus", &EEEnergyMaxMinus, "EEEnergyMaxMinus/D");
	 EETree->Branch("EEEtaMaxPlus", &EEEtaMaxPlus, "EEEtaMaxPlus/D");
	 EETree->Branch("EEPhiMaxPlus", &EEPhiMaxPlus, "EEPhiMaxPlus/D");
	 EETree->Branch("EEEtaMaxMinus", &EEEtaMaxMinus, "EEEtaMaxMinus/D");
	 EETree->Branch("EEPhiMaxMinus", &EEPhiMaxMinus, "EEPhiMaxMinus/D");
      } else {
	 EETree->Branch("EEPhi", &EEPhi, "EEPhi[EEPulseCount]/F");
	 EETree->Branch("EEEta", &EEEta, "EEEta[EEPulseCount]/F");
	 EETree->Branch("EEEnergy", &EEEnergy, "EEEnergy[EEPulseCount]/D");
      }
   }
}
//---------------------------------------------------------------------------
void UPCRecHitAnalyzer::endJob()
{
}
//---------------------------------------------------------------------------
void UPCRecHitAnalyzer::ClearVariables()
{
   PulseCount = 0;
   HFPulseCount = 0;
   EBPulseCount = 0;
   EEPulseCount = 0;
   HBEnergySumPlus = 0;
   HBEnergySumMinus = 0;
   HEEnergySumPlus = 0;
   HEEnergySumMinus = 0;
   HBEnergyMaxPlus = 0;
   HBEnergyMaxMinus = 0;
   HEEnergyMaxPlus = 0;
   HEEnergyMaxMinus = 0;
   HBEtaMaxPlus = 0;
   HBPhiMaxPlus = 0;
   HBEtaMaxMinus = 0;
   HBPhiMaxMinus = 0;
   HEEtaMaxPlus = 0;
   HEPhiMaxPlus = 0;
   HEEtaMaxMinus = 0;
   HEPhiMaxMinus = 0;
   HFEnergySumPlus = 0;
   HFEnergySumMinus = 0;
   HFEnergyMaxPlus = 0;
   HFEnergyMaxMinus = 0;
   HFEtaMaxPlus = 0;
   HFPhiMaxPlus = 0;
   HFEtaMaxMinus = 0;
   HFPhiMaxMinus = 0;
   EBEnergySumPlus = 0;
   EBEnergySumMinus = 0;
   EBEnergyMaxPlus = 0;
   EBEnergyMaxMinus = 0;
   EBEtaMaxPlus = 0;
   EBPhiMaxPlus = 0;
   EBEtaMaxMinus = 0;
   EBPhiMaxMinus = 0;
   EEEnergySumPlus = 0;
   EEEnergySumMinus = 0;
   EEEnergyMaxPlus = 0;
   EEEnergyMaxMinus = 0;
   EEEtaMaxPlus = 0;
   EEPhiMaxPlus = 0;
   EEEtaMaxMinus = 0;
   EEPhiMaxMinus = 0;
}

DEFINE_FWK_MODULE(UPCRecHitAnalyzer);
