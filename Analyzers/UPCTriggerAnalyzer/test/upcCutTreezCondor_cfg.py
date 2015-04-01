import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
import sys

infile=open(sys.argv[2])
outfile=(sys.argv[3])
process = cms.Process("UPCTreeMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")
process.load("Analyzers.UPCTriggerAnalyzer.upcPixelClusterShapeAnalyzer_cfi")
process.load("Analyzers.UPCTriggerAnalyzer.upcPixelTrack_cff")
process.load("Analyzers.UPCTriggerAnalyzer.upcExCaloCuts_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1)); 

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
	 triggerConditions = cms.vstring("HLT_HIUPCNeuMuPixel_SingleTrack_v1"),
	 hltResults = cms.InputTag("TriggerResults::HLT"),
	 l1tResults = cms.InputTag("gtDigis"),
	 daqPartitions = cms.uint32( 0x01 ),
	 l1tIgnoreMask = cms.bool( False ),
	 l1techIgnorePrescales = cms.bool( False ),
	 throw = cms.bool( True )
)

process.primaryVertexFilter.cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2")

process.GlobalTag.globaltag = 'GR_R_44_V13::All'

process.l1bitana = cms.EDAnalyzer('L1BitAnalyzer',
	l1GtRR=cms.InputTag("gtDigis"),
	hltresults=cms.InputTag("TriggerResults::HLT")	
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

process.upctrackpix=cms.EDAnalyzer('UPCTrackAnalyzer',
	trackCollection=cms.string("hiPixelTracks")
)

process.upctrackselana = cms.EDAnalyzer('UPCTrackAnalyzer',
	trackCollection=cms.string("hiSelectedTracks")
)

process.upcvertexana = cms.EDAnalyzer('UPCVertexAnalyzer',
	vertexCollection=cms.string("hiSelectedVertex")
)

process.upccentralityana = cms.EDAnalyzer('UPCCentralityAnalyzer',
	centralityVariable=process.HeavyIonGlobalParameters.centralityVariable
)

process.zdcana = cms.EDAnalyzer('ZDCAnalyzer')

process.calana = cms.EDAnalyzer('UPCCalEnergyAnalyzer')	

process.castorana = cms.EDAnalyzer('UPCCastorAnalyzer')	

process.candtraana = cms.EDAnalyzer("UPCPatCandidateAnalyzer",
	patDiMuon=cms.InputTag("onia2MuMuPatTraTra"),
	hltTrigger=cms.string("HLT_HIUPCNeuMuPixel_SingleTrack_v1")
)

process.candCount = cms.EDFilter("CandCountFilter",
	src = cms.InputTag("onia2MuMuPatTraTra"),
	minNumber = cms.uint32(1)
)

process.candCountVeto = cms.EDFilter("CandCountFilter",
	src = cms.InputTag("onia2MuMuPatTraTra"),
	minNumber = cms.uint32(2)
)

process.trackCount = cms.EDFilter("CandViewCountFilter",
	src = cms.InputTag("hiSelectedTracks"),
	minNumber = cms.uint32(2)
)

process.trackCountVeto = cms.EDFilter("CandViewCountFilter",
	src = cms.InputTag("hiSelectedTracks"),
	minNumber = cms.uint32(3)
)

process.rechitana = cms.EDAnalyzer('UPCRecHitAnalyzer',
	TotalChargeThreshold=cms.untracked.double(0.0),
	FillEB=cms.untracked.bool(True),	
	FillEE=cms.untracked.bool(True),
	FillHBHESumOnly=cms.untracked.bool(True),	
	FillHFSumOnly=cms.untracked.bool(True),
	FillEBSumOnly=cms.untracked.bool(True),	
	FillEESumOnly=cms.untracked.bool(True)
)

process.hfana = cms.EDAnalyzer('UPCHFEnergyAnalyzer')

process.upcCaloFilter = cms.EDFilter('UPCMaxCalFilter',
	hfThreshold=cms.untracked.double(3.62),
	heThreshold=cms.untracked.double(2.48),
	hbThreshold=cms.untracked.double(1.44),
	eeThreshold=cms.untracked.double(2.97),
	ebThreshold=cms.untracked.double(1.28)
)

process.trackCount2 = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag('hiSelectedTracks'),
    minNumber = cms.uint32(2) 
)

process.trackCount3 = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag('hiSelectedTracks'),
    minNumber = cms.uint32(3) 
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

process.maxcalana = cms.EDAnalyzer('UPCMaxCalAnalyzer')

process.siTrackSeq = cms.Sequence(process.siPixSeq+process.upctrackpix)
process.trackSeq = cms.Sequence(process.upctrackselana)
process.runSeq = cms.Sequence(process.siPixelRecHits+process.upcPixelClusterShapeAnalyzer+process.upcvertexana+process.upccentralityana)
process.zdcSeq = cms.Sequence(process.zdcana)
process.ecalclustSeq = cms.Sequence(process.eclustbana+process.eclusteana)
process.muSeq = cms.Sequence(process.upcmuana)
process.calSeq= cms.Sequence(process.calana+process.castorana)
process.triggerSeq = cms.Sequence(process.l1bitana+process.hltbitanalysis)
process.candSeq = cms.Sequence(process.candtraana)
process.rechitSeq = cms.Sequence(process.rechitana)
process.evSelSeq = cms.Sequence(process.noBSChalo*process.primaryVertexFilter*process.siPixelRecHits*process.hltPixelClusterShapeFilter)

process.path = cms.Path(#process.triggerSelection+
				process.evSelSeq+
				process.trackCount2+
				~process.trackCount3+
				process.triggerSeq+
				process.runSeq+
				process.muSeq+
				process.trackSeq+
				process.zdcSeq+
				process.candSeq+
				process.siTrackSeq+
				process.calSeq+
#				process.hfSeq+
				process.rechitSeq
#				process.ecalclustSeq
)
