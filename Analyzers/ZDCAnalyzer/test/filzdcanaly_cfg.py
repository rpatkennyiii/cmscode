import FWCore.ParameterSet.Config as cms
import sys

if len(sys.argv) > 2:
	infile=sys.argv[2]
	outfile='histos.root'

	process = cms.Process("PatDemo")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
	process.load("HLTrigger.HLTfilters.hltLevel1GTSeed_cfi")
	process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
	process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')

	raw=0

	if infile.find('RAW')+1:
		process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
		raw=1

	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
	process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile
	    #'/store/data/BeamCommissioning09/BeamHalo/RAW/v1/000/120/013/B60078A5-D4CB-DE11-8E7F-001617E30D12.root'
	    #'/store/data/BeamCommissioning09/BeamHalo/RAW/v1/000/120/015/22209C11-F0CB-DE11-AC55-001D09F253FC.root'
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
	process.GlobalTag.globaltag = "GR_R_38X_V9::All"

	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
	process.hltLevel1GTSeed.L1UseL1TriggerObjectMaps = cms.bool(False)
	process.hltLevel1GTSeed.L1NrBxInEvent = cms.int32(1)
	process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

	process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

	process.ns = cms.EDFilter("FilterOutScraping",
                              applyfilter = cms.untracked.bool(True),
                              debugOn = cms.untracked.bool(True),
                              numtrack = cms.untracked.uint32(10),
                              thresh = cms.untracked.double(0.25)
                              )

	process.pVF = cms.EDFilter("GoodVertexFilter",
			      vertexCollection = cms.InputTag('offlinePrimaryVertices'),
			      minimumNDOF = cms.uint32(4) ,
			      maxAbsZ = cms.double(15),	
			      maxd0 = cms.double(2)	
			      )

	process.patdemo = cms.EDAnalyzer('ZDCAnalyzer')

	if raw:
		process.p = cms.Path(process.hcalDigis*process.patdemo)
	else:
		process.p = cms.Path(process.hltLevel1GTSeed*process.hltPhysicsDeclared*process.pVF*process.ns*process.patdemo)
else:
	print 'error: no input file'	
