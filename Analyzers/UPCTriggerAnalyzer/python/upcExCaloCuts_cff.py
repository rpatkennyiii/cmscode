import FWCore.ParameterSet.Config as cms

caloTowerCands = cms.EDProducer("CaloTowerCandidateCreator",
    src = cms.InputTag("towerMaker"),
    verbose = cms.untracked.int32(0),
    minimumE = cms.double(0.0),
    minimumEt = cms.double(0.0),
)

upcHBFilter = cms.EDFilter("CandSelector",
   src = cms.InputTag( "caloTowerCand" ),
   cut = cms.string( "energyInHB<=1.44" )
)
upcHEFilter = cms.EDFilter("CandSelector",
   src = cms.InputTag( "caloTowerCand" ),
   cut = cms.string( "energyInHE<=2.48" )
)
upcHFFilter = cms.EDFilter("CandSelector",
   src = cms.InputTag( "caloTowerCand" ),
   cut = cms.string( "energyInHF<=3.62" )
)
upcEBFilter = cms.EDFilter("CandSelector",
   src = cms.InputTag( "caloTowerCand" ),
   cut = cms.string( "emEnergy<=1.28 && ( abs(eta)<=1.47 )" )
)
upcEEFilter = cms.EDFilter("CandSelector",
   src = cms.InputTag( "caloTowerCand" ),
   cut = cms.string( "emEnergy<=2.97 && ( abs(eta)>1.47 && abs(eta)<=3 )" )
)
upcExCaloCuts = cms.Sequence(caloTowerCands*upcHBFilter*upcHEFilter*upcHFFilter*upcEBFilter*upcEEFilter)
