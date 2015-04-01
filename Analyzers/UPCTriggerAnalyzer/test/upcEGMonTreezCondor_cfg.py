import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import addMCinfo, changeRecoMuonInput, useL1MatchingWindowForSinglets, changeTriggerProcessName, switchOffAmbiguityResolution
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
process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")

changeTriggerProcessName(process, "HLT")
switchOffAmbiguityResolution(process)
process.patMuonsWithoutTrigger.pvSrc = "hiSelectedVertex"

process.muonL1Info.maxDeltaR = 0.3
process.muonL1Info.fallbackToME1 = True
process.muonMatchHLTL1.maxDeltaR = 0.3
process.muonMatchHLTL1.fallbackToME1 = True
process.muonMatchHLTL2.maxDeltaR = 0.3
process.muonMatchHLTL2.maxDPtRel = 10.0
process.muonMatchHLTL3.maxDeltaR = 0.1
process.muonMatchHLTL3.maxDPtRel = 10.0
process.muonMatchHLTCtfTrack.maxDeltaR = 0.1
process.muonMatchHLTCtfTrack.maxDPtRel = 10.0
process.muonMatchHLTTrackMu.maxDeltaR = 0.1
process.muonMatchHLTTrackMu.maxDPtRel = 10.0 

MC = False

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
	 triggerConditions = cms.vstring("HLT_HIUPCNeuEG2Pixel_SingleTrack_v1 OR HLT_HIUPCNeuEG5Pixel_SingleTrack_v1"),
	 hltResults = cms.InputTag("TriggerResults::HLT"),
	 l1tResults = cms.InputTag("gtDigis"),
	 daqPartitions = cms.uint32( 0x01 ),
	 l1tIgnoreMask = cms.bool( False ),
	 l1techIgnorePrescales = cms.bool( False ),
	 throw = cms.bool( True )
)

process.onia2MuMuPatTraTra = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("hiSelectedVertex"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string("(isTrackerMuon)"), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string("(isTrackerMuon)"),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)

process.upcpatmuana = cms.EDAnalyzer('UPCPatMuonAnalyzer',
	muonLabel=cms.InputTag("patMuonsWithTrigger")
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

process.maxcalana = cms.EDAnalyzer('UPCMaxCalAnalyzer')



process.siTrackSeq = cms.Sequence(process.siPixSeq+process.upctrackpix)
process.trackSeq = cms.Sequence(process.upctrackselana)
process.runSeq = cms.Sequence(process.siPixelRecHits+process.upcPixelClusterShapeAnalyzer+process.upcvertexana+process.upccentralityana)
process.zdcSeq = cms.Sequence(process.zdcana)
process.ecalclustSeq = cms.Sequence(process.eclustbana+process.eclusteana)
process.muSeq = cms.Sequence(process.upcpatmuana)
process.calSeq= cms.Sequence(process.maxcalana+process.castorana)
process.triggerSeq = cms.Sequence(process.l1bitana+process.hltbitanalysis)
process.patMuCandSeq = cms.Sequence(process.patMuonsWithTriggerSequence+process.onia2MuMuPatTraTra)
process.candSeq = cms.Sequence(process.candtraana)
process.rechitSeq = cms.Sequence(process.rechitana)
#process.evSelSeq = cms.Sequence(process.noBSChalo*process.primaryVertexFilter*process.siPixelRecHits*process.hltPixelClusterShapeFilter+process.upcCaloFilter)
process.evSelSeq = cms.Sequence(process.noBSChalo)

process.path = cms.Path(process.triggerSelection+
#				process.evSelSeq+
				process.triggerSeq+
				process.runSeq+
				process.trackSeq+
				process.zdcSeq+
	                        process.patMuCandSeq+
				process.candSeq+
				process.muSeq+
				process.siTrackSeq+
				process.calSeq+
#				process.hfSeq+
				process.rechitSeq
#				process.ecalclustSeq
)
