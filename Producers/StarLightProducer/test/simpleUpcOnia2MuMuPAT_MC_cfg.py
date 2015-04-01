from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import addMCinfo, changeRecoMuonInput, useL1MatchingWindowForSinglets, changeTriggerProcessName, switchOffAmbiguityResolution
import FWCore.ParameterSet.Config as cms
import sys
import HLTrigger.HLTfilters.hltHighLevel_cfi

process = cms.Process("UpcOnia2MuMuPAT")

process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'STARTHI44_V12::All'

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
infile=open(sys.argv[2])
outfile=(sys.argv[3])

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile.readlines())
)

#process.hltUpcOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
#process.hltUpcOniaHI.HLTPaths = ["HLT_HIUPCNeuMuPixel_SingleTrack_v1"]

changeTriggerProcessName(process, "HLT")
switchOffAmbiguityResolution(process)
process.patMuonsWithoutTrigger.pvSrc = "hiSelectedVertex"

process.muonL1Info.maxDeltaR = 0.3
process.muonL1Info.fallbackToME1 = True
process.muonMatchHLTL1.maxDeltaR = 0.3
process.muonMatchHLTL1.fallbackToME1 = True
process.muonMatchHLTL2.maxDeltaR = 0.3
process.muonMatchHLTL2.maxDPtRel = 10.0
process.muonMatchHLTL3.maxDeltaR = 0.1
process.muonMatchHLTL3.maxDPtRel = 10.0
process.muonMatchHLTCtfTrack.maxDeltaR = 0.1
process.muonMatchHLTCtfTrack.maxDPtRel = 10.0
process.muonMatchHLTTrackMu.maxDeltaR = 0.1
process.muonMatchHLTTrackMu.maxDPtRel = 10.0 

MC = False

process.onia2MuMuPatStaSta = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("hiSelectedVertex"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string("(isStandAloneMuon)"), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string("(isStandAloneMuon)"),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)

process.onia2MuMuPatTraTra = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("hiSelectedVertex"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string("(isTrackerMuon)"), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string("(isTrackerMuon)"),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)

process.onia2MuMuPatGlbGlb = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("hiSelectedVertex"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string("(isGlobalMuon)"), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string("(isGlobalMuon)"),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)
#process.onia2MuMuPatGlbGlbFilter = cms.EDFilter("CandViewCountFilter",
#    src = cms.InputTag('onia2MuMuPatGlbGlb'),
#    minNumber = cms.uint32(1),
#)

process.run = cms.Path(
#	process.hltUpcOniaHI
	process.patMuonsWithTriggerSequence
	+process.onia2MuMuPatStaSta
	+process.onia2MuMuPatTraTra
	+process.onia2MuMuPatGlbGlb
)

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands =  cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string(outfile),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('run'))
)

#process.output.outputCommands.extend(cms.untracked.vstring(
#        'keep *_mergedtruth_*_*',                              # tracking particles and tracking vertices for hit by hit matching
#        'keep *_hiGenParticles_*_*',                           # generated particles
#        'keep *_genMuons_*_*',                      # generated muons and parents
#        'keep patMuons_patMuonsWithTrigger_*_*',    # All PAT muons including matches to triggers
#        'keep patCompositeCandidates_*__*',         # PAT di-muons
#        'keep *_offlinePrimaryVertices_*_*',                   # Primary vertices: you want these to compute impact parameters
#        'keep *_offlineBeamSpot_*_*',                          # Beam spot: you want this for the same reason                                   
#        'keep edmTriggerResults_TriggerResults_*_*',           # HLT info, per path (cheap)
#        'keep l1extraL1MuonParticles_hltL1extraParticles_*_*', # L1 info (cheap)
#        'keep l1extraL1MuonParticles_l1extraParticles_*_*',    # L1 info (cheap)
#        'keep *_hiCentrality_*_*',
#        'keep *_hiSelectedVertex_*_*',
#        'keep *_hiSelectedTracks_*_*',
#        'keep *_standAloneMuons_*_*',
#        'keep *_castorreco_*_*',
#        'keep *_hfreco_*_*',
#        'keep *_towerMaker_*_*',
#        'keep ZDC*_*_*_*')
#)

process.e = cms.EndPath(process.output)
