import FWCore.ParameterSet.Config as cms
import sys
process = cms.Process('Demo')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

outfile=sys.argv[3]

process.source = cms.Source("HcalTBSource",
    fileNames= cms.untracked.vstring(
	  sys.argv[2]
    ),
    streams = cms.untracked.vstring('HCAL_Trigger', 'HCAL_SlowData','HCAL_DCC700')
)

process.tbunpacker = cms.EDFilter("HcalTBObjectUnpacker",
    HcalTriggerFED       = cms.untracked.int32(1),
    HcalSlowDataFED      = cms.untracked.int32(3),
    HcalTDCFED           = cms.untracked.int32(-1),
    HcalSourcePosFED     = cms.untracked.int32(-1),
    IncludeUnmatchedHits = cms.untracked.bool(False)
)

process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    dump = cms.untracked.vstring(''),
    file = cms.untracked.string('')
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string(outfile)
)

process.plots = cms.EDAnalyzer("ZDCFSCAnalyzer")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_R_53_V18::All'
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

process.hcalDigis = cms.EDProducer("HcalRawToDigi",
    UnpackZDC = cms.untracked.bool(True),
    FilterDataQuality = cms.bool(True),
    HcalFirstFED = cms.untracked.int32(700),
    InputLabel = cms.InputTag("source"),
    UnpackCalib = cms.untracked.bool(True),
    FEDs = cms.untracked.vint32(722), 
#        700, 701, 702, 703, 704,
#        705, 706, 707, 708, 709,
#        710, 711, 712, 713, 714),
#        715, 716, 717, 718, 
#        719, 720, 721, 723, 724,
#        725, 726, 727, 728, 729),
#        730, 731, 
#    	 732),
    streams = cms.untracked.vstring(
          'HCAL_Trigger','HCAL_SlowData','HCAL_QADCTDC'
    ),
    lastSample = cms.int32(9),
    firstSample = cms.int32(0),
    ComplainEmptyData = cms.untracked.bool(True)
)

# Change reconstruction
process.load("RecoLocalCalo.Configuration.RecoLocalCalo_cff")

#then you can have first last ts for reconstruction e.g.
process.hfreco.firstSample  = 1
process.hfreco.samplesToAdd = 6
#process.load("L1Trigger.Configuration.L1DummyConfig_cff")
#process.load("EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi")
#process.l1GtUnpack.DaqGtInputTag = 'source'
process.p = cms.Path(process.hcalDigis+process.plots)
