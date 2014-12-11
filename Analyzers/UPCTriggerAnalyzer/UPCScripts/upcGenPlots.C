#include "TString.h"
#include "TMath.h"

TString genInvMass(){
	return(TString("sqrt(")
	+"pow(sqrt(pow(mass[4],2)+pow(px[4],2)+pow(py[4],2)+pow(pz[4],2))+"
	+"sqrt(pow(mass[5],2)+pow(px[5],2)+pow(py[5],2)+pow(pz[5],2)),2)"
	+"-(pow(px[4]+px[5],2)+pow(py[4]+py[5],2)+pow(pz[4]+pz[5],2)))");
}

TString recoInvMass( TString type="Tracker", TString tree="", TString mass="0.1057"){
	TString E0="sqrt("+mass+"*"+mass+"+"+tree+"p"+type+"[0]*"+tree+"p"+type+"[0])";
	TString E1="sqrt("+mass+"*"+mass+"+"+tree+"p"+type+"[1]*"+tree+"p"+type+"[1])";
	TString px0=""+tree+"p"+type+"[0]*cos("+tree+"lambda"+type+"[0])*cos("+tree+"phi"+type+"[0])";
	TString px1=""+tree+"p"+type+"[1]*cos("+tree+"lambda"+type+"[1])*cos("+tree+"phi"+type+"[1])";
	TString py0=""+tree+"p"+type+"[0]*cos("+tree+"lambda"+type+"[0])*sin("+tree+"phi"+type+"[0])";
	TString py1=""+tree+"p"+type+"[1]*cos("+tree+"lambda"+type+"[1])*sin("+tree+"phi"+type+"[1])";
	TString pz0=""+tree+"p"+type+"[0]*sin("+tree+"lambda"+type+"[0])";
	TString pz1=""+tree+"p"+type+"[1]*sin("+tree+"lambda"+type+"[1])";
	return("sqrt(("+E0+"+"+E1+")*("+E0+"+"+E1+")-("+px0+"+"+px1+")*("+px0+"+"+px1+")-("+py0+"+"+py1+")*("+py0+"+"+py1+")-("+pz0+"+"+pz1+")*("+pz0+"+"+pz1+"))");
}

TString genPairPt(){return(TString("sqrt(pow(px[4]+px[5],2)+pow(py[4]+py[5],2))"));}

TString genPairRapidity(){return(
	TString("0.5*log(")
	+"(sqrt(pow(mass[4],2)+pow(px[4],2)+pow(py[4],2)+pow(pz[4],2))+sqrt(pow(mass[5],2)+pow(px[5],2)+pow(py[5],2)+pow(pz[5],2))"
	+"+pz[4]+pz[5])/"
	+"(sqrt(pow(mass[4],2)+pow(px[4],2)+pow(py[4],2)+pow(pz[4],2))+sqrt(pow(mass[5],2)+pow(px[5],2)+pow(py[5],2)+pow(pz[5],2))"
	+"-pz[4]-pz[5]))");
}

TString genPairPseudoRapidity(){return(
	TString("-1*abs(pz[4]+pz[5])/(pz[4]+pz[5])*log(tan(")
	+"atan(abs(sqrt(pow(px[4]+px[5],2)+pow(py[4]+py[5],2))/(pz[4]+pz[5])))/2))");
}

TString genPairPseudoRapidity2(){return(
	TString("0.5*log(")
	+"(sqrt(pow(px[4]+px[5],2)+pow(py[4]+py[5],2)+pow(pz[4]+pz[5],2))"
	+"+pz[4]+pz[5])/"
	+"(sqrt(pow(px[4]+px[5],2)+pow(py[4]+py[5],2)+pow(pz[4]+pz[5],2))"
	+"-pz[4]-pz[5]))");
}

TString genPt(TString number=""){return("sqrt(pow(px"+number+",2)+pow(py"+number+",2))");}

TString genRapidity(TString number=""){return(
	TString("0.5*log(")
	+"(sqrt(pow(mass"+number+",2)+pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"+pz"+number+")/"
	+"(sqrt(pow(mass"+number+",2)+pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"-pz"+number+"))");
}

TString genPseudoRapidity(TString number=""){return(
	TString("-1*abs(pz"+number+")/(pz"+number+")*log(tan(")
	+"atan(abs(sqrt(pow(px"+number+",2)+pow(py"+number+",2))/(pz"+number+")))/2))");
}

