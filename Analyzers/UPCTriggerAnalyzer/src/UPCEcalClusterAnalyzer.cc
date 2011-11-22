#include "Analyzers/UPCTriggerAnalyzer/interface/UPCEcalClusterAnalyzer.h"

using namespace edm;
using namespace std;
using namespace reco;

UPCEcalClusterAnalyzer::UPCEcalClusterAnalyzer(const ParameterSet& iConfig):
	EcalColName(iConfig.getParameter<string>("ecalClusterCollection")){}

UPCEcalClusterAnalyzer::~UPCEcalClusterAnalyzer(){}

void UPCEcalClusterAnalyzer::beginJob(){
	mFileServer->file().SetCompressionLevel(9);
	mFileServer->file().cd();
	
	string EcalSecName;
	if (EcalColName.find("Barrel") < EcalColName.size()){EcalSecName="Barrel";}
	else if (EcalColName.find("Endcap") < EcalColName.size()){EcalSecName="Endcap";}
	else {EcalSecName="";}

   	UPCEcalClstTree = new TTree(("UPC"+EcalSecName+"ClusterTree").c_str(),("UPC"+EcalSecName+"ClusterTree").c_str());
	UPCEcalClstTree->Branch(("n"+EcalSecName+"Cluster").c_str(),&nEcalClst,("n"+EcalSecName+"Cluster/i").c_str());
	UPCEcalClstTree->Branch((EcalSecName+"ClusterEnergy").c_str(),EcalClstEnergy,(EcalSecName+"ClusterEnergy[n"+EcalSecName+"Cluster]/F").c_str());
	UPCEcalClstTree->Branch((EcalSecName+"ClusterX").c_str(),EcalClstX,(EcalSecName+"ClusterX[n"+EcalSecName+"Cluster]/F").c_str());
	UPCEcalClstTree->Branch((EcalSecName+"ClusterY").c_str(),EcalClstY,(EcalSecName+"ClusterY[n"+EcalSecName+"Cluster]/F").c_str());
	UPCEcalClstTree->Branch((EcalSecName+"ClusterZ").c_str(),EcalClstZ,(EcalSecName+"ClusterZ[n"+EcalSecName+"Cluster]/F").c_str());
	UPCEcalClstTree->Branch((EcalSecName+"ClusterPhiWidth").c_str(),EcalClstPhiWidth,(EcalSecName+"ClusterPhiWidth[n"+EcalSecName+"Cluster]/F").c_str());
	UPCEcalClstTree->Branch((EcalSecName+"ClusterEtaWidth").c_str(),EcalClstEtaWidth,(EcalSecName+"ClusterEtaWidth[n"+EcalSecName+"Cluster]/F").c_str());
}

void UPCEcalClusterAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
	nEcalClst=0;	
	Handle<vector<SuperCluster> > ecal_supClsts_h;

	iEvent.getByLabel("islandSuperClusters",EcalColName,ecal_supClsts_h);
	const vector<SuperCluster> *ecal_supClsts = ecal_supClsts_h.failedToGet()? 0 : &*ecal_supClsts_h;
	
	if(ecal_supClsts){
		nEcalClst=ecal_supClsts->size();

		for(unsigned int i=0; i<ecal_supClsts->size();i++){
			EcalClstEnergy[i]=(*ecal_supClsts)[i].energy();
			EcalClstX[i]=((*ecal_supClsts)[i].position()).X();
			EcalClstY[i]=((*ecal_supClsts)[i].position()).Y();
			EcalClstZ[i]=((*ecal_supClsts)[i].position()).Z();
			EcalClstPhiWidth[i]=(*ecal_supClsts)[i].phiWidth();
			EcalClstEtaWidth[i]=(*ecal_supClsts)[i].etaWidth();
		}
	}

	UPCEcalClstTree->Fill();
}
