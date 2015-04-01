import FWCore.ParameterSet.Config as cms

onia2MuMuPatDiTrks = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("upcPatMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string(""), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string(""),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(False),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)
