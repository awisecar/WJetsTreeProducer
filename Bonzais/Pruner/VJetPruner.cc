#include "Pruner.h"
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include <cstdlib>
using namespace std;

//Includes code generated by TTree:MakeClass():
//The fields of EventTree.h class will be used
//to access the EventTree contents. EventTree::Init(TTree*)
//should be called by the init(Tree*) method to link these
//fields to the EventTree contents.
#define EventTree_cxx
#include "EventTree.h"
void EventTree::Loop(){} //To make the compiler/linker happy.
#define DEBUG 0

/** Selection parameters **/
static float minWbosonPt = 0.;
static float minLepPt = 20.;
static float minMET = 0.;
static float minJetAk04Pt = 20.;
static float minJetAk08Pt = 20.;
static float minGLepClosePhotPt = 0.;

static float mll_low = 66;
static float mll_high = 126;

/**************************/

class VJetPruner: public Pruner, EventTree{
protected:
  void declareSubSelections();
  bool init(TChain* tree);
  bool filterEvent();
  
  // GEN
  bool filterGWboson(int iGenWboson);
  bool filterGLepBare(int iGenLep);
  bool filterGLepClosePhot(int iGenLepClosePhot);
  bool filterGMET(int iGenMET);
  bool filterGJetAk04(int iGenJetAk04);
  bool filterGJetAk08(int iGenJetAk08);

  // RECO
  bool filterMu(int iMu);
  bool filterMET(int iMET);
  bool filterJetAk04(int iJetAk04);
  bool filterJetAk08(int iJetAk08);
  
  void makeFilterMask(bool (VJetPruner::*filter)(int), std::vector<bool>& mask);
  void skimCollections();
  bool eventSelection();
  double mll(std::vector<float>* lepColPt, std::vector<float>* lepColEta,
	     std::vector<float>* lepColPhi, std::vector<float>* lepColE,
	     std::vector<int>* lepColId = 0, int pid = 0);
  // enum { DMu, DMuUnf, DMuMass, DE, DEUnf, DEMass, EMu, SE, SEUnf, SMu, SMuUnf, SLep, SLepUnf, NSubSels};
  enum { DMu, DMuUnf, DMuMass, SMu, SMuUnf, SLep, SLepUnf, NSubSels};

  static const int kEl = 11;
  static const int kMu = 13;
};

DECLARE_PRUNER(VJetPruner, "Pruner of W/Z + jet analyses")

void VJetPruner::declareSubSelections(){
  subSelections_.resize(NSubSels);
  subSelections_[DMu]    = SubSelection("DMu", "Dimuon selection for Z+jet analysis");
  subSelections_[DMuUnf] = SubSelection("DMuUnf", "Dimuon selection for Z+jet analysis with MC selection suitable to fill the detector response matrices for the unfolding.");
  subSelections_[DMuMass] = SubSelection("DMuMass", "Dielectron selection for Z+jet analysis including dilepton mass window");
  // subSelections_[DE]     = SubSelection("DE","Dielectron selection for Z+jet analysis");
  // subSelections_[DEUnf]  = SubSelection("DEUnf", "Dielectron selection for Z+jet analysis with MC selection suitable to fill the detector response matrices for the unfolding.");
  // subSelections_[DEMass] = SubSelection("DEMass", "Dimuon selection for Z+jet analysis including dilepton mass window");
  // subSelections_[EMu]    = SubSelection("EMu", "Electron-muon selection for background control region");
  // subSelections_[SE]     = SubSelection("SE", "Single electron selection for W+jet analysis");
  // subSelections_[SEUnf]  = SubSelection("SEUnf", "Single electron selection for W+jet analysis with MC selection suitable to fill the detector response matrices for the unfolding.");
  subSelections_[SMu]    = SubSelection("SMu", "Single muon selection for W+jet analysis");
  subSelections_[SMuUnf] = SubSelection("SMuUnf", "Single muon selection for W+jet analysis with MC selection suitable to fill the detector response matrices for the unfolding.");
  subSelections_[SLep]    = SubSelection("SLep", "Single lepton for any analysis");
  subSelections_[SLepUnf] = SubSelection("SLepUnf", "Single lepton unf for any analysis");
}

