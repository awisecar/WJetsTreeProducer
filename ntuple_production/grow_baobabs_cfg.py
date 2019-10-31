import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

################################################################################
## OPTIONS PARSING
##

options = VarParsing.VarParsing('analysis')
options.register('year', '2017', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Year of data/MC to process.')
options.register('isData', '1', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Switch to run on real data (isData=1) or MC (isData=0).')
options.parseArguments()

################################################################################
## SETUP
##

process = cms.Process("GrowBaobabs")

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
  fileNames = cms.untracked.vstring(options.inputFiles)
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(options.maxEvents)
)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntupleOut.root')
)

globalTagString = ""
if (options.isData == 1):
  globalTagString += "94X_dataRun2_v11"
elif (options.isData == 0):
  globalTagString += "94X_mc2017_realistic_v17"

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

process.GlobalTag.globaltag = globalTagString

########################################


################################################################################
## MAIN ANALYZER
##

process.tupel = cms.EDAnalyzer("Tupel",
  yearToProcess = cms.untracked.string(options.year),
  ##### Vertex information, HLT Trigger Bits
  vertexSrc        = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'), 
  triggerSrc       = cms.InputTag("TriggerResults", "", "HLT"),
  triggerObjectTag = cms.untracked.InputTag("slimmedPatTrigger"),
  triggerPrescalesTag = cms.untracked.InputTag("patTrigger"),
  ## HLT paths (need to be set by year; currently 2017)
  muonHLTTriggerPath1 = cms.untracked.string("HLT_IsoMu24_v"),
  muonHLTTriggerPath2 = cms.untracked.string("HLT_IsoMu27_v"),
  muonHLTTriggerPath3 = cms.untracked.string("HLT_Mu27_v"),
  ##### Muons, Jets, MET
  muonSrc      = cms.untracked.InputTag("slimmedMuons"),
  jetSrc       = cms.untracked.InputTag("slimmedJets"), #default ak4 chs jet colleciton in miniAOD
  jetAK8Src    = cms.untracked.InputTag("slimmedJetsAK8"), #default ak8 puppi jet colleciton in miniAOD
  metSrc       = cms.untracked.InputTag("slimmedMETs"),
  ##### MET Filters
  noiseFilterTag = cms.InputTag("TriggerResults","","PAT"),
  ##### GEN objects, MC truth pileup information
  genInfoSrc       = cms.untracked.InputTag('generator'),
  lheSrc       = cms.untracked.InputTag('externalLHEProducer'),
  puSrc        = cms.untracked.InputTag('slimmedAddPileupInfo'),
  genSrc       = cms.untracked.InputTag("prunedGenParticles"),
  gjetSrc      = cms.untracked.InputTag('slimmedGenJets'),
  gjetAK8Src      = cms.untracked.InputTag('slimmedGenJetsAK8'),
  ##### Other
  mSrcRho      = cms.untracked.InputTag('fixedGridRhoFastjetAll'),
  ##### Verbosity of output
  DJALOG       = cms.untracked.bool(False),
  printLHEWeightsInfo = cms.untracked.bool(False)
)

########################################

process.p = cms.Path(
 process.tupel 
)



