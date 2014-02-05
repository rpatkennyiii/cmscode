import FWCore.ParameterSet.Config as cms
import sys
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

if len(sys.argv) > 2:
	infile=open(sys.argv[2])

	outfile='UPCTree.root'

	process = cms.Process("UPCTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.load('Configuration.StandardSequences.MagneticField_38T_cff')
	process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
	process.load("Configuration.StandardSequences.GeometryDB_cff")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#	process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")
	process.load("HLTrigger.HLTanalyzers.HI_HLTAnalyser_cff")
	process.load("Analyzers.UPCTriggerAnalyzer.upcPixelClusterShapeAnalyzer_cfi")

	process.MessageLogger.cerr.FwkReport.reportEvery = 10
	nEventz=int(sys.argv[len(sys.argv)-1])
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEventz))
	process.hltanalysis.RunParameters.DoHeavyIon = cms.untracked.bool(False)

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile.readlines())
	)

	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("HFtowers"),#HFhits"),#"PixelHits"),
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

	process.GlobalTag.globaltag = 'STARTHI44_V12::All'
	
	process.l1bitana = cms.EDAnalyzer('L1BitAnalyzer',
		l1GtRR=cms.InputTag("gtDigis"),
		hltresults=cms.InputTag("TriggerResults::HLT")
	)
	
	process.hfana = cms.EDAnalyzer('UPCHFEnergyAnalyzer')

	process.genpartana = cms.EDAnalyzer('UPCGenParticleAnalyzer',
		genParticleCollection = cms.string("hiGenParticles")
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
	
	process.genpartSequence = cms.Sequence(process.genpartana)
	process.trackSequence = cms.Sequence(process.upctrackselana+process.upctrackgpana)
        process.zdcSequence = cms.Sequence(process.zdcana)
	process.ecalSequence = cms.Sequence(process.ecalesana+process.ecaleeana+process.ecalebana)
	process.ecalclustSequence = cms.Sequence(process.eclustbana+process.eclusteana)
	process.muSequence = cms.Sequence(process.upcmuana)
	process.hfSequence= cms.Sequence(process.hfana)
	process.triggerSequence = cms.Sequence(process.l1bitana)
	process.runSequence = cms.Sequence(process.upcvertexana+process.siPixelRecHits+process.upcPixelClusterShapeAnalyzer)
#		+process.upccentralityana+process.siPixelRecHits+process.upcPixelClusterShapeAnalyzer

	process.path = cms.Path(process.triggerSequence
					+process.genpartSequence
					+process.runSequence
					+process.muSequence
					+process.trackSequence
					+process.zdcSequence
					+process.hfSequence
#					+process.ecalclustSequence
        )
else:
	print 'error: no input file'	
