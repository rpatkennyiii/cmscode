# Auto generated configuration file
# using: 
# Revision: 1.341.2.2 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions STARTHI44_V12::All -s RAW2DIGI,L1Reco,RECO --datatier RECO --eventcontent RECO --filein file:../STARlight_jPsi2mumu_RAW44Y_0.root --fileout file:testPPReRECO.root --mc
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import addMCinfo, changeRecoMuonInput, useL1MatchingWindowForSinglets, changeTriggerProcessName, switchOffAmbiguityResolution
import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi
import sys
#import Producers.StarLightProducer.thinningSTARlightMC_cfi as outputStuff

process = cms.Process('RECO')

# import of standard configurations
process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:STARlight_jPsi2mumu_RAW44Y_'+sys.argv[2]+'.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.341.2.2 $'),
    annotation = cms.untracked.string('step3 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

changeTriggerProcessName(process, "HLT")
switchOffAmbiguityResolution(process)

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
# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('file:testPPReRECO.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

process.onia2MuMuPatGlbTra = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string("(isGlobalMuon)"), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string("(isTrackerMuon)"),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)

process.onia2MuMuPatTraTra = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
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
    primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
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

# Additional output definition
process.RECOoutput.fileName=cms.untracked.string('STARlight_jPsi2mumu_RECOnia44Y_'+sys.argv[2]+'.root')
process.RECOoutput.outputCommands.append('keep *_*onia*_*_*')
process.RECOoutput.outputCommands.append('keep *_*patMuons*_*_*')
process.RECOoutput.outputCommands.append('keep *_*hiGenParticles*_*_*')

#outputStuff.keepThese.append('drop *Raw*_*_*_*')
#outputStuff.keepThese.append('drop *_towerMakerWithHO_*_*')

#process.RECODEBUGoutput.outputCommands=outputStuff.keepThese
# Other statements
process.GlobalTag.globaltag = 'STARTHI44_V12::All'

# Path and EndPath definitions
process.muonPat_step = cms.Path(process.patMuonsWithTriggerSequence+process.onia2MuMuPatGlbTra+process.onia2MuMuPatTraTra+process.onia2MuMuPatGlbGlb)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.muonPat_step,process.endjob_step,process.RECOoutput_step)

