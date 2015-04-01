#! /bin/bash
# args: jobNum  channel(JPsi,PsiPrime etc.) finalState(MuMu or EE) breakup(Xn0n or XnXn) conherency(Co or InCo) nEvents Cut Backward/Forward

export X509_USER_PROXY="~/xrootdCert"

SLPRO=/afs/cern.ch/work/m/mojoe137/private/CMSSW_4_4_5_patch5/src/Producers/StarLightProducer
cd $SLPRO
eval `scram runtime -sh`
SLOUT=/store/caf/user/mojoe137/STARlightData/ #output file location
LOGS=/store/caf/user/mojoe137/logs/

cd -
mkdir StLiJob$2$3$4$5$7$1
cd StLiJob$2$3$4$5$7$1

echo >> jobLogPPRECO$2$3$4$5$7$1; echo `date` starting StarLightLHE conversion to CMSSW RAW >> jobLogPPRECO$2$3$4$5$7$1
cp $SLPRO/input/lhe$2$3$4$5$1.txt ./
cmsRun $SLPRO/test/starlightLHEtoRAWHcalNzs.py $1 lhe$2$3$4$5$1.txt 2>> jobLogPPRECO$2$3$4$5$7$1

echo >> jobLogPPRECO$2$3$4$5$7$1; echo `date` starting StarLightRAW conversion to CMSSW RECO Onia>> jobLogPPRECO$2$3$4$5$7$1
cmsRun $SLPRO/test/test_recoStarLightOniaPPRECO.py $1 2>> jobLogPPRECO$2$3$4$5$7$1

for a in `ls | grep jPsi`; do mv $a `echo $a | sed 's/jPsi/'$2$3$4$5'/g'`;done;
for a in `ls | grep 2mumu`; do mv $a `echo $a | sed 's/2mumu//g'`;done;

#begining of tree creation

echo >> jobLogPPRECO$2$3$4$5$7$1; echo `date` starting UPCTree creation >> jobLogPPRECO$2$3$4$5$7$1
echo file:STARlight_$2$3$4$5_RECOnia44Y_$1.root > inputfile.txt
cmsRun $SLPRO/test/upcSplitTreesCondorPPRECO_MC_cfg.py inputfile.txt UPCSTARlight$2$52$3$4_$1.root 2>> jobLogPPRECO$2$3$4$5$7$1

rm STARlight_$2$3$4$5_RAW44Y_$1.root
cmsStage -f STARlight_$2$3$4$5_RECOnia44Y_$1.root $SLOUT/OniaPPRECO/$2/$3/$4/$5/STARlight_$2$3$4$5$7_NZSRECOnia44YPPRECO_$1.root
cmsStage -f UPCSTARlight$2$52$3$4_$1.root $SLOUT/UPCTrees/UPCSTARlight$2$52$3$4$7NZSPPRECO_$1.root
cmsStage -f jobLogPPRECO$2$3$4$5$7$1 $LOGS

cd ../
rm -r StLiJob$2$3$4$5$7$1 
