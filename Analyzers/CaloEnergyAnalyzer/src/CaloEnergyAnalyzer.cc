#include "Analyzers/CaloEnergyAnalyzer/interface/CaloEnergyAnalyzer.h"

using namespace edm;

CaloEnergyAnalyzer::CaloEnergyAnalyzer(const ParameterSet& iConfig):
	etaBinSize(iConfig.getParameter<bool>("etaBinSize")),
	HFCorrection(iConfig.getParameter<bool>("HFCorrection")),
	realHFbins(iConfig.getParameter<bool>("realHFbins")),
	noiseCut(iConfig.getParameter<bool>("noiseCut"))
{
	if(HFCorrection){
		float tHFcor[11]={0.982,0.978,0.974,0.969,0.963,0.956
				,0.948,0.938,0.926,0.912,0.895};

		for(int i=0;i<11;i++){HFcor[i]=tHFcor[i];}
	}

	
	if(etaBinSize){	
		float tetabin[83]={-5.191,-4.889,-4.716,-4.538,-4.363,-4.191,-4.013
		   ,-3.839,-3.664,-3.489,-3.314,-3.139,-2.964,-2.853
		   ,-2.650,-2.500,-2.322,-2.172,-2.043,-1.930,-1.830
		   ,-1.740,-1.653,-1.566,-1.479,-1.392,-1.305,-1.218
		   ,-1.131,-1.044,-0.957,-0.879,-0.783,-0.696,-0.609
		   ,-0.522,-0.435,-0.348,-0.261,-0.174,-0.087,0.000
		   ,0.087,0.174,0.261,0.348,0.435,0.522,0.609,0.696
		   ,0.783,0.879,0.957,1.044,1.131,1.218,1.305,1.392
		   ,1.479,1.566,1.653,1.740,1.830,1.930,2.043,2.172
		   ,2.322,2.500,2.650,2.853,2.964,3.139,3.314,3.489
		   ,3.664,3.839,4.013,4.191,4.363,4.538,4.716,4.889
		   ,5.191}; 
	
		if(realHFbins){
			float xBins[14] = {2.866,2.976,3.152,3.327,3.503,3.677,3.853,4.027,4.204,4.377,4.552,4.730,4.903,5.205};
			
			for(int i=0;i<14;i++){
				tetabin[i+83-14]=xBins[i];
				tetabin[13-i]=-tetabin[i+83-14];
			}
		}

		for(int i=0;i<83;i++){etabin[i]=tetabin[i];}

	}else{for(int i=0; i<43;i++){etabin[i]=-5.25+i*.25;}}
}


CaloEnergyAnalyzer::~CaloEnergyAnalyzer(){}


void CaloEnergyAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();

	int nbins=etaBinSize?83:43;
	TVectorT<float> EtaBins(nbins,etabin);
	mFileServer->file().WriteTObject(&EtaBins,"EtaBins");
	
	RunTree = new TTree("RunTree","Run Tree");
	CenTree = new TTree("CentTree","Centrality Tree");
	CaloTree = new TTree("CaloTree","Calo Tree");
	VertTree = new TTree("VertTree","Vertex Tree");
		
	RunTree->Branch("BunchXing",&RunData[0],"BunchXing/I");
	RunTree->Branch("LumiBlock",&RunData[1],"LumiBlock/I");
	RunTree->Branch("Event",&RunData[2],"Event/I");
	RunTree->Branch("Run",&RunData[3],"Run/I");

	CenTree->Branch("CentralityNpart",&cent[0],"NpartMean/F");
	CenTree->Branch("CentralityValue",&cent[1],"centralityValue/F");
	CenTree->Branch("CentralityBin",&centi,"Bin/I");

//	CaloTree->Branch("CaloSize",&CaloSize);
//	CaloTree->Branch("CaloEmEnergy",&CaloEmEnergy,"CaloEmEnergy[CaloSize]/F");
//	CaloTree->Branch("CaloHadEnergy",&CaloHadEnergy,"CaloHadEnergy[CaloSize]/F");
//	CaloTree->Branch("CaloEta",&CaloEta,"CaloEta[CaloSize]/F");
//	CaloTree->Branch("CaloPhi",&CaloPhi,"CaloPhi[CaloSize]/F");

	if(etaBinSize){
		CaloTree->Branch("CalodEtdEta",&CalodEtdEta,"CalodEtdEta[82]/F");
	}else{
		CaloTree->Branch("CalodEtdEta",&CalodEtdEta,"CalodEtdEta[42]/F");
	}

	VertTree->Branch("VertSel",VertData);
	VertTree->Branch("VertSelErr",&VertData[1]);
	VertTree->Branch("VertAdp",&VertData[2]);
	VertTree->Branch("VertAdpErr",&VertData[3]);
