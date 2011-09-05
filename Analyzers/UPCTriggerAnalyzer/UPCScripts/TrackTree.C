#define TrackTree_cxx
#include "TrackTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void TrackTree::Loop()
{
   if (fChain == 0) return;
   

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      selLorentzVectors=new TClonesArray("TLorentzVector", nSelTracks);
      gpLorentzVectors=new TClonesArray("TLorentzVector",nGloPrimTracks);

      outputTree->SetBranchAddress("sel4vec",&selLorentzVectors);
      outputTree->SetBranchAddress("gp4vec",&gpLorentzVectors);

      SubLoop(nSelTracks,selLorentzVectors,pSel,lambdaSel,phiSel);

      SubLoop(nGloPrimTracks,gpLorentzVectors,pGloPrim,lambdaGloPrim,phiGloPrim);
       
      outputTree->Fill();
   }
  outputTree->Write();
  outputFile->Close();
}

void TrackTree::SubLoop(Int_t nentries, TClonesArray *track4vec, double *p, double *lambda, double *phi) 
{
   double px,py,pz,E,m=mass; //muon mass in GeV
   for (Long64_t entry=0; entry<nentries;entry++) {
      px=p[entry]*cos(lambda[entry])*cos(phi[entry]);
      py=p[entry]*cos(lambda[entry])*sin(phi[entry]);
      pz=p[entry]*sin(lambda[entry]);
      E=sqrt(m*m+p[entry]*p[entry]);

      new ((*track4vec)[entry]) TLorentzVector(px,py,pz,E);
   }
}
