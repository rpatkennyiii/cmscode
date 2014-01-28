import FWCore.ParameterSet.Config as cms
import sys


infile=open(sys.argv[2])
process = cms.Process("UPCTreeMaker")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000000
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1)); 

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile.readlines())
)
