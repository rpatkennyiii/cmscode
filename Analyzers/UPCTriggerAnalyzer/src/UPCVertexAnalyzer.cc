#include "Analyzers/UPCTriggerAnalyzer/interface/UPCVertexAnalyzer.h"

using namespace edm; 

UPCVertexAnalyzer::UPCVertexAnalyzer(const edm::ParameterSet& iConfig):vertexCName(iConfig.getParameter<string>("vertexCollection")){}

UPCVertexAnalyzer::~UPCVertexAnalyzer(){}

void UPCVertexAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();

	string tName(vertexCName+"Tree");	
        VertTree = new TTree(tName.c_str(),tName.c_str()); 
	VertTree->Branch("n",&nVert,"n/I");
	VertTree->Branch("x",x,"x[n]/F");
	VertTree->Branch("y",y,"y[n]/F");
	VertTree->Branch("z",z,"z[n]/F");
	VertTree->Branch("xError",xError,"xError[n]/F");
	VertTree->Branch("yError",yError,"yError[n]/F");
	VertTree->Branch("zError",zError,"zError[n]/F");
	VertTree->Branch("isFake",isFake,"isFake[n]/O");
	VertTree->Branch("nTracks",nTracks,"nTracks[n]/i");
}

void UPCVertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<VertexCollection> vertexCollection;

	iEvent.getByLabel(vertexCName.c_str(),vertexCollection);
	const VertexCollection * vertices = vertexCollection.product();
	
	nVert=vertices->size()<=10?vertices->size():10;
	getVertexData(vertices,x,xError,y,yError,z,zError,isFake,nTracks);
	VertTree->Fill();
}

void UPCVertexAnalyzer::getVertexData(const VertexCollection *vert, float* x,float* xError,float* y,float* yError,float* z,float* zError, bool* fake, int* nTks){
	for(unsigned int i=0;i<vert->size()&&i<10;i++){
	   x[i]=vert->at(i).x();
	   xError[i]=vert->at(i).xError();
	   y[i]=vert->at(i).y();
	   yError[i]=vert->at(i).yError();
	   z[i]=vert->at(i).z();
	   zError[i]=vert->at(i).zError();
	   fake[i]=vert->at(i).isFake();	
	   nTks[i]=vert->at(i).nTracks(0);
	}
}