bool VJetPruner::init(TChain* tree){
  if(DEBUG) printf("VJetPruner::init(TChain* tree)\n");
  if(tree == 0) return false;
  EventTree::Init(tree);
  return true;
}

void VJetPruner::makeFilterMask(bool (VJetPruner::*filter)(int), std::vector<bool>& mask){
  for(unsigned i = 0; i < mask.size(); ++i){
    mask[i] = (this->*filter)(i);
  }
}

bool VJetPruner::filterEvent(){
  if(DEBUG) printf("VJetPruner::filterEvent()\n");
  skimCollections();
  return eventSelection() && Pruner::filterEvent();
}
  
void VJetPruner::skimCollections(){
  if(DEBUG) printf("VJetPruner::skimCollections()");
  std::vector<bool> mask;

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterGWboson, mask);\n");
  //Gen W boson collections --------------------
  mask.resize(GWbosonPt->size());
  makeFilterMask(&VJetPruner::filterGWboson, mask);
  filter(GWbosonPt,  mask);
  
  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterGLepBare, mask);\n");
  //Gen lepton collections --------------------
  mask.resize(GLepBarePt->size());
  makeFilterMask(&VJetPruner::filterGLepBare, mask);
  filter(GLepBarePt,  mask);
  filter(GLepBareEta, mask);
  filter(GLepBarePhi, mask);
  filter(GLepBareE,   mask);
  filter(GLepBareId,  mask);
  filter(GLepBareSt,  mask);
  filter(GLepBarePrompt, mask);

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterGLepClosePhot, mask);\n");
  //FSR gen photon collections --------------------
  mask.resize(GLepClosePhotPt->size());
  makeFilterMask(&VJetPruner::filterGLepClosePhot, mask);
  filter(GLepClosePhotPt,  mask);
  filter(GLepClosePhotEta,  mask);
  filter(GLepClosePhotPhi,  mask);
  filter(GLepClosePhotE,  mask);
  filter(GLepClosePhotId,  mask);
  filter(GLepClosePhotMother0Id,  mask);
  filter(GLepClosePhotMotherCnt,  mask);
  filter(GLepClosePhotSt,  mask);

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterGMET, mask);\n");
  mask.resize(GMETPt->size());
  makeFilterMask(&VJetPruner::filterGMET, mask);
  filter(GMETPt, mask);
  filter(GMETPx, mask);
  filter(GMETPy, mask);
  filter(GMETE, mask);
  filter(GMETPhi, mask);

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterGJetAk04, mask);\n");
  //Gen jet AK4 collections --------------------
  mask.resize(GJetAk04Pt->size());
  makeFilterMask(&VJetPruner::filterGJetAk04, mask);
  filter(GJetAk04Pt,  mask);
  filter(GJetAk04Eta, mask);
  filter(GJetAk04Phi, mask);
  filter(GJetAk04E,   mask);

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterGJetAk08, mask);\n");
  //Gen jet AK8 collections --------------------
  mask.resize(GJetAk08Pt->size());
  makeFilterMask(&VJetPruner::filterGJetAk08, mask);
  filter(GJetAk08Pt,  mask);
  filter(GJetAk08Eta, mask);
  filter(GJetAk08Phi, mask);
  filter(GJetAk08E,   mask);

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterMu, mask);\n");
  //Reco muon collections:
  mask.resize(MuPt->size());
  makeFilterMask(&VJetPruner::filterMu, mask);
  filter(MuPt, mask);
  filter(MuEta, mask);
  filter(MuPhi, mask);
  filter(MuE, mask);
  filter(MuPtRoch, mask);
  filter(MuEtaRoch, mask);
  filter(MuPhiRoch, mask);
  filter(MuERoch, mask);
  filter(MuIdLoose, mask);
  filter(MuIdMedium, mask);
  filter(MuIdTight, mask);
  filter(MuCh, mask);
  filter(MuVtxZ, mask);
  filter(MuDxy, mask);
  filter(MuPfIso, mask);
  filter(MuDz, mask);
  filter(MuHltMatch, mask);
  filter(MuHltTrgPath1, mask);
  filter(MuHltTrgPath2, mask);
  filter(MuHltTrgPath3, mask);

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterMET, mask);\n");
  //Reco MET:
  mask.resize(METPt->size());
  makeFilterMask(&VJetPruner::filterMET, mask);
  filter(METPt, mask);
  filter(METPx, mask);
  filter(METPy, mask);
  filter(METE, mask);
  filter(METPhi, mask);
  filter(METFilterPath1, mask);
  filter(METFilterPath2, mask);
  filter(METFilterPath3, mask);
  filter(METFilterPath4, mask);
  filter(METFilterPath5, mask);
  filter(METFilterPath6, mask);
  filter(METFilterPath7, mask);
  
  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterJetAk04, mask);\n");
  //AK4 reco jet collections:
  mask.resize(JetAk04Pt->size());
  makeFilterMask(&VJetPruner::filterJetAk04, mask);
  filter(JetAk04Pt, mask);
  filter(JetAk04Eta, mask);
  filter(JetAk04Phi, mask);
  filter(JetAk04E, mask);
  filter(JetAk04Id, mask);
  filter(JetAk04PuMva, mask);
  filter(JetAk04PuId, mask);
  filter(JetAk04PuIdLoose, mask);
  filter(JetAk04PuIdMedium, mask);
  filter(JetAk04PuIdTight, mask);
  filter(JetAk04BDiscCisvV2, mask);
  filter(JetAk04BDiscDeepCSV, mask);
  filter(JetAk04HadFlav, mask);
  filter(JetAk04hasGoodSVIVF, mask);
  filter(JetAk04SVIVFflightDist, mask);
  filter(JetAk04SVIVFflightDistSig, mask);
  filter(JetAk04SVIVFmass, mask);
  filter(JetAk04SVIVFnumTracks, mask);
  filter(JetAk04hasGoodSVSSV, mask);
  filter(JetAk04SVSSVflightDist, mask);
  filter(JetAk04SVSSVflightDistSig, mask);
  filter(JetAk04SVSSVmass, mask);
  filter(JetAk04SVSSVnumTracks, mask);
  filter(JetAk04JecUncUp, mask);
  filter(JetAk04JecUncDwn, mask);

  if(DEBUG) printf("makeFilterMask(&VJetPruner::filterJetAk08, mask);\n");
  //AK8 reco jet collections:
  mask.resize(JetAk08Pt->size());
  makeFilterMask(&VJetPruner::filterJetAk08, mask);
  filter(JetAk08Pt, mask);
  filter(JetAk08Eta, mask);
  filter(JetAk08Phi, mask);
  filter(JetAk08E, mask);
  filter(JetAk08Id, mask);
  // filter(JetAk08BDiscCisvV2, mask);
  filter(JetAk08BDiscDeepCSV, mask);
  filter(JetAk08HadFlav, mask);
  filter(JetAk08JecUncUp, mask);
  filter(JetAk08JecUncDwn, mask);
  filter(JetAk08JecUncSrcs, mask);
}

