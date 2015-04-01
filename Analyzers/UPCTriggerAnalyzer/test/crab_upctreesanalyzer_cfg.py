import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList
from CmsHi.Analysis2010.CommonFunctions_cff import *


outfile='UPCTree.root'

process = cms.Process("UPCTreeMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

#	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
overrideCentrality(process)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring())

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

process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("HFhits"),#"PixelHits"),
	centralitySrc = cms.InputTag("hiCentrality")
)

process.upctrackselana = cms.EDAnalyzer('UPCTrackAnalyzer',
	trackCollection=cms.string("hiSelectedTracks")
)

process.upctrackgpana = cms.EDAnalyzer('UPCTrackAnalyzer',
	trackCollection=cms.string("hiGlobalPrimTracks")
)

process.upcvertexana = cms.EDAnalyzer('UPCVertexAnalyzer',
	vertexCollection=cms.string("hiSelectedVertex")
)
process.upcrunana = cms.EDAnalyzer('UPCRunAnalyzer')

process.upccentralityana = cms.EDAnalyzer('UPCCentralityAnalyzer',
	centralityVariable=process.HeavyIonGlobalParameters.centralityVariable
)

process.zdcana = cms.EDAnalyzer('ZDCAnalyzer')

process.path = cms.Path(process.upctrackselana+process.upctrackgpana+process.upcvertexana+process.upcrunana+process.upccentralityana+process.zdcana)
