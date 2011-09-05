//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep  2 14:27:53 2011 by ROOT version 5.27/04
// from TTree LorentzTree/LorentzTree
// found on file: UPCLorentzTree.root
//////////////////////////////////////////////////////////

#ifndef LorentzTree_h
#define LorentzTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include <TClonesArray.h>

class LorentzTree : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   TClonesArray    *sel4vec;
   TClonesArray    *gp4vec;

   // List of branches
   TBranch        *b_sel4vec;   //!
   TBranch        *b_gp4vec;   //!

   LorentzTree(TTree * /*tree*/ =0) { }
   virtual ~LorentzTree() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   TLorentzVector  muPair;
   TH1F 	  *muPairInvMass;

   ClassDef(LorentzTree,0);
};

#endif

#ifdef LorentzTree_cxx
void LorentzTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   sel4vec = 0;
   gp4vec = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("sel4vec", &sel4vec, &b_sel4vec);
   fChain->SetBranchAddress("gp4vec", &gp4vec, &b_gp4vec);
}

Bool_t LorentzTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef LorentzTree_cxx
