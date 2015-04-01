import FWCore.ParameterSet.Config as cms
import sys
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

if len(sys.argv) > 2:
	infile=open(sys.argv[2])

	outfile='UPCTree.root'

	process = cms.Process("UPCTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.MessageLogger.cerr.FwkReport.reportEvery = 10
	process.load('Configuration.StandardSequences.MagneticField_38T_cff')
	process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
	process.load("Configuration.StandardSequences.GeometryDB_cff")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

	nEventz=int(sys.argv[len(sys.argv)-1])
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEventz))

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile.readlines())
	)

	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("PixelHits"),#HFhits"),#"PixelHits"),
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

	process.GlobalTag.globaltag = 'GR_R_44_V12::All'

	process.l1bitana = cms.EDAnalyzer('L1BitAnalyzer',
		l1GtRR=cms.InputTag("gtDigis")
	)
	
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
	process.triggerPath = cms.Path(process.triggerSelection+process.l1bitana)

	process.schedule = cms.Schedule(process.triggerPath,process.muPath,process.trackPath,process.runPath,process.zdcPath,process.ecalclustPath)
else:
	print 'error: no input file'	
