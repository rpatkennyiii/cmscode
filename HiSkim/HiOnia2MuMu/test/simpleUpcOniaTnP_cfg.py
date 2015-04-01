import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("UpcOniaTnP")

process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_R_44_V14::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
infile=open(sys.argv[2])
outfile=(sys.argv[3])

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile.readlines())
)

process.tagMuonsTrg = cms.EDFilter("PATMuonSelector",
			       src = cms.InputTag("patMuonsWithTrigger"),
			       cut = cms.string("!triggerObjectMatchesByPath('HLT_HIUPCNeuMuPixel_SingleTrack_v1').empty()")
			       )

process.tpPairsTrg = cms.EDProducer("CandViewShallowCloneCombiner",
				cut = cms.string('2.0 < mass < 5.0'),
				decay = cms.string('tagMuonsTrg@+ patMuonsWithTrigger@-')
				)

process.tpPairsTrgFilter = cms.EDFilter("CandViewCountFilter",
				    src = cms.InputTag('tpPairsTrg'),
				    minNumber = cms.uint32(1),
				    )

process.TagAndProbeTrg = cms.Path(
	process.tagMuonsTrg *
	process.tpPairsTrg *
	process.tpPairsTrgFilter 
)

process.outTnP = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string(outfile),
        outputCommands = cms.untracked.vstring('drop *',
            'keep recoCompositeCandidates_tpPairsTrg__UpcOniaTnP',        # RECO di-muons, tpPairs for trigger efficiency
        ),
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('TagAndProbeTrg' ))
)

process.e = cms.EndPath(process.outTnP)
