import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import re

########################################

# Additional VarParsing Option switches
opt = VarParsing.VarParsing ('analysis')
opt.register('isMC', -1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Flag indicating if the input samples are from MC (1) or from the detector (0).')
opt.register('year', '2017', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Year of data/MC to process.')
opt.parseArguments()

# Print out all options
print("\n>>>>>>> Options:")
print("year = "+str(opt.year))
print("isMC = "+str(opt.isMC))
print("maxEvents = "+str(opt.maxEvents))
print("")

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
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

if (opt.year == "2016"):
  print(">>>>>>> Using 2016 real data sample")
  # 2016 03Feb2017 reMiniAOD, SingleMuon dataset
  inputFilename = "/store/data/Run2016D/SingleMuon/MINIAOD/03Feb2017-v1/80000/FE050076-2FEB-E611-8E4A-0025905C5476.root"
elif (opt.year == "2017"):
  print(">>>>>>> Using 2017 real data sample")
  # 2017 31Mar2018 reMiniAOD, SingleMuon dataset
  inputFilename = "root://xrootd-cms.infn.it//store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root"

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(inputFilename)
)

# opt.maxEvents set as -1 in the corresponding CRAB cfg file
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(opt.maxEvents))

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntupleOut.root')
)

if opt.isMC < 0 and len(process.source.fileNames) > 0:
  if re.match(r'.*/(MINI)?AODSIM/.*', process.source.fileNames[0]):
    print "MC dataset detected."
    opt.isMC = 1
  elif re.match(r'.*/(MINI)?AOD/.*', process.source.fileNames[0]):
    print "Real data dataset detected."
    opt.isMC = 0

if opt.isMC < 0:
  raise Exception("Failed to detect data type. Data type need to be specify with the isMC cmsRun command line option")

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

########################
# MODULES FOR REAL DATA

#Update the JECs associated with my input GT
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
# AK4 PF CHS
updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJets'),
   labelName = 'UpdatedJECAK4PFchs',
   jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  
   # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
)
# AK8 PF PUPPI
updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJetsAK8'),
   labelName = 'UpdatedJECAK8PFPuppi',
   jetCorrections = ('AK8PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  
   # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
)
process.jecSequenceAK4 = cms.Sequence(process.patJetCorrFactorsUpdatedJECAK4PFchs * process.updatedPatJetsUpdatedJECAK4PFchs)
process.jecSequenceAK8 = cms.Sequence(process.patJetCorrFactorsUpdatedJECAK8PFPuppi * process.updatedPatJetsUpdatedJECAK8PFPuppi)

# Need to recorrect MET since we updated JECs
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(
  process,
  isData=True
)

# 2017 EE noise MET fix
runMetCorAndUncFromMiniAOD (
  process,
  isData = True, # false for MC
  fixEE2017 = True,
  fixEE2017Params = {'userawPt': True, 'ptThreshold':50.0, 'minEtaThreshold':2.65, 'maxEtaThreshold': 3.139} ,
  postfix = "ModifiedMET"
)

#################
# MODULES FOR MC

## TO BE ADDED

################
# MAIN ANALYZER

process.tupel = cms.EDAnalyzer("Tupel",
  yearToProcess = cms.untracked.string(opt.year),
  ##### Vertex information, HLT Trigger Bits
  vertexSrc        = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'), 
  triggerSrc       = cms.InputTag("TriggerResults", "", "HLT"),
  ##### Muons, Jets, MET
  muonSrc      = cms.untracked.InputTag("slimmedMuons"),
  # jetSrc       = cms.untracked.InputTag("slimmedJets"), #default ak4 chs jet colleciton in miniAOD
  # jetAK8Src    = cms.untracked.InputTag("slimmedJetsAK8"), #default ak8 puppi jet colleciton in miniAOD
  jetSrc       = cms.untracked.InputTag("updatedPatJetsUpdatedJECAK4PFchs"), #updated with JECs
  jetAK8Src    = cms.untracked.InputTag("updatedPatJetsUpdatedJECAK8PFPuppi"), #updated with JECs
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
  puSrc        = cms.untracked.InputTag('slimmedAddPileupInfo'),
  genSrc       = cms.untracked.InputTag("prunedGenParticles"),
  gjetSrc      = cms.untracked.InputTag('slimmedGenJets'),
  gjetAK8Src      = cms.untracked.InputTag('slimmedGenJetsAK8'),

  ##### Other stuff
  mSrcRho      = cms.untracked.InputTag('fixedGridRhoFastjetAll'),
  lheSrc       = cms.untracked.InputTag('externalLHEProducer'),
  triggerObjectTag = cms.untracked.InputTag("slimmedPatTrigger"),
  triggerStat  = cms.untracked.bool(False), #more information on trigger statistics (?)
  ##### Extra printout statements
  DJALOG       = cms.untracked.bool(False)
)

########################################

process.p = cms.Path(
  process.jecSequenceAK4 * 
  process.jecSequenceAK8 *
  process.fullPatMetSequence *
  process.fullPatMetSequenceModifiedMET *
  process.tupel 
)
