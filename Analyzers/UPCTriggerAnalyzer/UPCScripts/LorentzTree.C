#define LorentzTree_cxx
// The class definition in LorentzTree.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("LorentzTree.C")
// Root > T->Process("LorentzTree.C","some options")
// Root > T->Process("LorentzTree.C+")
//

#include "LorentzTree.h"
#include <TH2.h>
#include <TStyle.h>


void LorentzTree::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   gpPairInvMass = new TH1F(("GP"+option).Data(),("GP"+option).Data(),500,0,20);
   selPairInvMass = new TH1F(("Sel"+option).Data(),("Sel"+option).Data(),500,0,20);
}

void LorentzTree::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t LorentzTree::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either LorentzTree::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   GetEntry(entry);
   if (fChain->GetBranch("gp4vec")){
      TLorentzVector *Gp0=(*gp4vec)[0], *Gp1=(*gp4vec)[1];
      TLorentzVector pairGp=((*Gp0)+(*Gp1));
      gpPairInvMass->Fill(pairGp.M());
   }

   if (fChain->GetBranch("sel4vec")){
      TLorentzVector *Sel0=(*sel4vec)[0], *Sel1=(*sel4vec)[1];
      TLorentzVector pairSel=((*Sel0)+(*Sel1));
      selPairInvMass->Fill(pairSel.M());
   }

   return kTRUE;
}

void LorentzTree::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void LorentzTree::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   if (fChain->GetBranch("gp4vec")){ 
      gpPairInvMass->Draw();

      if (fChain->GetBranch("sel4vec")){
         selPairInvMass->SetLineColor(2);
         selPairInvMass->Draw("same");
      }

    }else{
       selPairInvMass->Draw();
    }      
}
