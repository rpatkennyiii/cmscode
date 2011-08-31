#include "Analyzers/UPCTriggerAnalyzer/interface/UPCVertexAnalyzer.h"

using namespace edm;

UPCVertexAnalyzer::UPCVertexAnalyzer(const edm::ParameterSet& iConfig):vertexCName(iConfig.getParameter<string>("vertexCollection")){}

UPCVertexAnalyzer::~UPCVertexAnalyzer(){}

void UPCVertexAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();

	string tName(vertexCName+"Tree");	
        VertTree = new TTree(tName.c_str(),tName.c_str()); 
	VertTree->Branch("VertSel",VertData,"pVx:pVxError:pVy:pVyError:pVz:pVzError");
}

void UPCVertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	Handle<VertexCollection> vertexCollection;

	iEvent.getByLabel(vertexCName.c_str(),vertexCollection);
	const VertexCollection * vertices = vertexCollection.product();

	if(getVertexData(vertices,VertData)){VertTree->Fill();}

	VertTree->Fill();
}

bool UPCVertexAnalyzer::getVertexData(const VertexCollection *vert, float* vdata){
	if(vert->size()>0) {
		vdata[0]=vert->begin()->x();
		vdata[1]=vert->begin()->xError();
		vdata[2]=vert->begin()->y();
		vdata[3]=vert->begin()->yError();
		vdata[4]=vert->begin()->z();
		vdata[5]=vert->begin()->zError();
		return(vert->size());
	}else{return(-999.9);}
}
