{
	TChain * chain = new TChain("TrackTree");
	chain->Add("~/UPCTreeFiles/UPCTree_*.root");

	int Tracks;
	double qoverp[5000];

	TFile *skimfile[6]; 
	TTree *tree[6];

	chain->LoadTree(0); //force 1st tree to be loaded

	string name[3]={"OppositeChargeTree","SameChargeTree","MannyTracksTree"};
	string col[2]={"GP","Sel"};

	Int_t nevents = (Int_t)chain->GetEntries();

	for(int k=0; k<2; k++){	
		chain->SetBranchStatus("*",0);

		if(k==0){
 			chain->SetBranchStatus("*GloPrim*",1);
			chain->SetBranchAddress("nGloPrimTracks",&Tracks);
			chain->SetBranchAddress("qoverpGloPrim",qoverp);
		}
		
		if(k==1){
			chain->SetBranchStatus("*Sel*",1);
			chain->SetBranchAddress("nSelTracks",&Tracks);
			chain->SetBranchAddress("qoverpSel",qoverp);
		}

		for(int j=0;j<3;j++){
			skimfile[k*3+j] = new TFile((col[k]+"Skim"+name[j]+".root").c_str(),"recreate");
			tree[k*3+j]=chain->GetTree()->CloneTree(0);

			for(int ievent=0; ievent<nevents; ievent++)
			{
			  chain->GetEntry(ievent);

			  if((Tracks == 2) && (j==0)){
				if(qoverp[0]/qoverp[1] < 0){ 
					tree[k*3+j]->Fill();
				}
			  }
			  
			  if((Tracks == 2) && (j==1)){
				if(qoverp[0]/qoverp[1] > 0){
					tree[k*3+j]->Fill();
				}
			  }

			  if((Tracks > 1) && (j==2)){
				  tree[k*3+j]->Fill();
			  }
			}
			tree[k*3+j]->Write();
			skimfile[k*3+j]->Close();
		}
	}
}
