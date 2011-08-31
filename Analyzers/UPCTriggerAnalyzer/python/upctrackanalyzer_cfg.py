import sys
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList
from CmsHi.Analysis2010.CommonFunctions_cff import *

if len(sys.argv) > 2:
	infile=[sys.argv[2]]
	for x in sys.argv[3:(len(sys.argv)-2)]:
		infile.append(x)

	outfile='UPCTrackTree.root'

	process = cms.Process("UPCTriggerTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
	process.load('Configuration.StandardSequences.MagneticField_38T_cff')
	process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
	process.load("Configuration.StandardSequences.GeometryDB_cff")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#	process.load('L1Trigger.Skimmer.l1Filter_cfi')
#	process.load('HLTrigger.HLTfilters.hltLevel1GTSeed_cfi')
#	process.load("HLTrigger.HLTanalyzers.HI_HLTAnalyser_cff")

#	process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('L1_BscMinBiasThreshold1')

#	process.upcNeutron = !(process.l1Filter.clone(
#		algorithms = cms.vstring('L1_BscMinBiasThreshold1')
#	))

#	process.hltanalysis.RunParameters.Debug = False
#	process.hltanalysis.RunParameters.UseTFileService = True
#	process.hltanalysis.RunParameters.Monte = False
#	process.hltanalysis.RunParameters.DoMC = False
#	process.hltanalysis.RunParameters.DoJets = True
#	process.hltanalysis.RunParameters.DoPhotons = True
#	process.hltanalysis.RunParameters.DoSuperClusters = True
#	process.hltanalysis.recjets  = "iterativeConePu5CaloJets"
#	process.hltanalysis.BarrelPhoton = "correctedIslandBarrelSuperClusters"
#	process.hltanalysis.EndcapPhoton = "correctedIslandEndcapSuperClusters"
#	process.hltanalysis.l1GtReadoutRecord = cms.InputTag("gtDigis")

	nEventz=int(sys.argv[len(sys.argv)-1])
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEventz))

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile)
	)

#	myLumis = LumiList.LumiList(filename = 'eflow.json').getCMSSWString().split(',')
#	process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#	process.source.lumisToProcess.extend(myLumis)

	#3_3_6 GlobalTag for reReco
	#process.GlobalTag.globaltag = 'GR09_R_V5::All'
	#3_4_1 GlobalTag fore reReco
	#process.GlobalTag.globaltag = 'MC_37Y_V5::All'
	#process.GlobalTag.globaltag = "CRAFT09_R_V1::All"
	#process.GlobalTag.globaltag = "CRUZETALL_V8::All"
	#process.GlobalTag.globaltag = "GR09_31X_V6P::All"
	#process.GlobalTag.globaltag = "GR10_P_V4::All"
	#process.GlobalTag.globaltag = "GR_R_37X_V5A::All"
	#process.GlobalTag.globaltag = "GR_R_38X_V9::All"
	#HI global tags
	#"GR10_P_V12::All"
	#process.GlobalTag.globaltag="GR_R_43_V2::ALL"
	process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'
	
	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.upctana = cms.EDAnalyzer('UPCTrackAnalyzer',
			  trackCollection = cms.string("hiSelectedTracks")
                          )

	process.path = cms.Path(process.upctana)
else:
	print 'error: no input file'	
