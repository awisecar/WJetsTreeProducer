import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

########################################

# Additional VarParsing Option switches
opt = VarParsing.VarParsing ('analysis')
opt.register('isMC', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Flag indicating if the input samples are from MC (1) or from the detector (0).')
opt.register('year', '2017', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Year of data/MC to process.')
opt.parseArguments()

########################################

process = cms.Process("GrowBaobabs")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

if (opt.year == "2016"):
  print(">>>>>>> Using 2016 real data sample")
  # 2016 03Feb2017 reMiniAOD, SingleMuon dataset
  inputFilename = "/store/data/Run2016D/SingleMuon/MINIAOD/03Feb2017-v1/80000/FE050076-2FEB-E611-8E4A-0025905C5476.root"
elif (opt.year == "2017"):
  print(">>>>>>> Using 2017 real data sample")
  # 2017 31Mar2018 reMiniAOD, SingleMuon dataset
  inputFilename = "/store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root"

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(inputFilename)
)

# opt.maxEvents set as -1 in the corresponding CRAB cfg file
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(opt.maxEvents))

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntupleOut.root')
)

## Global tags
# For 2017 real data, re-miniAOD 31Mar2018 --
dataGlobalTag ='94X_dataRun2_v11'
# For 2017 MC, RunIIFall17MiniAODv2 campaign --
mcGlobalTag = '94X_mc2017_realistic_v17'

#Condition DB tag
from Configuration.AlCa.GlobalTag import GlobalTag
if opt.isMC == 1:
  process.GlobalTag.globaltag = mcGlobalTag
else:
  process.GlobalTag.globaltag = dataGlobalTag

########################################


################
# MAIN ANALYZER

process.tupel = cms.EDAnalyzer("Tupel",
  yearToProcess = cms.untracked.string(opt.year),
  ##### Vertex information, HLT Trigger Bits
  vertexSrc        = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'), 
  triggerSrc       = cms.InputTag("TriggerResults", "", "HLT"),
  triggerObjectTag = cms.untracked.InputTag("slimmedPatTrigger"),
  triggerPrescalesTag = cms.untracked.InputTag("patTrigger"),
  ## HLT paths (need to be set by year; currently 2017)
  muonHLTTriggerPath1 = cms.untracked.string("HLT_IsoMu24_v6"),
  muonHLTTriggerPath2 = cms.untracked.string("HLT_IsoMu27_v9"),
  ##### Muons, Jets, MET
  muonSrc      = cms.untracked.InputTag("slimmedMuons"),
  jetSrc       = cms.untracked.InputTag("slimmedJets"), #default ak4 chs jet colleciton in miniAOD
  jetAK8Src    = cms.untracked.InputTag("slimmedJetsAK8"), #default ak8 puppi jet colleciton in miniAOD
  metSrc       = cms.untracked.InputTag("slimmedMETs"),
  mSrcRho      = cms.untracked.InputTag('fixedGridRhoFastjetAll'),
  ##### MET Filters
  noiseFilterTag = cms.InputTag("TriggerResults","","PAT"),
  ##### GEN objects, MC truth pileup information
  genInfoSrc       = cms.untracked.InputTag('generator'),
  lheSrc       = cms.untracked.InputTag('externalLHEProducer'),
  puSrc        = cms.untracked.InputTag('slimmedAddPileupInfo'),
  genSrc       = cms.untracked.InputTag("prunedGenParticles"),
  gjetSrc      = cms.untracked.InputTag('slimmedGenJets'),
  gjetAK8Src      = cms.untracked.InputTag('slimmedGenJetsAK8'),
  ##### Other stuff
  DJALOG       = cms.untracked.bool(False)
)

########################################

#process.p = cms.Path(
#  process.tupel 
#)

process.p = cms.Path()
process.p += process.tupel

