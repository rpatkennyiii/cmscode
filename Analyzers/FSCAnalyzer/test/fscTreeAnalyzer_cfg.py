import FWCore.ParameterSet.Config as cms
import sys
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

if len(sys.argv) > 2:
	infile=[sys.argv[2]]
	for x in sys.argv[3:(len(sys.argv)-2)]:
		infile.append(x)

	outfile='FSCTree.root'

	process = cms.Process("FSCTreeMaker")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.MessageLogger.cerr.FwkReport.reportEvery = 10
	process.load('Configuration.StandardSequences.MagneticField_38T_cff')
	process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
	process.load("Configuration.StandardSequences.GeometryDB_cff")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

	#	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

	nEventz=int(sys.argv[len(sys.argv)-1])
	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEventz))

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile)
	)

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
	#process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'
	process.GlobalTag.globaltag = 'GR_R_43_V3::All'

	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.fscAna = cms.EDAnalyzer('FSCAnalyzer',
		l1GtRR=cms.InputTag("gtDigis")
	)

	process.path = cms.Path(process.fscAna)
else:
	print 'error: no input file'	
