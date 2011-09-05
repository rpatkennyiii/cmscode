{
//get the file w/ name stored in filename char*
TFile file("hazsv2-run152791to152957-unfiltered-jetoh-v1.root");
TDirectory *hltana= (file->Get("hltanalysis"));
TTree *hlt=hltana->Get("HltTree");

//Create cuts
TCut run("Run==152957");
TCut zdcP("L1_ZdcCaloPlus_BptxAND");
TCut zdcM("L1_ZdcCaloMinus_BptxAND");
TCut zdcAnd("L1_ZdcCaloPlus_ZdcCaloMinus_BptxAND");
TCut minBias("L1Tech_BSC_minBias_threshold1.v0");
TCut singleMuOpen("L1_SingleMuOpen");
TCut singleEG2("L1_SingleEG2");
TCut UPCNeutron((zdcP||zdcM)&&(!minBias));
TCut hiMinBias("L1_HcalHfCoincPmORBscMinBiasThresh1_BptxAND");

//store prescale values
double zdcPprescale=4000;
double zdcMprescale=4000;
double zdcAndprescale=40;
double hiMinBiasprescale=3;

//store ZDC+, ZDC-, and ZDCAnd prescaled rates
hlt->Draw("LumiBlock >> ZDCPRate(995,0,995)",zdcP&&run,"goff");
hlt->Draw("LumiBlock >> ZDCMRate(995,0,995)",zdcM&&run,"goff");
hlt->Draw("LumiBlock >> ZDCAndRate(995,0,995)",zdcAnd&&run,"goff");
hlt->Draw("LumiBlock >> ZDCOrPSRate(995,0,995)",(zdcP||zdcM)&&run,"goff");
hlt->Draw("LumiBlock >> UPCNeutronPSRate(995,0,995)",UPCNeutron&&run,"goff");
hlt->Draw("LumiBlock >> SingleMuPSRate(995,0,995)",singleMuOpen&&UPCNeutron&&run,"goff");
hlt->Draw("LumiBlock >> SingleEG2PSRate(995,0,995)",singleEG2&&UPCNeutron&&run,"goff");
hlt->Draw("LumiBlock >> HIMinBiasRate(995,0,995)",hiMinBias&&run,"goff");

//calculate the rescale value
double zdcPscalfactor=zdcPprescale/double(ZDCPRate->GetBinWidth(1)*23.31);
double zdcMscalfactor=zdcMprescale/double(ZDCMRate->GetBinWidth(1)*23.31);
double zdcAndscalfactor=zdcAndprescale/double(ZDCAndRate->GetBinWidth(1)*23.31);
double hiMinBiasscalefactor=hiMinBiasprescale/double(HIMinBiasRate->GetBinWidth(1)*23.31);

//rescale the histograms
ZDCMRate->Scale(zdcMscalfactor);
ZDCPRate->Scale(zdcPscalfactor);
ZDCAndRate->Scale(zdcAndscalfactor);
HIMinBiasRate->Scale(hiMinBiasscalefactor);

//create ZDCOr from ZDC+ + ZDC-, and UPCNeutron plot
TH1F ZDCOrRate=(*ZDCMRate+*ZDCPRate);
TH1F ZDCOrAndSubRate=(*ZDCMRate+*ZDCPRate-*ZDCAndRate);
TH1F UPCNeutronRatio=(*UPCNeutronPSRate)/(*ZDCOrPSRate);
TH1F SingleMuRatio=(*SingleMuPSRate)/(*ZDCOrPSRate);
TH1F SingleEG2Ratio=(*SingleEG2PSRate)/(*ZDCOrPSRate);

TH1F *ZDCOrDirectRate=ZDCOrPSRate->Clone();
ZDCOrDirectRate->Scale(4000);

ZDCOrRate.SetYTitle("Trigger Rate (Hz)");
ZDCOrRate.SetXTitle("LumiBlock");

TH1F UPCNeutronRate=(ZDCOrRate*UPCNeutronRatio);
TH1F SingleMuRate=(ZDCOrRate*UPCNeutronRatio*SingleMuRatio);
TH1F SingleEG2Rate=(ZDCOrRate*UPCNeutronRatio*SingleEG2Ratio);

//lable axes and set colors
ZDCOrRate->SetTitle("ZDC_OR");
UPCNeutronRate->SetTitle("UPCNeutronRate Estimate");
SingleMuRate->SetTitle("SingleMuRate Estimate");
SingleEG2Rate->SetTitle("SingleEG2Rate Estimate");

ZDCMRate->SetLineColor(2);
ZDCPRate->SetLineColor(3);
ZDCAndRate->SetLineColor(4);

//calculate ratio, error calc using sqrt(Nevents), and errors using error propagation between selections 
double nZDC=double(hlt->GetEntries((zdcP||zdcM)));
double nUPC=double(hlt->GetEntries(UPCNeutron));
double nSingleMuEvents=hlt->GetEntries(UPCNeutron&&singleMuOpen);
double nSingleEG2Events=hlt->GetEntries(UPCNeutron&&singleEG2);

double UpcRatioZdcor=nUPC/nZDC;
double MuRatioUpc=nSingleMuEvents/nUPC;
double Eg2RatioUpc=nSingleEG2Events/nUPC;
double MuRatioZdcor=nSingleMuEvents/nZDC;
double Eg2RatioZdcor=nSingleEG2Events/nZDC;

double ErrUpcRatioZdcor=sqrt(nUPC/pow(nZDC,2)+pow(nUPC,2)/pow(nZDC,3));
double ErrMuRatioUpc=sqrt(nSingleMuEvents/pow(nUPC,2)+pow(nSingleMuEvents,2)/pow(nUPC,3));
double ErrEg2RatioUpc=sqrt(nSingleEG2Events/pow(nUPC,2)+pow(nSingleEG2Events,2)/pow(nUPC,3));
double ErrMuRatioZdcor=sqrt(nSingleMuEvents/pow(nZDC,2)+pow(nSingleMuEvents,2)/pow(nZDC,3));
double ErrEg2RatioZdcor=sqrt(nSingleEG2Events/pow(nZDC,2)+pow(nSingleEG2Events,2)/pow(nZDC,3));

//create UPCNeutron, singleMu&&UPCNeutron, and singleEG2&&UPCNeutron rate plots
TH1F *UPCNeutronRateSimple=ZDCOrRate.Clone();
TH1F *SingleMuRateSimple=UPCNeutronRateSimple->Clone();
TH1F *SingleEG2RateSimple=UPCNeutronRateSimple->Clone();
UPCNeutronRateSimple->Scale(UpcRatioZdcor);
SingleMuRateSimple->Scale(MuRatioZdcor);
SingleEG2RateSimple->Scale(Eg2RatioZdcor);

//Calculate rate
double averageHiMinBiasRate = double(HIMinBiasRate->Integral())/double(HIMinBiasRate->GetNbinsX());
double minBiasUpScaleFactor = 3000./averageHiMinBiasRate; 
double singleMuOpenMaxRate =  ZDCOrRate->GetMaximum()*MuRatioZdcor;
double singleEG2OpenMaxRate =  ZDCOrRate->GetMaximum()*Eg2RatioZdcor;
double singleMuOpen2011MaxRate = singleMuOpenMaxRate*minBiasUpScaleFactor;
double singleEG2Open2011MaxRate =  singleEG2OpenMaxRate*minBiasUpScaleFactor;
}
