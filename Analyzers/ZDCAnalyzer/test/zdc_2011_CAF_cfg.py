import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("ZDC")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration/StandardSequences/Reconstruction_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
# 
# readFiles = cms.untracked.vstring()
# readFiles.extend( (
# 'file:file:/data/users/belt/PbPb_data/HICorePhysics/HICorePromptv3_151353_EE0FFA35-30F2-DF11-9F88-0030486730C6.root'
# ) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 
'file:/data/users/belt/PbPb_data/HICorePhysics/HICorePromptv3_151353_EE0FFA35-30F2-DF11-9F88-0030486730C6.root'
)
)

	#3_3_6 GlobalTag for reReco
	#process.GlobalTag.globaltag = 'GR09_R_V5::All'
	#3_4_1 GlobalTag fore reReco
	#process.GlobalTag.globaltag = 'MC_37Y_V5::All'
	#process.GlobalTag.globaltag = "CRAFT09_R_V1::All"
	#process.GlobalTag.globaltag = "CRUZETALL_V8::All"
	#process.GlobalTag.globaltag = "GR09_31X_V6P::All"
	#process.GlobalTag.globaltag = "GR10_P_V4::All"
	#process.GlobalTag.globaltag = "GR_R_37X_V5A::All"
process.GlobalTag.globaltag = 'GR_R_44_V10::All'

# load centrality
process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFhits"),
	nonDefaultGlauberModel = cms.string(""),
	centralitySrc = cms.InputTag("hiCentrality")
)


process.TFileService = cms.Service("TFileService",
	    fileName = cms.string('zdctrees_HIExpress_181529To181352.root')
)

process.zdcana = cms.EDAnalyzer('ZDCAnalyzer',)

process.p = cms.Path(process.zdcreco*process.zdcana)
