# Auto generated configuration file
# using: 
# Revision: 1.341.2.2 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: MinBias.cfi -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT:HIon,RAW2DIGI,L1Reco -n 100 --geometry DB --conditions STARTHI44_V12::All --datatier GEN-SIM-DIGI-RAW-HLTDEBUG --eventcontent RAWSIM --mc --scenario=HeavyIons --filein file:slight.lhe --filetype LHE --fileout STARlight_44Y.root --no_exec --python_filename TestLHE.py
import FWCore.ParameterSet.Config as cms
from Configuration.GenProduction.customiseCaloDigisNZS import * 
import sys
from random import randint

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealisticHI2011Collision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_HIon_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

customiseHcalNZS(process) 

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(int(sys.argv[6]))
)

etaSign=int(sys.argv[5])*2-1

# Input source
process.source = cms.Source("EmptySource",
     firstRun=cms.untracked.uint32(int(sys.argv[2])+1)
)

process.options = cms.untracked.PSet()

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.341.2.2 $'),
    annotation = cms.untracked.string('MinBias.cfi nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('jPsiGun2mumu_RAW44Y_'+sys.argv[2]+'.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW-HLTDEBUG')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'STARTHI44_V12::All'

your_seed=int(sys.argv[2])

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
     generator = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed),
     ),
     VtxSmeared = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     g4SimHits = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simSiPixelDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simSiStripDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simEcalUnsuppressedDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simHcalUnsuppressedDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simCastorDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simMuonCSCDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simMuonDTDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     ),
     simMuonRPCDigis = cms.PSet(
         initialSeed = cms.untracked.uint32(your_seed+randint(1,100)),
     )
)

process.generator = cms.EDProducer("Pythia6PtYDistGun",
    maxEventsToPrint = cms.untracked.int32(5),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(True),    
    PGunParameters = cms.PSet(
        ParticleID = cms.vint32(443),
        AddAntiParticle = cms.bool(False),
	kinematicsFile = cms.FileInPath('Producers/StarLightProducer/data/jpsiupcFlatY.root'),
	PtBinning = cms.int32(100000),
	YBinning = cms.int32(500),
        MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359),
        MinPt = cms.double(0.0),
        MaxPt = cms.double(0.5),
        MinY = cms.double(float(sys.argv[3])*etaSign),
        MaxY = cms.double(float(sys.argv[4])*etaSign)
    ),
    PythiaParameters = cms.PSet(
        pythiaJpsiDecays = cms.vstring('MDME(858,1)=0  ! J/psi -> ee turned ON',
            'MDME(859,1)=1                 ! J/psi -> mumu turned ON', 
            'MDME(860,1)=0                 ! J/psi -> random turned OFF'
        ),
        pythiaUESettings = cms.vstring(
            'MSTJ(11)=3     ! Choice of the fragmentation function',
            'MSTJ(22)=2     ! Decay those unstable particles',
            'PARJ(71)=10 .  ! for which ctau  10 mm',
            'MSTP(2)=1      ! which order running alphaS',
            'MSTP(33)=0     ! no K factors in hard cross sections',
            'MSTP(51)=7     ! structure function chosen',
            'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default',
            'MSTP(82)=4     ! Defines the multi-parton model',
            'MSTU(21)=1     ! Check on possible errors during program execution',
            'PARP(82)=1.9409   ! pt cutoff for multiparton interactions',
            'PARP(89)=1960. ! sqrts for which PARP82 is set',
            'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter',
            'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter',
            'PARP(90)=0.16  ! Multiple interactions: rescaling power',
            'PARP(67)=2.5    ! amount of initial-state radiation',
            'PARP(85)=1.0  ! gluon prod. mechanism in MI',
            'PARP(86)=1.0  ! gluon prod. mechanism in MI',
            'PARP(62)=1.25   ! ',
            'PARP(64)=0.2    ! ',
            'MSTP(91)=1     !',
            'PARP(91)=2.1   ! kt distribution',
            'PARP(93)=15.0  ! '
        ),
        parameterSets = cms.vstring(
            'pythiaUESettings',
            'pythiaJpsiDecays'
        )
    )
)

process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen_hi)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.L1Reco_step,process.endjob_step,process.RAWSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

