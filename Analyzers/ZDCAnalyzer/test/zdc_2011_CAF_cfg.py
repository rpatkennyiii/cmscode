import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("ZDC")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration/StandardSequences/Reconstruction_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 
       '/store/express/HIRun2011/HIExpressPhysics/FEVT/Express-v1/000/181/611/049C5E56-0F0E-E111-B98B-001D09F25401.root'
)
)

process.GlobalTag.globaltag = 'GR_R_44_V10::All'

# load centrality
process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFhits"),
	nonDefaultGlauberModel = cms.string(""),
	centralitySrc = cms.InputTag("hiCentrality")
)


process.TFileService = cms.Service("TFileService",
	    fileName = cms.string('zdctrees_181611_HIExpress_test_100ev.root')
)

process.zdcana = cms.EDAnalyzer('ZDCAnalyzer', 
   l1GtRR=cms.InputTag("gtDigis")
)
process.p = cms.Path(process.zdcreco*process.zdcana)
