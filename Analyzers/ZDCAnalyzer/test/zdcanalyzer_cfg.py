import FWCore.ParameterSet.Config as cms
import sys

if len(sys.argv) > 2:
	infile=[sys.argv[2]]
	for x in sys.argv[3:]:
		infile.append(x)

	outfile='zdctrees.root'

	process = cms.Process("ZDCosmo")

	process.load("FWCore.MessageService.MessageLogger_cfi")
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

	raw=0

	if infile[0].find('RAW')+1:
		process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
		raw=1

	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

	process.source = cms.Source("PoolSource",
	    fileNames = cms.untracked.vstring(infile
	    #'/store/data/BeamCommissioning09/BeamHalo/RAW/v1/000/120/013/B60078A5-D4CB-DE11-8E7F-001617E30D12.root'
	    #'/store/data/BeamCommissioning09/BeamHalo/RAW/v1/000/120/015/22209C11-F0CB-DE11-AC55-001D09F253FC.root'
	    )
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
	process.GlobalTag.globaltag = "GR_R_38X_V9::All"

	process.TFileService = cms.Service("TFileService",
	    fileName = cms.string(outfile)
	)

	process.zdcana = cms.EDAnalyzer('ZDCAnalyzer',)

	if raw:
		process.p = cms.Path(process.hcalDigis*process.zdcana)
	else:
		process.p = cms.Path(process.zdcana)
else:
	print 'error: no input file'	
