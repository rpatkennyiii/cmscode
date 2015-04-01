#! /bin/bash
# args: jobNum  channel(JPsi,PsiPrime etc.) finalState(MuMu or EE) breakup(Xn0n or XnXn) conherency(Co or InCo) nEvents Cut Backward/Forward

if test -z "${_CONDOR_SCRATCH_DIR}"; then
    export _CONDOR_SCRATCH_DIR=./
fi

SLPRO=/net/hidsk0001/d00/scratch/mojoe137/CMSSW_4_4_5_patch1/src/Producers/StarLightProducer
cd $SLPRO
eval `scram runtime -sh`
SLOUT=/mnt/hadoop/cms/store/user/pkenny/STARlightData/ #output file location

cd ${_CONDOR_SCRATCH_DIR}
mkdir StLiJob$2$3$4$5$7$8$1
cd StLiJob$2$3$4$5$7$8$1
cp $SLPRO/starlightv87/slight$2$3$4$5$7$8.in ./slight.in

SEED=`date +%N | sed 's/0//g'`
cat slight.in | sed 's/RND_SEED.*/RND_SEED = '`gawk "BEGIN{srand($SEED);print (10000*rand())}"`'/' > slight.in.tmp
mv slight.in.tmp slight.in
cat slight.in | sed 's/N_EVENTS.*/N_EVENTS ='$6'/' > slight.in.tmp
mv slight.in.tmp slight.in

echo `date`: starting Starlight v87... > jobLog$1
$SLPRO/starlightv87/starlight >> jobLog$1

echo >> jobLog$1; echo `date` starting StarLight conversion to LHE >> jobLog$1
root -b -l -q "$SLPRO/starlightv87/utils/ConvertStarlightToLHE.C($1)" >> jobLog$1

echo >> jobLog$1; echo `date` starting StarLightLHE conversion to CMSSW RAW >> jobLog$1
cmsRun $SLPRO/test/starlightLHEtoRAWHcalNzs.py $1 2>> jobLog$1

echo >> jobLog$1; echo `date` starting StarLightRAW conversion to CMSSW RECO Onia>> jobLog$1
cmsRun $SLPRO/test/test_recoStarLightOniaPPRECO.py $1 2>> jobLog$1

for a in `ls | grep jPsi`; do mv $a `echo $a | sed 's/jPsi/'$2$3$4$5'/g'`;done;
for a in `ls | grep 2mumu`; do mv $a `echo $a | sed 's/2mumu//g'`;done;

#begining of tree creation

echo >> jobLog$1; echo `date` starting UPCTree creation >> jobLog$1
echo file:STARlight_$2$3$4$5_RECOnia44Y_$1.root > inputfile.txt
cmsRun $SLPRO/test/upcSplitTreesCondorPPRECO_MC_cfg.py inputfile.txt UPCSTARlight$2$52$3$4_$1.root 2>> jobLog$1

rm STARlight_$2$3$4$5_RAW44Y_$1.root
mv STARlight_$2$3$4$5_RECOnia44Y_$1.root $SLOUT/OniaPPRECO/$2/$3/$4/$5/STARlight_$2$3$4$5$7$8_NZSRECOnia44YPPRECO_$1.root
mv UPCSTARlight$2$52$3$4_$1.root $SLOUT/UPCTrees/UPCSTARlight$2$52$3$4$7$8NZSPPRECO_$1.root

cd ../
rm -r StLiJob$2$3$4$5$7$8$1 
