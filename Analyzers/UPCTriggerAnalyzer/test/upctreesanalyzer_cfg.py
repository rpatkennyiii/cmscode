import FWCore.ParameterSet.Config as cms
import sys
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList
#from CmsHi.Analysis2011.CommonFunctions_cff import *

if len(sys.argv) > 2:
	infile=[sys.argv[2]]
	for x in sys.argv[3:(len(sys.argv)-2)]:
		infile.append(x)

	outfile='UPCTree.root'

	process = cms.Process("UPCTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.MessageLogger.cerr.FwkReport.reportEvery = 10
	process.load('Configuration.StandardSequences.MagneticField_38T_cff')
	process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
	process.load("Configuration.StandardSequences.GeometryDB_cff")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

	#	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
#	overrideCentrality(process)

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
	#"GR10_P_V12::All"
	#process.GlobalTag.globaltag="GR_R_43_V2::ALL"
	#process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'

	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("HFhits"),#"PixelHits"),
		centralitySrc = cms.InputTag("hiCentrality")
	)
	
	process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
		 triggerConditions = cms.vstring("(HLT_HIUPCNeuEG2Pixel_SingleTrack_v1 OR HLT_HIUPCNeuEG5Pixel_SingleTrack_v1 OR HLT_HIUPCNeuMuPixel_SingleTrack_v1)"),
		 hltResults = cms.InputTag("TriggerResults::HLT"),
		 l1tResults = cms.InputTag("gtDigis"),
		 daqPartitions = cms.uint32( 0x01 ),
		 l1tIgnoreMask = cms.bool( False ),
		 l1techIgnorePrescales = cms.bool( False ),
		 throw = cms.bool( True )
	)

	process.GlobalTag.globaltag = 'GR_R_44_V11::All'
	
	process.eclustbana = cms.EDAnalyzer('UPCEcalClusterAnalyzer',
        	ecalClusterCollection=cms.string("islandBarrelSuperClusters")
	)

	process.eclusteana = cms.EDAnalyzer('UPCEcalClusterAnalyzer',
		ecalClusterCollection=cms.string("islandEndcapSuperClusters")
	)

	process.upcmuana = cms.EDAnalyzer('UPCMuonAnalyzer',
		muonLabel=cms.InputTag("muons")
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

	process.ecalesana = cms.EDAnalyzer('UPCEcalAnalyzer',
		ecalCollection=cms.string("EcalRecHitsES")
	)
	
	process.ecaleeana = cms.EDAnalyzer('UPCEcalAnalyzer',
		ecalCollection=cms.string("EcalRecHitsEE")
	)

	process.ecalebana = cms.EDAnalyzer('UPCEcalAnalyzer',
		ecalCollection=cms.string("EcalRecHitsEB")
	)

	process.trackPath = cms.Path(process.triggerSelection+process.upctrackselana+process.upctrackgpana)
	process.runPath = cms.Path(process.triggerSelection+process.upcvertexana+process.upcrunana+process.upccentralityana)
        process.zdcPath = cms.Path(process.triggerSelection+process.zdcana)
	process.ecalPath = cms.Path(process.triggerSelection+process.ecalesana+process.ecaleeana+process.ecalebana)
	process.ecalclustPath = cms.Path(process.triggerSelection+process.eclustbana+process.eclusteana)
	process.muPath = cms.Path(process.triggerSelection+process.upcmuana)

	process.schedule = cms.Schedule(process.muPath,process.trackPath,process.runPath,process.zdcPath,process.ecalclustPath)
else:
	print 'error: no input file'	
