import FWCore.ParameterSet.Config as cms
import sys
process = cms.Process('Demo')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

outfile=sys.argv[3]
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("PoolSource",
    fileNames= cms.untracked.vstring(sys.argv[2]),
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

# Change reconstruction
process.load("RecoLocalCalo.Configuration.RecoLocalCalo_cff")

#then you can have first last ts for reconstruction e.g.
process.hfreco.firstSample  = 1
process.hfreco.samplesToAdd = 6

process.p = cms.Path(process.hcalDigis * process.plots)
