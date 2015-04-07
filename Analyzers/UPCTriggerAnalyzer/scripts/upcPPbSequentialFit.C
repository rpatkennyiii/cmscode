{
using namespace RooFit;
/////////////////////////////////////////////////////////////
// global variables
/////////////////////////////////////////////////////////////

// canvas for plots
//TCanvas *c = new TCanvas("c","c",600,600);

//void upcPPbFit()

  int mTemp=0; //0 one crystal ball, 1 m templates from MC, 2 two crystal balls
  double ptMin=0,ptMax=5,mMin=2.4, mMax=3.6, rapMin=-2.1, rapMax=-1.8;
  int nMassBins=20*(mMax-mMin)/(3.5-2.6), nPtBins=40;

  RooRealVar m("m", "dilepton m [GeV]",mMin,mMax);
  RooRealVar y("y", "dilepton rapidity",rapMin,rapMax);
  RooRealVar pt("pt", "dilepton pT [GeV]",ptMin,ptMax);
  // load all data
  RooDataSet* mcJpsiPtYM = RooDataSet::read("/home/mojoe137/Dropbox/dttGroupPresentations/scripts/pPb/txtFiles/mc/MpTy_data-jpsi.txt",RooArgList(m,pt,y)) ;
  RooDataSet* mcGammaPtYM = RooDataSet::read("/home/mojoe137/Dropbox/dttGroupPresentations/scripts/pPb/txtFiles/mc/MpTy_data-gg.txt",RooArgList(m,pt,y)) ;
  RooDataSet* dataNonExPtYM = RooDataSet::read("/home/mojoe137/Dropbox/dttGroupPresentations/scripts/pPb/txtFiles/case1/MpTy_data-pPb-moreThanTwoTracks.txt",RooArgList(m,pt,y)); 
  RooDataSet* dataExPtYM = (RooDataSet*)RooDataSet::read("/home/mojoe137/Dropbox/dttGroupPresentations/scripts/pPb/txtFiles/case1/MpTy_data-pPb.txt",RooArgList(m,pt,y));
  RooDataSet* massSmallDataP = (RooDataSet*)dataExPtYM->reduce(m);
  RooDataSet* ptData = (RooDataSet*)dataExPtYM->reduce(pt);
  RooDataSet massSmallData=(*massSmallDataP);

  TH2F *dataNonExHistPtM= dataNonExPtYM->createHistogram(m,pt,nMassBins,nPtBins);
  TH2F *mcJpsiHistPtM=mcJpsiPtYM->createHistogram(m,pt,nMassBins,nPtBins);
  TH2F *mcGammaHistPtM=mcGammaPtYM->createHistogram(m,pt,nMassBins,nPtBins);

	TH1D *jpExPt=mcJpsiHistPtM->ProjectionY();
	TH1D *jpNonExPt=(TH1D*)dataNonExHistPtM->ProjectionY();
	TH1D *gammaPt=(TH1D*)mcGammaHistPtM->ProjectionY();
	TH1D *jpExMass=(TH1D*)mcJpsiHistPtM->ProjectionX();
	TH1D *jpNonExMass=(TH1D*)dataNonExHistPtM->ProjectionX();
	TH1D *gammaMass=(TH1D*)mcGammaHistPtM->ProjectionX();

  jpNonExPt->Smooth(5);
  RooHistPdf bgPdf("bgPdf",   "bgPdf",  pt, RooDataHist("bgDH",  "bgDH",  RooArgList(pt),jpNonExPt),1);
  RooDataSet *toyMCdata = bgPdf.generate(pt,100000);

  TH1D *jpNonExPt2= (TH1D*)toyMCdata->createHistogram("pt",nPtBins);

  RooDataHist jpEx("jpEx","scaled jpEx",pt,Import(*jpExPt));
  RooDataHist jpNonEx("jpNonEx","scaled jpNonEx",pt,Import(*jpNonExPt));
  RooDataHist gamma("gamma","scaled gamma",pt,Import(*gammaPt));
  RooDataHist mJpEx("mJpEx","scaled m jpEx",m,Import(*jpExMass));
  RooDataHist mJpNonEx("mNonEx","scaled m jpNonEx",m,Import(*jpNonExMass));
  RooDataHist mGamma("mGamma","scaled m gamma",m,Import(*gammaMass));

  RooHistPdf jpExPdf("jpExPdf","jpExPdf",pt,jpEx);
  RooHistPdf jpNonExPdf("jpNonExPdf","jpNonExPdf",pt,jpNonEx);
  RooHistPdf gammaPdf("gammaPdf","gammaPdf",pt,gamma);
  RooHistPdf mJpExPdf("mJpExPdf","mJpExPdf",m,mJpEx);
  RooHistPdf mJpNonExPdf("mNonExPdf","mNonExPdf",m,mJpNonEx);
  RooHistPdf mGammaPdf("mGammaPdf","mGammaPdf",m,mGamma);

	RooRealVar mean1("mean1","mean",3,2.9,3.2);
	RooRealVar mean2("mean2","mean",3.686,3.5,3.9);
	RooRealVar sigma1("sigma1","#sigma_{co}",0.04,0.001,1);
	RooRealVar sigma2("sigma2","#sigma_{inco}",0.04,0.001,1);
  RooRealVar alpha("alpha","tail shift",2.0,0.2,4);
	RooRealVar npow("npow","power order",1,.1,3);
	RooCBShape cball1("cball1","cball1",m,mean1,sigma1,alpha,npow);
	RooCBShape cball2("cball2","cball2",m,mean2,sigma2,alpha,npow);

  RooRealVar MPV("MPV","MPV",3.660);
  RooRealVar sigmaL("sigmaL","sigmaL",0.54);
	RooLandau bkgL("bkgP","BackgroundLandau",m,MPV,sigmaL);

  RooRealVar a0P("a0P","a0P",0.1,-4.,4.);
  RooRealVar a1P("a1P","a1P",0.1,-4.,4.);
	RooPolynomial bkgPoly("bkgP","BackgroundPoly",m,RooArgList(a0P,a1P));

  int nTot=ptData->numEntries();
  int nSideBandsPre=nTot/100.0;
  RooRealVar nSideBands("nSideBands","nSideBands",nSideBandsPre,0,nTot);
  RooRealVar nEx("nEx","nEx",nTot/3,0,nTot);
  RooRealVar nNonEx("nNonEx","nNonEx",nTot/3,0,nTot);
  RooRealVar nJpsi("nJpsi","nJ/#psi",2*nTot/3.0,0,nTot);
  //RooFormulaVar nJpsi("nJpsi","nEx+nNonEx",RooArgSet(nEx,nNonEx));

//mass pdfs  
	RooAddPdf twoSigBackLand("twoSigBack","cb1+bck",RooArgList(bkgL,cball1),RooArgList(nSideBands,nJpsi));
	RooAddPdf cbPolyBack("twoSigBack","cb1+bck",RooArgList(bkgPoly,cball1),RooArgList(nSideBands,nJpsi));
	RooAddPdf cbMassTempBack("twoSigBack","cb1+bck",RooArgList(mGammaPdf,cball1),RooArgList(nSideBands,nJpsi));
	RooAddPdf threeSigBack("twoSigBack","cb1+bck",RooArgList(bkgL,cball1,cball2),RooArgList(nSideBands,nEx,nNonEx));
  RooAddPdf coNonExGammaMass("coNonExGammaMass","coNonExGammaMass",RooArgList(mGammaPdf,mJpExPdf,mJpNonExPdf),RooArgList(nSideBands,nEx,nNonEx));
  RooAddPdf coNonExGamma("coNonExGamma","coNonExGamma",RooArgList(gammaPdf,jpExPdf,jpNonExPdf),RooArgList(nSideBands,nEx,nNonEx));
	RooAddPdf gammaBall("gammaBall","cb1+bck",RooArgList(mGammaPdf,cball1),RooArgList(nSideBands,nJpsi));
  RooAddPdf *twoSigBack;

  RooCategory ptVm("ptVm","ptVm");
  ptVm.defineType("pt");
  ptVm.defineType("m");

  RooAbsReal *massNll;

  int massTemp=1;
  if(massTemp==1){
    twoSigBack=&twoSigBackLand;
    massNll=twoSigBackLand.createNLL(massSmallData, RooFit::Extended(kTRUE));
  }else if(massTemp==2){
    twoSigBack=&cballGammaMass;
    massNll=cballGammaMass.createNLL(massSmallData, RooFit::Extended(kTRUE));
  }else{
    twoSigBack=&cbPolyBack;
    massNll=twoSigBack.createNLL(massSmallData, RooFit::Extended(kTRUE));
  }

  RooMinuit massM(*massNll);
  massM.migrad();
  massM.hesse();
//  massM.minos();

  RooPlot *fitFrameMass = m.frame();
  massSmallData->plotOn(fitFrameMass,Binning(19),Name("theData"));
  twoSigBack->plotOn(fitFrameMass,Name("thePdf"),LineStyle(8), LineColor(kBlack), LineWidth(3));
  twoSigBack->plotOn(fitFrameMass,Components("bkgP"),LineStyle(9),LineColor(kOrange), LineWidth(3));
  twoSigBack->plotOn(fitFrameMass,Components("cball1"),LineStyle(3),LineColor(kViolet), LineWidth(3));

  RooArgSet * pars = twoSigBack->getParameters(massSmallData);
  int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize();
  double mychsq = fitFrameMass->chiSquare("thePdf","theData", nfloatpars);
  double myndof = 19 - nfloatpars;
  twoSigBack.paramOn(fitFrameMass,Layout(0.50,0.99,0.99),Format("NEA",AutoPrecision(2))
      ,Label(Form("#chi^{2}/ndf = %2.0f/%2.0f", myndof*mychsq, myndof))
      );

  TCanvas *c1=new TCanvas("c1","c1");
  fitFrameMass->Draw();

  RooRealVar nGammaSmall("nGammaSmall","nGammaSmall",nSideBands.getVal(),0,nSideBands.getVal()+nSideBands.getError());//nGammaPre-widthGamma,nGammaPre+widthGamma);
  RooAddPdf coInCoGammaSmall("coInCoGammaSmall","coInCoGammaSmall",RooArgList(gammaPdf,jpExPdf,jpNonExPdf),RooArgList(nGammaSmall,nEx,nNonEx));
  RooAbsReal *ptNll=coInCoGammaSmall.createNLL(*ptData, RooFit::Extended(kTRUE));
  RooMinuit ptM(*ptNll);
  ptM.migrad();
  ptM.hesse();
  ptM.minos();

  RooPlot *fitFramePt = pt.frame();
  ptData->plotOn(fitFramePt,Binning(nPtBins),Name("theData"));
  coInCoGammaSmall->plotOn(fitFramePt,Name("thePdf"),LineStyle(8), LineColor(kBlack), LineWidth(3));
  coInCoGammaSmall->plotOn(fitFramePt,Components("gammaPdf"),LineStyle(9),LineColor(kGreen+2), LineWidth(3));
  coInCoGammaSmall->plotOn(fitFramePt,Components("jpNonExPdf"),LineStyle(3),LineColor(kRed), LineWidth(3));
  coInCoGammaSmall->plotOn(fitFramePt,Components("jpExPdf"),LineStyle(3),LineColor(kBlue), LineWidth(3));

  RooArgSet * pars = coInCoGammaSmall->getParameters(massSmallData);
  int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize();
  double mychsq = fitFramePt->chiSquare("thePdf","theData", nfloatpars);
  double myndof = 19 - nfloatpars;
  coInCoGammaSmall.paramOn(fitFramePt,Layout(0.50,0.99,0.99),Format("NEA",AutoPrecision(2))
      ,Label(Form("#chi^{2}/ndf = %2.0f/%2.0f", myndof*mychsq, myndof))
      );

  TCanvas *c2=new TCanvas("c2","c2");
  fitFramePt->Draw();

/*
  double nExS=nEx.getVal(), nExSError=nEx.getError();
  double nNonExS=nNonEx.getVal(), nNonExSError=nNonEx.getError();
  double nGammaS=nGammaSmall.getVal(), nGammaError=nGammaSmall.getError();

  cout << "two step fit fco (pT < 150 MeV)=" << nEx.getVal()*jpExPt->Integral(1,15)/jpExPt->Integral()/ptData->sumEntries("pt<.150") << " +/- " 
       << nEx.getError()*jpExPt->Integral(1,15)/jpExPt->Integral()/ptData->sumEntries("pt<.150") << endl;
  cout << "two step fit nCo (pT < 150 MeV)=" << nEx.getVal()*jpExPt->Integral(1,15)/jpExPt->Integral() << " +/- " 
       << nEx.getError()*jpExPt->Integral(1,15)/jpExPt->Integral() << endl;
  cout << "two step fit finco (pT < 150 MeV)=" << nNonEx.getVal()*jpNonExPt->Integral(1,15)/jpNonExPt->Integral()/ptData->sumEntries("pt<.150") << " +/- " 
       << nNonEx.getError()*jpNonExPt->Integral(1,15)/jpNonExPt->Integral()/ptData->sumEntries("pt<.150") << endl;
  cout << "two step fit nInco (pT < 150 MeV)=" << nNonEx.getVal()*jpNonExPt->Integral(1,15)/jpNonExPt->Integral() << " +/- " 
       << nNonEx.getError()*jpNonExPt->Integral(1,15)/jpNonExPt->Integral() << endl;
 
  TCanvas *c3 = new TCanvas("c3","c3");
//	jpExPt->Rebin(5);
//	jpNonExPt->Rebin(5);
//	gammaPt->Rebin(5);

  TH1D *dataHist = ptData->createHistogram("pt",nPtBins);
  dataHist->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  dataHist->GetXaxis()->SetTitleOffset(1.2);
  dataHist->GetYaxis()->SetTitle("Events / (0.05 GeV/c)");
  dataHist->GetYaxis()->SetTitleOffset(1.25);
  dataHist->SetBinErrorOption( TH1::kPoisson );
  dataHist->Draw("PE0");

	jpExPt->SetLineColor(kBlue);
	jpExPt->SetLineStyle(2);
	jpExPt->SetLineWidth(3);
	
	jpNonExPt->SetLineColor(kRed);
	jpNonExPt->SetLineStyle(5);
	jpNonExPt->SetLineWidth(3);
	
	gammaPt->SetLineColor(kGreen+2);
	gammaPt->SetLineStyle(9);
	gammaPt->SetLineWidth(3);
	
	dataHist->SetMarkerStyle(20);
	dataHist->SetMarkerSize(1);
	
	TH1F *simjpExPt = (TH1F *)jpExPt->Clone();
	TH1F *simjpNonExPt = (TH1F *)jpNonExPt->Clone();
	TH1F *simgammaPt = (TH1F *)gammaPt->Clone();
	simjpExPt->Scale(nExS/simjpExPt->Integral());
	simjpNonExPt->Scale(nNonExS/simjpNonExPt->Integral());
	simgammaPt->Scale(nGammaS/simgammaPt->Integral());

  TH1F *allMC= (TH1F*) jpExPt->Clone();
  allMC->Add(jpNonExPt);
  allMC->Add(gammaPt);

	allMC->SetLineColor(kBlack);
	allMC->SetLineStyle(8);
	allMC->SetLineWidth(3);

  TH1F *allMCSim= (TH1F*) simjpExPt->Clone();
  allMCSim->Add(simjpNonExPt);
  allMCSim->Add(simgammaPt);

	allMCSim->SetLineColor(kBlack);
	allMCSim->SetLineStyle(8);
	allMCSim->SetLineWidth(3);

	TLegend *l2 = new TLegend(0.6,0.6,0.89,0.89,NULL,"brNDC");
	l2->AddEntry(dataHist,"data","lp");
	l2->AddEntry(allMC,"Sum","l");
	
	l2->AddEntry(jpExPt,"Exclusive J/#psi","l");
	l2->AddEntry(jpNonExPt,"Non-Exclusive","l");
	l2->AddEntry(gammaPt,"#gamma#gamma #rightarrow #mu^{+}#mu^{-}","l");
	
	l2->SetFillColor(10);
	l2->Draw();
  
  TText *t1 = new TText(0.2,140,"CMS Preliminary");
  t1->SetTextSize(0.03) ;
  TLatex *t2 = new TLatex(0.2,130,"PbPb #sqrt{s_{NN}} = 2.76 TeV");
  t2->SetTextSize(0.03) ;
  TLatex *t3 = new TLatex(0.2,120,"L_{int} = 150 #mub^{-1}");
  t3->SetTextSize(0.03) ;
  TLatex *t4 = new TLatex(0.2,136,"1.2 < p_{T}^{#mu} < 1.8 GeV/c");
  t4->SetTextSize(0.03) ;
  TLatex *t5 = new TLatex(0.2,140,"1.2 < |#eta^{#mu}| < 2.4");
  t5->SetTextSize(0.03) ;
  
  dataHist->Draw("PE0");
	simjpExPt->Draw("sames");
	simjpNonExPt->Draw("sames");
	simgammaPt->Draw("sames");
  allMCSim->Draw("sames");
	l2->Draw();
  t1->Draw("same");
  t2->Draw("same");
  t3->Draw("same");
*/
}
