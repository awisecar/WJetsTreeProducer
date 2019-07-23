import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

####################

options = VarParsing.VarParsing()
options.register('year',
  '2017',
  VarParsing.VarParsing.multiplicity.singleton,
  VarParsing.VarParsing.varType.string,
  'Year of data/MC to process.'
)
options.register('isData',
  '1',
  VarParsing.VarParsing.multiplicity.singleton,
  VarParsing.VarParsing.varType.int,
  'Switch to run on real data (isData=1) or MC (isData=0).'
)
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

if (options.year == "2016"):
  print(">>>>>>> Using 2016 real data sample")
  # 2016 03Feb2017 reMiniAOD, SingleMuon dataset
  inputFilename = "/store/data/Run2016D/SingleMuon/MINIAOD/03Feb2017-v1/80000/FE050076-2FEB-E611-8E4A-0025905C5476.root"
elif (options.year == "2017"):
  print(">>>>>>> Using 2017 real data sample")
  # 2017 31Mar2018 reMiniAOD, SingleMuon dataset
  inputFilename = "root://xrootd-cms.infn.it//store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root"

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(inputFilename)
)

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

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

# below block doesn't work
# globaltagName = ''
# if (options.year == "2017"):
#   if (options.isData == 1):
#     print(">>>>>>> Using 2017 real data GT")
#     # For 2017 real data, re-miniAOD 31Mar2018:
#     globaltagName = '94X_dataRun2_v11'
#   elif (options.isData == 0):
#     print(">>>>>>> Using 2017 MC GT")
#     # For 2017 MC, RunIIFall17MiniAODv2 campaign:
#     globaltagName = '94X_mc2017_realistic_v17'
# process.GlobalTag.globaltag = globaltagName

process.GlobalTag.globaltag = '94X_dataRun2_v11'

#--------------------------------------------                                                                       

# process.prefiringweight = cms.EDProducer("L1ECALPrefiringWeightProducer",
#     ThePhotons = cms.InputTag("slimmedPhotons"),
#     TheJets = cms.InputTag("slimmedJets"),
#     L1Maps = cms.string("L1PrefiringMaps_new.root"), # update this line with the location of this file
#     DataEra = cms.string("2016BtoH"), #Use 2016BtoH for 2016
#     UseJetEMPt = cms.bool(False), #can be set to true to use jet prefiring maps parametrized vs pt(em) instead of pt
#     PrefiringRateSystematicUncty = cms.double(0.2) #Minimum relative prefiring uncty per object
# )

#Attempting to update the JECs associated with my input GT (rather than a txt file or db file)
#Doing this because assuming miniAOD made for a different set of JECs associated with an older GT
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
# AK4 PF CHS
updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJets'),
   labelName = 'UpdatedJECAK4PFchs',
   jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  
   # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
)
# new jet collection will be called: updatedPatJetsUpdatedJECAK4PFchs
# because new jet colleciton takes the form "updatedPatJets+labelName+postfix"

# AK8 PF PUPPI
updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJetsAK8'),
   labelName = 'UpdatedJECAK8PFPuppi',
   jetCorrections = ('AK8PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  
   # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
)
# new jet collection will be called: updatedPatJetsUpdatedJECAK8PFPuppi
# because new jet colleciton takes the form "updatedPatJets+labelName+postfix"

# add these sequences to the path below
process.jecSequenceAK4 = cms.Sequence(process.patJetCorrFactorsUpdatedJECAK4PFchs * process.updatedPatJetsUpdatedJECAK4PFchs)
process.jecSequenceAK8 = cms.Sequence(process.patJetCorrFactorsUpdatedJECAK8PFPuppi * process.updatedPatJetsUpdatedJECAK8PFPuppi)

# Need to recorrect MET since we updated JECs
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(
  process,
  isData=True,
  # isData=options.isData #doesn't work
)
# Need also to correct Puppi MET for AK8 jets
# Also to run over PUPPI MET?

# 2017 EE noise MET fix
# from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD   #already included above
runMetCorAndUncFromMiniAOD (
  process,
  isData = True, # false for MC
  # isData = options.isData, #doesnt work
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

# we are not running in unscheduled mode (?)
process.p = cms.Path(
  process.jecSequenceAK4 * 
  process.jecSequenceAK8 *
  process.fullPatMetSequence *
  process.fullPatMetSequenceModifiedMET *
  process.tupel 
)
