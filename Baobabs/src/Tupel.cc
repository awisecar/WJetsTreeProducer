/* -*- c-basic-offset: 2; -*-
*/
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <TLorentzVector.h>
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/GenericHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/TimeOfDay.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Version/interface/GetReleaseVersion.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "CondFormats/DataRecord/interface/HBHENegativeEFilterRcd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"
#include "EgammaAnalysis/ElectronTools/interface/PFIsolationEstimator.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TreeHelper.h"

const double pi = 4*atan(1.);

#define QUOTE2(a) #a
#define QUOTE(a) QUOTE2(a)
const static char* checksum = QUOTE(MYFILE_CHECKSUM);

/**
 * Define a bit of a bit field.
 * 1. Document the bit in the ROOT tree
 * 2. Set the variable with the bit mask (integer with the relevant bit set).
 * The second version DEF_BIT2 omits this setting. It can be useful when we
 * want to avoid defining the variable.
 * 3. Set a map to match bit name to bit mask
 * A field named after label prefixed with a k and with an undercore
 * appended to it must be defined in the class members. This macro will set it.
 * @param bitField: name (verbatim string without quotes) of the event tree branch the bit belongs to
 * @param bitNum: bit position, LSB is 0, MSB is 31
 * @param label: used to build the class field names (k<label>_ for the mask
 * and <label>Map_ for the name->mask map). The label is stored as
 * description in the ROOT file. Use a verbatim string without quotes.
 *
 *  For long description including spaces please use the DEF_BIT_L version
 */
