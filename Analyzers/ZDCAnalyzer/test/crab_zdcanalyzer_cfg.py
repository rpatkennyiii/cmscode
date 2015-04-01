import FWCore.ParameterSet.Config as cms
import sys

outfile='histos.root'

process = cms.Process("PatDemo")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring())

#3_3_6 GlobalTag for reReco
#process.GlobalTag.globaltag = 'GR09_R_V5::All'
#3_4_1 GlobalTag fore reReco
#process.GlobalTag.globaltag = 'MC_37Y_V5::All'
#process.GlobalTag.globaltag = "CRAFT09_R_V1::All"
#process.GlobalTag.globaltag = "CRUZETALL_V8::All"
#process.GlobalTag.globaltag = "GR09_31X_V6P::All"
#process.GlobalTag.globaltag = "GR10_P_V4::All"
#process.GlobalTag.globaltag = "GR_R_37X_V5A::All"
#process.GlobalTag.globaltag = "GR10_P_V6::All"
process.GlobalTag.globaltag = 'GR_R_37X_V6D::All'

process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
process.prefer("GlobalTag")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outfile)
)

process.patdemo = cms.EDAnalyzer('ZDCAnalyzer',
#	    HLTTag = cms.InputTag("TriggerResults::HLT")
)

#process.p = cms.Path(process.hcalDigis * process.patdemo)
process.p = cms.Path(process.patdemo)
