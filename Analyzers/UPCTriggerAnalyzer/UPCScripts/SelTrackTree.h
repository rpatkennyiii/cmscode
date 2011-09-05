//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep  5 15:15:48 2011 by ROOT version 5.27/04
// from TTree TrackTree/TrackTree
// found on file: SelSkimOppositeChargeTree.root
//////////////////////////////////////////////////////////

#ifndef SelTrackTree_h
#define SelTrackTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <iostream>
#include <vector>
#include <TVectorD.h>

class SelTrackTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TTree	  *outputTree;
   TFile          *outputFile;
   Int_t           fCurrent; //!current Tree number in a TChain
   Int_t	   maxNsel;
   double 	   mass;

   // Declaration of leaf types
   Int_t           nSelTracks;
   Int_t           ndofSel[2];   //[nSelTracks]
   Double_t        chi2Sel[2];   //[nSelTracks]
   Double_t        pSel[2];   //[nSelTracks]
   Double_t        qoverpSel[2];   //[nSelTracks]
   Double_t        lambdaSel[2];   //[nSelTracks]
   Double_t        phiSel[2];   //[nSelTracks]
   Double_t        varQoverpSel[2];   //[nSelTracks]
   Double_t        varLambdaSel[2];   //[nSelTracks]
   Double_t        varPhiSel[2];   //[nSelTracks]
   Double_t        covarQoverpLambdaSel[2];   //[nSelTracks]
   Double_t        covarQoverpPhiSel[2];   //[nSelTracks]
   Double_t        covarLambdaPhiSel[2];   //[nSelTracks]
   Double_t        xSel[2];   //[nSelTracks]
   Double_t        ySel[2];   //[nSelTracks]
   Double_t        zSel[2];   //[nSelTracks]

   // Declaration of Output Leaf types
   TClonesArray	   *selLorentzVectors;

   // List of branches
   TBranch        *b_nSelTracks;   //!
   TBranch        *b_ndofSel;   //!
   TBranch        *b_chi2Sel;   //!
   TBranch        *b_pSel;   //!
   TBranch        *b_qoverpSel;   //!
   TBranch        *b_lambdaSel;   //!
   TBranch        *b_phiSel;   //!
   TBranch        *b_varQoverpSel;   //!
   TBranch        *b_varLambdaSel;   //!
   TBranch        *b_varPhiSel;   //!
   TBranch        *b_covarQoverpLambdaSel;   //!
   TBranch        *b_covarQoverpPhiSel;   //!
   TBranch        *b_covarLambdaPhiSel;   //!
   TBranch        *b_xSel;   //!
   TBranch        *b_ySel;   //!
   TBranch        *b_zSel;   //!

   SelTrackTree(TTree *tree=0, char *outFileName=0, double _mass=0);
   virtual ~SelTrackTree();
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

#ifdef SelTrackTree_cxx
SelTrackTree::SelTrackTree(TTree *tree, char *outFileName, double _mass)
{
   mass=_mass;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("SelSkimOppositeChargeTree.root");
      if (!f) {
         f = new TFile("SelSkimOppositeChargeTree.root");
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

SelTrackTree::~SelTrackTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SelTrackTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SelTrackTree::LoadTree(Long64_t entry)
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

void SelTrackTree::Init(TTree *tree)
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

   fChain->SetBranchAddress("nSelTracks", &nSelTracks, &b_nSelTracks);
   fChain->SetBranchAddress("ndofSel", ndofSel, &b_ndofSel);
   fChain->SetBranchAddress("chi2Sel", chi2Sel, &b_chi2Sel);
   fChain->SetBranchAddress("pSel", pSel, &b_pSel);
   fChain->SetBranchAddress("qoverpSel", qoverpSel, &b_qoverpSel);
   fChain->SetBranchAddress("lambdaSel", lambdaSel, &b_lambdaSel);
   fChain->SetBranchAddress("phiSel", phiSel, &b_phiSel);
   fChain->SetBranchAddress("varQoverpSel", varQoverpSel, &b_varQoverpSel);
   fChain->SetBranchAddress("varLambdaSel", varLambdaSel, &b_varLambdaSel);
   fChain->SetBranchAddress("varPhiSel", varPhiSel, &b_varPhiSel);
   fChain->SetBranchAddress("covarQoverpLambdaSel", covarQoverpLambdaSel, &b_covarQoverpLambdaSel);
   fChain->SetBranchAddress("covarQoverpPhiSel", covarQoverpPhiSel, &b_covarQoverpPhiSel);
   fChain->SetBranchAddress("covarLambdaPhiSel", covarLambdaPhiSel, &b_covarLambdaPhiSel);
   fChain->SetBranchAddress("xSel", xSel, &b_xSel);
   fChain->SetBranchAddress("ySel", ySel, &b_ySel);
   fChain->SetBranchAddress("zSel", zSel, &b_zSel);
   Notify();
}

void SelTrackTree::StoreMaxTracks()
{
   fChain->Draw("nSelTracks","","goff");

   int nentries=fChain->GetEntries();
   double *nseltracks=fChain->GetV1();

   TVectorT<double> ntemp1(nentries,nseltracks);

   maxNsel=int(ntemp1.Max());
}

void SelTrackTree::InitOutput(){
   outputTree = new TTree("LorentzTree","LorentzTree"); 

   StoreMaxTracks();
   
   selLorentzVectors=new TClonesArray("TLorentzVector",maxNsel); 
   outputTree->Branch("sel4vec","TClonesArray",&selLorentzVectors,32000,0);

   Notify();
}

Bool_t SelTrackTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void SelTrackTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SelTrackTree::Cut(Long64_t entry)
{
   if(entry);
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef SelTrackTree_cxx
