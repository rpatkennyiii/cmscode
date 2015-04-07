#! /bin/bash

cmsRun /net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_2_patch5/src/Analyzers/UPCTriggerAnalyzer/test/upcTreezCondor_MC_cfg.py $1 $2 
mv $2 /mnt/hadoop/cms/store/user/pkenny/STARlightData/UPCTrees/