//	VertTree->Branch("VertMed",&VertData[4]);
//	VertTree->Branch("VertMedErr",&VertData[5]);
}


void CaloEnergyAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	CentProv = new CentralityProvider(iSetup);
	CentProv->newEvent(iEvent,iSetup);
	
	RunData[0]=iEvent.bunchCrossing();
	RunData[1]=iEvent.id().luminosityBlock();
	RunData[2]=iEvent.id().event();
	RunData[3]=iEvent.id().run();

	cent[0]=CentProv->NpartMean();
	cent[1]=CentProv->centralityValue();
	centi=CentProv->getBin();
	
	RunTree->Fill();
	CenTree->Fill();

	Handle<CaloTowerCollection> calotower;
	Handle<VertexCollection> vertexCollection3;
	Handle<VertexCollection> vertexCollection2;
//	Handle<VertexCollection> vertexCollection;

	iEvent.getByType(calotower);
	iEvent.getByLabel("hiSelectedVertex",vertexCollection3);
	iEvent.getByLabel("hiPixelAdaptiveVertex",vertexCollection2);
//	iEvent.getByLabel("hiPixelMedianVertex",vertexCollection);
	
	const VertexCollection * vertices3 = vertexCollection3.product();
	const VertexCollection * vertices2 = vertexCollection2.product();
//	const VertexCollection * vertices = vertexCollection.product();

//	typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double> > Point;
	const math::XYZPoint pv = vertices3->begin()->position();
//	cout << getVertexData(vertices,&VertData[4]) << endl;

	if(getVertexData(vertices3,VertData)||getVertexData(vertices2,&VertData[2])){VertTree->Fill();}
//	vector<float> cee;
//	vector<float> ceh;
//	vector<float> ceta;
//	vector<float> cphi;

	if(!calotower.failedToGet()){
		for(int j=0;j<82;j++){CalodEtdEta[j]=0;}

		for(CaloTowerCollection::const_iterator calt=(&*calotower)->begin();calt!=(&*calotower)->end();calt++){
			float cor=((abs(calt->ieta())>=30)&&(abs(calt->ieta())<=40)&&(HFCorrection))?(1/HFcor[abs(calt->ieta())-30]):1;

//			cee.push_back(calt->emEnergy()*cor);
//			ceh.push_back(calt->hadEnergy());
//			ceta.push_back(calt->eta());
//			cphi.push_back(calt->phi());

			for(int k=0;k<(etaBinSize?82:42);k++){
				if(calt->p4(pv).eta()>etabin[k]&&calt->p4(pv).eta()<=etabin[k+1]){

					if(!noiseCut||calt->energy()>4){//CalodEtdEta[k]+=calt->energy()*cor;}
						CalodEtdEta[k]+=calt->et(pv)*cor/(etabin[k+1]-etabin[k]);
					}

					k=(etaBinSize?82:42);
				}
			}
		}
	}


//	CaloSize=cee.size();

//	CaloTree->SetBranchAddress("CaloEmEnergy",&cee[0]);
//	CaloTree->SetBranchAddress("CaloHadEnergy",&ceh[0]);
//	CaloTree->SetBranchAddress("CaloEta",&ceta[0]);
//	CaloTree->SetBranchAddress("CaloPhi",&cphi[0]);

	CaloTree->Fill();

//	cee.clear();
//	ceh.clear();
//	ceta.clear();
//	cphi.clear();

//	CaloTowerz=&*calotower;
}


bool CaloEnergyAnalyzer::getVertexData(const VertexCollection *vert, float* vdata){
	if(vert->size()>0) {
		vdata[0]=vert->begin()->z();
		vdata[1]=vert->begin()->zError();
		return(vert->size());
	}else{return(-999.9);}
}
