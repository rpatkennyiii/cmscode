import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList
from CmsHi.Analysis2010.CommonFunctions_cff import *


outfile='UPCTree.root'

process = cms.Process("UPCTreeMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
#	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#	process.load('L1Trigger.Skimmer.l1Filter_cfi')
#	process.load('HLTrigger.HLTfilters.hltLevel1GTSeed_cfi')
#	process.load("HLTrigger.HLTanalyzers.HI_HLTAnalyser_cff")

#	process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('L1_BscMinBiasThreshold1')

#	process.upcNeutron = !(process.l1Filter.clone(
#		algorithms = cms.vstring('L1_BscMinBiasThreshold1')
#	))

#	process.hltanalysis.RunParameters.Debug = False
#	process.hltanalysis.RunParameters.UseTFileService = True
#	process.hltanalysis.RunParameters.Monte = False
#	process.hltanalysis.RunParameters.DoMC = False
#	process.hltanalysis.RunParameters.DoJets = True
#	process.hltanalysis.RunParameters.DoPhotons = True
#	process.hltanalysis.RunParameters.DoSuperClusters = True
#	process.hltanalysis.recjets  = "iterativeConePu5CaloJets"
#	process.hltanalysis.BarrelPhoton = "correctedIslandBarrelSuperClusters"
#	process.hltanalysis.EndcapPhoton = "correctedIslandEndcapSuperClusters"
#	process.hltanalysis.l1GtReadoutRecord = cms.InputTag("gtDigis")

overrideCentrality(process)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring())

#	myLumis = LumiList.LumiList(filename = 'eflow.json').getCMSSWString().split(',')
#	process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#	process.source.lumisToProcess.extend(myLumis)

#3_3_6 GlobalTag for reReco
#process.GlobalTag.globaltag = 'GR09_R_V5::All'
#3_4_1 GlobalTag fore reReco
#process.GlobalTag.globaltag = 'MC_37Y_V5::All'
#process.GlobalTag.globaltag = "CRAFT09_R_V1::All"
#process.GlobalTag.globaltag = "CRUZETALL_V8::All"
#process.GlobalTag.globaltag = "GR09_31X_V6P::All"
#process.GlobalTag.globaltag = "GR10_P_V4::All"
#process.GlobalTag.globaltag = "GR_R_37X_V5A::All"
#process.GlobalTag.globaltag = "GR_R_38X_V9::All"
#HI global tags
#"GR10_P_V12::All"
#process.GlobalTag.globaltag="GR_R_43_V2::ALL"
process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outfile)
)

process.upctana = cms.EDAnalyzer('UPCTriggerAnalyzer')
process.zdcana = cms.EDAnalyzer('ZDCAnalyzer')
process.anaSelTrack = cms.EDAnalyzer('TrackAnalyzer',
		  trackPtMin = cms.untracked.double(0.4),
		  simTrackPtMin = cms.untracked.double(0.4),
		  vertexSrc = cms.vstring('hiSelectedVertex'),
		  trackSrc = cms.InputTag('hiSelectedTracks'),
		  pfCandSrc = cms.InputTag('particleFlow'),
		  doPFMatching = cms.untracked.bool(False),
		  doSimTrack = cms.untracked.bool(False)
		  )

process.anaGpTrack = cms.EDAnalyzer('TrackAnalyzer',
		  trackPtMin = cms.untracked.double(0.4),
		  simTrackPtMin = cms.untracked.double(0.4),
		  vertexSrc = cms.vstring('hiSelectedVertex'),
		  trackSrc = cms.InputTag('hiGlobalPrimTracks'),
		  pfCandSrc = cms.InputTag('particleFlow'),
		  doPFMatching = cms.untracked.bool(False),
		  doSimTrack = cms.untracked.bool(False)
		  )

#process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
#	triggerConditions = cms.vstring("L1_ZdcCaloPlus_BptxAND OR L1_ZdcCaloMinus_BptxAND AND ( NOT L1_BscMinBiasThreshold1_BptxAND )"),
#	hltResults = cms.InputTag(""),
#	l1tResults = cms.InputTag("gtDigis","","RECO"),
#	daqPartitions = cms.uint32( 0x01 ),
#	l1tIgnoreMask = cms.bool( False ),
#	l1techIgnorePrescales = cms.bool( False ),
#	throw = cms.bool( True )
#)

process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("HFhits"),#"PixelHits"),
	centralitySrc = cms.InputTag("hiCentrality")
)

#	process.load("HLTrigger.HLTfilters.hltHighLevel_cfi"
#	process.hltMinBiasHFOrBSC = process.hltHighLevel.clone()
#	process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHfOrBSC_Core"] # don't forget '_Core' if working on HICorePhysics

#	process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHf_Core","HLT_HIMinBiasBSC_Core"]
#	process.hltMinBiasHFOrBSC.andOr = cms.bool(True) # this is the default meaning either of the paths above
#	process.hltMinBiasHFOrBSC.throw = cms.bool(False) # don't throw exception since some runs have only one trigger or the other	

#	process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

#	process.path = cms.Path(process.hltMinBiasHFOrBSC*process.collisionEventSelection*process.caloana)
process.path = cms.Path(process.anaSelTrack+process.anaGpTrack+process.upctana+process.zdcana)
