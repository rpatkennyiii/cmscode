#! /bin/bash
if test -z "${_CONDOR_SCRATCH_DIR}"; then
    export _CONDOR_SCRATCH_DIR=.
fi

ZBPRO=/net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_2_patch5/src/Analyzers/UPCTriggerAnalyzer/condor/zeroBiasCondor/

cd ${_CONDOR_SCRATCH_DIR}
cmsRun /net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_2_patch5/src/Analyzers/UPCTriggerAnalyzer/test/upcZeroBiasMaxCalTreezCondor_cfg.py $ZBPRO$1 ${_CONDOR_SCRATCH_DIR}/$2
mv ${_CONDOR_SCRATCH_DIR}/$2 /mnt/hadoop/cms/store/user/pkenny/ZeroBiasData/UPCTrees/
