//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Aug  5 22:59:33 2015 by ROOT version 5.34/26
// from TTree EventTree/ EventTree
// found on file: ntuple_10.root
//////////////////////////////////////////////////////////

#ifndef EventTree_h
#define EventTree_h
#define DEBUG 0

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class EventTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           EvtIsRealData;
   UInt_t          EvtNum;
   UInt_t          EvtRunNum;
   Int_t           EvtLumiNum;
   Int_t           EvtBxNum;
   Int_t           EvtVtxCnt;
   Int_t           EvtPuCntObs;
   Int_t           EvtPuCntTruth;
   vector<Double_t>*EvtWeights;
   Float_t         EvtFastJetRho;
   Double_t        firstGoodVertexIdx;
  //  UInt_t          TrigHlt;
  //  ULong64_t       TrigHltMu;
  //  ULong64_t       TrigMET;
  //  ULong64_t       TrigMETBit;
   Double_t        PreFiringWeight;
   Double_t        PreFiringWeightUp;
   Double_t        PreFiringWeightDown;
   TBranch        *b_EvtIsRealData;   //!
   TBranch        *b_EvtNum;   //!
   TBranch        *b_EvtRunNum;   //!
   TBranch        *b_EvtLumiNum;   //!
   TBranch        *b_EvtBxNum;   //!
   TBranch        *b_EvtVtxCnt;   //!
   TBranch        *b_EvtPuCntObs;   //!
   TBranch        *b_EvtPuCntTruth;   //!
   TBranch        *b_EvtWeights;   //!
   TBranch        *b_EvtFastJetRho;   //!
   TBranch        *b_firstGoodVertexIdx;
  //  TBranch        *b_TrigHlt;   //!
  //  TBranch        *b_TrigHltMu;   //!
  //  TBranch        *b_TrigMET;   //!
  //  TBranch        *b_TrigMETBit;   //!
   TBranch        *b_PreFiringWeight;
   TBranch        *b_PreFiringWeightUp;
   TBranch        *b_PreFiringWeightDown;

   ///////////////////////////////////////
   // GENERATOR LEVEL

   vector<float>   *GLepBarePt;
   vector<float>   *GLepBareEta;
   vector<float>   *GLepBarePhi;
   vector<float>   *GLepBareE;
   vector<int>     *GLepBareId;
   vector<int>     *GLepBareSt;
   vector<bool>    *GLepBarePrompt;
   TBranch        *b_GLepBarePt;   //!
   TBranch        *b_GLepBareEta;   //!
   TBranch        *b_GLepBarePhi;   //!
   TBranch        *b_GLepBareE;   //!
   TBranch        *b_GLepBareId;   //!
   TBranch        *b_GLepBareSt;   //!
   TBranch        *b_GLepBarePrompt;  //!

   vector<float>   *GLepClosePhotPt;
   vector<float>   *GLepClosePhotEta;
   vector<float>   *GLepClosePhotPhi;
   vector<float>   *GLepClosePhotE;
   vector<int>     *GLepClosePhotId;
   vector<int>     *GLepClosePhotMother0Id;
   vector<int>     *GLepClosePhotMotherCnt;
   vector<int>     *GLepClosePhotSt;
   TBranch        *b_GLepClosePhotPt;   //!
   TBranch        *b_GLepClosePhotEta;   //!
   TBranch        *b_GLepClosePhotPhi;   //!
   TBranch        *b_GLepClosePhotE;   //!
   TBranch        *b_GLepClosePhotId;   //!
   TBranch        *b_GLepClosePhotMother0Id;   //!
   TBranch        *b_GLepClosePhotMotherCnt;   //!
   TBranch        *b_GLepClosePhotSt;   //!

   vector<float>   *GMETPt;
   vector<float>   *GMETPx;
   vector<float>   *GMETPy;
   vector<float>   *GMETE;
   vector<float>   *GMETPhi;
   TBranch         *b_GMETPt;
   TBranch         *b_GMETPx;
   TBranch         *b_GMETPy;
   TBranch         *b_GMETE;
   TBranch         *b_GMETPhi;

   vector<float>   *GJetAk04Pt;
   vector<float>   *GJetAk04Eta;
   vector<float>   *GJetAk04Phi;
   vector<float>   *GJetAk04E;
   TBranch        *b_GJetAk04Pt;   //!
   TBranch        *b_GJetAk04Eta;   //!
   TBranch        *b_GJetAk04Phi;   //!
   TBranch        *b_GJetAk04E;   //!

   vector<float>   *GJetAk08Pt;
   vector<float>   *GJetAk08Eta;
   vector<float>   *GJetAk08Phi;
   vector<float>   *GJetAk08E;
   TBranch        *b_GJetAk08Pt;   //!
   TBranch        *b_GJetAk08Eta;   //!
   TBranch        *b_GJetAk08Phi;   //!
   TBranch        *b_GJetAk08E;   //!

   Int_t             GNup;
   int               npNLO;
   TBranch           *b_GNup;   //!
   TBranch           *b_npNLO;  //!

   ///////////////////////////////////////
   // RECONSTRUCTED LEVEL

   vector<float>   *MuPt;
   vector<float>   *MuEta;
   vector<float>   *MuPhi;
   vector<float>   *MuE;
   vector<float>   *MuPtRoch;
   vector<float>   *MuEtaRoch;
   vector<float>   *MuPhiRoch;
   vector<float>   *MuERoch;
   vector<bool>    *MuIdLoose;
   vector<bool>    *MuIdMedium;
   vector<bool>    *MuIdTight;
   vector<float>   *MuCh;
   vector<float>   *MuVtxZ;
   vector<float>   *MuDxy;
   vector<float>   *MuPfIso;
   vector<float>   *MuDz;
   vector<unsigned int> *MuHltMatch;
   vector<bool>    *MuHltTrgPath1;
   vector<bool>    *MuHltTrgPath2;
   vector<bool>    *MuHltTrgPath3;
   TBranch        *b_MuPt;   //!
   TBranch        *b_MuEta;   //!
   TBranch        *b_MuPhi;   //!
   TBranch        *b_MuE;   //!
   TBranch        *b_MuPtRoch;   //!
   TBranch        *b_MuEtaRoch;   //!
   TBranch        *b_MuPhiRoch;   //!
   TBranch        *b_MuERoch;   //!
   TBranch        *b_MuIdLoose;   //!
   TBranch        *b_MuIdMedium;   //!
   TBranch        *b_MuIdTight;   //!
   TBranch        *b_MuCh;   //!
   TBranch        *b_MuVtxZ;   //!
   TBranch        *b_MuDxy;   //!
   TBranch        *b_MuPfIso;   //!
   TBranch        *b_MuDz;   //!
   TBranch        *b_MuHltMatch;   //!
   TBranch        *b_MuHltTrgPath1;   //!
   TBranch        *b_MuHltTrgPath2;   //!
   TBranch        *b_MuHltTrgPath3;   //!

   vector<float>   *METPt;
   vector<float>   *METPx;
   vector<float>   *METPy;
   vector<float>   *METE;
   vector<float>   *METPhi;
   vector<bool>    *METFilterPath1;
   vector<bool>    *METFilterPath2;
   vector<bool>    *METFilterPath3;
   vector<bool>    *METFilterPath4;
   vector<bool>    *METFilterPath5;
   vector<bool>    *METFilterPath6;
   vector<bool>    *METFilterPath7;
   TBranch         *b_METPt;
   TBranch         *b_METPx;
   TBranch         *b_METPy;
   TBranch         *b_METE;
   TBranch         *b_METPhi;
   TBranch         *b_METFilterPath1;
   TBranch         *b_METFilterPath2;
   TBranch         *b_METFilterPath3;
   TBranch         *b_METFilterPath4;
   TBranch         *b_METFilterPath5;
   TBranch         *b_METFilterPath6;
   TBranch         *b_METFilterPath7;

   vector<float>   *JetAk04Pt;
   vector<float>   *JetAk04Eta;
   vector<float>   *JetAk04Phi;
   vector<float>   *JetAk04E;
   vector<float>   *JetAk04Id;
   vector<float>   *JetAk04PuMva;
   vector<int>     *JetAk04PuId;
   vector<bool>    *JetAk04PuIdLoose;
   vector<bool>    *JetAk04PuIdMedium;
   vector<bool>    *JetAk04PuIdTight;
   vector<float>   *JetAk04BDiscCisvV2;
   vector<float>   *JetAk04BDiscDeepCSV;
   vector<float>   *JetAk04HadFlav;
   vector<float>   *JetAk04JecUncUp;
   vector<float>   *JetAk04JecUncDwn;
   TBranch        *b_JetAk04Pt;   //!
   TBranch        *b_JetAk04Eta;   //!
   TBranch        *b_JetAk04Phi;   //!
   TBranch        *b_JetAk04E;   //!
   TBranch        *b_JetAk04Id;   //!
   TBranch        *b_JetAk04PuMva;   //!
   TBranch        *b_JetAk04PuId;   //!
   TBranch        *b_JetAk04PuIdLoose;   //!
   TBranch        *b_JetAk04PuIdMedium;   //!
   TBranch        *b_JetAk04PuIdTight;   //!
   TBranch        *b_JetAk04BDiscCisvV2;   //!
   TBranch        *b_JetAk04BDiscDeepCSV;   //!
   TBranch        *b_JetAk04HadFlav;   //!
   TBranch        *b_JetAk04JecUncUp;
   TBranch        *b_JetAk04JecUncDwn;

   vector<float>   *JetAk08Pt;
   vector<float>   *JetAk08Eta;
   vector<float>   *JetAk08Phi;
   vector<float>   *JetAk08E;
   vector<float>   *JetAk08Id;
  //  vector<float>   *JetAk08BDiscCisvV2;
   vector<float>   *JetAk08BDiscDeepCSV;
   vector<float>   *JetAk08HadFlav;
   vector<float>   *JetAk08JecUncUp;
   vector<float>   *JetAk08JecUncDwn;
   TBranch        *b_JetAk08Pt;   //!
   TBranch        *b_JetAk08Eta;   //!
   TBranch        *b_JetAk08Phi;   //!
   TBranch        *b_JetAk08E;   //!
   TBranch        *b_JetAk08Id;   //!
  //  TBranch        *b_JetAk08BDiscCisvV2;  //!
   TBranch        *b_JetAk08BDiscDeepCSV;  //!
   TBranch        *b_JetAk08HadFlav;   //!
   TBranch        *b_JetAk08JecUncUp;
   TBranch        *b_JetAk08JecUncDwn;

   EventTree(TTree *tree=0);
   virtual ~EventTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EventTree_cxx
