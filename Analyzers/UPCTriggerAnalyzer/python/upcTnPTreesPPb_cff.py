import FWCore.ParameterSet.Config as cms

trackSoftQuality="track.hitPattern.trackerLayersWithMeasurement > 5 && track.quality(reco::TrackBase::highPurity) && track.hitPattern.pixelLayersWithMeasurement > 0 && abs(dB) < 0.3 && abs(track.dz) < 20"
trackerMuAnd="isTrackerMuon && muonID('TMOneStationTight') && "
trigAnd="!triggerObjectMatchesByPath('HLT_PAUpcSingleMuOpenFull_TrackVeto7_v1').empty() && "

tagMuons = cms.EDFilter("PATMuonRefSelector",
	src = cms.InputTag("upcPatMuonsWithTrigger"),
	cut = cms.string(trackerMuAnd+trigAnd+trackSoftQuality),   
)

probeTrigMuons = cms.EDFilter("PATMuonSelector",
	src = cms.InputTag("upcPatMuonsWithTrigger"),
	cut = cms.string(trigAnd+trackSoftQuality),
)

probeMuQMuons = cms.EDFilter("PATMuonSelector",
	src = cms.InputTag("upcPatMuonsWithTrigger"),
	cut = cms.string(trackSoftQuality)
)

tpTrigPairs = cms.EDProducer("CandViewShallowCloneCombiner",
	cut = cms.string('0 < mass < 100'),
	decay = cms.string('tagMuons@+ probeTrigMuons@-')
)

tpMuQPairs = cms.EDProducer("CandViewShallowCloneCombiner",
	cut = cms.string('0 < mass < 100'),
	decay = cms.string('tagMuons@+ probeMuQMuons@-')
)

tnpTrigTree = cms.EDAnalyzer("TagProbeFitTreeProducer",
	tagProbePairs = cms.InputTag("tpTrigPairs"),
	arbitration   = cms.string("None"), 
	variables = cms.PSet(
		pt     = cms.string("pt"),
		abseta = cms.string("abs(eta)"),
		charge = cms.string("charge"),
		l1dr = cms.string("userFloat('muonL1Info:deltaR')"),
		l1q  = cms.string("userInt('muonL1Info:quality')"),
	),
	flags = cms.PSet(
		isTrackerMuon = cms.string("isTrackerMuon"),
		isl1dr = cms.string("userFloat('muonL1Info:deltaR')<0.3"),
		isl1q  = cms.string("userInt('muonL1Info:quality')>=2."),
		isHLTupc     = cms.string("!triggerObjectMatchesByPath('HLT_HIUPCNeuMuPixel_SingleTrack_v1').empty()"),  
		ismuq  = cms.string("isTrackerMuon && muonID('TMOneStationTight') && track.hitPattern.trackerLayersWithMeasurement > 5 && track.normalizedChi2 < 3.0 && track.hitPattern.pixelLayersWithMeasurement > 0 && abs(dB) < 0.3 && abs(track.dz) < 20") 
	),
	tagVariables = cms.PSet(
		pt = cms.string('pt'),
		abseta = cms.string("abs(eta)"),
		charge = cms.string("charge"),
		l1dr = cms.string("userFloat('muonL1Info:deltaR')"), #not working for the moment 
		l1q  = cms.string("userInt('muonL1Info:quality')"), #not working for the moment 
	),
	tagFlags = cms.PSet(
		isTrackerMuon = cms.string("isTrackerMuon"),
		isHLTupc     = cms.string("!triggerObjectMatchesByPath('HLT_HIUPCNeuMuPixel_SingleTrack_v1').empty()"),  
		isl1dr = cms.string("userFloat('muonL1Info:deltaR')<0.3"),
		isl1q  = cms.string("userInt('muonL1Info:quality')>=2."),
		ismuq  = cms.string("isTrackerMuon && muonID('TMOneStationTight') && track.hitPattern.trackerLayersWithMeasurement > 5 && track.normalizedChi2 < 3.0 && track.hitPattern.pixelLayersWithMeasurement > 0 && abs(dB) < 0.3 && abs(track.dz) < 20") 
	),
	pairVariables = cms.PSet(
		pt = cms.string("pt"),
		abseta = cms.string("abs(eta)"),
		absy = cms.string("abs(rapidity)"),
		charge = cms.string("charge")
	),
	pairFlags = cms.PSet(),
	addRunLumiInfo = cms.bool(True),
	isMC = cms.bool(False),
)

tnpMuQTree = tnpTrigTree.clone( 
	tagProbePairs = cms.InputTag("tpMuQPairs") 
)

tandpTrigSeq = cms.Sequence(tagMuons+probeTrigMuons+tpTrigPairs+tnpTrigTree) 
tandpMuQSeq = cms.Sequence(tagMuons+probeMuQMuons+tpMuQPairs+tnpMuQTree) 
