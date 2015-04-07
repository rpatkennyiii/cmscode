#! /bin/bash

if test -z "${_CONDOR_SCRATCH_DIR}"; then
    export _CONDOR_SCRATCH_DIR=
fi

MUPRO=/net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_2_patch5/src/Analyzers/UPCTriggerAnalyzer/condor/upcMuonCondor/

cd ${_CONDOR_SCRATCH_DIR}
cmsRun /net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_2_patch5/src/Analyzers/UPCTriggerAnalyzer/test/upcTreezCondorDoubleMu_cfg.py $MUPRO$1 ${_CONDOR_SCRATCH_DIR}$2 
mv ${_CONDOR_SCRATCH_DIR}$2 /mnt/hadoop/cms/store/user/pkenny/UPCData/UPCTrees/
