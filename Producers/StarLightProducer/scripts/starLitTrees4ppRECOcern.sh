export X509_USER_PROXY="~/xrootdCert"

SLPRO=/afs/cern.ch/work/m/mojoe137/private/CMSSW_4_4_5_patch5/src/Producers/StarLightProducer
cd $SLPRO
eval `scram runtime -sh`
SLOUT=/store/caf/user/mojoe137/STARlightData/ #output file location
SLIN=$SLOUT/OniaPPRECO/$2/$3/$4/$5/
LOGS=/store/caf/user/mojoe137/logs/

cd -

echo $SLIN\STARlight_$2$3$4$5$7_NZSRECOnia44YPPRECO_$1.root > inputfile.txt
cmsRun $SLPRO/test/upcSplitTreesCondorPPRECO_MC_cfg.py inputfile.txt UPCSTARlight$2$52$3$4_$1.root

cmsStage -f UPCSTARlight$2$52$3$4_$1.root $SLOUT/UPCTrees/UPCSTARlight$2$52$3$4$7NZSPPRECO_$1.root
rm UPCSTARlight$2$52$3$4_$1.root
