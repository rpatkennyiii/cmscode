import FWCore.ParameterSet.Config as cms
import sys
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

if len(sys.argv) > 2:
	infile=open(sys.argv[2])
	outfile=(sys.argv[3])

	process = cms.Process("FSCTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

	process.MessageLogger.cerr.FwkReport.reportEvery = 100
	#	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

	nEventz=int(-1)
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEventz))

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile.readlines())
	)

	process.GlobalTag.globaltag = 'GR_R_44_V15::All'

	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.l1bitAna = cms.EDAnalyzer('L1BitAnalyzer',
		l1GtRR=cms.InputTag("gtDigis"),
		hltresults=cms.InputTag("TriggerResults::HLT")	
	)

	process.hltbitanalysis = cms.EDAnalyzer("HLTBitAnalyzer",
             ### Trigger objects
             l1GctHFBitCounts                = cms.InputTag("gctDigis"),
             l1GctHFRingSums                 = cms.InputTag("gctDigis"),
             l1GtObjectMapRecord             = cms.InputTag("hltL1GtObjectMap::HLT"),
             l1GtReadoutRecord               = cms.InputTag("gtDigis::RECO"),

             l1extramc                       = cms.string('l1extraParticles'),
             l1extramu                       = cms.string('l1extraParticles'),
             hltresults                      = cms.InputTag("TriggerResults::HLT"),
             HLTProcessName                  = cms.string("HLT"),
             UseTFileService                 = cms.untracked.bool(True),

             ### Run parameters
             RunParameters = cms.PSet(
             HistogramFile = cms.untracked.string('hltbitanalysis.root')
             )
         )

	process.monTriggerSelection = cms.EDFilter( "TriggerResultsFilter",
		 triggerConditions = cms.vstring("HLT_HIMinBiasZDC_Calo_PlusOrMinus_v1 OR HLT_HIMinBiasZDC_PlusOrMinusPixel_SingleTrack_v1"),
		 hltResults = cms.InputTag("TriggerResults::HLT"),
		 l1tResults = cms.InputTag("gtDigis"),
		 daqPartitions = cms.uint32( 0x01 ),
		 l1tIgnoreMask = cms.bool( False ),
		 l1techIgnorePrescales = cms.bool( False ),
		 throw = cms.bool( True )
	)

	process.path = cms.Path(process.monTriggerSelection+process.l1bitAna+process.hltbitanalysis)
else:
	print 'error: no input file'	
