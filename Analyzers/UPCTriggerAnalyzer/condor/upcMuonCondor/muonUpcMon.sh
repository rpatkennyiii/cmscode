#! /bin/bash

if test -z "${_CONDOR_SCRATCH_DIR}"; then
    export _CONDOR_SCRATCH_DIR=.
fi

PYTHONPATH=$PYTHONPATH:/usr/lib64/python2.4:/usr/lib64/python2.4/site-packages

cmsRun /net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_2_patch5/src/Analyzers/UPCTriggerAnalyzer/test/upcMonTreezCondor_cfg.py $1 ${_CONDOR_SCRATCH_DIR}/$2 
mv ${_CONDOR_SCRATCH_DIR}/$2 /mnt/hadoop/cms/store/user/pkenny/UPCData/UPCTrees/
