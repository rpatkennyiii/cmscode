import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList
from CmsHi.Analysis2010.CommonFunctions_cff import *


outfile='UPCEG2Tree.root'

process = cms.Process("UPCTreeMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
#	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

overrideCentrality(process)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring())

process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outfile)
)

process.upctana = cms.EDAnalyzer('UPCTriggerAnalyzer')
process.zdcana = cms.EDAnalyzer('ZDCAnalyzer')
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    		triggerConditions = cms.vstring("L1_SingleEG2"),
		hltResults = cms.InputTag("TriggerResults","","HLT"),
		l1tResults = cms.InputTag("gtDigis","","RECO"),
		daqPartitions = cms.uint32( 0x01 ),
		l1tIgnoreMask = cms.bool( False ),
    		l1techIgnorePrescales = cms.bool( False ),
    		throw = cms.bool( True )
		)

process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("HFhits"),#"PixelHits"),
	centralitySrc = cms.InputTag("hiCentrality")
)

process.path = cms.Path(process.triggerSelection+process.upctana+process.zdcana)