//to be run after skimCollections
bool VJetPruner::eventSelection(){
  //count gen leptons:
  int GElCnt = 0;
  int GMuCnt = 0;
  for(unsigned i = 0; i < GLepBareId->size(); ++i){
    if(fabs((*GLepBareId)[i]) == kEl) ++GElCnt;
    if(fabs((*GLepBareId)[i]) == kMu) ++GMuCnt;
  }  

  float m;
  switch(iSubSelection_){
  case DMu:
    return MuPt->size() > 1;
  case DMuUnf:
    return MuPt->size() > 1  || GMuCnt > 1;
  case DMuMass:
    m = mll(MuPt,MuEta,MuPhi,MuE);
    return MuPt->size() > 1 && mll_low < m && m < mll_high;
  // case DE:
  //   return ElPt->size() > 1;
  // case DEUnf:
  //   return ElPt->size() > 1 || GElCnt > 1;
  // case DEMass:
  //   m = mll(ElPt,ElEta,ElPhi,ElE);
  //   return ElPt->size() > 1 && mll_low < m && m < mll_high;
  // case EMu:
  //   return ElPt->size() > 0 && MuPt->size() > 0;
  // case SE:
  //   return ElPt->size() > 0;
  // case SEUnf:
  //   return ElPt->size() > 0 || GElCnt > 0;
  case SMu:
    return MuPt->size() > 0;
  case SMuUnf:
    return MuPt->size() > 0 || GMuCnt > 0;
  case SLep:
    // return MuPt->size() > 0 || ElPt->size() > 0;
    return MuPt->size() > 0;
  case SLepUnf:
    // return MuPt->size() > 0 || ElPt->size() > 0 || GElCnt > 0 || GMuCnt > 0;
    return MuPt->size() > 0 || GElCnt > 0 || GMuCnt > 0;
  case NSubSels:
  default:
    return false;
  }
};

