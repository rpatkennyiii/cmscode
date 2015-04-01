#! /bin/bash

export X509_USER_PROXY="~/xrootdCert"

if test -z "${_CONDOR_SCRATCH_DIR}"; then
    export _CONDOR_SCRATCH_DIR=`pwd`
fi

MUPRO=/net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_3/src/Analyzers/UPCTriggerAnalyzer/condor/upcMuonCondor/
cd $MUPRO
eval `scram runtime -sh`

cd ${_CONDOR_SCRATCH_DIR}
hadd -f $1 `cat $2`  
hadoop fs -rm /cms/store/user/pkenny/MergedUPCTrees/$1
hadoop fs -put ${_CONDOR_SCRATCH_DIR}/$1 /cms/store/user/pkenny/MergedUPCTrees/
rm ${_CONDOR_SCRATCH_DIR}/$1
