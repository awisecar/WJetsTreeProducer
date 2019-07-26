import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

####################

options = VarParsing.VarParsing()
options.register('year', '2017', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Year of data/MC to process.')
options.register('isData', '0', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Switch to run on real data (isData=1) or MC (isData=0).')
options.parseArguments()

# Print out all options
print("\n>>>>>>> Options:")
print("year = "+str(options.year))
print("isData = "+str(options.isData))
print("")

####################

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

if (options.isData == 1):
  if (options.year == "2016"):
    print(">>>>>>> Using 2016 real data sample")
    # 2016 03Feb2017 reMiniAOD, SingleMuon dataset
    inputFilename = "/store/data/Run2016D/SingleMuon/MINIAOD/03Feb2017-v1/80000/FE050076-2FEB-E611-8E4A-0025905C5476.root"
  elif (options.year == "2017"):
    print(">>>>>>> Using 2017 real data sample")
    # 2017 31Mar2018 reMiniAOD, SingleMuon dataset
    inputFilename = "root://xrootd-cms.infn.it//store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root"
else:
  if (options.year == "2017"):
    print(">>>>>>> Using 2017 W+Jets MC sample")
    # RunIIFall17MiniAODv2 campaign, WJets MLM sample
    inputFilename = "/store/mc/RunIIFall17MiniAODv2/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/100000/7A8364F3-A394-E811-8419-0CC47A78A418.root"


process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(inputFilename)
)

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(30000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

outputFilename = "ntupleTest"
if (options.year == "2016"):
  outputFilename += "_2016"
elif (options.year == "2017"):
  outputFilename += "_2017"
if (options.isData == 1):
  outputFilename += "_DATA"
elif (options.isData == 0):
  outputFilename += "_MC"
outputFilename += ".root"

process.TFileService = cms.Service("TFileService",
  # fileName = cms.string("ntupleTest_"+str(options.year)+".root")
  fileName = cms.string(outputFilename)
)

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

# process.GlobalTag.globaltag = '94X_dataRun2_v11' #data
process.GlobalTag.globaltag = '94X_mc2017_realistic_v17' #MC

#--------------------------------------------                                                                       

# 2017 EE noise MET fix
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD (
  process,
  isData = False, # false for MC
  fixEE2017 = True,
  fixEE2017Params = {'userawPt': True, 'ptThreshold':50.0, 'minEtaThreshold':2.65, 'maxEtaThreshold': 3.139} ,
  postfix = "ModifiedMET"
)

process.tupel = cms.EDAnalyzer("Tupel",
  yearToProcess = cms.untracked.string(options.year),
  ##### Vertex information, HLT Trigger Bits
  vertexSrc        = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'), 
  triggerSrc       = cms.InputTag("TriggerResults", "", "HLT"),
  ##### Muons, Jets, MET
  muonSrc      = cms.untracked.InputTag("slimmedMuons"),
  jetSrc       = cms.untracked.InputTag("slimmedJets"), #default ak4 chs jet colleciton in miniAOD
  jetAK8Src    = cms.untracked.InputTag("slimmedJetsAK8"), #default ak8 puppi jet colleciton in miniAOD
  # jetSrc       = cms.untracked.InputTag("updatedPatJetsUpdatedJECAK4PFchs"), #updated with JECs
  # jetAK8Src    = cms.untracked.InputTag("updatedPatJetsUpdatedJECAK8PFPuppi"), #updated with JECs
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
  ##### GEN objects, MC truth pileup information
  genInfoSrc       = cms.untracked.InputTag('generator'),
  lheSrc       = cms.untracked.InputTag('externalLHEProducer'),
  puSrc        = cms.untracked.InputTag('slimmedAddPileupInfo'),
  genSrc       = cms.untracked.InputTag("prunedGenParticles"),
  gjetSrc      = cms.untracked.InputTag('slimmedGenJets'),
  gjetAK8Src      = cms.untracked.InputTag('slimmedGenJetsAK8'),

  ##### Other stuff
  mSrcRho      = cms.untracked.InputTag('fixedGridRhoFastjetAll'),
  triggerObjectTag = cms.untracked.InputTag("slimmedPatTrigger"),
  triggerStat  = cms.untracked.bool(False), #more information on trigger statistics (?)
  ##### Extra printout statements
  DJALOG       = cms.untracked.bool(False)
)

# we are not running in unscheduled mode (?)
process.p = cms.Path(
  process.fullPatMetSequenceModifiedMET *
  process.tupel 
)
