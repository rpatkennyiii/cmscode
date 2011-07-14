import sys
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList
from CmsHi.Analysis2010.CommonFunctions_cff import *

if len(sys.argv) > 2:
	infile=[sys.argv[2]]
	for x in sys.argv[3:(len(sys.argv)-2)]:
		infile.append(x)

	outfile='CaloEnergy.root'

	process = cms.Process("CaloEnergyTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.MessageLogger.cerr.FwkReport.reportEvery = 1
	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
	process.load('Configuration.StandardSequences.MagneticField_38T_cff')
	process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
	process.load("Configuration.StandardSequences.GeometryDB_cff")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

	overrideCentrality(process)

	nEventz=int(sys.argv[len(sys.argv)-1])
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEventz))

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile)
	)

	myLumis = LumiList.LumiList(filename = 'eflow.json').getCMSSWString().split(',')
	process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
	process.source.lumisToProcess.extend(myLumis)

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
	process.GlobalTag.globaltag="GR10_P_V12::All"
	
	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.caloana = cms.EDAnalyzer('CaloEnergyAnalyzer',
		etaBinSize = cms.bool(True),
		HFCorrection = cms.bool(True),
		realHFbins = cms.bool(True),
		noiseCut = cms.bool(True)
	)

	process.HeavyIonGlobalParameters=cms.PSet(centralityVariable= cms.string("PixelHits"),
    		centralitySrc = cms.InputTag("hiCentrality")
	)

	process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
	process.hltMinBiasHFOrBSC = process.hltHighLevel.clone()
	process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHfOrBSC_Core"] # don't forget '_Core' if working on HICorePhysics

#	process.hltMinBiasHFOrBSC.HLTPaths = ["HLT_HIMinBiasHf_Core","HLT_HIMinBiasBSC_Core"]
#	process.hltMinBiasHFOrBSC.andOr = cms.bool(True) # this is the default meaning either of the paths above
#	process.hltMinBiasHFOrBSC.throw = cms.bool(False) # don't throw exception since some runs have only one trigger or the other	

	process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

	process.path = cms.Path(process.hltMinBiasHFOrBSC*process.collisionEventSelection*process.caloana)
	
else:
	print 'error: no input file'	