TString genPseudoRapidity2(TString number=""){return(
	TString("0.5*log(")
	+"(sqrt(pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"+pz"+number+")/"
	+"(sqrt(pow(px"+number+",2)+pow(py"+number+",2)+pow(pz"+number+",2))"
	+"-pz"+number+"))");
}

TString recoRapidity( TString type="Tracker", TString tree="", TString mass="0.1057"){
	TString E="sqrt("+mass+"*"+mass+"+"+tree+"p"+type+"*"+tree+"p"+type+")";
	TString pz=tree+"p"+type+"*sin("+tree+"lambda"+type+")";
	return("0.5*log(("+E+"+"+pz+")/("+E+"-"+pz+"))");
}

TString recoPseudoRapidity( TString type="Tracker", TString tree=""){return(
	TString("-1*log(tan(("+tree+"lambda"+type+"+TMath::Pi()/2)/2))"));
}

TString recoPt(TString type="Tracker", TString tree=""){
	TString pt("cos("+tree+"lambda"+type+")*"+tree+"p"+type);
	return(pt);
}

TString recoPairRapidity( TString type="Tracker", TString tree="", TString mass="0.1057"){
	TString E0="sqrt("+mass+"*"+mass+"+"+tree+"p"+type+"[0]*"+tree+"p"+type+"[0])";
	TString E1="sqrt("+mass+"*"+mass+"+"+tree+"p"+type+"[1]*"+tree+"p"+type+"[1])";
	TString pz0=""+tree+"p"+type+"[0]*sin("+tree+"lambda"+type+"[0])";
	TString pz1=""+tree+"p"+type+"[1]*sin("+tree+"lambda"+type+"[1])";
	TString E="("+E0+"+"+E1+")";
	TString pz="("+pz0+"+"+pz1+")";
	return("0.5*log(("+E+"+"+pz+")/("+E+"-"+pz+"))");
}

TString recoPairPseudoRapidity( TString type="Tracker", TString tree=""){
	TString px0="("+tree+"p"+type+"[0]*cos("+tree+"lambda"+type+"[0])*cos("+tree+"phi"+type+"[0]))";
	TString px1="("+tree+"p"+type+"[1]*cos("+tree+"lambda"+type+"[1])*cos("+tree+"phi"+type+"[1]))";
	TString py0="("+tree+"p"+type+"[0]*cos("+tree+"lambda"+type+"[0])*sin("+tree+"phi"+type+"[0]))";
	TString py1="("+tree+"p"+type+"[1]*cos("+tree+"lambda"+type+"[1])*sin("+tree+"phi"+type+"[1]))";
	TString pz0="("+tree+"p"+type+"[0]*sin("+tree+"lambda"+type+"[0]))";
	TString pz1="("+tree+"p"+type+"[1]*sin("+tree+"lambda"+type+"[1]))";
	TString p="sqrt(pow(("+px0+"+"+px1+"),2)+pow(("+py0+"+"+py1+"),2)+pow(("+pz0+"+"+pz1+"),2))";
	TString pz="("+pz0+"+"+pz1+")";
	return("0.5*log(("+p+"+"+pz+")/("+p+"-"+pz+"))");
}

TString recoPairPt( TString type="Tracker", TString tree=""){
	TString px0="("+tree+"p"+type+"[0]*cos("+tree+"lambda"+type+"[0])*cos("+tree+"phi"+type+"[0]))";
	TString px1="("+tree+"p"+type+"[1]*cos("+tree+"lambda"+type+"[1])*cos("+tree+"phi"+type+"[1]))";
	TString py0="("+tree+"p"+type+"[0]*cos("+tree+"lambda"+type+"[0])*sin("+tree+"phi"+type+"[0]))";
	TString py1="("+tree+"p"+type+"[1]*cos("+tree+"lambda"+type+"[1])*sin("+tree+"phi"+type+"[1]))";
	TString pz0="("+tree+"p"+type+"[0]*sin("+tree+"lambda"+type+"[0]))";
	TString pz1="("+tree+"p"+type+"[1]*sin("+tree+"lambda"+type+"[1]))";
	TString pT="sqrt(pow(("+px0+"+"+px1+"),2)+pow(("+py0+"+"+py1+"),2))";
	return(pT);
}

float etaToLambda(float eta) {return TMath::Pi()/2-2*atan(exp(-1*eta));}
