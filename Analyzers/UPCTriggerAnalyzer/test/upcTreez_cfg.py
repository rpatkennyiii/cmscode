import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import addMCinfo, changeRecoMuonInput, useL1MatchingWindowForSinglets, changeTriggerProcessName, switchOffAmbiguityResolution
import sys

infile=open(sys.argv[2])
outfile=sys.argv[3]

process = cms.Process("UPCTreeMaker")

process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Analyzers.UPCTriggerAnalyzer.upcPixelClusterShapeAnalyzer_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.GlobalTag.globaltag = 'GR_R_53_V19::All'

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

process.onia2MuMuPatTraTra = cms.EDProducer('HiOnia2MuMuPAT',
    muons = cms.InputTag("patMuonsWithTrigger"),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
    # At least one muon must pass this selection
    higherPuritySelection = cms.string("(isTrackerMuon)"), 
    # BOTH muons must pass this selection
    lowerPuritySelection  = cms.string("(isTrackerMuon)"),
    dimuonSelection  = cms.string("2 < mass"), ## The dimuon must pass this selection before vertexing
    addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
    addMuonlessPrimaryVertex = cms.bool(False), ## Embed the primary vertex re-made from all the tracks except the two muons
    addMCTruth = cms.bool(False),      ## Add the common MC mother of the two muons, if any
    resolvePileUpAmbiguity = cms.bool(False)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(infile.readlines())
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string(outfile)
)



process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("PixelHits"),#HFhits"),#"PixelHits"),
       centralitySrc = cms.InputTag("hiCentrality")
)

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

process.upcgeneraltrackana = cms.EDAnalyzer('UPCTrackAnalyzer',
       trackCollection=cms.string("generalTracks")
)

process.upcpixeltrackana = cms.EDAnalyzer('UPCTrackAnalyzer',
       trackCollection=cms.string("pixelTracks")
)

process.upcvertexana = cms.EDAnalyzer('UPCVertexAnalyzer',
       vertexCollection=cms.string("offlinePrimaryVerticesWithBS")
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

process.candtraana = cms.EDAnalyzer("UPCPatCandidateAnalyzer",
       patDiMuon=cms.InputTag("onia2MuMuPatTraTra"),
       hltTrigger=cms.string("HLT_PAUpcSingleMuOpenFull_TrackVeto7_v1")
)

process.hcalDigis = cms.EDProducer("HcalRawToDigi",
    UnpackZDC = cms.untracked.bool(True),
    FilterDataQuality = cms.bool(True),
    HcalFirstFED = cms.untracked.int32(700),
    InputLabel = cms.InputTag("rawDataCollector"),
    UnpackCalib = cms.untracked.bool(True),
    FEDs = cms.untracked.vint32(722), 
    streams = cms.untracked.vstring(
          'HCAL_Trigger','HCAL_SlowData','HCAL_QADCTDC'
    ),
    lastSample = cms.int32(9),
    firstSample = cms.int32(0),
    ComplainEmptyData = cms.untracked.bool(True)
)

process.zdcfscana = cms.EDAnalyzer("ZDCFSCAnalyzer")

process.hltbitanalysis = cms.EDAnalyzer("HLTBitAnalyzer",
   ### Trigger objects
   l1GctHFBitCounts                = cms.InputTag("gctDigis"),
   l1GctHFRingSums                 = cms.InputTag("gctDigis"),
   l1GtObjectMapRecord             = cms.InputTag("hltL1GtObjectMap"),
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

process.hfana = cms.EDAnalyzer('UPCHFEnergyAnalyzer')	

process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
	triggerConditions = cms.vstring("(HLT_PAUpcSingleEG5Pixel_TrackVeto_v1 OR HLT_PAUpcSingleMuOpenPixel_TrackVeto_v1 OR HLT_PAUpcSingleEG5Full_TrackVeto7_v1 OR HLT_PAUpcSingleMuOpenFull_TrackVeto7_v1 OR HLT_PAUpcSingleMuOpenTkMu_Onia_v1 OR HLT_PAZeroBiasPixel_SingleTrack_v1)"),
	hltResults = cms.InputTag("TriggerResults::HLT"),
	l1tResults = cms.InputTag("gtDigis"),
	daqPartitions = cms.uint32( 0x01 ),
	l1tIgnoreMask = cms.bool( False ),
	l1techIgnorePrescales = cms.bool( False ),
	throw = cms.bool( True )
)


process.muonPat_step = cms.Sequence(process.patMuonsWithTriggerSequence+process.onia2MuMuPatTraTra)
process.trackSequence = cms.Sequence(process.upcgeneraltrackana+process.upcpixeltrackana )
process.runSequence = cms.Sequence(process.siPixelRecHits+process.upcvertexana+process.upcPixelClusterShapeAnalyzer)
process.zdcSequence = cms.Sequence(process.zdcana)
process.zdcFscSequence = cms.Sequence(process.hcalDigis+process.zdcfscana)
process.ecalSequence = cms.Sequence(process.ecalesana+process.ecaleeana+process.ecalebana)
process.ecalclustSequence = cms.Sequence(process.eclustbana+process.eclusteana)
process.muSequence = cms.Sequence(process.upcmuana+process.candtraana)
process.hfSequence= cms.Sequence(process.hfana)
process.triggerSequence = cms.Sequence(process.hltbitanalysis) 

process.path = cms.Path( process.triggerSelection+
			 process.runSequence+
			 process.muonPat_step+
			 process.muSequence+
			 process.triggerSequence+
			 process.trackSequence+
			 process.zdcSequence+
			 process.hfSequence
)
