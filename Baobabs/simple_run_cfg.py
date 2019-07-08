import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("BAOBAB")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "root://xrootd-cms.infn.it//store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root"
))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('ntupleTest.root' )
)

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
# For 2017 real data:
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_v11', '')

#--------------------------------------------                                                                       

# process.prefiringweight = cms.EDProducer("L1ECALPrefiringWeightProducer",
#     ThePhotons = cms.InputTag("slimmedPhotons"),
#     TheJets = cms.InputTag("slimmedJets"),
#     L1Maps = cms.string("L1PrefiringMaps_new.root"), # update this line with the location of this file
#     DataEra = cms.string("2016BtoH"), #Use 2016BtoH for 2016
#     UseJetEMPt = cms.bool(False), #can be set to true to use jet prefiring maps parametrized vs pt(em) instead of pt
#     PrefiringRateSystematicUncty = cms.double(0.2) #Minimum relative prefiring uncty per object
# )

process.tupel = cms.EDAnalyzer("Tupel",
  ##### (Primary) Vertex information, PU info, HLT Trigger Bits
  vertexSrc        = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'), 
  puSrc        = cms.untracked.InputTag('slimmedAddPileupInfo'),
  triggerSrc       = cms.InputTag("TriggerResults", "", "HLT"),
  ##### Muons, Jets, MET
  muonSrc      = cms.untracked.InputTag("slimmedMuons"),
  jetSrc       = cms.untracked.InputTag("slimmedJets"),
  jetAK8Src    = cms.untracked.InputTag("slimmedJetsAK8"),
  metSrc       = cms.untracked.InputTag("slimmedMETs"),
  # metSrc       = cms.untracked.InputTag("slimmedMETsPuppi"),
  ##### MET Filters
  noiseFilterTag = cms.InputTag("TriggerResults","","PAT"),
  GoodVtxNoiseFilter_Selector_ = cms.string("Flag_goodVertices"),
  GlobalSuperTightHalo2016NoiseFilter_Selector_ = cms.string("Flag_globalSuperTightHalo2016Filter"),
  HBHENoiseFilter_Selector_ = cms.string("Flag_HBHENoiseFilter"),
  HBHENoiseIsoFilter_Selector_ = cms.string("Flag_HBHENoiseIsoFilter"),
  EcalDeadCellTriggerPrimitiveNoiseFilter_Selector_ = cms.string("Flag_EcalDeadCellTriggerPrimitiveFilter"),
  BadPFMuonFilter_Selector_ = cms.string("Flag_BadPFMuonFilter"),
  EEBadScNoiseFilter_Selector_ = cms.string("Flag_eeBadScFilter"),
  ##### GEN objects, other options
  genSrc       = cms.untracked.InputTag("prunedGenParticles"),
  gjetSrc      = cms.untracked.InputTag('slimmedGenJets'),
  mSrcRho      = cms.untracked.InputTag('fixedGridRhoFastjetAll'),
  lheSrc       = cms.untracked.InputTag('externalLHEProducer'),
  triggerObjectTag = cms.untracked.InputTag("slimmedPatTrigger"),
  triggerMenu = cms.untracked.string("2017"),
  triggerStat  = cms.untracked.bool(False), #more information on trigger statistics (?)
  puMvaName    = cms.untracked.string('pileupJetId:fullDiscriminan'),
  puJetIdSrc   = cms.untracked.InputTag("pileupJetId"),
  
  ##### Extra printout statements
  DJALOG       = cms.untracked.bool(False)
)

process.p = cms.Path(
 process.tupel 
)
