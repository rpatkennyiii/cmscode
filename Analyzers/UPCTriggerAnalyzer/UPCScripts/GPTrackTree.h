//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep  5 15:16:47 2011 by ROOT version 5.27/04
// from TTree TrackTree/TrackTree
// found on file: GPSkimOppositeChargeTree.root
//////////////////////////////////////////////////////////

#ifndef GPTrackTree_h
#define GPTrackTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <iostream>
#include <vector>
#include <TVectorD.h>

class GPTrackTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TTree	  *outputTree;
   TFile          *outputFile;
   Int_t           fCurrent; //!current Tree number in a TChain
   Int_t	   maxNgp;
   double 	   mass;

   // Declaration of leaf types
   Int_t           nGloPrimTracks;
   Int_t           ndofGloPrim[2];   //[nGloPrimTracks]
   Double_t        chi2GloPrim[2];   //[nGloPrimTracks]
   Double_t        pGloPrim[2];   //[nGloPrimTracks]
   Double_t        qoverpGloPrim[2];   //[nGloPrimTracks]
   Double_t        lambdaGloPrim[2];   //[nGloPrimTracks]
   Double_t        phiGloPrim[2];   //[nGloPrimTracks]
   Double_t        varQoverpGloPrim[2];   //[nGloPrimTracks]
   Double_t        varLambdaGloPrim[2];   //[nGloPrimTracks]
   Double_t        varPhiGloPrim[2];   //[nGloPrimTracks]
   Double_t        covarQoverpLambdaGloPrim[2];   //[nGloPrimTracks]
   Double_t        covarQoverpPhiGloPrim[2];   //[nGloPrimTracks]
   Double_t        covarLambdaPhiGloPrim[2];   //[nGloPrimTracks]
   Double_t        xGloPrim[2];   //[nGloPrimTracks]
   Double_t        yGloPrim[2];   //[nGloPrimTracks]
   Double_t        zGloPrim[2];   //[nGloPrimTracks]

   // Declaration of Output Leaf types
   TClonesArray	   *gpLorentzVectors;

   // List of branches
   TBranch        *b_nGloPrimTracks;   //!
   TBranch        *b_ndofGloPrim;   //!
   TBranch        *b_chi2GloPrim;   //!
   TBranch        *b_pGloPrim;   //!
   TBranch        *b_qoverpGloPrim;   //!
   TBranch        *b_lambdaGloPrim;   //!
   TBranch        *b_phiGloPrim;   //!
   TBranch        *b_varQoverpGloPrim;   //!
   TBranch        *b_varLambdaGloPrim;   //!
   TBranch        *b_varPhiGloPrim;   //!
   TBranch        *b_covarQoverpLambdaGloPrim;   //!
   TBranch        *b_covarQoverpPhiGloPrim;   //!
   TBranch        *b_covarLambdaPhiGloPrim;   //!
   TBranch        *b_xGloPrim;   //!
   TBranch        *b_yGloPrim;   //!
   TBranch        *b_zGloPrim;   //!

   GPTrackTree(TTree *tree=0, char *outFileName=0, double _mass=0);
   virtual ~GPTrackTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     InitOutput();
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual void     SubLoop(Int_t nentries, TClonesArray *track4vec, double *p, double *lambda, double *phi);
   virtual void     StoreMaxTracks();
};

#endif

#ifdef GPTrackTree_cxx
GPTrackTree::GPTrackTree(TTree *tree, char *outFileName, double _mass)
{
   mass=_mass;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("GPSkimOppositeChargeTree.root");
      if (!f) {
         f = new TFile("GPSkimOppositeChargeTree.root");
      }
      tree = (TTree*)gDirectory->Get("TrackTree");
   }

   if (outFileName == 0){
      outputFile = new TFile("UPCLorentzTree.root","RECREATE");
   }else{
      outputFile = new TFile(outFileName,"RECREATE");
   }

   Init(tree);
   InitOutput();
}

GPTrackTree::~GPTrackTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t GPTrackTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t GPTrackTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void GPTrackTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nGloPrimTracks", &nGloPrimTracks, &b_nGloPrimTracks);
   fChain->SetBranchAddress("ndofGloPrim", ndofGloPrim, &b_ndofGloPrim);
   fChain->SetBranchAddress("chi2GloPrim", chi2GloPrim, &b_chi2GloPrim);
   fChain->SetBranchAddress("pGloPrim", pGloPrim, &b_pGloPrim);
   fChain->SetBranchAddress("qoverpGloPrim", qoverpGloPrim, &b_qoverpGloPrim);
   fChain->SetBranchAddress("lambdaGloPrim", lambdaGloPrim, &b_lambdaGloPrim);
   fChain->SetBranchAddress("phiGloPrim", phiGloPrim, &b_phiGloPrim);
   fChain->SetBranchAddress("varQoverpGloPrim", varQoverpGloPrim, &b_varQoverpGloPrim);
   fChain->SetBranchAddress("varLambdaGloPrim", varLambdaGloPrim, &b_varLambdaGloPrim);
   fChain->SetBranchAddress("varPhiGloPrim", varPhiGloPrim, &b_varPhiGloPrim);
   fChain->SetBranchAddress("covarQoverpLambdaGloPrim", covarQoverpLambdaGloPrim, &b_covarQoverpLambdaGloPrim);
   fChain->SetBranchAddress("covarQoverpPhiGloPrim", covarQoverpPhiGloPrim, &b_covarQoverpPhiGloPrim);
   fChain->SetBranchAddress("covarLambdaPhiGloPrim", covarLambdaPhiGloPrim, &b_covarLambdaPhiGloPrim);
   fChain->SetBranchAddress("xGloPrim", xGloPrim, &b_xGloPrim);
   fChain->SetBranchAddress("yGloPrim", yGloPrim, &b_yGloPrim);
   fChain->SetBranchAddress("zGloPrim", zGloPrim, &b_zGloPrim);
   Notify();
}

void GPTrackTree::StoreMaxTracks()
{
   fChain->Draw("nGloPrimTracks","","goff");

   int nentries=fChain->GetEntries();
   double *ngptracks=fChain->GetV1();

   TVectorT<double> ntemp1(nentries,ngptracks);

   maxNgp=int(ntemp1.Max());
}

void GPTrackTree::InitOutput(){
   outputTree = new TTree("LorentzTree","LorentzTree"); 

   StoreMaxTracks();
   
   gpLorentzVectors = new TClonesArray("TLorentzVector",maxNgp); 
   outputTree->Branch("gp4vec","TClonesArray",&gpLorentzVectors,32000,0);

   Notify();
}

Bool_t GPTrackTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void GPTrackTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t GPTrackTree::Cut(Long64_t entry)
{
   if(entry);
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef GPTrackTree_cxx
