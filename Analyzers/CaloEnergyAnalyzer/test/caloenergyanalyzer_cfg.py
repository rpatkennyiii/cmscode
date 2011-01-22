import FWCore.ParameterSet.Config as cms
import sys
from CmsHi.Analysis2010.CommonFunctions_cff import *

if len(sys.argv) > 2:
	infile=[sys.argv[2]]
	for x in sys.argv[3:(len(sys.argv)-2)]:
		infile.append(x)

	outfile='CaloEnergy.root'

	process = cms.Process("CaloEnergyTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

	overrideCentrality(process)

	nEventz=int(sys.argv[len(sys.argv)-1])
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEventz))

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile)
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
		etaBinSize = cms.bool(False),
		HFCorrection = cms.bool(True)
	)

	process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("PixelHits"),
    		centralitySrc = cms.InputTag("hiCentrality")
	)

	process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
	process.hltMinBiasHFOrBSC = process.hltHighLevel.clone()
	process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHf_Core","HLT_HIMinBiasBSC_Core"]
	process.hltMinBiasHFOrBSC.andOr = cms.bool(True) # this is the default meaning either of the paths above
	process.hltMinBiasHFOrBSC.throw = cms.bool(False) # don't throw exception since some runs have only one trigger or the other	

	process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
	process.HFCoincFilter = process.hfCoincFilter3	

	process.primaryVertexFilter = cms.EDFilter("VertexSelector",
	    src = cms.InputTag("hiSelectedVertex"),
	    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"), 
	    filter = cms.bool(True),   # otherwise it won't filter the events, instead making an empty vertex collection
	)
	
	process.load("HLTrigger.special.hltPixelClusterShapeFilter_cfi")
	process.hltPixelClusterShapeFilter.inputTag = "siPixelRecHits"
	process.SiPixFilter = process.hltPixelClusterShapeFilter

	process.load("L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff")
	process.load("HLTrigger.HLTfilters.hltLevel1GTSeed_cfi")
	process.noBSChalo = process.hltLevel1GTSeed.clone(
	    L1TechTriggerSeeding = cms.bool(True),
	    L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)')
	)
	
	process.p = cms.Path(process.hltMinBiasHFOrBSC*
		process.primaryVertexFilter*
		#process.siPixelRecHits*
		process.SiPixFilter*
		process.HFCoincFilter*
		process.noBSChalo*
		process.caloana)

#	process.p = cms.Path(process.HFCoincFilter*process.noBSChalo*process.caloana)
else:
	print 'error: no input file'	
