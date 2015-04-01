import FWCore.ParameterSet.Config as cms

##    __  __       _          ____   _  _____   __  __                       
##   |  \/  | __ _| | _____  |  _ \ / \|_   _| |  \/  |_   _  ___  _ __  ___ 
##   | |\/| |/ _` | |/ / _ \ | |_) / _ \ | |   | |\/| | | | |/ _ \| '_ \/ __|
##   | |  | | (_| |   <  __/ |  __/ ___ \| |   | |  | | |_| | (_) | | | \__ \
##   |_|  |_|\__,_|_|\_\___| |_| /_/   \_\_|   |_|  |_|\__,_|\___/|_| |_|___/
##                                                                           
##   
### ==== Make PAT Muons ====
from RecoMuon.MuonIdentification.calomuons_cfi import calomuons

mergedMuons = cms.EDProducer("CaloMuonMerger",
    muons     = cms.InputTag("muons"), 
    caloMuons = cms.InputTag("calomuons"),
    tracks    = cms.InputTag("generalTracks"),
    mergeTracks = cms.bool(True),
    mergeCaloMuons = cms.bool(True),
    minCaloCompatibility = calomuons.minCaloCompatibility
)

import PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi
upcPatMuonsWithoutTrigger = PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi.patMuons.clone(
    muonSource = "mergedMuons",
    # embed the tracks, so we don't have to carry them around
    embedTrack          = True,
    embedCombinedMuon   = True,
    embedStandAloneMuon = True,
    embedPFCandidate    = False,
    embedCaloMETMuonCorrs = cms.bool(False),
    embedTcMETMuonCorrs   = cms.bool(False),
    # then switch off some features we don't need
    #addTeVRefits = False, ## <<--- this doesn't work. PAT bug ??
    embedPickyMuon = False,
    embedTpfmsMuon = False, 
    userIsolation = cms.PSet(),   # no extra isolation beyond what's in reco::Muon itself
    isoDeposits = cms.PSet(), # no heavy isodeposits
    addGenMatch = False,       # no mc: T&P doesn't take it from here anyway.
)
# Reset all these; the default in muonProducer_cfi is not empty, but wrong
upcPatMuonsWithoutTrigger.userData.userInts.src    = []
upcPatMuonsWithoutTrigger.userData.userFloats.src  = []
upcPatMuonsWithoutTrigger.userData.userCands.src   = []
upcPatMuonsWithoutTrigger.userData.userClasses.src = []

##    __  __       _       _       ____      ___        __  _     _ 
##   |  \/  | __ _| |_ ___| |__   |  _ \    / \ \      / / | |   / |
##   | |\/| |/ _` | __/ __| '_ \  | |_) |  / _ \ \ /\ / /  | |   | |
##   | |  | | (_| | || (__| | | | |  _ <  / ___ \ V  V /   | |___| |
##   |_|  |_|\__,_|\__\___|_| |_| |_| \_\/_/   \_\_/\_/    |_____|_|
##                                                                  
##   
from MuonAnalysis.MuonAssociators.muonL1Match_cfi import muonL1Match as muonL1Info
muonL1Info.src = "mergedMuons"

## Define a generic function, so that it can be used with existing PAT Muons
def addL1UserData(patMuonProducer, l1ModuleLabel = "muonL1Info"):
    "Load variables inside PAT muon, from module <l1ModuleLabel> that you must run before it"
    patMuonProducer.userData.userInts.src += [
        cms.InputTag(l1ModuleLabel, "quality"), # will be -999 in case of no match
    ]
    patMuonProducer.userData.userFloats.src += [  
        cms.InputTag(l1ModuleLabel, "deltaR"),  # will be 999 in case of no match
    ]
    patMuonProducer.userData.userCands.src += [
        cms.InputTag(l1ModuleLabel)
    ]

## Do it for this collection of pat Muons
addL1UserData(upcPatMuonsWithoutTrigger, "muonL1Info")

##    __  __       _       _       _   _ _   _____ 
##   |  \/  | __ _| |_ ___| |__   | | | | | |_   _|
##   | |\/| |/ _` | __/ __| '_ \  | |_| | |   | |  
##   | |  | | (_| | || (__| | | | |  _  | |___| |  
##   |_|  |_|\__,_|\__\___|_| |_| |_| |_|_____|_|  
##                                                 
##   

### ==== Unpack trigger, and match ====
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import patTrigger
patTrigger.onlyStandAlone = False

### ==== Then perform a match for all HLT triggers of interest
muonTriggerMatchHLT = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "upcPatMuonsWithoutTrigger" ),
    matched = cms.InputTag( "patTrigger" ),
    matchedCuts = cms.string(""),
#    andOr          = cms.bool( False ),
#    filterIdsEnum  = cms.vstring( '*' ),
#    filterIds      = cms.vint32( 0 ),
#    filterLabels   = cms.vstring( '*' ),
#    pathNames      = cms.vstring( '*' ),
#    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( True ) #change with respect to previous tag
)

### == For HLT triggers which are just L1s, we need a different matcher
from MuonAnalysis.MuonAssociators.muonHLTL1Match_cfi import muonHLTL1Match
muonMatchL1 = muonHLTL1Match.clone(
    src     = muonTriggerMatchHLT.src,
    matched = muonTriggerMatchHLT.matched,
)

