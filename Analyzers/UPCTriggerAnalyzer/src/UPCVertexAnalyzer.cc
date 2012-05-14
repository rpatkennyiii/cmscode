#include "Analyzers/UPCTriggerAnalyzer/interface/UPCVertexAnalyzer.h"

using namespace edm; 

UPCVertexAnalyzer::UPCVertexAnalyzer(const edm::ParameterSet& iConfig):vertexCName(iConfig.getParameter<string>("vertexCollection")){}

UPCVertexAnalyzer::~UPCVertexAnalyzer(){}

void UPCVertexAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();

	string tName(vertexCName+"Tree");	
        VertTree = new TTree(tName.c_str(),tName.c_str()); 
	VertTree->Branch("VertData",VertData,"pVx:pVxError:pVy:pVyError:pVz:pVzError");
	VertTree->Branch("isFake",&isFake,"isFake/O");
	VertTree->Branch("nTracks",&nTracks,"nTracks/i");
}

void UPCVertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<VertexCollection> vertexCollection;

	iEvent.getByLabel(vertexCName.c_str(),vertexCollection);
	const VertexCollection * vertices = vertexCollection.product();

	getVertexData(vertices,VertData);
	isFake=vertices->begin()->isFake();	
	nTracks=vertices->begin()->nTracks(0);
	
	VertTree->Fill();
}

void UPCVertexAnalyzer::getVertexData(const VertexCollection *vert, float* vdata){
	vdata[0]=vert->begin()->x();
	vdata[1]=vert->begin()->xError();
	vdata[2]=vert->begin()->y();
	vdata[3]=vert->begin()->yError();
	vdata[4]=vert->begin()->z();
	vdata[5]=vert->begin()->zError();
}
