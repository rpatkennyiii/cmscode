# Auto generated configuration file
# using: 
# Revision: 1.341.2.2 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: hiReco -n 100 -s RAW2DIGI,RECO --mc --scenario HeavyIons --conditions STARTHI44_V12::All --datatier RECO --eventcontent RECODEBUG --no_exec --filein file:STARlight_44Y.root --fileout STARlight_RECO_44Y.root --python_filename=test_recoStarLight.py
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import addMCinfo, changeRecoMuonInput, useL1MatchingWindowForSinglets, changeTriggerProcessName, switchOffAmbiguityResolution
import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi
import sys
import Producers.StarLightProducer.thinningSTARlightMC_cfi as outputStuff

process = cms.Process('RECO')

# import of standard configurations
process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('L1Trigger.Configuration.L1Extra_cff')


process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:jPsiGun2mumu_RAW44Y_'+sys.argv[2]+'.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.341.2.2 $'),
    annotation = cms.untracked.string('hiReco nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)

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


# Output definition

process.RECODEBUGoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring(),
    fileName = cms.untracked.string('jPsiGun2mumu_RECOnia44Y_'+sys.argv[2]+'.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

outputStuff.keepThese.append('drop *Raw*_*_*_*')
outputStuff.keepThese.append('drop *_towerMakerWithHO_*_*')

process.RECODEBUGoutput.outputCommands=outputStuff.keepThese

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'STARTHI44_V12::All'

# Path and EndPath definitions
process.muonPat_step = cms.Path(process.patMuonsWithTriggerSequence+process.onia2MuMuPatStaSta+process.onia2MuMuPatTraTra+process.onia2MuMuPatGlbGlb)
process.raw2digi_step = cms.Path(process.RawToDigi+process.L1Extra)
process.reconstruction_step = cms.Path(process.reconstructionHeavyIons)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECODEBUGoutput_step = cms.EndPath(process.RECODEBUGoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.muonPat_step,process.endjob_step,process.RECODEBUGoutput_step)
