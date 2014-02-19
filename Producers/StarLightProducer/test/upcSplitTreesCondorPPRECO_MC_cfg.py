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
process.load("HiSkim.HiOnia2MuMu.upcOnia2MuMuDiTrk_cfi")
process.load("HiSkim.HiOnia2MuMu.upcPatMuonsWithTrigger_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1)); 

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile.readlines())
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outfile)
)

process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("PixelHits"),#HFhits"),#"PixelHits"),
	nonDefaultGlauberModel = cms.string("Hydjet_2760GeV"),
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

process.GlobalTag.globaltag = 'STARTHI44_V12::All'

process.l1bitana = cms.EDAnalyzer('L1BitAnalyzer',
	l1GtRR=cms.InputTag("gtDigis"),
	hltresults=cms.InputTag("TriggerResults::HLT"),
	treeName=cms.string("L1BitTree")
)

process.eclustbana = cms.EDAnalyzer('UPCEcalClusterAnalyzer',
	ecalClusterCollection=cms.string("islandBarrelSuperClusters")
)

process.eclusteana = cms.EDAnalyzer('UPCEcalClusterAnalyzer',
	ecalClusterCollection=cms.string("islandEndcapSuperClusters")
)

process.upcmuana = cms.EDAnalyzer('UPCPatMuonAnalyzer',
	muonLabel=cms.InputTag("upcPatMuonsWithTrigger")
)	

process.upctrackpix=cms.EDAnalyzer('UPCTrackAnalyzer',
	trackCollection=cms.string("pixelTracks")
)

process.upctrackselana = cms.EDAnalyzer('UPCTrackAnalyzer',
	trackCollection=cms.string("generalTracks")
)

process.upcvertexana = cms.EDAnalyzer('UPCVertexAnalyzer',
	vertexCollection=cms.string("offlinePrimaryVertices")
)

process.upccentralityana = cms.EDAnalyzer('UPCCentralityAnalyzer',
	centralityVariable=process.HeavyIonGlobalParameters.centralityVariable
)

process.zdcana = cms.EDAnalyzer('ZDCAnalyzer')

process.calana = cms.EDAnalyzer('UPCCalEnergyAnalyzer')	

process.castorana = cms.EDAnalyzer('UPCCastorAnalyzer')	

process.candtraana = cms.EDAnalyzer("UPCPatCandidateAnalyzer",
	patDiMuon=cms.InputTag("onia2MuMuPatDiTrks"),
	hltTrigger=cms.string("HLT_HIUPCNeuMuPixel_SingleTrack_v1")
)

process.candglbana = cms.EDAnalyzer("UPCPatCandidateAnalyzer",
	patDiMuon=cms.InputTag("onia2MuMuPatGlbGlb"),
	hltTrigger=cms.string("HLT_HIUPCNeuMuPixel_SingleTrack_v1")
)

process.candglbtraana = cms.EDAnalyzer("UPCPatCandidateAnalyzer",
	patDiMuon=cms.InputTag("onia2MuMuPatGlbTra"),
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

process.allgenpartana = cms.EDAnalyzer('UPCGenParticleAnalyzer',
	genParticleCollection = cms.string("hiGenParticles"),
	treeName = cms.string("allHiGenParticlesTree")
)

process.matchedgenpartana = cms.EDAnalyzer('UPCGenParticleAnalyzer',
	genParticleCollection = cms.string("hiGenParticles"),
	treeName = cms.string("matchedHiGenParticlesTree")
)

process.maxcalana = cms.EDAnalyzer('UPCMaxCalAnalyzer')

process.onia2MuMuPatGlbTra = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string("(isGlobalMuon)"), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string("(isTrackerMuon)"),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(False),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)

process.trackCount1 = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag('generalTracks'),
    minNumber = cms.uint32(1) 
)

process.siTrackSeq = cms.Sequence(process.upctrackpix)
process.trackSeq = cms.Sequence(process.upctrackselana)
process.runSeq = cms.Sequence(process.siPixelRecHits+process.upcPixelClusterShapeAnalyzer+process.upcvertexana)
process.zdcSeq = cms.Sequence(process.zdcana)
process.ecalclustSeq = cms.Sequence(process.eclustbana+process.eclusteana)
process.muSeq = cms.Sequence(process.upcPatMuonsWithTriggerSequence+process.upcmuana)
process.candSeq = cms.Sequence(process.onia2MuMuPatDiTrks+process.candtraana)
process.calSeq= cms.Sequence(process.maxcalana+process.castorana)
process.triggerSeq = cms.Sequence(process.l1bitana)
process.rechitSeq = cms.Sequence(process.rechitana)
#process.evSelSeq = cms.Sequence(process.noBSChalo*process.primaryVertexFilter*process.siPixelRecHits*process.hltPixelClusterShapeFilter+process.upcCaloFilter)
process.evSelSeq = cms.Sequence(process.trackCount1)

process.path = cms.Path(#process.triggerSelection+
				process.allgenpartana+
				process.evSelSeq+
				process.matchedgenpartana+
				process.triggerSeq+
				process.runSeq+
				process.muSeq+
				process.candSeq+
				process.trackSeq+
				process.zdcSeq+
				process.siTrackSeq+
				process.calSeq+
#				process.hfSeq+
				process.rechitSeq
#				process.ecalclustSeq
)