#define DEF_BIT(bitField, bitNum, label)		\
  k ## label ## _ = 1 <<bitNum;				\
  bitField ## Map ## _[#label] = 1LL <<bitNum;		\
  treeHelper_->defineBit(#bitField, bitNum, #label);

#define DEF_BIT2(bitField, bitNum, label)		\
  bitField ## Map ## _[#label] = 1LL <<bitNum;		\
  treeHelper_->defineBit(#bitField, bitNum, #label);

/**
 * See DEF_BIT. Version for long description which can not be used
 * as variable name. The argument desc must be a c string including
 * the quotes or a c-string (char*) type variable.
 */
#define DEF_BIT_L(bitField, bitNum, label, desc)	\
  k ## label ## _ = 1 <<bitNum;				\
  bitField ## Map ## _[#label] = 1 <<bitNum;		\
  treeHelper_->defineBit(#bitField, bitNum, desc);

#define DEF_BIT2_L(bitField, bitNum, label, desc)	\
  bitField ## Map ## _[#label] = 1 <<bitNum;		\
  treeHelper_->defineBit(#bitField, bitNum, desc);


//#name -> "name", name ## _ -> name_
#define ADD_BRANCH_D(name, desc) treeHelper_->addBranch(#name, name ## _, desc)
#define ADD_BRANCH(name) treeHelper_->addBranch(#name, name ## _)
#define ADD_MOMENTUM_BRANCH_D(name, desc) treeHelper_->addMomentumBranch(#name, name ## Pt_, name ## Eta_, name ## Phi_, name ## E_, desc)
#define ADD_MOMENTUM_BRANCH(name) treeHelper_->addMomentumBranch(#name, name ## Pt_, name ## Eta_, name ## Phi_, name ## E_)

class TTree;
class Tupel : public edm::EDAnalyzer {

public:
  explicit Tupel(const edm::ParameterSet&);
  ~Tupel();

private:

  //virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  virtual void endRun(edm::Run const& iRun, edm::EventSetup const&);
  edm::Service<TFileService> fs;

  //help function to compute pseudo rapidity of a TLorentz without
  //ROOT exception in case of infinite pseudo rapidity
  double eta(TLorentzVector v){
    if(fabs(v.CosTheta()) < 1) return -0.5 * log((1.0-v.CosTheta())/(1.0+v.CosTheta()));
    else if(v.Pz() > 0) return 10e10;
    else return -10e10;
  }
  
  //help function to compute DR between two TLorentz vectors without
  //ROOT exception in case of infinite pseudo rapidity
  double deltar(TLorentzVector v1, TLorentzVector v2){
    return sqrt(pow(v1.DeltaPhi(v2), 2) + pow(eta(v1)-eta(v2), 2));
  }

  void allocateTrigMap(int ntrigMax);
  void defineBitFields();
    
  //Write Job information tree in the output. The tree
  //contains one event, with general information
  void writeHeader();

  //Main analysis functions ---
  void readEvent(const edm::Event& iEvent);
  void processLHE(const edm::Event& iEvent);
  bool processVtx(const edm::Event& iEvent);
  void processGenJets(const edm::Event& iEvent);
  void processGenJetsAK8(const edm::Event& iEvent);
  void processGenParticles(const edm::Event& iEvent);
  void processMET(const edm::Event& iEvent);
  void processPu(const edm::Event& iEvent);
  void processTrigger(const edm::Event& iEvent);
  void processMETFilter(const edm::Event& iEvent);
  void processMuons(const edm::Event& iEvent);
  void processJets();    
  void processJetsAK8();    

  enum triggerObjectType { hltmuons, hltelectrons} ;
  ULong64_t matchWithTriggerObject(const edm::Event&, TLorentzVector, triggerObjectType);

  /** Check if the argument is one of the
   * HLT trigger considered to be stored and if it is set
   * the corresponding bit.
   */
  void fillTrig(const std::string& trigname, int triggerPrescalesForThisIndex);

  void writeTriggerStat();

  /** Checks trigger name, ignoring the version number.
   * @param triggerBasename trigger base name without the version number
   * @param triggerName with possibly the _vNNN version suffix
   * @return true iff triggerName is identical to triggerBasename once the
   * version suffix discarded
   */
  bool compTrigger(const char* triggerBasename, const char* triggerName) const;
  
  // Select the year you want to process
  std::string yearToProcess_;
 
  // Collections/parameters grabbed from cfg file ---
  // std::string recoSw_;
  edm::EDGetTokenT<std::vector<pat::Muon> > muonToken_;
  edm::EDGetTokenT<reco::ConversionCollection> conversionsToken_;
  edm::EDGetTokenT<std::vector<pat::Jet> > jetToken_;
  edm::EDGetTokenT<std::vector<pat::Jet> > jetAK8Token_;
  edm::EDGetTokenT<std::vector<reco::Vertex> > vertexToken_;
  edm::EDGetTokenT<double> mSrcRhoToken_;
  edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescalesToken_;
  edm::EDGetTokenT<edm::TriggerResults> HLTTagToken_;
  edm::EDGetTokenT<std::vector<pat::TriggerObjectStandAlone> > HLTtriggerObjectToken_;
  edm::EDGetTokenT<std::vector<pat::MET> > metToken_;
  
  
  // GEN tokens
  edm::EDGetTokenT<LHEEventProduct> lheEventToken_;
  edm::EDGetTokenT<LHERunInfoProduct> lheRunToken_;
  edm::EDGetTokenT<GenEventInfoProduct> generatorToken_;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puToken_;
  edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticleToken_;
  edm::EDGetTokenT<std::vector<reco::GenJet> > gjetToken_;
  edm::EDGetTokenT<std::vector<reco::GenJet> > gjetAK8Token_;

  
  
  edm::EDGetTokenT<edm::TriggerResults> noiseFilterToken_;
  // Tags for MET filters
  std::string GoodVtxNoiseFilter_Selector_;
  std::string GlobalSuperTightHalo2016NoiseFilter_Selector_;
  std::string HBHENoiseFilter_Selector_;
  std::string HBHENoiseIsoFilter_Selector_;
  std::string EcalDeadCellTriggerPrimitiveNoiseFilter_Selector_;
  std::string BadPFMuonFilter_Selector_;
  std::string EEBadScNoiseFilter_Selector_;

  bool triggerStat_;
    
  //2016 L1 Prefire
  edm::EDGetTokenT< double > prefweight_token;
  edm::EDGetTokenT< double > prefweightup_token;
  edm::EDGetTokenT< double > prefweightdown_token;

  /** Total number of events analyzed so far
   */
  Long64_t analyzedEventCnt_;

  bool photonIdsListed_;
  bool elecIdsListed_;
  bool hltListed_;

  // ----------member data ---------------------------
  TTree *headTree;
  TTree *dataTree;
  TTree *descTree;
  TTree *bitTree;
  TreeHelper * treeHelper_;

  // Branches to be filled into output tree ---
  //Event
  std::unique_ptr<int>      EvtIsRealData_;
  std::unique_ptr<unsigned> EvtNum_;
  std::unique_ptr<unsigned> EvtRunNum_;
  std::unique_ptr<int> 	  EvtLumiNum_;
  std::unique_ptr<int> 	  EvtBxNum_;
  std::unique_ptr<int> 	  EvtVtxCnt_;
  std::unique_ptr<int> 	  EvtPuCntObs_;
  std::unique_ptr<int> 	  EvtPuCntTruth_;
  std::unique_ptr<std::vector<double> > EvtWeights_;
  // std::unique_ptr<double> originalXWGTUP_;
  std::unique_ptr<float>    EvtFastJetRho_;
  std::unique_ptr<double>    PreFiringWeight_;
  std::unique_ptr<double>    PreFiringWeightUp_;
  std::unique_ptr<double>    PreFiringWeightDown_;
  std::unique_ptr<int> firstGoodVertexIdx_;

  // MET Filter
  std::unique_ptr<ULong64_t>              TrigMET_;
  std::unique_ptr<ULong64_t>              TrigMETBit_;
  std::unique_ptr<std::vector<unsigned> > TrigMETBit_prescale_;
  std::map<std::string, ULong64_t>        TrigMETBitMap_; //bit assignment

  //Trigger
  std::unique_ptr<unsigned>               TrigHlt_;
  std::map<std::string, unsigned>         TrigHltMap_; //bit assignment
  std::unique_ptr<ULong64_t>              TrigHltMu_;
  std::unique_ptr<std::vector<unsigned> > TrigHltMu_prescale_;
  std::map<std::string, ULong64_t>        TrigHltMuMap_; //bit assignment
  std::map<std::string, ULong64_t>        TrigHltMuObjMap_;//bit assignment

  struct  TrigHltMapRcd {
    TrigHltMapRcd(): pMap(0), pTrig(0), pPrescale(0) {}
    TrigHltMapRcd(std::map<std::string, ULong64_t>* pMap_, ULong64_t* pTrig_, std::vector<unsigned>* pPrescale_): pMap(pMap_), pTrig(pTrig_), pPrescale(pPrescale_) {
      assert(pTrig_);
    }
    //    TrigHltMapRcd(const TrigHltMapRcd& a){ this->pMap = a.pMap; this->pTrig = a.pTrig; }
    std::map<std::string, ULong64_t>* pMap;
    ULong64_t* pTrig;
    std::vector<unsigned>* pPrescale;
  };

  std::vector<TrigHltMapRcd> trigHltMapList_; //list of trigger maps.
  std::vector<TrigHltMapRcd> TrigMETBitMapList_;

  //Missing energy
  std::unique_ptr<std::vector<float> > METPt_;
  std::unique_ptr<std::vector<float> > METPx_;
  std::unique_ptr<std::vector<float> > METPy_;
  // std::unique_ptr<std::vector<float> > METPz_;
  std::unique_ptr<std::vector<float> > METE_;
  std::unique_ptr<std::vector<float> > METPhi_;
  // std::unique_ptr<std::vector<float> > METEta_;

  std::unique_ptr<std::vector<float> > GMETPt_;
  std::unique_ptr<std::vector<float> > GMETPx_;
  std::unique_ptr<std::vector<float> > GMETPy_;
  // std::unique_ptr<std::vector<float> > GMETPz_;
  std::unique_ptr<std::vector<float> > GMETE_;
  std::unique_ptr<std::vector<float> > GMETPhi_;

  //Generator level leptons, not-dressed
  std::unique_ptr<std::vector<float> > 	GLepBarePt_;
  std::unique_ptr<std::vector<float> > 	GLepBareEta_;
  std::unique_ptr<std::vector<float> > 	GLepBarePhi_;
  std::unique_ptr<std::vector<float> > 	GLepBareE_;
  std::unique_ptr<std::vector<int> >    GLepBareId_;
  std::unique_ptr<std::vector<int> > 	  GLepBareSt_;
  std::unique_ptr<std::vector<bool> >   GLepBarePrompt_;

  //Photons in the vicinity of the leptons
  std::unique_ptr<std::vector<float> > GLepClosePhotPt_;
  std::unique_ptr<std::vector<float> > GLepClosePhotEta_;
  std::unique_ptr<std::vector<float> > GLepClosePhotPhi_;
  std::unique_ptr<std::vector<float> > GLepClosePhotE_;
  std::unique_ptr<std::vector<float> > GLepClosePhotM_;
  std::unique_ptr<std::vector<int> >   GLepClosePhotId_;
  std::unique_ptr<std::vector<int> >   GLepClosePhotMother0Id_;
  std::unique_ptr<std::vector<int> >   GLepClosePhotMotherCnt_;
  std::unique_ptr<std::vector<int> >   GLepClosePhotSt_;

  //Gen Jets, AK4
  std::unique_ptr<std::vector<float> > GJetAk04Pt_;
  std::unique_ptr<std::vector<float> > GJetAk04Eta_;
  std::unique_ptr<std::vector<float> > GJetAk04Phi_;
  std::unique_ptr<std::vector<float> > GJetAk04E_;

  //Gen Jets, AK8
  std::unique_ptr<std::vector<float> > GJetAk08Pt_;
  std::unique_ptr<std::vector<float> > GJetAk08Eta_;
  std::unique_ptr<std::vector<float> > GJetAk08Phi_;
  std::unique_ptr<std::vector<float> > GJetAk08E_;

  //Exta generator information
  std::unique_ptr<int>                 GNup_;
  //std::unique_ptr<int>                   npLO_;
  std::unique_ptr<int>                   npNLO_;

  // std::unique_ptr<std::vector<int> >      LHEZid_;
  // std::unique_ptr<std::vector<int> >      LHEZStatus_;
  // std::unique_ptr<std::vector<float> >    LHEZPx_;
  // std::unique_ptr<std::vector<float> >    LHEZPy_;
  // std::unique_ptr<std::vector<float> >    LHEZPz_;
  // std::unique_ptr<std::vector<float> >    LHEZE_;

  ///Muons
  std::unique_ptr<std::vector<float> > 	MuPt_;
  std::unique_ptr<std::vector<float> > 	MuEta_;
  std::unique_ptr<std::vector<float> > 	MuPhi_;
  std::unique_ptr<std::vector<float> > 	MuE_;
  std::unique_ptr<std::vector<bool> > MuIdLoose_;
  std::unique_ptr<std::vector<bool> > MuIdMedium_;
  std::unique_ptr<std::vector<bool> > MuIdTight_;
  std::unique_ptr<std::vector<float> > 	MuCh_;
  std::unique_ptr<std::vector<float> > 	MuVtxZ_;
  std::unique_ptr<std::vector<float> > 	MuDxy_;
  std::unique_ptr<std::vector<float> > 	MuPfIso_;
  std::unique_ptr<std::vector<float> > 	MuDz_;
  std::unique_ptr<std::vector<unsigned> > MuHltMatch_;
  // std::unique_ptr<std::vector<float> > MuTkNormChi2_;
  // std::unique_ptr<std::vector<int> > MuTkHitCnt_;
  // std::unique_ptr<std::vector<int> > MuMatchedStationCnt_;
  // std::unique_ptr<std::vector<int> > MuPixelHitCnt_;
  // std::unique_ptr<std::vector<int> > MuTkLayerCnt_;

  //PF Jets - AK4
  std::unique_ptr<std::vector<float> > JetAk04Pt_;
  std::unique_ptr<std::vector<float> > JetAk04Eta_;
  // std::unique_ptr<std::vector<float> > JetAk04Rap_;
  std::unique_ptr<std::vector<float> > JetAk04Phi_;
  std::unique_ptr<std::vector<float> > JetAk04E_;
  std::unique_ptr<std::vector<float> > JetAk04Id_;
  std::unique_ptr<std::vector<int> >   JetAk04PuId_;
  std::unique_ptr<std::vector<bool> > JetAk04PuIdLoose_;
  std::unique_ptr<std::vector<bool> > JetAk04PuIdMedium_;
  std::unique_ptr<std::vector<bool> > JetAk04PuIdTight_;
  std::unique_ptr<std::vector<float> > JetAk04PuMva_;
  std::unique_ptr<std::vector<float> > JetAk04BDiscCisvV2_;
  std::unique_ptr<std::vector<float> > JetAk04JecUncUp_;
  std::unique_ptr<std::vector<float> > JetAk04JecUncDwn_;
  std::unique_ptr<std::vector<float> > JetAk04HadFlav_;
  std::unique_ptr<std::vector<float> > JetAk04PtUncorr_;
  std::unique_ptr<std::vector<float> > JetAk04EtaUncorr_;
  std::unique_ptr<std::vector<float> > JetAk04EUncorr_;

  //PF Jets - AK8
  std::unique_ptr<std::vector<float> > JetAk08Pt_;
  std::unique_ptr<std::vector<float> > JetAk08Eta_;
  // std::unique_ptr<std::vector<float> > JetAk08Rap_;
  std::unique_ptr<std::vector<float> > JetAk08Phi_;
  std::unique_ptr<std::vector<float> > JetAk08E_;
  std::unique_ptr<std::vector<float> > JetAk08Id_;
  std::unique_ptr<std::vector<int> >   JetAk08BDiscCisvV2_;
  std::unique_ptr<std::vector<float> > JetAk08HadFlav_;
  std::unique_ptr<std::vector<float> > JetAk08CHSPt_;
  std::unique_ptr<std::vector<float> > JetAk08CHSEta_;
  std::unique_ptr<std::vector<float> > JetAk08CHSPhi_;
  std::unique_ptr<std::vector<float> > JetAk08JecUncUp_;
  std::unique_ptr<std::vector<float> > JetAk08JecUncDwn_;
  std::unique_ptr<std::vector<float> > JetAk08PtUncorr_;
  std::unique_ptr<std::vector<float> > JetAk08EtaUncorr_;
  std::unique_ptr<std::vector<float> > JetAk08EUncorr_;

  /// EDM Handles for collections ---
  edm::Handle<GenEventInfoProduct> genEventInfoProd;
  edm::Handle<std::vector<pat::Muon> > muons;
  edm::Handle<std::vector<pat::MET> > mets;
  edm::Handle<edm::TriggerResults> metfilters;
  edm::Handle<reco::ConversionCollection> conversions;
  edm::Handle<std::vector<pat::Jet> > jets;
  edm::Handle<std::vector<pat::Jet> > jetsAK8;
  edm::ESHandle<JetCorrectorParametersCollection> JetCorParCollAK4;
  edm::ESHandle<JetCorrectorParametersCollection> JetCorParCollAK8;
  edm::Handle<std::vector<reco::Vertex> >  vertices;
  double rhoIso;
  edm::Handle<reco::BeamSpot> beamSpotHandle;
  reco::BeamSpot beamSpot;
  edm::Handle<double> rho;

  // GEN Handles
  edm::Handle<LHEEventProduct> lheEventProd;
  edm::Handle<std::vector<PileupSummaryInfo> >  PupInfo;
  edm::Handle<std::vector<reco::GenParticle> > genParticles;
  edm::Handle<reco::GenJetCollection> genjetColl_;
  edm::Handle<reco::GenJetCollection> genjetAK8Coll_;

  edm::Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects_;
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  edm::Handle<edm::TriggerResults> HLTResHandle;

  std::vector<std::vector<int> > trigAccept_;
  std::vector<std::string> trigNames_;

  bool trigStatValid_;

  //keep track if the input file contained LHE weights:
  bool weightsFromLhe_;
  //keep track if GenRunInfoProduct weights which were found:
  // enum {UNKNOWN, YES, NO, MIXTURE} weightFromGenEventInfo_ = UNKNOWN;

  struct TrigSorter{
    TrigSorter(Tupel* t): tupel_(t){}
    bool operator()(int i, int j) const{
      return tupel_->trigAccept_[1+i][0] > tupel_->trigAccept_[1+j][0];
    }
    Tupel* tupel_;
  };

  // EffectiveAreas effectiveAreas_;
  bool DJALOG_;
  int failedVtx;
  int failedGen;
  int failedLHE;
  int failedGenJets;
  int failedGenJetsAK8;
  int failedMuons;
  int failedJets;
  int failedJetsAK8;
  int failedMET;
};

Tupel::Tupel(const edm::ParameterSet& iConfig):
  yearToProcess_(iConfig.getUntrackedParameter<std::string>("yearToProcess", "2017")),
  muonToken_(consumes<std::vector<pat::Muon> >(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc"))),
  jetToken_(consumes<std::vector<pat::Jet> >(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc"))),
  jetAK8Token_(consumes<std::vector<pat::Jet> >(iConfig.getUntrackedParameter<edm::InputTag>("jetAK8Src"))),
  triggerStat_(iConfig.getUntrackedParameter<bool>("triggerStat")),
  analyzedEventCnt_(0),
  elecIdsListed_(false),
  hltListed_(false),
  trigStatValid_(true),
  weightsFromLhe_(true)
{

  DJALOG_ = iConfig.getUntrackedParameter<bool>("DJALOG");  

  failedVtx = 0;
  failedGen = 0;
  failedLHE = 0;
  failedGenJets = 0;
  failedGenJetsAK8 = 0;
  failedMuons = 0;
  failedJets = 0;
  failedJetsAK8 = 0;
  failedMET = 0;

  // RECO tokens
  vertexToken_ = consumes<std::vector<reco::Vertex> >(iConfig.getUntrackedParameter<edm::InputTag>("vertexSrc"));

  // GEN tokens
  lheEventToken_ = consumes<LHEEventProduct>(iConfig.getUntrackedParameter<edm::InputTag>("lheSrc"));
  lheRunToken_ = consumes<LHERunInfoProduct, edm::InRun>(iConfig.getUntrackedParameter<edm::InputTag>("lheSrc"));
  generatorToken_ = consumes<GenEventInfoProduct>(iConfig.getUntrackedParameter<edm::InputTag>("genInfoSrc"));
  puToken_ = consumes<std::vector<PileupSummaryInfo> >(iConfig.getUntrackedParameter<edm::InputTag>("puSrc"));
  genParticleToken_ = consumes<std::vector<reco::GenParticle> >(iConfig.getUntrackedParameter<edm::InputTag>("genSrc"));
  gjetToken_ = consumes<std::vector<reco::GenJet> >(iConfig.getUntrackedParameter<edm::InputTag>("gjetSrc"));
  gjetAK8Token_ = consumes<std::vector<reco::GenJet> >(iConfig.getUntrackedParameter<edm::InputTag>("gjetAK8Src"));

  // HLT trigger bits
  HLTTagToken_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerSrc"));
  HLTtriggerObjectToken_ = consumes<std::vector<pat::TriggerObjectStandAlone> >(iConfig.getUntrackedParameter<edm::InputTag>("triggerObjectTag"));
  conversionsToken_ = consumes<reco::ConversionCollection>(edm::InputTag("reducedEgamma","reducedConversions"));

  // MET
  metToken_ = consumes<std::vector<pat::MET> >(iConfig.getUntrackedParameter<edm::InputTag>("metSrc"));
  noiseFilterToken_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("noiseFilterTag"));
  // MET filters
  GoodVtxNoiseFilter_Selector_ =  iConfig.getParameter<std::string> ("GoodVtxNoiseFilter_Selector_");
  GlobalSuperTightHalo2016NoiseFilter_Selector_ =  iConfig.getParameter<std::string> ("GlobalSuperTightHalo2016NoiseFilter_Selector_");
  HBHENoiseFilter_Selector_ =  iConfig.getParameter<std::string> ("HBHENoiseFilter_Selector_");
  HBHENoiseIsoFilter_Selector_ =  iConfig.getParameter<std::string> ("HBHENoiseIsoFilter_Selector_");
  EcalDeadCellTriggerPrimitiveNoiseFilter_Selector_ =  iConfig.getParameter<std::string> ("EcalDeadCellTriggerPrimitiveNoiseFilter_Selector_");
  BadPFMuonFilter_Selector_ =  iConfig.getParameter<std::string> ("BadPFMuonFilter_Selector_");
  EEBadScNoiseFilter_Selector_ =  iConfig.getParameter<std::string> ("EEBadScNoiseFilter_Selector_");

  // //L1 Prefire
  // prefweight_token = consumes< double >(edm::InputTag("prefiringweight:NonPrefiringProb"));
  // prefweightup_token = consumes< double >(edm::InputTag("prefiringweight:NonPrefiringProbUp"));
  // prefweightdown_token = consumes< double >(edm::InputTag("prefiringweight:NonPrefiringProbDown"));

  // Other
  mSrcRhoToken_ = consumes<double>(iConfig.getUntrackedParameter<edm::InputTag>("mSrcRho" ));
  beamSpotToken_ = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
  triggerPrescalesToken_ = consumes<pat::PackedTriggerPrescales>(edm::InputTag("patTrigger"));

}

Tupel::~Tupel(){
}

///////////////////////////////////////////////////////////////////////////////
// General functions

void Tupel::defineBitFields(){
  
  trigHltMapList_.push_back(TrigHltMapRcd(&TrigHltMuMap_,     TrigHltMu_.get(),     TrigHltMu_prescale_.get()));
  TrigMETBitMapList_.push_back(TrigHltMapRcd(&TrigMETBitMap_, TrigMETBit_.get(),    TrigMETBit_prescale_.get()));

  // andrew - do the placement of these bits need to match how the filters are read in?
  // shifted off by three..?
	DEF_BIT2(TrigMETBit,  0, Flag_duplicateMuons);
	DEF_BIT2(TrigMETBit,  1, Flag_badMuons);
	DEF_BIT2(TrigMETBit,  2, Flag_noBadMuons);
	DEF_BIT2(TrigMETBit,  3, Flag_HBHENoiseFilter);
	DEF_BIT2(TrigMETBit,  4, Flag_HBHENoiseIsoFilter);
	DEF_BIT2(TrigMETBit,  5, Flag_CSCTightHaloFilter);
	DEF_BIT2(TrigMETBit,  6, Flag_CSCTightHaloTrkMuUnvetoFilter);
	DEF_BIT2(TrigMETBit,  7, Flag_CSCTightHalo2015Filter);
	DEF_BIT2(TrigMETBit,  8, Flag_globalTightHalo2016Filter);
	DEF_BIT2(TrigMETBit,  9, Flag_globalSuperTightHalo2016Filter);
	DEF_BIT2(TrigMETBit, 10, Flag_HcalStripHaloFilter);
	DEF_BIT2(TrigMETBit, 11, Flag_hcalLaserEventFilter);
	DEF_BIT2(TrigMETBit, 12, Flag_EcalDeadCellTriggerPrimitiveFilter);
	DEF_BIT2(TrigMETBit, 13, Flag_EcalDeadCellBoundaryEnergyFilter);
	DEF_BIT2(TrigMETBit, 14, Flag_goodVertices);
	DEF_BIT2(TrigMETBit, 15, Flag_eeBadScFilter);
	DEF_BIT2(TrigMETBit, 16, Flag_ecalLaserCorrFilter);
	DEF_BIT2(TrigMETBit, 17, Flag_trkPOGFilters);
	DEF_BIT2(TrigMETBit, 18, Flag_chargedHadronTrackResolutionFilter);
	DEF_BIT2(TrigMETBit, 19, Flag_muonBadTrackFilter);
	DEF_BIT2(TrigMETBit, 20, Flag_trkPOG_manystripclus53X);
	DEF_BIT2(TrigMETBit, 21, Flag_trkPOG_toomanystripclus53X);
	DEF_BIT2(TrigMETBit, 22, Flag_trkPOG_logErrorTooManyClusters);
	DEF_BIT2(TrigMETBit, 23, Flag_METFilters);

  if ( yearToProcess_==std::string("2016") ){
    #include "trigger2016.h"
  }
  else if ( yearToProcess_==std::string("2017") ){
    #include "trigger2017.h"
  }
  
}

void Tupel::allocateTrigMap(int nTrigMax){
  std::vector<int> row(nTrigMax+2, 0);
  std::vector<std::vector<int> > newMap(nTrigMax+1, row);
  std::vector<std::string> newTrigNames(nTrigMax, "");
  for(unsigned i = 0; i < trigAccept_.size(); ++i){
    if(i < trigNames_.size()) newTrigNames[i] = trigNames_[i];
    for(unsigned j = 0; j < trigAccept_[i].size(); ++j){
      newMap[i][j] = trigAccept_[i][j];
    }
  }
  trigNames_.swap(newTrigNames);
  trigAccept_.swap(newMap);
}

void Tupel::writeTriggerStat(){
  std::ofstream f("trigger_stat.txt");
  if(!trigStatValid_){
    f << "Trigger statistics is not available. \n\n"
      "Events with different trigger index maps were processed. Trigger statistics"
      "is not supported for such case.\n\n";
    f << "Column S contains the number of events the trigger indicated in second column of is the only one fired.\n"
      << "The columns on the right of the column S contain for each combination of two triggers the number of events "
      "for which both triggers have been fired. The number in the column header refers to the trigger indices "
      "contained in the first column of the table.\n\n";
  } else {

    f << "Trigger statistics\n"
      << "------------------\n\n"
      << "Total number of processed events: " << analyzedEventCnt_ << "\n\n";

    f  << "#\tTrigger line\tAccept count\tAccept rate\tS";
    //sort the map per value by inverting key and value:
    std::vector<int> trigSortedIndex(trigNames_.size());
    size_t maxTrigForCol = 10;
    for(unsigned i = 0; i < trigSortedIndex.size(); ++i){
      trigSortedIndex[i] = i;
      if(i<maxTrigForCol) f << "\t" << (1+i);
    }
    f << "\n";
    sort(trigSortedIndex.begin(), trigSortedIndex.end(), TrigSorter(this));
    //    trigSortedIndex.resize(10);
    f << "0\tNone\t" << trigAccept_[0][0] << "\n";
    for(unsigned i = 0; i < trigSortedIndex.size(); ++i){
      unsigned ii = trigSortedIndex[i];
      f << (i+1) << "\t" << trigNames_[ii] << " (" << ii << ")"
	<< "\t" << trigAccept_[1+ii][0]
	<< "\t" << double(trigAccept_[1+ii][0]) / analyzedEventCnt_
	<< "\t" << trigAccept_[1+ii][1];
      for(unsigned j = 0; j < std::min(maxTrigForCol, trigSortedIndex.size()); ++j){
	unsigned jj = trigSortedIndex[j];
	f << "\t" << trigAccept_[1+ii][2+jj];
      }
      f << "\n";
    }
  }
}

bool Tupel::compTrigger(const char* a, const char* bv) const{
  int i = 0;
  for(;a[i]!=0 && bv[i]!=0; ++i){
    if(a[i]!= bv[i]) return false;
  }
  if(a[i]) return false;
  if(bv[i]==0) return true;
  if(bv[i] != '_') return false;
  if(bv[++i]!='v') return false;
  for(;;){
    if(bv[++i]==0) return true;
    if(!isdigit(bv[i])) return false;
  }
  return true;
};

ULong64_t Tupel::matchWithTriggerObject(const edm::Event& iEvent, TLorentzVector leptonMomentum, triggerObjectType theType){
  ULong64_t matchingResult = 0;
  std::map<std::string, ULong64_t> TrigObjMap;
  if(theType==triggerObjectType::hltmuons) TrigObjMap = TrigHltMuObjMap_;
  // else if(theType==triggerObjectType::hltelectrons) TrigObjMap = TrigHltElObjMap_;
  
  for(pat::TriggerObjectStandAlone obj : *triggerObjects_) {
    obj.unpackFilterLabels(iEvent,*HLTResHandle);
    TString collectionName = obj.collection();
    // if(analyzedEventCnt_==1){
    //   std::cout << "obj.collection() = " << obj.collection() << std::endl;
    // }
    obj.unpackNamesAndLabels(iEvent,*HLTResHandle);
    
    if( (theType==triggerObjectType::hltmuons) && ( !(collectionName.Contains("Muon")||collectionName.Contains("muon")) )) continue;
    if( (theType==triggerObjectType::hltelectrons) && ( !(collectionName.Contains("Egamma")) )) continue;

    // printf("collectionName = %s\n",collectionName.Data());

    for(std::map<std::string, ULong64_t>::const_iterator it = TrigObjMap.begin(); it != TrigObjMap.end(); ++it){
      for(unsigned h = 0; h < obj.filterLabels().size(); ++h){
        if(obj.filterLabels()[h]==it->first.c_str()){
          TLorentzVector hltCandMomentum(0,0,0,0);
          hltCandMomentum.SetPtEtaPhiE(obj.pt(), obj.eta(), obj.phi(), obj.energy());
          float theDeltaR = deltar(leptonMomentum, hltCandMomentum);
          if(theDeltaR<0.2) matchingResult |= it->second;
        }
      }
    }
  }
  return matchingResult;
}

void Tupel::fillTrig(const std::string& trigname, int triggerPrescalesForThisIndex){
  for(std::vector<TrigHltMapRcd>::iterator itTrigHltMap = trigHltMapList_.begin(); itTrigHltMap != trigHltMapList_.end(); ++itTrigHltMap){ //loop on the list of all triggers
    const std::map<std::string, ULong64_t>& trigHltMap = *(itTrigHltMap->pMap);
    ULong64_t* pTrig = itTrigHltMap->pTrig;
    std::vector<unsigned>* pPrescale = itTrigHltMap->pPrescale;
    for(std::map<std::string, ULong64_t>::const_iterator it = trigHltMap.begin(); it != trigHltMap.end(); ++it){ //loop on the map with all trigger of one type (e.g. Photon)
      if(compTrigger(it->first.c_str(), trigname.c_str())){
        *pTrig |= it->second; //to this 'all type of a kind of trigger' we have one list with bit fields where we enable some while looking at the value
        unsigned int bitPosition = log2(it->second);
        if(pPrescale->size() <= bitPosition) pPrescale->resize(bitPosition+1); 
        pPrescale->at(bitPosition) = triggerPrescalesForThisIndex; //And at the same position we add the prescale
      //      std::cout << it->first.c_str() << ", " <<  trigname.c_str() << " -> "
      //    << (compTrigger(it->first.c_str(), trigname.c_str()) ? "identical" : "different") << "\n";
      }
    }
  }
}

void Tupel::writeHeader(){

  headTree = fs->make<TTree>("Header", "Header");

  TString checksum_(checksum);
  headTree->Branch("Tupel_cc_githash", &checksum_);

  TString cmssw_release(edm::getReleaseVersion().c_str());
  headTree->Branch("CMSSW_RELEASE", &cmssw_release);

  char hostname[256];
  gethostname(hostname, sizeof(hostname));
  hostname[sizeof(hostname)-1] = 0;
  TString hostname_(hostname);
  headTree->Branch("Hostname", &hostname_);

  edm::TimeOfDay ts;
  std::stringstream s;
  s << std::setprecision(0) << ts;
  TString ts_(s.str().c_str());
  headTree->Branch("CreationTime", &ts_);

  headTree->Fill();
}

void Tupel::readEvent(const edm::Event& iEvent){

  if(analyzedEventCnt_==1) {
    if( yearToProcess_==std::string("2016") ){
        std::cout << "\nProcessing 2016 data/MC!" << std::endl;
    }
    else if ( yearToProcess_==std::string("2017") ){
        std::cout << "\nProcessing 2017 data/MC!" << std::endl;
    }
    else if ( yearToProcess_==std::string("2018") ){
        std::cout << "\nProcessing 2018 data/MC!" << std::endl;
    }
    else {
        std::cout << "\nPlease pick a correct year!" << std::endl;
    }
  }

  *EvtNum_        = iEvent.id().event();
  *EvtRunNum_     = iEvent.id().run();
  *EvtLumiNum_    = iEvent.luminosityBlock();
  *EvtBxNum_      = iEvent.bunchCrossing();
  *EvtIsRealData_ = iEvent.isRealData();

  if(DJALOG_) {
    std::cout << "*EvtNum_ = "        << iEvent.id().event() << std::endl;
    std::cout << "*EvtRunNum_ = "     << iEvent.id().run() << std::endl;
    std::cout << "*EvtLumiNum_ = "    << iEvent.luminosityBlock() << std::endl;
    std::cout << "*EvtBxNum_ = "      << iEvent.bunchCrossing() << std::endl;
    std::cout << "*EvtIsRealData_ = " << iEvent.isRealData() << std::endl;
  }

  const pat::helper::TriggerMatchHelper matchHelper;

  // Linking Handles with Tokens to access collections inside analysis functions ---
  iEvent.getByToken(vertexToken_, vertices);
  iEvent.getByToken(metToken_, mets);
  iEvent.getByToken(HLTTagToken_, HLTResHandle);
  iEvent.getByToken(muonToken_, muons);
  iEvent.getByToken(jetToken_, jets);  
  iEvent.getByToken(jetAK8Token_, jetsAK8);
  // GEN collections
  iEvent.getByToken(lheEventToken_, lheEventProd);
  iEvent.getByToken(puToken_, PupInfo);
  iEvent.getByToken(genParticleToken_, genParticles);
  iEvent.getByToken(gjetToken_, genjetColl_);
  iEvent.getByToken(gjetAK8Token_, genjetAK8Coll_);
  // some others...
  iEvent.getByToken(triggerPrescalesToken_, triggerPrescales);
  iEvent.getByToken(generatorToken_, genEventInfoProd);
  iEvent.getByToken(HLTtriggerObjectToken_, triggerObjects_);
  iEvent.getByToken(conversionsToken_, conversions);
  iEvent.getByToken(mSrcRhoToken_, rho);
  iEvent.getByToken(noiseFilterToken_, metfilters);

  
  rhoIso=-1;
  if(!rho.failedToGet()) rhoIso = *rho;
  *EvtFastJetRho_ =  rhoIso;

  iEvent.getByToken(beamSpotToken_, beamSpotHandle);
  
  // edm::Handle< double > theprefweight;
  // iEvent.getByToken(prefweight_token, theprefweight ) ;
  // *PreFiringWeight_ =(*theprefweight);
  // //printf("prefireWeight = %F\n", *PreFiringWeight_);

  // edm::Handle< double > theprefweightup;
  // iEvent.getByToken(prefweightup_token, theprefweightup ) ;
  // *PreFiringWeightUp_ =(*theprefweightup);

  // edm::Handle< double > theprefweightdown;
  // iEvent.getByToken(prefweightdown_token, theprefweightdown ) ;
  // *PreFiringWeightDown_ =(*theprefweightdown);

}

///////////////////////////////////////////////////////////////////////////////
/// GEN Functions
/// Done if event gives !iEvent.isRealData()

void Tupel::processLHE(const edm::Event& iEvent){
  if(genEventInfoProd.failedToGet() || !genEventInfoProd.isValid()){
    printf("processLHE failed at genEventInfoProd\n");
    failedLHE++;
    return;
  }

  // Main weight we use to normalize MC
  // The weight() method returns the value which is a result 
  // of multiplication of all elements in the weights container
  EvtWeights_->push_back(genEventInfoProd->weight());   

  if(EvtWeights_->size() == 0){
    EvtWeights_->push_back(1.);     
    // if(weightFromGenEventInfo_== NO) weightFromGenEventInfo_ = MIXTURE;     
    // else if (weightFromGenEventInfo_==UNKNOWN) weightFromGenEventInfo_ = YES;   
  } 
  // else{     
  //   if(weightFromGenEventInfo_== YES) weightFromGenEventInfo_ = MIXTURE;     
  //   else if (weightFromGenEventInfo_==UNKNOWN) weightFromGenEventInfo_ = NO;   
  // }   

  if(lheEventProd.failedToGet() || !lheEventProd.isValid()){
    printf("processLHE failed at lheEventProd\n");
    failedLHE++;
    return;
  }

  // *originalXWGTUP_ = lheEventProd->originalXWGTUP();

  // Weights from LHEEventProduct used for things like PDF, scale variations
  for(unsigned iw = 0; iw < lheEventProd->weights().size(); ++iw){       
    //printf("lheEventProd->weights()[iw].id = %s,  lheEventProd->weights()[iw].wgt=%F\n",lheEventProd->weights()[iw].id.c_str(),lheEventProd->weights()[iw].wgt);
    EvtWeights_->push_back(lheEventProd->weights()[iw].wgt);     
  }

  *GNup_ = lheEventProd->hepeup().NUP;

  // Number of partons at LO and NLO (?)
  //*npLO_ = lheEventProd->npLO();
  *npNLO_ = lheEventProd->npNLO();

  // //Loop over all particles to look for Z or photon coming from two quarks.
  // //printf("lheEventProd->hepeup().NUP=%d\n\n",lheEventProd->hepeup().NUP);
  // for(int iParticle=0; iParticle<lheEventProd->hepeup().NUP; iParticle++ ) {
  //   int pdgid = abs(lheEventProd->hepeup().IDUP[iParticle]);
  //   int status = lheEventProd->hepeup().ISTUP[iParticle];
  //   float px = (lheEventProd->hepeup().PUP[iParticle])[0];
  //   float py = (lheEventProd->hepeup().PUP[iParticle])[1];
  //   //Check for DY
  //   //if( (pdgid==22 || pdgid==23) && mom1id < 7 && mom2id < 7 && mom1id==mom2id){
  //   if( (pdgid==22 || pdgid==23) ){
  //     //Loop over all particles to find the children.
  //     //int nChild=0;

  //     LHEZid_->push_back(pdgid);
  //     LHEZStatus_->push_back(status);
  //     LHEZPx_->push_back(px);
  //     LHEZPy_->push_back(py);
  //     LHEZPz_->push_back((lheEventProd->hepeup().PUP[iParticle])[2]);
  //     LHEZE_->push_back((lheEventProd->hepeup().PUP[iParticle])[3]);
  //   }      
  // } // Loop over generator particles

}

void Tupel::processGenJets(const edm::Event& iEvent){    
  if(genjetColl_.failedToGet() || !genjetColl_.isValid()){
    printf("processGenJets failed\n");
    failedGenJets++;
    return;
  }
  
  for(unsigned int k=0; k < genjetColl_->size(); ++k){
    const reco::GenJet & genJet = genjetColl_->at(k);    
    if(genJet.pt() > 20.0 && fabs(genJet.eta()) < 2.7){

      GJetAk04Pt_->push_back(genJet.pt());
      GJetAk04Eta_->push_back(genJet.eta());
      GJetAk04Phi_->push_back(genJet.phi());
      GJetAk04E_->push_back(genJet.energy());

    }
  }

}

void Tupel::processGenJetsAK8(const edm::Event& iEvent){  
  if(genjetAK8Coll_.failedToGet() || !genjetAK8Coll_.isValid()){
    printf("processGenJetsAK8 failed\n");
    failedGenJetsAK8++;
    return;
  }

  for(unsigned int k=0; k < genjetAK8Coll_->size(); ++k){
    const reco::GenJet & genJetAK8 = genjetAK8Coll_->at(k);    
    if(genJetAK8.pt() > 200.0 && fabs(genJetAK8.eta()) < 2.7){
      GJetAk08Pt_->push_back(genJetAK8.pt());
      GJetAk08Eta_->push_back(genJetAK8.eta());
      GJetAk08Phi_->push_back(genJetAK8.phi());
      GJetAk08E_->push_back(genJetAK8.energy());
    }
  }

}

void Tupel::processGenParticles(const edm::Event& iEvent){
  if(genParticles.failedToGet() || !genParticles.isValid()){
    printf("processGenParticles failed\n");
    failedGen++;
    return;
  }
  
  for(size_t i=0; i < genParticles->size(); ++i){
    const reco::GenParticle & gen = genParticles->at(i);

    int id = gen.pdgId();
    // Stable particles have status() = 1 (it's a Pythia8 thing)
    int st = gen.status(); 
    //isPromptFinalState() = is particle prompt (not from hadron, muon, or tau decay) and final state
    bool isPrompt = gen.isPromptFinalState(); 
      
    //Putting a soft pt cut on leptons to reduce size of ntuple
    if( st==1 
       && ( abs(id)==13 || abs(id)==14 )  // either a muon or muon neutrino
       && gen.pt() > 20.0
       && fabs(gen.eta()) < 2.7 ){
      
      TLorentzVector genLep1(0,0,0,0);
      genLep1.SetPtEtaPhiE(gen.pt(),gen.eta(),gen.phi(),gen.energy());

      // For muons, find photons within dR cone of 0.1 (used for dressing muons)
      if( st==1 && abs(id)==13 ){
	      for(size_t j=0; j < genParticles->size(); ++j){
          const reco::GenParticle & gen2 = genParticles->at(j);

          // Look at particles (e.g. radiated photons) with an associated mother particle
          // prunedGenParticles collection should keep link to first ancestor
	        if(gen2.numberOfMothers()){
	          //Skip non-photons, all photons above 10 GeV should be kept (?)
	          if( gen2.status()!=1 || gen2.pdgId()!=22 || gen2.energy()<0.000001 ) continue;

            TLorentzVector thisPho1(0,0,0,0);
            thisPho1.SetPtEtaPhiE(gen2.pt(),gen2.eta(),gen2.phi(),gen2.energy());
            double dR = deltar(genLep1,thisPho1);

            if(dR <= 0.1){
              GLepClosePhotPt_->push_back(thisPho1.Pt());
              GLepClosePhotEta_->push_back(thisPho1.Eta());
              GLepClosePhotPhi_->push_back(thisPho1.Phi());
              GLepClosePhotE_->push_back(thisPho1.Energy());
              GLepClosePhotId_->push_back(gen2.pdgId());
              GLepClosePhotMother0Id_->push_back(fabs(gen2.mother()->pdgId())); //not being accessed correctly?
              GLepClosePhotMotherCnt_->push_back(gen2.numberOfMothers());
              GLepClosePhotSt_->push_back(gen2.status());
            }
          }
	      }
      }//end photon loop
	
      GLepBarePt_->push_back(genLep1.Pt());
      GLepBareEta_->push_back(genLep1.Eta());
      GLepBarePhi_->push_back(genLep1.Phi());
      GLepBareE_->push_back(genLep1.Energy());
      GLepBareId_->push_back(id);
      GLepBareSt_->push_back(st);
      GLepBarePrompt_->push_back(isPrompt);

    } //end status lepton or neutrino
  } //end gen particle loop
}

void Tupel::processPu(const edm::Event& iEvent){
  if(!PupInfo.failedToGet()){
    
    float npT=-1.;
    float npIT=-1.;

    std::vector<PileupSummaryInfo>::const_iterator PVI;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
      int BX = PVI->getBunchCrossing();
      // central bunch crossing
      if(BX == 0) {
        // getTrueNumInteractions() = "true" average number of PU vertices selected for the event
        npT = PVI->getTrueNumInteractions(); 
        // getPU_NumInteractions() = number of PU vertices selected from Poisson distribution with mean
        // selected from that of "true" pileup distribution --> the "observed" number of PU vertices
        // considers both in-time and out-of-time PU vertices (?)
        npIT = PVI->getPU_NumInteractions();
      }
    }
    *EvtPuCntTruth_ = npT;
    *EvtPuCntObs_   = npIT;
  }
  else {
    *EvtPuCntTruth_ = -99.;
    *EvtPuCntObs_ = -99.;
  }
}

///////////////////////////////////////////////////////////////////////////////
/// RECO Functions

bool Tupel::processVtx(const edm::Event& iEvent){
  if(vertices.failedToGet() || !vertices.isValid() || vertices->empty()){
    printf("processVtx failed!!!!!\n");
    failedVtx++;
    return false;
  }

  // Getting number of primary vertices for event
  for (std::vector<reco::Vertex>::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx){
    bool isFake = vtx->chi2()==0 && vtx->ndof()==0;
    if (vtx->isValid() && !isFake) ++(*EvtVtxCnt_);
  }

  // Find the first vertex in the collection that passes good quality criteria
  // Assuming this is the PV that we use for muon TightID, etc.
  int index = 0;
  *firstGoodVertexIdx_ = -1;
  std::vector<reco::Vertex>::const_iterator firstGoodVertex = vertices->end();

  for (std::vector<reco::Vertex>::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx){
    // Replace isFake() for miniAOD because it requires tracks and miniAOD vertices don't have tracks:
    // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}
    bool isFake = vtx->chi2()==0 && vtx->ndof()==0;
    // Check the goodness
    if(vtx->isValid() && !isFake
       && (vtx->ndof() >= 4.0)
       && (vtx->position().Rho() <= 2.0)
       && (fabs(vtx->position().Z()) <= 24.0) ){
      firstGoodVertex = vtx;
      *firstGoodVertexIdx_ = index;      
      break;
    }
    index++;
  }

  if(firstGoodVertex==vertices->end()){
    std::cout << " >>> Event #" << analyzedEventCnt_ << ": No good vertices" << std::endl;
    return false;
  } else return true;

}

void Tupel::processMET(const edm::Event& iEvent){

  if(mets.failedToGet() || !mets.isValid()){
    printf("processMET, collection failed\n");
    failedMET++;
    return;
  }
  
  const pat::MET &met = mets->front();

  // standard methods (pt(), etc.) return the default type1 corrected MET
  METPt_->push_back(met.pt());
  METPx_->push_back(met.px());
  METPy_->push_back(met.py());
  // METPz_->push_back(met.pz());
  METE_->push_back(met.energy());
  METPhi_->push_back(met.phi());
  // METEta_->push_back(met.eta());

  // can also grab MET shifted up and down for JES uncertainties?
  // met.shiftedPt(pat::MET::JetEnUp), met.shiftedPt(pat::MET::JetEnDown)

  // also different levels of corrections, like
  // met.uncorPt() - raw MET
  // met.corPt(pat::MET::Type1) - Type1 corrected MET
  // met.corPt(pat::MET::Type1XY) - phi-corrected MET

  if(!*EvtIsRealData_){
    GMETPt_->push_back(met.genMET()->pt());
    GMETPx_->push_back(met.genMET()->px());
    GMETPy_->push_back(met.genMET()->py());
    // GMETPz_->push_back(met.genMET()->pz());
    GMETE_->push_back(met.genMET()->energy());
    GMETPhi_->push_back(met.genMET()->phi());
  }

}

void Tupel::processTrigger(const edm::Event& iEvent){
  bool trigNameFilled = trigNames_.size();
  
  std::vector<int> trigIndexList;
  if(triggerStat_) trigIndexList.reserve(30);

  // Saving record of triggers in output file
  std::ofstream f;
  if(analyzedEventCnt_==1){
    if (triggerStat_) {
      f.open("trigger_list.txt");
      f << "List of triggers extracted from event " << iEvent.id().event()
        << " of run " << iEvent.id().run() << "\n\n";
    }
  }

  // -----------
  int ntrigs;
  if(HLTResHandle.isValid() && !HLTResHandle.failedToGet()){
    // Next lines gets all the HLT trigger paths for the event
    edm::RefProd<edm::TriggerNames> trigNames( &(iEvent.triggerNames(*HLTResHandle)) );
    ntrigs = (int)trigNames->size();

    // Prints out passed trigger paths
    if(analyzedEventCnt_==1){
      std::cout << "\n--> Total trigger paths: " << ntrigs << std::endl;
      std::cout << "--> Passed trigger paths:" << std::endl;
      for (int i = 0; i < ntrigs; i++) {
        if (HLTResHandle->accept(i)){
          std::cout << "#" << i << ": " << trigNames->triggerName(i);
          std::cout << ": " <<  (HLTResHandle->accept(i) ? "PASS" : "FAIL") << std::endl;
        }
      }
    }

    if(triggerStat_) allocateTrigMap(ntrigs);

    for (int i = 0; i < ntrigs; i++) {
      // Prints all found triggers in output file
      if(analyzedEventCnt_==1) {
        if (triggerStat_){
          f << trigNames->triggerName(i) << "\n";
        }
      }

      if(triggerStat_){
        if(!trigNameFilled) trigNames_[i] = trigNames->triggerName(i);
        else if(trigNames_[i] != trigNames->triggerName(i)) trigStatValid_ = false;
      }

      //insert trigger name in the acceptance map if not yet in:
      if (HLTResHandle->accept(i)){
        if(triggerStat_) trigIndexList.push_back(i);
        //andrew - main analysis line?
        std::string thisTrigger = trigNames->triggerName(i);
        fillTrig(std::string(trigNames->triggerName(i)), triggerPrescales->getPrescaleForIndex(i));
      }

    }

    if(triggerStat_){
      if(trigIndexList.size()==0) trigAccept_[0][0] += 1;
      for(std::vector<int>::iterator it1 = trigIndexList.begin(); it1 != trigIndexList.end(); ++it1){
        trigAccept_[1 + *it1][0] += 1;
        if(trigIndexList.size()==1) trigAccept_[1+*it1][1] += 1;
        for(std::vector<int>::iterator it2 = trigIndexList.begin(); it2 != trigIndexList.end(); ++it2){
          trigAccept_[1 + *it1][2 + *it2] += 1;
        }
      }
    }

  }
}

void Tupel::processMETFilter(const edm::Event& iEvent){
  *TrigMET_ = 0;

  if (metfilters.isValid() && !metfilters.failedToGet()){
    const edm::TriggerNames &names = iEvent.triggerNames(*metfilters);

    if(analyzedEventCnt_==1) {
      std::cout << "\n--> MET Filters" << std::endl;
      for (unsigned int i = 0, n = metfilters->size(); i < n; ++i) {
        // std::cout << names.triggerName(i) << std::endl;
        if (names.triggerName(i) == GoodVtxNoiseFilter_Selector_) std::cout << "Flag_goodVertices is i=" << i << std::endl;
        if (names.triggerName(i) == GlobalSuperTightHalo2016NoiseFilter_Selector_) std::cout << "Flag_globalSuperTightHalo2016Filter is i=" << i << std::endl;
        if (names.triggerName(i) == HBHENoiseFilter_Selector_) std::cout << "Flag_HBHENoiseFilter is i=" << i << std::endl;
        if (names.triggerName(i) == HBHENoiseIsoFilter_Selector_) std::cout << "Flag_HBHENoiseIsoFilter is i=" << i << std::endl;
        if (names.triggerName(i) == EcalDeadCellTriggerPrimitiveNoiseFilter_Selector_) std::cout << "Flag_EcalDeadCellTriggerPrimitiveFilter is i=" << i << std::endl;
        if (names.triggerName(i) == BadPFMuonFilter_Selector_) std::cout << "Flag_BadPFMuonFilter is i=" << i << std::endl;
        if (names.triggerName(i) == EEBadScNoiseFilter_Selector_) std::cout << "Flag_eeBadScFilter is i=" << i << std::endl;
      }
      std::cout << std::endl;
    }

    // For 2017 data/MC
    // Using "TriggerResults", PAT
    // Flag_HBHENoiseFilter is i=0
    // Flag_HBHENoiseIsoFilter is i=1
    // Flag_globalSuperTightHalo2016Filter is i=6
    // Flag_EcalDeadCellTriggerPrimitiveFilter is i=9
    // Flag_goodVertices is i=12
    // Flag_eeBadScFilter is i=13
    // Flag_BadPFMuonFilter is i=19

    //neglect MC for now
    for (unsigned int i = 0, n = metfilters->size(); i < n; ++i) {
      if(!*EvtIsRealData_){
        if(analyzedEventCnt_==1) {
          std::cout << "neglect MC MET filters for now" << std::endl;
        }
      }
      else{
        if (metfilters->accept(i)) *TrigMET_ |= 1LL << i;
      }
    }

  }
}

void Tupel::processMuons(const edm::Event& iEvent){
  if(muons.failedToGet() || !muons.isValid()){
    printf("processMuons failed\n");
    failedMuons++;
    return;
  }

  for(unsigned int j = 0; j < muons->size(); ++j){
    const pat::Muon & mu = muons->at(j);

    if(mu.pt() > 20.0 && mu.isLooseMuon()){

      // Muon ID --------
      bool loose = mu.isLooseMuon();
      bool medium = mu.isMediumMuon();
      bool tight = false;
      if(*firstGoodVertexIdx_ >= 0){
        tight = mu.isTightMuon( vertices->at(*firstGoodVertexIdx_) ); // use PV obtained in processVtx function
      }

      TLorentzVector muon4Momentum(0,0,0,0);
      muon4Momentum.SetPtEtaPhiE(mu.pt(), mu.eta(), mu.phi(), mu.energy());
      Long64_t muonMatchingResults = matchWithTriggerObject(iEvent, muon4Momentum, triggerObjectType::hltmuons);

      MuHltMatch_->push_back(muonMatchingResults);
      MuIdLoose_->push_back(loose);
      MuIdMedium_->push_back(medium);
      MuIdTight_->push_back(tight);

      // PF Isolation score --------
      double chargedHadronIso = mu.pfIsolationR04().sumChargedHadronPt;
      double neutralHadronIso  = mu.pfIsolationR04().sumNeutralHadronEt;
      double photonIso  = mu.pfIsolationR04().sumPhotonEt;
      double chargedHadronIsoPU = mu.pfIsolationR04().sumPUPt;
      float relativeIsolationDBetaCorr = ( chargedHadronIso + std::max(0., neutralHadronIso + photonIso - 0.5*chargedHadronIsoPU) )/mu.pt();
      MuPfIso_->push_back(relativeIsolationDBetaCorr);

      // Kinematics --------
      MuPt_->push_back(mu.pt());
      MuEta_->push_back(mu.eta());
      MuPhi_->push_back(mu.phi());
      MuE_->push_back(mu.energy());
      MuCh_->push_back(mu.charge());
      MuVtxZ_->push_back(mu.vz());
      MuDxy_->push_back(mu.dB());

      // Other --------
      double dZ = 99.0;
      if(*firstGoodVertexIdx_ >= 0) dZ = mu.muonBestTrack()->dz( vertices->at(*firstGoodVertexIdx_).position() );
      MuDz_->push_back(dZ);

      // if(mu.isGlobalMuon()){
      //   MuTkNormChi2_->push_back(mu.globalTrack()->normalizedChi2());
      // 	MuTkHitCnt_->push_back(mu.globalTrack()->hitPattern().numberOfValidMuonHits());
      // } else {
      // 	MuTkNormChi2_->push_back(-1);
      // 	MuTkHitCnt_->push_back(-1);
      // }	
      // MuMatchedStationCnt_->push_back(mu.numberOfMatchedStations());
      // MuPixelHitCnt_->push_back(mu.innerTrack()->hitPattern().numberOfValidPixelHits());
      // MuTkLayerCnt_->push_back(mu.innerTrack()->hitPattern().trackerLayersWithMeasurement());      

    }
  }
}

void Tupel::processJets(){
  if(jets.failedToGet() || !jets.isValid()){
    printf("processJets failed\n");
    failedJets++;
    return;
  }

  double NHF = 0;
  double NEMF = 0;
  double NCONST = 0;
  double CHF = 0;
  double CMULTI = 0;
  
  // Reading in JES uncertainties w/ associated input GT on the fly
  JetCorrectorParameters const & JetCorParAK4 = (*JetCorParCollAK4)["Uncertainty"];
  JetCorrectionUncertainty *jecUncAK4 = new JetCorrectionUncertainty(JetCorParAK4);
  
  for(unsigned int i=0; i<jets->size(); ++i){
    const pat::Jet & jet = jets->at(i);

    // grabbing associated gen jet in MC?
    // used for jet matching ???
    // const reco::GenJet* ref = jet.genJet();

    // if(i==0 && analyzedEventCnt_== 1){
    //   std::cout << "\n--> Jet user float list:\n";
    //   for(unsigned j = 0; j < jet.userFloatNames().size(); ++j){
	  //     std::cout << jet.userFloatNames()[j] << "\n";
    //   }
    //   std::cout << "\n--> Jet user int list:\n";
    //   for(unsigned j = 0; j < jet.userIntNames().size(); ++j){
	  //     std::cout << jet.userIntNames()[j] << "\n";
    //   }
    // }

    //Soft cut on pt and eta to reduce Baobab size
    if(jet.isPFJet() && jet.pt() > 20.0 && fabs(jet.eta()) < 2.7){

      // jet ID  --------
      // the ""...EnergyFraction" functions grab the raw, uncorrected energy fractions
      // which is what we want when computing jet ID
      NHF = jet.neutralHadronEnergyFraction();
      NEMF = jet.neutralEmEnergyFraction();
      NCONST = jet.chargedMultiplicity() + jet.neutralMultiplicity();
      CHF = jet.chargedHadronEnergyFraction();
      CMULTI = jet.chargedMultiplicity();
      
      // 2017 jet ID is by default tight ID
      double tempJetID=0;
      if( abs(jet.eta()) <= 2.4 ){
        if ( (NHF < 0.90) && (NEMF < 0.90) && (NCONST > 1) && (CHF > 0) && (CMULTI > 0) ) tempJetID=1;
      }

      JetAk04Id_->push_back(tempJetID);

      // jet PU MVA ID --------
      if (jet.hasUserInt("pileupJetId:fullId")){
        JetAk04PuId_->push_back(jet.userInt("pileupJetId:fullId"));  
      }
      else JetAk04PuId_->push_back(0); 

      // try this method instead
      bool jetPuIdLoose = false;
      bool jetPuIdMedium = false;
      bool jetPuIdTight = false;
      if (jet.hasUserInt("pileupJetId:fullId")){
        jetPuIdLoose = ( jet.userInt("pileupJetId:fullId") & (1 << 2) );
        jetPuIdMedium = ( jet.userInt("pileupJetId:fullId") & (1 << 1) );
        jetPuIdTight = ( jet.userInt("pileupJetId:fullId") & (1 << 0) );
      }
      JetAk04PuIdLoose_->push_back(jetPuIdLoose); 
      JetAk04PuIdMedium_->push_back(jetPuIdMedium); 
      JetAk04PuIdTight_->push_back(jetPuIdTight); 

      // and the full score
      if(jet.hasUserFloat("pileupJetId:fullDiscriminant")) {
	      JetAk04PuMva_->push_back(jet.userFloat("pileupJetId:fullDiscriminant"));
      }
      else JetAk04PuMva_->push_back(-99.);

      //JEC uncertainty --------
      jecUncAK4->setJetEta(jet.eta());
      jecUncAK4->setJetPt(jet.pt()); // here you must use the CORRECTED jet pt
      double unc = jecUncAK4->getUncertainty(true);
      // if(DJALOG_) printf("JEC Unc, Jet %d = %F\n", i, unc);
      // if(DJALOG_) printf("JEC Unc, Up = %F, Down = %F\n",1+unc,1-unc);
      JetAk04JecUncUp_->push_back(1. + unc);
      JetAk04JecUncDwn_->push_back(1. - unc);

      //b-tag score, hadron flavor --------
      JetAk04BDiscCisvV2_->push_back(jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      JetAk04HadFlav_->push_back(jet.hadronFlavour());

      //kinematics --------
      JetAk04E_->push_back(jet.energy());
      JetAk04Pt_->push_back(jet.pt());
      JetAk04Eta_->push_back(jet.eta());
      // JetAk04Rap_->push_back(jet.rapidity());
      JetAk04Phi_->push_back(jet.phi());

      //kinematics of uncorrected jet --------
      reco::Candidate::LorentzVector uncorrJet = jet.correctedP4(0);
      JetAk04PtUncorr_->push_back(uncorrJet.pt());
      JetAk04EtaUncorr_->push_back(uncorrJet.eta());
      JetAk04EUncorr_->push_back(uncorrJet.energy());

      // can also use
      // j.pt()*j.jecFactor("Uncorrected")
      // to get raw jet pt
    
    }
  }
  delete jecUncAK4;
}

void Tupel::processJetsAK8(){
  if(jetsAK8.failedToGet() || !jetsAK8.isValid()){
    printf("processJetsAK8 failed\n");
    failedJets++;
    return;
  }

  // No jet PU MVA for PUPPI collection

  double NHF = 0;
  double NEMF = 0;
  double NCONST = 0;
  double CHF = 0;
  double CMULTI = 0;

  JetCorrectorParameters const & JetCorParAK8 = (*JetCorParCollAK8)["Uncertainty"];
  JetCorrectionUncertainty *jecUncAK8 = new JetCorrectionUncertainty(JetCorParAK8);
  
  for(unsigned int i=0; i<jetsAK8->size(); ++i){
    const pat::Jet & jetAK8 = jetsAK8->at(i);

    // if(i==0 && analyzedEventCnt_== 2){
    //   std::cout << "\n--> Jet AK8 user float list:\n";
    //   for(unsigned j = 0; j < jetAK8.userFloatNames().size(); ++j){
	  //     std::cout << jetAK8.userFloatNames()[j] << "\n";
    //   }
    //   std::cout << std::endl;
    // }

    // PF jets in AK8 collection start at pT of 200 GeV 
    // (PF jets start at 170, but should start at 200 GeV if JECs reapplied)
    if(jetAK8.isPFJet() && jetAK8.pt() > 200.0 && fabs(jetAK8.eta()) < 2.7){

      // jet ID --------
      NHF = jetAK8.neutralHadronEnergyFraction();
      NEMF = jetAK8.neutralEmEnergyFraction();
      NCONST = jetAK8.chargedMultiplicity() + jetAK8.neutralMultiplicity();
      CHF = jetAK8.chargedHadronEnergyFraction();
      CMULTI = jetAK8.chargedMultiplicity();
      
      // 2017 jet ID is by default tight ID
      // applying the same for AK8 that exists for AK4
      double tempJetID = 0;
      if( abs(jetAK8.eta()) <= 2.4 ){
        if ( (NHF < 0.90) && (NEMF < 0.90) && (NCONST > 1) && (CHF > 0) && (CMULTI > 0) ) tempJetID=1;
      }
      JetAk08Id_->push_back(tempJetID);

      //JEC uncertainty - AK8 jets
      jecUncAK8->setJetEta(jetAK8.eta());
      jecUncAK8->setJetPt(jetAK8.pt()); // here you must use the CORRECTED jet pt
      double unc = jecUncAK8->getUncertainty(true);
      // if(DJALOG_) printf("JEC Unc, Jet %d = %F\n", i, unc);
      // if(DJALOG_) printf("JEC Unc, Up = %F, Down = %F\n",1+unc,1-unc);
      JetAk08JecUncUp_->push_back(1 + unc);
      JetAk08JecUncDwn_->push_back(1 - unc);

      // jet flavour  --------
      JetAk08BDiscCisvV2_->push_back(jetAK8.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
      JetAk08HadFlav_->push_back(jetAK8.hadronFlavour());

      //kinematics --------
      JetAk08E_->push_back(jetAK8.energy());
      JetAk08Pt_->push_back(jetAK8.pt());
      JetAk08Eta_->push_back(jetAK8.eta());
      // JetAk08Rap_->push_back(jetAK8.rapidity());
      JetAk08Phi_->push_back(jetAK8.phi());

      // grab CHS quantities instead of PUPPI here --------
      if(jetAK8.hasUserFloat("ak8PFJetsCHSValueMap:pt")) {
        float jetAK8CHSpT = jetAK8.userFloat("ak8PFJetsCHSValueMap:pt");
        JetAk08CHSPt_->push_back(jetAK8CHSpT);
      }
      if(jetAK8.hasUserFloat("ak8PFJetsCHSValueMap:eta")) {
        float jetAK8CHSeta = jetAK8.userFloat("ak8PFJetsCHSValueMap:eta");
        JetAk08CHSEta_->push_back(jetAK8CHSeta);
      }
      if(jetAK8.hasUserFloat("ak8PFJetsCHSValueMap:phi")) {
        float jetAK8CHSphi = jetAK8.userFloat("ak8PFJetsCHSValueMap:phi");
        JetAk08CHSPhi_->push_back(jetAK8CHSphi);
      }

      //kinematics of uncorrected jet --------
      reco::Candidate::LorentzVector uncorrJetAK8 = jetAK8.correctedP4(0);
      JetAk08PtUncorr_->push_back(uncorrJetAK8.pt());
      JetAk08EtaUncorr_->push_back(uncorrJetAK8.eta());
      JetAk08EUncorr_->push_back(uncorrJetAK8.energy());

    }
  }
}

///////////////////////////////////////////////////////////////////////////////
/// EDAnalyzer Functions

void Tupel::beginJob(){
  // First setup for the output TTree
  // Writes "Header" in output TTree in beginJob
  writeHeader();
  dataTree = fs->make<TTree>("EventTree", "EventTree");
  descTree = fs->make<TTree>("Description", "Description");
  bitTree  = fs->make<TTree>("BitFields", "BitFields");
  treeHelper_ = new TreeHelper(dataTree, descTree, bitTree);

  // Now, the addition of branches for filling in the analysis code ---
  //Event
  ADD_BRANCH_D(EvtIsRealData, "True if real data, false if MC");
  ADD_BRANCH_D(EvtNum, "Event number");
  ADD_BRANCH_D(EvtRunNum, "Run number");
  ADD_BRANCH_D(EvtLumiNum, "Luminosity block number");
  ADD_BRANCH_D(EvtBxNum, "Bunch crossing number");
  ADD_BRANCH_D(EvtVtxCnt, "Number of reconstructed primary vertices");
  ADD_BRANCH_D(EvtPuCntObs, "Number of observed/measured pile-up events");
  ADD_BRANCH_D(EvtPuCntTruth, "True number of pile-up events");
  ADD_BRANCH(EvtWeights); //description filled in endRun()
  // ADD_BRANCH(originalXWGTUP);
  ADD_BRANCH_D(EvtFastJetRho, "Fastjet pile-up variable \\rho");
  ADD_BRANCH(firstGoodVertexIdx);

  //Trigger
  ADD_BRANCH_D(TrigHlt, "HLT triggger bits. See BitField.TrigHlt for bit description."); 
  ADD_BRANCH_D(TrigHltMu, "HLT Muon triggger bits. See BitField.TrigHltMu for bit description.");
  ADD_BRANCH_D(TrigHltMu_prescale, "HLT Muon triggger prescales for the corresponding trigger bits. See BitField.TrigHltMu for bit description.");

  ADD_BRANCH(TrigMET);
  ADD_BRANCH_D(TrigMETBit, "MET filter bits. See BitField.TrigMETBit for bit description.");
  ADD_BRANCH_D(TrigMETBit_prescale, "MET filter prescales for the corresponding trigger bits. See BitField.TrigHltPhot for bit description.");

  ADD_BRANCH_D(PreFiringWeight,     "L1 Prefire Weight");
  ADD_BRANCH_D(PreFiringWeightUp,   "L1 Prefire Weight Up");
  ADD_BRANCH_D(PreFiringWeightDown, "L1 Prefire Weight Down");

  ////////////////// GENERATOR LEVEL

  //Generator level leptons.
  treeHelper_->addDescription("GLepBare", "Generator-level leptons, status 1 without dressing.");
  ADD_BRANCH(GLepBarePt);
  ADD_BRANCH(GLepBareEta);
  ADD_BRANCH(GLepBarePhi);
  ADD_BRANCH(GLepBareE);
  ADD_BRANCH(GLepBareId);
  ADD_BRANCH(GLepBareSt);
  ADD_BRANCH(GLepBarePrompt);

  //Photons in the vicinity of the leptons
  treeHelper_->addDescription("GLepClosePhot", "Photons aroud leptons. Selection cone size: R = 0.2");
  ADD_BRANCH(GLepClosePhotPt);
  ADD_BRANCH(GLepClosePhotEta);
  ADD_BRANCH(GLepClosePhotPhi);
  ADD_BRANCH(GLepClosePhotE);
  ADD_BRANCH(GLepClosePhotId);
  ADD_BRANCH_D(GLepClosePhotMother0Id, "Photon mother PDG Id. Filled only for first mother. Number of mothers can be checked in GLepoSt3MotherCnt.");
  ADD_BRANCH(GLepClosePhotMotherCnt);
  ADD_BRANCH(GLepClosePhotSt);

  // Generator-level MET
  ADD_BRANCH(GMETPt);
  ADD_BRANCH(GMETPx);
  ADD_BRANCH(GMETPy);
  // ADD_BRANCH(GMETPz);
  ADD_BRANCH(GMETE);
  ADD_BRANCH(GMETPhi);

  //Gen Jets, AK4
  treeHelper_->addDescription("GJetAk04", "Generator-level reconstructed with the anti-kt algorithm with distance parameter R = 0.4");
  ADD_BRANCH(GJetAk04Pt);
  ADD_BRANCH(GJetAk04Eta);
  ADD_BRANCH(GJetAk04Phi);
  ADD_BRANCH(GJetAk04E);

  //Gen Jets, AK8
  treeHelper_->addDescription("GJetAk08", "Generator-level reconstructed with the anti-kt algorithm with distance parameter R = 0.8");
  ADD_BRANCH(GJetAk08Pt);
  ADD_BRANCH(GJetAk08Eta);
  ADD_BRANCH(GJetAk08Phi);
  ADD_BRANCH(GJetAk08E);

  //Extra generator information
  ADD_BRANCH_D(GNup, "Number of particles/partons included in the matrix element.");
  //ADD_BRANCH_D(npLO, "npLO");
  ADD_BRANCH_D(npNLO,"npNLO");

  // ADD_BRANCH(LHEZid);
  // ADD_BRANCH(LHEZStatus);
  // ADD_BRANCH(LHEZPx);
  // ADD_BRANCH(LHEZPy);
  // ADD_BRANCH(LHEZPz);
  // ADD_BRANCH(LHEZE);

  ////////////////// RECO LEVEL

  //Muons
  treeHelper_->addDescription("Mu", "PF reconstruced muons.");
  ADD_BRANCH(MuPt);
  ADD_BRANCH(MuEta);
  ADD_BRANCH(MuPhi);
  ADD_BRANCH(MuE);
  ADD_BRANCH(MuIdLoose);
  ADD_BRANCH(MuIdMedium);
  ADD_BRANCH_D(MuIdTight, "Muon tight id. Bit field, one bit per primary vertex hypothesis. Bit position corresponds to index in EvtVtx");
  ADD_BRANCH(MuCh);
  ADD_BRANCH(MuVtxZ);
  ADD_BRANCH(MuDxy);
  ADD_BRANCH(MuPfIso);
  ADD_BRANCH(MuDz);
  ADD_BRANCH(MuHltMatch);
  // ADD_BRANCH(MuTkNormChi2);
  // ADD_BRANCH(MuTkHitCnt);
  // ADD_BRANCH(MuMatchedStationCnt);
  // ADD_BRANCH(MuPixelHitCnt);
  // ADD_BRANCH(MuTkLayerCnt);

  //MET
  ADD_BRANCH(METPt);
  ADD_BRANCH(METPx);
  ADD_BRANCH(METPy);
  // ADD_BRANCH(METPz);
  ADD_BRANCH(METE);
  ADD_BRANCH(METPhi);
  // ADD_BRANCH(METEta);

  //PF Jets - AK4
  treeHelper_->addDescription("JetAk04", "Reconstricuted jets clustered with the anti-ket algorithm with distance parameter R = 0.4");
  ADD_BRANCH(JetAk04Pt);
  ADD_BRANCH(JetAk04Eta);
  // ADD_BRANCH(JetAk04Rap);
  ADD_BRANCH(JetAk04Phi);
  ADD_BRANCH(JetAk04E);
  ADD_BRANCH_D(JetAk04Id, "Id to reject fake jets from electronic noise");
  ADD_BRANCH_D(JetAk04PuMva, "MVA based descriminant for PU jets");
  ADD_BRANCH_D(JetAk04PuId, "MVA based ID for PU jets");
  ADD_BRANCH(JetAk04PuIdLoose);
  ADD_BRANCH(JetAk04PuIdMedium);
  ADD_BRANCH(JetAk04PuIdTight);
  treeHelper_->addDescription("JetAk04BTag", "B tagging with different algorithms");
  ADD_BRANCH_D(JetAk04BDiscCisvV2, "pfCombinedInclusiveSecondaryVertexV2BJetTags");
  ADD_BRANCH_D(JetAk04HadFlav, "Hadron-based jet flavor.");
  ADD_BRANCH(JetAk04JecUncUp);
  ADD_BRANCH(JetAk04JecUncDwn);
  ADD_BRANCH(JetAk04PtUncorr);
  ADD_BRANCH(JetAk04EtaUncorr);
  ADD_BRANCH(JetAk04EUncorr);

  //PF Jets - AK8
  treeHelper_->addDescription("JetAk08", "Reconstricuted jets clustered with the anti-ket algorithm with distance parameter R = 0.8");
  ADD_BRANCH(JetAk08Pt);
  ADD_BRANCH(JetAk08Eta);
  // ADD_BRANCH(JetAk08Rap);
  ADD_BRANCH(JetAk08Phi);
  ADD_BRANCH(JetAk08E);
  ADD_BRANCH(JetAk08Id);
  ADD_BRANCH(JetAk08BDiscCisvV2);
  ADD_BRANCH(JetAk08HadFlav);
  ADD_BRANCH(JetAk08CHSPt);
  ADD_BRANCH(JetAk08CHSEta);
  ADD_BRANCH(JetAk08CHSPhi);
  ADD_BRANCH(JetAk08JecUncUp);
  ADD_BRANCH(JetAk08JecUncDwn);
  ADD_BRANCH(JetAk08PtUncorr);
  ADD_BRANCH(JetAk08EtaUncorr);
  ADD_BRANCH(JetAk08EUncorr);

  // Defining trigger bits
  defineBitFields();
}

void Tupel::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  ++analyzedEventCnt_;

  if(DJALOG_) std::cout << "\n ------------ New Event #" << analyzedEventCnt_ << " ----------- "  << std::endl;  
  readEvent(iEvent);

  //Get the event weights for MC
  if(!*EvtIsRealData_){
    if(DJALOG_) std::cout << "\n ~~~ processLHE() ~~~ "  << std::endl;
    processLHE(iEvent);
  }

  if(DJALOG_) std::cout << "\n ~~~ processVtx() ~~~ "  << std::endl;
  processVtx(iEvent);
  
  if(!*EvtIsRealData_){
    if(DJALOG_) std::cout << "\n ~~~ processGenJets() ~~~ "  << std::endl;
    processGenJets(iEvent);
    if(DJALOG_) std::cout << "\n ~~~ processGenJetsAK8() ~~~ "  << std::endl;
    processGenJetsAK8(iEvent);
    if(DJALOG_) std::cout << "\n ~~~ processGenParticles() ~~~ "  << std::endl;
    processGenParticles(iEvent);
  }
  
  if(DJALOG_) std::cout << "\n ~~~ processMET() ~~~ " << std::endl;
  if (!mets.failedToGet()) processMET(iEvent);
  
  if (!*EvtIsRealData_) {
    if(DJALOG_) std::cout << "\n ~~~ processPu() ~~~ "  << std::endl;
    processPu(iEvent);  
  }     

  if(DJALOG_) std::cout << "\n ~~~ processTrigger() ~~~ "  << std::endl;
  processTrigger(iEvent);

  if(DJALOG_) std::cout << "\n ~~~ processMETFilter() ~~~ "  << std::endl;
  if (!metfilters.failedToGet()) processMETFilter(iEvent);

  if(DJALOG_) std::cout << "\n ~~~ processMuons() ~~~ "  << std::endl;
  if (!muons.failedToGet()) processMuons(iEvent);

  if(DJALOG_) std::cout << "\n ~~~ processJets() ~~~ "  << std::endl;
  iSetup.get<JetCorrectionsRecord>().get("AK4PFchs", JetCorParCollAK4); 
  processJets();

  if(DJALOG_) std::cout << "\n ~~~ processJetsAK8() ~~~ "  << std::endl;
  iSetup.get<JetCorrectionsRecord>().get("AK8PFPuppi", JetCorParCollAK8); 
  processJetsAK8();

  //Stores the EvtNum in the output tree
  treeHelper_->fill();
}

void Tupel::endJob(){
  treeHelper_->fillDescriptionTree();
  if(triggerStat_) writeTriggerStat();
}

void Tupel::endRun(edm::Run const& iRun, edm::EventSetup const&){

  std::string desc = "List of MC event weights. Use the first one by default.";
  desc += " The first element contains the GenInfoProduct weight, or it was not found and set to 1.";
  if(weightsFromLhe_){
    desc += "Elements starting from index 1 contains the weights from LHEEventProduct.";
  }

  // Get information about LHE weights --
  // edm::Handle<LHERunInfoProduct> lheRun;
  // iRun.getByToken(lheRunToken_, lheRun );
  // if(!lheRun.failedToGet()){
  //   const LHERunInfoProduct& myLHERunInfoProduct = *(lheRun.product());
  //   for (std::vector<LHERunInfoProduct::Header>::const_iterator iter = myLHERunInfoProduct.headers_begin(); iter != myLHERunInfoProduct.headers_end(); iter++){
  //     std::cout << iter->tag() << std::endl;
  //     std::vector<std::string> lines = iter->lines();
  //     for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
  //       std::cout << lines.at(iLine);
  //     }
  //   }
  // }

  // Description of LHE weights is here in the tree
  treeHelper_->addDescription("EvtWeights", desc.c_str());
  
  printf("\n >>>>> Failed: Vtx=%d, Gen=%d, LHE=%d, Genjets=%d, GenjetsAK8=%d, Muons=%d, Jets=%d, MET=%d\n\n", failedVtx, failedGen, failedLHE, failedGenJets, failedGenJetsAK8, failedMuons, failedJets, failedMET);
}

DEFINE_FWK_MODULE(Tupel);
