import sys
import FWCore.ParameterSet.Config as cms
import PhysicsTools.PythonAnalysis.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
from CmsHi.Analysis2010.CommonFunctions_cff import *

outfile='CaloEnergy.root'

process = cms.Process("CaloEnergyTreeMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
process.load("HLTrigger.special.hltPixelClusterShapeFilter_cfi")
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi") 
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")

overrideCentrality(process)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/hidata/HIRun2010/HICorePhysics/RECO/PromptReco-v3/000/153/368/887B497F-DC01-E011-86DB-001D09F28D54.root'
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
#process.GlobalTag.globaltag = "GR_R_38X_V9::All"
#HI global tags
process.GlobalTag.globaltag="GR10_P_V12::All"

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outfile)
)

process.caloana = cms.EDAnalyzer('CaloEnergyAnalyzer',
	etaBinSize = cms.bool(True),
	HFCorrection = cms.bool(False),
	realHFbins = cms.bool(True),
	noiseCut = cms.bool(True)
)

process.HeavyIonGlobalParameters=cms.PSet(
	centralityVariable= cms.string("PixelHits"),
	centralitySrc = cms.InputTag("hiCentrality")
)

process.hltMinBiasHFOrBSC = process.hltHighLevel.clone()
process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHfOrBSC_Core"] # don't forget '_Core' if working on HICorePhysics
#process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHf_Core","HLT_HIMinBiasBSC_Core"]
#process.hltMinBiasHFOrBSC.andOr = cms.bool(True) # this is the default meaning either of the paths above
#process.hltMinBiasHFOrBSC.throw = cms.bool(False) # don't throw exception since some runs have only one trigger or the other	

process.HFCoincFilter = process.hfCoincFilter3	

process.primaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("hiSelectedVertex"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"), 
    filter = cms.bool(True),   # otherwise it won't filter the events, instead making an empty vertex collection
)

process.hltPixelClusterShapeFilter.inputTag = "siPixelRecHits"
process.SiPixFilter = process.hltPixelClusterShapeFilter

process.load("L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff")
process.load("HLTrigger.HLTfilters.hltLevel1GTSeed_cfi")
process.noBSChalo = process.hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)')
)

#	process.p = cms.Path(process.hltMinBiasHFOrBSC*
#		process.primaryVertexFilter*
#		#process.siPixelRecHits*
#		process.SiPixFilter*
#		process.HFCoincFilter*
#		process.noBSChalo*
#		process.caloana)

process.p = cms.Path(process.hltMinBiasHFOrBSC*process.noBSChalo*process.HFCoincFilter*process.siPixelRecHits*process.SiPixFilter*process.caloana)
