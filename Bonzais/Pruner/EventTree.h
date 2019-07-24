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
   Int_t           EvtPuCnt;
   Int_t           EvtPuCntTruth;
   vector<Double_t>*EvtWeights;
   Double_t        originalXWGTUP;
   Float_t         EvtFastJetRho;
   ULong64_t       TrigMET;
   ULong64_t       TrigMETBit;
   Double_t        PreFiringWeight;
   Double_t        PreFiringWeightUp;
   Double_t        PreFiringWeightDown;
   //DJALOGTMP change to int in next production
   Double_t        firstGoodVertexIdx;
   UInt_t          TrigHlt;
   ULong64_t       TrigHltMu;
   TBranch        *b_EvtIsRealData;   //!
   TBranch        *b_EvtNum;   //!
   TBranch        *b_EvtRunNum;   //!
   TBranch        *b_EvtLumiNum;   //!
   TBranch        *b_EvtBxNum;   //!
   TBranch        *b_EvtVtxCnt;   //!
   TBranch        *b_EvtPuCnt;   //!
   TBranch        *b_EvtPuCntTruth;   //!
   TBranch        *b_EvtWeights;   //!
   TBranch        *b_originalXWGTUP;   //!
   TBranch        *b_EvtFastJetRho;   //!
   TBranch        *b_TrigMET;   //!
   TBranch        *b_TrigMETBit;   //!
   TBranch        *b_PreFiringWeight;
   TBranch        *b_PreFiringWeightUp;
   TBranch        *b_PreFiringWeightDown;
   TBranch        *b_firstGoodVertexIdx;
   TBranch        *b_TrigHlt;   //!
   TBranch        *b_TrigHltMu;   //!

   ///////////////////////////////////////
   // GENERATOR LEVEL

   vector<float>   *GLepBarePt;
   vector<float>   *GLepBareEta;
   vector<float>   *GLepBarePhi;
   vector<float>   *GLepBareE;
   vector<int>     *GLepBareId;
   vector<int>     *GLepBareSt;
   vector<int>     *GLepBareMomId;
   vector<bool>    *GLepBarePrompt;
   vector<bool>    *GLepBareTauProd;
   TBranch        *b_GLepBarePt;   //!
   TBranch        *b_GLepBareEta;   //!
   TBranch        *b_GLepBarePhi;   //!
   TBranch        *b_GLepBareE;   //!
   TBranch        *b_GLepBareId;   //!
   TBranch        *b_GLepBareSt;   //!
   TBranch        *b_GLepBareMomId;   //!
   TBranch        *b_GLepBarePrompt;  //!
   TBranch        *b_GLepBareTauProd; //!

   vector<float>   *GLepSt3Pt;
   vector<float>   *GLepSt3Eta;
   vector<float>   *GLepSt3Phi;
   vector<float>   *GLepSt3E;
   vector<int>     *GLepSt3Id;
   vector<int>     *GLepSt3St;
   vector<int>     *GLepSt3Mother0Id;
   vector<int>     *GLepSt3MotherCnt;
   TBranch        *b_GLepSt3Pt;   //!
   TBranch        *b_GLepSt3Eta;   //!
   TBranch        *b_GLepSt3Phi;   //!
   TBranch        *b_GLepSt3E;   //!
   TBranch        *b_GLepSt3Id;   //!
   TBranch        *b_GLepSt3St;   //!
   TBranch        *b_GLepSt3Mother0Id;   //!
   TBranch        *b_GLepSt3MotherCnt;   //!

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

   Int_t             GNup;
   int               npNLO;
   vector<int>       *LHEZid;
   vector<int>       *LHEZStatus;
   vector<float>     *LHEZPx;
   vector<float>     *LHEZPy;
   vector<float>     *LHEZPz;
   vector<float>     *LHEZE;
   TBranch           *b_GNup;   //!
   TBranch           *b_npNLO;  //!
   TBranch           *b_LHEZid;  //!
   TBranch           *b_LHEZStatus;  //!
   TBranch           *b_LHEZPx;  //!
   TBranch           *b_LHEZPy;  //!
   TBranch           *b_LHEZPz;  //!
   TBranch           *b_LHEZE;  //!

   ///////////////////////////////////////
   // RECONSTRUCTED LEVEL

   vector<float>   *MuPt;
   vector<float>   *MuEta;
   vector<float>   *MuPhi;
   vector<float>   *MuE;
   vector<bool>    *MuIdLoose;
   vector<bool>    *MuIdMedium;
   vector<bool>    *MuIdTight;
   vector<float>   *MuCh;
   vector<float>   *MuVtxZ;
   vector<float>   *MuDxy;
   vector<float>   *MuPfIso;
   vector<float>   *MuDz;
   vector<unsigned int> *MuHltMatch;
   TBranch        *b_MuPt;   //!
   TBranch        *b_MuEta;   //!
   TBranch        *b_MuPhi;   //!
   TBranch        *b_MuE;   //!
   TBranch        *b_MuIdLoose;   //!
   TBranch        *b_MuIdMedium;   //!
   TBranch        *b_MuIdTight;   //!
   TBranch        *b_MuCh;   //!
   TBranch        *b_MuVtxZ;   //!
   TBranch        *b_MuDxy;   //!
   TBranch        *b_MuPfIso;   //!
   TBranch        *b_MuDz;   //!
   TBranch        *b_MuHltMatch;   //!

   vector<float>   *METPt;
   vector<float>   *METPx;
   vector<float>   *METPy;
   vector<float>   *METE;
   vector<float>   *METPhi;
   TBranch         *b_METPt;
   TBranch         *b_METPx;
   TBranch         *b_METPy;
   TBranch         *b_METE;
   TBranch         *b_METPhi;

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
   TBranch        *b_JetAk04HadFlav;   //!
   TBranch        *b_JetAk04JecUncUp;
   TBranch        *b_JetAk04JecUncDwn;

   vector<float>   *JetAk08Pt;
   vector<float>   *JetAk08Eta;
  //  vector<float>   *JetAk08Rap;
   vector<float>   *JetAk08Phi;
   vector<float>   *JetAk08E;
   vector<float>   *JetAk08Id;
   vector<float>   *JetAk08BDiscCisvV2;
   vector<float>   *JetAk08HadFlav;
  //  vector<float>   *JetAk08CHSPt;
  //  vector<float>   *JetAk08CHSEta;
  //  vector<float>   *JetAk08CHSPhi;
   TBranch        *b_JetAk08Pt;   //!
   TBranch        *b_JetAk08Eta;   //!
  //  TBranch        *b_JetAk08Rap;   //!
   TBranch        *b_JetAk08Phi;   //!
   TBranch        *b_JetAk08E;   //!
   TBranch        *b_JetAk08Id;   //!
   TBranch        *b_JetAk08BDiscCisvV2;  //!
   TBranch        *b_JetAk08HadFlav;   //!
  //  TBranch        *b_JetAk08CHSPt;
  //  TBranch        *b_JetAk08CHSEta;
  //  TBranch        *b_JetAk08CHSPhi;

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
   fChain->SetBranchAddress("EvtPuCnt", &EvtPuCnt, &b_EvtPuCnt);
   fChain->SetBranchAddress("EvtPuCntTruth", &EvtPuCntTruth, &b_EvtPuCntTruth);
   fChain->SetBranchAddress("EvtWeights", &EvtWeights, &b_EvtWeights);
   fChain->SetBranchAddress("originalXWGTUP", &originalXWGTUP, &b_originalXWGTUP);
   fChain->SetBranchAddress("EvtFastJetRho", &EvtFastJetRho, &b_EvtFastJetRho);
   fChain->SetBranchAddress("TrigMET", &TrigMET, &b_TrigMET);
   fChain->SetBranchAddress("TrigMETBit", &TrigMETBit, &b_TrigMETBit);
   fChain->SetBranchAddress("PreFiringWeight", &PreFiringWeight, &b_PreFiringWeight);
   fChain->SetBranchAddress("PreFiringWeightUp", &PreFiringWeightUp, &b_PreFiringWeightUp);
   fChain->SetBranchAddress("PreFiringWeightDown", &PreFiringWeightDown, &b_PreFiringWeightDown);
   fChain->SetBranchAddress("firstGoodVertexIdx", &firstGoodVertexIdx, &b_firstGoodVertexIdx);
   fChain->SetBranchAddress("TrigHlt", &TrigHlt, &b_TrigHlt);
   fChain->SetBranchAddress("TrigHltMu", &TrigHltMu, &b_TrigHltMu);

   GLepBarePt = 0;
   GLepBareEta = 0;
   GLepBarePhi = 0;
   GLepBareE = 0;
   GLepBareId = 0;
   GLepBareSt = 0;
   GLepBareMomId = 0;
   GLepBarePrompt = 0;
   GLepBareTauProd = 0;
   fChain->SetBranchAddress("GLepBarePt", &GLepBarePt, &b_GLepBarePt);
   fChain->SetBranchAddress("GLepBareEta", &GLepBareEta, &b_GLepBareEta);
   fChain->SetBranchAddress("GLepBarePhi", &GLepBarePhi, &b_GLepBarePhi);
   fChain->SetBranchAddress("GLepBareE", &GLepBareE, &b_GLepBareE);
   fChain->SetBranchAddress("GLepBareId", &GLepBareId, &b_GLepBareId);
   fChain->SetBranchAddress("GLepBareSt", &GLepBareSt, &b_GLepBareSt);
   fChain->SetBranchAddress("GLepBareMomId", &GLepBareMomId, &b_GLepBareMomId);
   fChain->SetBranchAddress("GLepBarePrompt", &GLepBarePrompt, &b_GLepBarePrompt);
   fChain->SetBranchAddress("GLepBareTauProd", &GLepBareTauProd, &b_GLepBareTauProd);
   
   GLepSt3Pt = 0;
   GLepSt3Eta = 0;
   GLepSt3Phi = 0;
   GLepSt3E = 0;
   GLepSt3Id = 0;
   GLepSt3St = 0;
   GLepSt3Mother0Id = 0;
   GLepSt3MotherCnt = 0;
   fChain->SetBranchAddress("GLepSt3Pt", &GLepSt3Pt, &b_GLepSt3Pt);
   fChain->SetBranchAddress("GLepSt3Eta", &GLepSt3Eta, &b_GLepSt3Eta);
   fChain->SetBranchAddress("GLepSt3Phi", &GLepSt3Phi, &b_GLepSt3Phi);
   fChain->SetBranchAddress("GLepSt3E", &GLepSt3E, &b_GLepSt3E);
   fChain->SetBranchAddress("GLepSt3Id", &GLepSt3Id, &b_GLepSt3Id);
   fChain->SetBranchAddress("GLepSt3St", &GLepSt3St, &b_GLepSt3St);
   fChain->SetBranchAddress("GLepSt3Mother0Id", &GLepSt3Mother0Id, &b_GLepSt3Mother0Id);
   fChain->SetBranchAddress("GLepSt3MotherCnt", &GLepSt3MotherCnt, &b_GLepSt3MotherCnt);
   
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

   LHEZid = 0;
   LHEZStatus = 0;
   LHEZPx = 0;
   LHEZPy = 0;
   LHEZPz = 0;
   LHEZE = 0;
   fChain->SetBranchAddress("GNup", &GNup, &b_GNup);
   fChain->SetBranchAddress("npNLO", &npNLO, &b_npNLO);
   fChain->SetBranchAddress("LHEZid", &LHEZid, &b_LHEZid);
   fChain->SetBranchAddress("LHEZStatus", &LHEZStatus, &b_LHEZStatus);
   fChain->SetBranchAddress("LHEZPx", &LHEZPx, &b_LHEZPx);
   fChain->SetBranchAddress("LHEZPy", &LHEZPy, &b_LHEZPy);
   fChain->SetBranchAddress("LHEZPz", &LHEZPz, &b_LHEZPz);
   fChain->SetBranchAddress("LHEZE", &LHEZE, &b_LHEZE);

   MuPt = 0;
   MuEta = 0;
   MuPhi = 0;
   MuE = 0;
   MuIdLoose = 0;
   MuIdMedium = 0;
   MuIdTight = 0;
   MuCh = 0;
   MuVtxZ = 0;
   MuDxy = 0;
   MuPfIso = 0;
   MuDz = 0;
   MuHltMatch = 0;
   fChain->SetBranchAddress("MuPt", &MuPt, &b_MuPt);
   fChain->SetBranchAddress("MuEta", &MuEta, &b_MuEta);
   fChain->SetBranchAddress("MuPhi", &MuPhi, &b_MuPhi);
   fChain->SetBranchAddress("MuE", &MuE, &b_MuE);
   fChain->SetBranchAddress("MuIdLoose", &MuIdLoose, &b_MuIdLoose);
   fChain->SetBranchAddress("MuIdMedium", &MuIdMedium, &b_MuIdMedium);
   fChain->SetBranchAddress("MuIdTight", &MuIdTight, &b_MuIdTight);
   fChain->SetBranchAddress("MuCh", &MuCh, &b_MuCh);
   fChain->SetBranchAddress("MuVtxZ", &MuVtxZ, &b_MuVtxZ);
   fChain->SetBranchAddress("MuDxy", &MuDxy, &b_MuDxy);
   fChain->SetBranchAddress("MuPfIso", &MuPfIso, &b_MuPfIso);
   fChain->SetBranchAddress("MuDz", &MuDz, &b_MuDz);
   fChain->SetBranchAddress("MuHltMatch", &MuHltMatch, &b_MuHltMatch);

   METPt = 0;
   METPx = 0;
   METPy = 0;
   METE = 0;
   METPhi = 0;
   fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
   fChain->SetBranchAddress("METPx", &METPx, &b_METPx);
   fChain->SetBranchAddress("METPy", &METPy, &b_METPy);
   fChain->SetBranchAddress("METE", &METE, &b_METE);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);

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
   fChain->SetBranchAddress("JetAk04HadFlav", &JetAk04HadFlav, &b_JetAk04HadFlav);
   fChain->SetBranchAddress("JetAk04JecUncUp", &JetAk04JecUncUp, &b_JetAk04JecUncUp);
   fChain->SetBranchAddress("JetAk04JecUncDwn", &JetAk04JecUncDwn, &b_JetAk04JecUncDwn);

   JetAk08Pt = 0;
   JetAk08Eta = 0;
  //  JetAk08Rap = 0;
   JetAk08Phi = 0;
   JetAk08E = 0;
   JetAk08Id = 0;
   JetAk08BDiscCisvV2 = 0;
   JetAk08HadFlav = 0;
  //  JetAk08CHSPt = 0;
  //  JetAk08CHSEta = 0;
  //  JetAk08CHSPhi = 0;
   fChain->SetBranchAddress("JetAk08Pt", &JetAk08Pt, &b_JetAk08Pt);
   fChain->SetBranchAddress("JetAk08Eta", &JetAk08Eta, &b_JetAk08Eta);
  //  fChain->SetBranchAddress("JetAk08Rap", &JetAk08Rap, &b_JetAk08Rap);
   fChain->SetBranchAddress("JetAk08Phi", &JetAk08Phi, &b_JetAk08Phi);
   fChain->SetBranchAddress("JetAk08E", &JetAk08E, &b_JetAk08E);
   fChain->SetBranchAddress("JetAk08Id", &JetAk08Id, &b_JetAk08Id);
   fChain->SetBranchAddress("JetAk08BDiscCisvV2", &JetAk08BDiscCisvV2, &b_JetAk08BDiscCisvV2);
   fChain->SetBranchAddress("JetAk08HadFlav", &JetAk08HadFlav, &b_JetAk08HadFlav);
  //  fChain->SetBranchAddress("JetAk08CHSPt", &JetAk08CHSPt, &b_JetAk08CHSPt);
  //  fChain->SetBranchAddress("JetAk08CHSEta", &JetAk08CHSEta, &b_JetAk08CHSEta);
  //  fChain->SetBranchAddress("JetAk08CHSPhi", &JetAk08CHSPhi, &b_JetAk08CHSPhi);

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
