#
import FWCore.ParameterSet.Config as cms

process = cms.Process('ZDC')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
#process.load('Configuration/StandardSequences/GeometryExtended_cff')
#process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
#process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
#process.load('Configuration/StandardSequences/L1Reco_cff')
#process.load('Configuration/StandardSequences/Reconstruction_cff')
#process.load('DQMOffline/Configuration/DQMOffline_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True) 
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 
#	'file:running/neutron_Landau4times_SimParam_300GeV_8.3eta_500evtRECO2_OneTS4_908_0.root'
	'/store/data/Run2010A/MinimumBias/RECO/v4/000/138/563/8018A91D-1780-DF11-9608-001D09F28F0C.root'
	#/store/data/Run2010A/MinimumBias/RECO/Jul6thReReco_v1/0054/C0756946-E189-DF11-B1CB-002481E14FE6.root
	#/store/data/Run2010A/MinimumBias/RECO/Jul15thReReco-v1/0000/CCEEEE4E-A990-DF11-95A6-0025B31E3C00.root
	#/store/data/Run2010A/Commissioning/RECO/v4/000/138/563/A8F579FC-1680-DF11-B38C-001D09F27067.root
    )
)


# Other statements
#3_3_6 GlobalTag for reReco
#process.GlobalTag.globaltag = 'GR09_R_V5::All'
#3_4_1 GlobalTag fore reReco
#process.GlobalTag.globaltag = 'MC_37Y_V5::All'
process.GlobalTag.globaltag = "GR10_P_V6::All"

process.endjob_step = cms.Path(process.endOfProcess)

process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
process.prefer("GlobalTag")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('500evts.root')
)
#    counterbir = cms.string('BothSides_CollisionEvents'),
#    HLTTag = cms.InputTag("TriggerResults::HLT"),

process.demo = cms.EDAnalyzer('SecMarZDCAnalyzer')

process.p = cms.Path(process.demo)
cms.Schedule(process.p,process.endjob_step)
