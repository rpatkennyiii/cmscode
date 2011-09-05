{
	TChain * chain = new TChain("TrackTree");
	chain->Add("UPCTreeFiles/UPCTree_*.root");

	int gpTracks;
	double qoverp[5000];

	chain->SetBranchAddress("nGloPrimTracks",&gpTracks);
	chain->SetBranchAddress("qoverpGloPrim",qoverp);

	TFile *skimfile[3]; 
	TTree *tree[3];

	chain->LoadTree(0); //force 1st tree to be loaded

	string name[3]={"OppositeChargeTree","SameChargeTree","MannyTracksTree"};

	Int_t nevents = (Int_t)chain->GetEntries();

	for(int j=0;j<3;j++)
	{
		skimfile[j] = new TFile(("Skim"+name[j]+".root").c_str(),"recreate");
		//TTree *tree=chain->GetTree()->CloneTree(0);
		tree[j]=chain->GetTree()->CloneTree(0);

		for(int ievent=0; ievent<nevents; ievent++)
		{
		  chain->GetEntry(ievent);

		  if((gpTracks == 2) && (j==0)){
			if(qoverp[0]/qoverp[1] < 0){ 
				tree[j]->Fill();
			}
		  }
		  
		  if((gpTracks == 2) && (j==1)){
			if(qoverp[0]/qoverp[1] > 0){
				tree[j]->Fill();
			}
		  }

		  if((gpTracks > 1) && (j==2)){
			  tree[j]->Fill();
		  }
		}
		tree[j]->Write();
		skimfile[j]->Close();
	}
}
