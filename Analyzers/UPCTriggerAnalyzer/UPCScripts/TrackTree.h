//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Aug 29 11:27:34 2011 by ROOT version 5.27/04
// from TTree TrackTree/TrackTree
// found on file: UPCTree.root
//////////////////////////////////////////////////////////

#ifndef TrackTree_h
#define TrackTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <iostream>
#include <vector>
#include <TVectorD.h>

class TrackTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TTree	  *outputTree;
   TFile          *outputFile;
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t	   maxNsel;
   Int_t           maxNgp;
   Int_t           nGloPrimTracks;
   Int_t           ndofGloPrim[2045];   //[nGloPrimTracks]
   Double_t        chi2GloPrim[2045];   //[nGloPrimTracks]
   Double_t        pGloPrim[2045];   //[nGloPrimTracks]
   Double_t        qoverpGloPrim[2045];   //[nGloPrimTracks]
   Double_t        lambdaGloPrim[2045];   //[nGloPrimTracks]
   Double_t        phiGloPrim[2045];   //[nGloPrimTracks]
   Double_t        varQoverpGloPrim[2045];   //[nGloPrimTracks]
   Double_t        varLambdaGloPrim[2045];   //[nGloPrimTracks]
   Double_t        varPhiGloPrim[2045];   //[nGloPrimTracks]
   Double_t        covarQoverpLambdaGloPrim[2045];   //[nGloPrimTracks]
   Double_t        covarQoverpPhiGloPrim[2045];   //[nGloPrimTracks]
   Double_t        covarLambdaPhiGloPrim[2045];   //[nGloPrimTracks]
   Double_t        xGloPrim[2045];   //[nGloPrimTracks]
   Double_t        yGloPrim[2045];   //[nGloPrimTracks]
   Double_t        zGloPrim[2045];   //[nGloPrimTracks]
   Int_t           nSelTracks;
   Int_t           ndofSel[1549];   //[nSelTracks]
   Double_t        chi2Sel[1549];   //[nSelTracks]
   Double_t        pSel[1549];   //[nSelTracks]
   Double_t        qoverpSel[1549];   //[nSelTracks]
   Double_t        lambdaSel[1549];   //[nSelTracks]
   Double_t        phiSel[1549];   //[nSelTracks]
   Double_t        varQoverpSel[1549];   //[nSelTracks]
   Double_t        varLambdaSel[1549];   //[nSelTracks]
   Double_t        varPhiSel[1549];   //[nSelTracks]
   Double_t        covarQoverpLambdaSel[1549];   //[nSelTracks]
   Double_t        covarQoverpPhiSel[1549];   //[nSelTracks]
   Double_t        covarLambdaPhiSel[1549];   //[nSelTracks]
   Double_t        xSel[1549];   //[nSelTracks]
   Double_t        ySel[1549];   //[nSelTracks]
   Double_t        zSel[1549];   //[nSelTracks]

   // Declaration of Output Leaf types
   TClonesArray	   *selLorentzVectors;
   TClonesArray	   *gpLorentzVectors;
    
   double mass;
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
   TBranch        *b_sel4vec;
   TBranch        *b_gp4vec;

   TrackTree(TTree *tree=0, char *outFileName=0, double _mass=0);
   virtual ~TrackTree();
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

#ifdef TrackTree_cxx
TrackTree::TrackTree(TTree *tree, char* outFileName, double _mass)
{
   mass=_mass;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("UPCTree.root");
      if (!f) {
         f = new TFile("UPCTree.root");
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

TrackTree::~TrackTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TrackTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t TrackTree::LoadTree(Long64_t entry)
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

void TrackTree::Init(TTree *tree)
{
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

void TrackTree::StoreMaxTracks()
{
   fChain->Draw("nSelTracks:nGloPrimTracks","","goff");

   int nentries=fChain->GetEntries();
   double *nseltracks=fChain->GetV1();
   double *ngptracks=fChain->GetV2();

   TVectorT<double> ntemp1(nentries,nseltracks);
   TVectorT<double> ntemp2(nentries,ngptracks);

   maxNsel=int(ntemp1.Max());
   maxNgp=int(ntemp2.Max());
}

void TrackTree::InitOutput(){
   outputTree = new TTree("LorentzTree","LorentzTree"); 

   StoreMaxTracks();
   
   selLorentzVectors=new TClonesArray("TLorentzVector",maxNsel); 
   gpLorentzVectors=new TClonesArray("TLorentzVector",maxNgp);

   outputTree->Branch("sel4vec","TClonesArray",&selLorentzVectors,32000,0);
   outputTree->Branch("gp4vec","TClonesArray",&gpLorentzVectors,32000,0);

   Notify();
}

Bool_t TrackTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TrackTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t TrackTree::Cut(Long64_t entry)
{
   if(entry==2);
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

#endif // #ifdef TrackTree_cxx