double VJetPruner::mll(std::vector<float>* lepColPt, std::vector<float>* lepColEta,
		       std::vector<float>* lepColPhi, std::vector<float>* lepColE, 
		       std::vector<int>* lepColId, int pid){
  int i0 = -1; double pt0 = 0;
  int i1 = -1; double pt1 = 0;
  //look for the two highest pt leptons:
  for(size_t i = 0; i < lepColPt->size(); ++i){
    if(lepColId && abs((*lepColId)[i]) != pid) continue;
    if((*lepColPt)[i] > pt0) {
      i1 = i0;
      pt1 = pt0;
      i0 = i;
      pt0 = (*lepColPt)[i];
    } else if((*lepColPt)[i] > pt1) {i1 = i; pt1 = (*lepColPt)[i]; }
  }
  //std::cerr << __FILE__ << ":" << __LINE__ << ": " << i0 << i1 << "\n";
  if(i1 <0) return -1;
  else{
    TLorentzVector v0;
    v0.SetPtEtaPhiE((*lepColPt)[i0],(*lepColEta)[i0],(*lepColPhi)[i0],(*lepColE)[i0]);
    TLorentzVector v1;
    v1.SetPtEtaPhiE((*lepColPt)[i1],(*lepColEta)[i1],(*lepColPhi)[i1],(*lepColE)[i1]);
    return (v0 + v1).M();
  }
}

bool VJetPruner::filterGWboson(int iGenWboson){
  return ((*GWbosonPt)[iGenWboson] > minWbosonPt);
}

bool VJetPruner::filterGLepBare(int iGenLep){
  return ((*GLepBarePt)[iGenLep] > minLepPt);
}

bool VJetPruner::filterGLepClosePhot(int iGenLepClosePhot){
  return ((*GLepClosePhotPt)[iGenLepClosePhot] > minGLepClosePhotPt);
}

bool VJetPruner::filterGMET(int iGenMET){
  return ((*GMETPt)[iGenMET] > minMET);
}

bool VJetPruner::filterGJetAk04(int iGenJetAk04){
  return (*GJetAk04Pt)[iGenJetAk04] > minJetAk04Pt;
}

bool VJetPruner::filterGJetAk08(int iGenJetAk08){
  return (*GJetAk08Pt)[iGenJetAk08] > minJetAk08Pt;
}

bool VJetPruner::filterMu(int iMu){
  return ((*MuPt)[iMu] > minLepPt);
}

bool VJetPruner::filterMET(int iMET){
  return ((*METPt)[iMET] > minMET);
}

bool VJetPruner::filterJetAk04(int iJetAk04){
  return (*JetAk04Pt)[iJetAk04] > minJetAk04Pt;
}

bool VJetPruner::filterJetAk08(int iJetAk08){
  return (*JetAk08Pt)[iJetAk08] > minJetAk08Pt;
}