### Single Mu L1
muonMatchHLTL1 = muonMatchL1.clone(matchedCuts = cms.string('coll("hltL1extraParticles")'))

patTriggerMatchers1Mu = cms.Sequence(
      muonMatchHLTL1 
)

patTriggerMatchers1MuInputTags = [
    cms.InputTag('muonMatchHLTL1','propagatedReco'), # fake, will match if and only if he muon did propagate to station 2
    cms.InputTag('muonMatchHLTL1'),
]

## ==== Embed ====
upcPatMuonsWithTrigger = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
    src     = cms.InputTag(  "upcPatMuonsWithoutTrigger" ),
    matches = cms.VInputTag()
)
upcPatMuonsWithTrigger.matches += patTriggerMatchers1MuInputTags

## ==== Trigger Sequence ====
patTriggerMatching = cms.Sequence(
    patTrigger * 
    patTriggerMatchers1Mu *
    upcPatMuonsWithTrigger
)

upcPatMuonsWithTriggerSequence = cms.Sequence(
    mergedMuons		    *
    muonL1Info             *
    upcPatMuonsWithoutTrigger *
    patTriggerMatching
)



def switchOffAmbiguityResolution(process):
    "Switch off ambiguity resolution: allow multiple reco muons to match to the same trigger muon"
    process.muonMatchHLTL1.resolveAmbiguities = False
    process.muonMatchHLTL2.resolveAmbiguities = False
    process.muonMatchHLTL3.resolveAmbiguities = False
    process.muonMatchHLTCtfTrack.resolveAmbiguities = False
    process.muonMatchHLTTrackMu.resolveAmbiguities  = False

def changeTriggerProcessName(process, triggerProcessName, oldProcessName="HLT"):
    "Change the process name under which the trigger was run"
    patTrigger.processName = triggerProcessName

def changeRecoMuonInput(process, recoMuonCollectionTag, oldRecoMuonCollectionTag=cms.InputTag("muons")):
    "Use a different input collection of reco muons"
    from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
    massSearchReplaceAnyInputTag(process.upcPatMuonsWithTriggerSequence, oldRecoMuonCollectionTag, recoMuonCollectionTag)

def useExistingPATMuons(process, newPatMuonTag, addL1Info=False):
    "Start from existing pat Muons instead of producing them"
    process.upcPatMuonsWithTriggerSequence.remove(process.upcPatMuonsWithoutTrigger)
    process.upcPatMuonsWithTriggerSequence.remove(process.muonL1Info)
    process.upcPatMuonsWithTrigger.src = newPatMuonTag
    from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
    massSearchReplaceAnyInputTag(process.upcPatMuonsWithTriggerSequence, cms.InputTag('upcPatMuonsWithoutTrigger'), newPatMuonTag)
    if addL1Info:
        process.muonL1Info.src = newPatMuonTag.muonSource
        addL1UserData(getattr(process,newPatMuonTag.moduleLabel), 'muonL1Info')

def addPreselection(process, cut):
    "Add a preselection cut to the muons before matching (might be relevant, due to ambiguity resolution in trigger matching!"
    process.upcPatMuonsWithoutTriggerUnfiltered = process.upcPatMuonsWithoutTrigger.clone()
    process.globalReplace('upcPatMuonsWithoutTrigger', cms.EDFilter("PATMuonSelector", src = cms.InputTag('upcPatMuonsWithoutTriggerUnfiltered'), cut = cms.string(cut))) 
    process.upcPatMuonsWithTriggerSequence.replace(process.upcPatMuonsWithoutTrigger, process.upcPatMuonsWithoutTriggerUnfiltered * process.upcPatMuonsWithoutTrigger)

def addMCinfo(process):
    "Add MC matching information to the muons"
    process.load("PhysicsTools.PatAlgos.mcMatchLayer0.muonMatch_cfi")
    process.upcPatMuonsWithTriggerSequence.replace(process.upcPatMuonsWithoutTrigger, process.muonMatch + process.upcPatMuonsWithoutTrigger)
    process.upcPatMuonsWithoutTrigger.addGenMatch = True
    process.upcPatMuonsWithoutTrigger.embedGenMatch = True
    process.upcPatMuonsWithoutTrigger.genParticleMatch = 'muonMatch'

def addDiMuonTriggers(process):
    print "[MuonAnalysis.MuonAssociators.upcPatMuonsWithTrigger_cff] Di-muon triggers are already enabled by default"

def useL1MatchingWindowForSinglets(process):
    "Change the L1 trigger matching window to be suitable also for CSC single triggers"
    if hasattr(process, 'muonL1Info'):
        process.muonL1Info.maxDeltaR     = 0.3 #Changed accordingly to Zoltan tuning. It was: 1.2
        process.muonL1Info.maxDeltaEta   = 0.2
        process.muonL1Info.fallbackToME1 = True
    if hasattr(process, 'muonMatchHLTL1'):
        process.muonMatchHLTL1.maxDeltaR     = 0.3 #Changed accordingly to Zoltan tuning. It was: 1.2
        process.muonMatchHLTL1.maxDeltaEta   = 0.2
        process.muonMatchHLTL1.fallbackToME1 = True