EventTree::EventTree(TTree *tree) : fChain(0){
  if(DEBUG) printf("EventTree::EventTree(TTree *tree) : fChain(0){\n");
  Init(tree);
}

EventTree::~EventTree(){
  if(DEBUG) printf("EventTree::~EventTree(){\n");
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t EventTree::GetEntry(Long64_t entry){
  if(DEBUG) printf("EventTree::GetEntry(Long64_t entry)\n");
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t EventTree::LoadTree(Long64_t entry){
  if(DEBUG) printf("EventTree::LoadTree(Long64_t entry)\n");
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent){
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void EventTree::Init(TTree *tree){
   if(DEBUG) printf("void EventTree::Init(TTree *tree)\n");
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   
   // Set branch addresses and branch pointers
   if(!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   // Set object pointer
   EvtWeights = 0;
   fChain->SetBranchAddress("EvtIsRealData", &EvtIsRealData, &b_EvtIsRealData);
   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
   fChain->SetBranchAddress("EvtRunNum", &EvtRunNum, &b_EvtRunNum);
   fChain->SetBranchAddress("EvtLumiNum", &EvtLumiNum, &b_EvtLumiNum);
   fChain->SetBranchAddress("EvtBxNum", &EvtBxNum, &b_EvtBxNum);
   fChain->SetBranchAddress("EvtVtxCnt", &EvtVtxCnt, &b_EvtVtxCnt);
   fChain->SetBranchAddress("EvtPuCntObs", &EvtPuCntObs, &b_EvtPuCntObs);
   fChain->SetBranchAddress("EvtPuCntTruth", &EvtPuCntTruth, &b_EvtPuCntTruth);
   fChain->SetBranchAddress("EvtWeights", &EvtWeights, &b_EvtWeights);
   fChain->SetBranchAddress("EvtFastJetRho", &EvtFastJetRho, &b_EvtFastJetRho);
   fChain->SetBranchAddress("firstGoodVertexIdx", &firstGoodVertexIdx, &b_firstGoodVertexIdx);
  //  fChain->SetBranchAddress("TrigHlt", &TrigHlt, &b_TrigHlt);
  //  fChain->SetBranchAddress("TrigHltMu", &TrigHltMu, &b_TrigHltMu);
  //  fChain->SetBranchAddress("TrigMET", &TrigMET, &b_TrigMET);
  //  fChain->SetBranchAddress("TrigMETBit", &TrigMETBit, &b_TrigMETBit);
   fChain->SetBranchAddress("PreFiringWeight", &PreFiringWeight, &b_PreFiringWeight);
   fChain->SetBranchAddress("PreFiringWeightUp", &PreFiringWeightUp, &b_PreFiringWeightUp);
   fChain->SetBranchAddress("PreFiringWeightDown", &PreFiringWeightDown, &b_PreFiringWeightDown);

   GLepBarePt = 0;
   GLepBareEta = 0;
   GLepBarePhi = 0;
   GLepBareE = 0;
   GLepBareId = 0;
   GLepBareSt = 0;
   GLepBarePrompt = 0;
   fChain->SetBranchAddress("GLepBarePt", &GLepBarePt, &b_GLepBarePt);
   fChain->SetBranchAddress("GLepBareEta", &GLepBareEta, &b_GLepBareEta);
   fChain->SetBranchAddress("GLepBarePhi", &GLepBarePhi, &b_GLepBarePhi);
   fChain->SetBranchAddress("GLepBareE", &GLepBareE, &b_GLepBareE);
   fChain->SetBranchAddress("GLepBareId", &GLepBareId, &b_GLepBareId);
   fChain->SetBranchAddress("GLepBareSt", &GLepBareSt, &b_GLepBareSt);
   fChain->SetBranchAddress("GLepBarePrompt", &GLepBarePrompt, &b_GLepBarePrompt);
   
   GLepClosePhotPt = 0;
   GLepClosePhotEta = 0;
   GLepClosePhotPhi = 0;
   GLepClosePhotE = 0;
   GLepClosePhotId = 0;
   GLepClosePhotMother0Id = 0;
   GLepClosePhotMotherCnt = 0;
   GLepClosePhotSt = 0;
   fChain->SetBranchAddress("GLepClosePhotPt", &GLepClosePhotPt, &b_GLepClosePhotPt);
   fChain->SetBranchAddress("GLepClosePhotEta", &GLepClosePhotEta, &b_GLepClosePhotEta);
   fChain->SetBranchAddress("GLepClosePhotPhi", &GLepClosePhotPhi, &b_GLepClosePhotPhi);
   fChain->SetBranchAddress("GLepClosePhotE", &GLepClosePhotE, &b_GLepClosePhotE);
   fChain->SetBranchAddress("GLepClosePhotId", &GLepClosePhotId, &b_GLepClosePhotId);
   fChain->SetBranchAddress("GLepClosePhotMother0Id", &GLepClosePhotMother0Id, &b_GLepClosePhotMother0Id);
   fChain->SetBranchAddress("GLepClosePhotMotherCnt", &GLepClosePhotMotherCnt, &b_GLepClosePhotMotherCnt);
   fChain->SetBranchAddress("GLepClosePhotSt", &GLepClosePhotSt, &b_GLepClosePhotSt);

   GMETPt = 0;
   GMETPx = 0;
   GMETPy = 0;
   GMETE = 0;
   GMETPhi = 0;
   fChain->SetBranchAddress("GMETPt", &GMETPt, &b_GMETPt);
   fChain->SetBranchAddress("GMETPx", &GMETPx, &b_GMETPx);
   fChain->SetBranchAddress("GMETPy", &GMETPy, &b_GMETPy);
   fChain->SetBranchAddress("GMETE", &GMETE, &b_GMETE);
   fChain->SetBranchAddress("GMETPhi", &GMETPhi, &b_GMETPhi);

   GJetAk04Pt = 0;
   GJetAk04Eta = 0;
   GJetAk04Phi = 0;
   GJetAk04E = 0;
   fChain->SetBranchAddress("GJetAk04Pt", &GJetAk04Pt, &b_GJetAk04Pt);
   fChain->SetBranchAddress("GJetAk04Eta", &GJetAk04Eta, &b_GJetAk04Eta);
   fChain->SetBranchAddress("GJetAk04Phi", &GJetAk04Phi, &b_GJetAk04Phi);
   fChain->SetBranchAddress("GJetAk04E", &GJetAk04E, &b_GJetAk04E);

   GJetAk08Pt = 0;
   GJetAk08Eta = 0;
   GJetAk08Phi = 0;
   GJetAk08E = 0;
   fChain->SetBranchAddress("GJetAk08Pt", &GJetAk08Pt, &b_GJetAk08Pt);
   fChain->SetBranchAddress("GJetAk08Eta", &GJetAk08Eta, &b_GJetAk08Eta);
   fChain->SetBranchAddress("GJetAk08Phi", &GJetAk08Phi, &b_GJetAk08Phi);
   fChain->SetBranchAddress("GJetAk08E", &GJetAk08E, &b_GJetAk08E);

   fChain->SetBranchAddress("GNup", &GNup, &b_GNup);
   fChain->SetBranchAddress("npNLO", &npNLO, &b_npNLO);

   MuPt = 0;
   MuEta = 0;
   MuPhi = 0;
   MuE = 0;
   MuPtRoch = 0;
   MuEtaRoch = 0;
   MuPhiRoch = 0;
   MuERoch = 0;
   MuIdLoose = 0;
   MuIdMedium = 0;
   MuIdTight = 0;
   MuCh = 0;
   MuVtxZ = 0;
   MuDxy = 0;
   MuPfIso = 0;
   MuDz = 0;
   MuHltMatch = 0;
   MuHltTrgPath1 = 0;
   MuHltTrgPath2 = 0;
   MuHltTrgPath3 = 0;
   fChain->SetBranchAddress("MuPt", &MuPt, &b_MuPt);
   fChain->SetBranchAddress("MuEta", &MuEta, &b_MuEta);
   fChain->SetBranchAddress("MuPhi", &MuPhi, &b_MuPhi);
   fChain->SetBranchAddress("MuE", &MuE, &b_MuE);
   fChain->SetBranchAddress("MuPtRoch", &MuPtRoch, &b_MuPtRoch);
   fChain->SetBranchAddress("MuEtaRoch", &MuEtaRoch, &b_MuEtaRoch);
   fChain->SetBranchAddress("MuPhiRoch", &MuPhiRoch, &b_MuPhiRoch);
   fChain->SetBranchAddress("MuERoch", &MuERoch, &b_MuERoch);
   fChain->SetBranchAddress("MuIdLoose", &MuIdLoose, &b_MuIdLoose);
   fChain->SetBranchAddress("MuIdMedium", &MuIdMedium, &b_MuIdMedium);
   fChain->SetBranchAddress("MuIdTight", &MuIdTight, &b_MuIdTight);
   fChain->SetBranchAddress("MuCh", &MuCh, &b_MuCh);
   fChain->SetBranchAddress("MuVtxZ", &MuVtxZ, &b_MuVtxZ);
   fChain->SetBranchAddress("MuDxy", &MuDxy, &b_MuDxy);
   fChain->SetBranchAddress("MuPfIso", &MuPfIso, &b_MuPfIso);
   fChain->SetBranchAddress("MuDz", &MuDz, &b_MuDz);
   fChain->SetBranchAddress("MuHltMatch", &MuHltMatch, &b_MuHltMatch);
   fChain->SetBranchAddress("MuHltTrgPath1", &MuHltTrgPath1, &b_MuHltTrgPath1);
   fChain->SetBranchAddress("MuHltTrgPath2", &MuHltTrgPath2, &b_MuHltTrgPath2);
   fChain->SetBranchAddress("MuHltTrgPath3", &MuHltTrgPath3, &b_MuHltTrgPath3);

   METPt = 0;
   METPx = 0;
   METPy = 0;
   METE = 0;
   METPhi = 0;
   METFilterPath1 = 0;
   METFilterPath2 = 0;
   METFilterPath3 = 0;
   METFilterPath4 = 0;
   METFilterPath5 = 0;
   METFilterPath6 = 0;
   METFilterPath7 = 0;
   fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
   fChain->SetBranchAddress("METPx", &METPx, &b_METPx);
   fChain->SetBranchAddress("METPy", &METPy, &b_METPy);
   fChain->SetBranchAddress("METE", &METE, &b_METE);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
   fChain->SetBranchAddress("METFilterPath1", &METFilterPath1, &b_METFilterPath1);
   fChain->SetBranchAddress("METFilterPath2", &METFilterPath2, &b_METFilterPath2);
   fChain->SetBranchAddress("METFilterPath3", &METFilterPath3, &b_METFilterPath3);
   fChain->SetBranchAddress("METFilterPath4", &METFilterPath4, &b_METFilterPath4);
   fChain->SetBranchAddress("METFilterPath5", &METFilterPath5, &b_METFilterPath5);
   fChain->SetBranchAddress("METFilterPath6", &METFilterPath6, &b_METFilterPath6);
   fChain->SetBranchAddress("METFilterPath7", &METFilterPath7, &b_METFilterPath7);

   JetAk04Pt = 0;
   JetAk04Eta = 0;
   JetAk04Phi = 0;
   JetAk04E = 0;
   JetAk04Id = 0;
   JetAk04PuMva = 0;
   JetAk04PuId = 0;
   JetAk04PuIdLoose = 0;
   JetAk04PuIdMedium = 0;
   JetAk04PuIdTight = 0;
   JetAk04BDiscCisvV2 = 0;
   JetAk04BDiscDeepCSV = 0;
   JetAk04HadFlav = 0;
   JetAk04JecUncUp = 0;
   JetAk04JecUncDwn = 0;
   fChain->SetBranchAddress("JetAk04Pt", &JetAk04Pt, &b_JetAk04Pt);
   fChain->SetBranchAddress("JetAk04Eta", &JetAk04Eta, &b_JetAk04Eta);
   fChain->SetBranchAddress("JetAk04Phi", &JetAk04Phi, &b_JetAk04Phi);
   fChain->SetBranchAddress("JetAk04E", &JetAk04E, &b_JetAk04E);
   fChain->SetBranchAddress("JetAk04Id", &JetAk04Id, &b_JetAk04Id);
   fChain->SetBranchAddress("JetAk04PuMva", &JetAk04PuMva, &b_JetAk04PuMva);
   fChain->SetBranchAddress("JetAk04PuId", &JetAk04PuId, &b_JetAk04PuId);
   fChain->SetBranchAddress("JetAk04PuIdLoose", &JetAk04PuIdLoose, &b_JetAk04PuIdLoose);
   fChain->SetBranchAddress("JetAk04PuIdMedium", &JetAk04PuIdMedium, &b_JetAk04PuIdMedium);
   fChain->SetBranchAddress("JetAk04PuIdTight", &JetAk04PuIdTight, &b_JetAk04PuIdTight);
   fChain->SetBranchAddress("JetAk04BDiscCisvV2", &JetAk04BDiscCisvV2, &b_JetAk04BDiscCisvV2);
   fChain->SetBranchAddress("JetAk04BDiscDeepCSV", &JetAk04BDiscDeepCSV, &b_JetAk04BDiscDeepCSV);
   fChain->SetBranchAddress("JetAk04HadFlav", &JetAk04HadFlav, &b_JetAk04HadFlav);
   fChain->SetBranchAddress("JetAk04JecUncUp", &JetAk04JecUncUp, &b_JetAk04JecUncUp);
   fChain->SetBranchAddress("JetAk04JecUncDwn", &JetAk04JecUncDwn, &b_JetAk04JecUncDwn);

   JetAk08Pt = 0;
   JetAk08Eta = 0;
   JetAk08Phi = 0;
   JetAk08E = 0;
   JetAk08Id = 0;
  //  JetAk08BDiscCisvV2 = 0;
   JetAk08BDiscDeepCSV = 0;
   JetAk08HadFlav = 0;
   JetAk08JecUncUp = 0;
   JetAk08JecUncDwn = 0;
   fChain->SetBranchAddress("JetAk08Pt", &JetAk08Pt, &b_JetAk08Pt);
   fChain->SetBranchAddress("JetAk08Eta", &JetAk08Eta, &b_JetAk08Eta);
   fChain->SetBranchAddress("JetAk08Phi", &JetAk08Phi, &b_JetAk08Phi);
   fChain->SetBranchAddress("JetAk08E", &JetAk08E, &b_JetAk08E);
   fChain->SetBranchAddress("JetAk08Id", &JetAk08Id, &b_JetAk08Id);
  //  fChain->SetBranchAddress("JetAk08BDiscCisvV2", &JetAk08BDiscCisvV2, &b_JetAk08BDiscCisvV2);
   fChain->SetBranchAddress("JetAk08BDiscDeepCSV", &JetAk08BDiscDeepCSV, &b_JetAk08BDiscDeepCSV);
   fChain->SetBranchAddress("JetAk08HadFlav", &JetAk08HadFlav, &b_JetAk08HadFlav);
   fChain->SetBranchAddress("JetAk08JecUncUp", &JetAk08JecUncUp, &b_JetAk08JecUncUp);
   fChain->SetBranchAddress("JetAk08JecUncDwn", &JetAk08JecUncDwn, &b_JetAk08JecUncDwn);

   Notify();
}

Bool_t EventTree::Notify(){
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EventTree::Show(Long64_t entry){
  if(DEBUG) printf("EventTree::Show(Long64_t entry)\n");
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t EventTree::Cut(Long64_t entry){
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EventTree_cxx
