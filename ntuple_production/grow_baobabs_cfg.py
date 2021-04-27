import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

################################################################################
## OPTIONS PARSING
##

options = VarParsing.VarParsing('analysis')
options.register('year', '2018ABC', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Year of data/MC to process.')
options.register('isData', '1', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Switch to run on real data (isData=1) or MC (isData=0).')
options.register('doGenInfo', '0', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Switch to process GEN-level info (doGenInfo=1).')
options.parseArguments()

################################################################################
## SETUP
##

process = cms.Process("GrowBaobabs")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( 
  wantSummary = cms.untracked.bool(False) 
)

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("RecoBTag.Configuration.RecoBTag_cff")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(options.inputFiles)
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(options.maxEvents)
)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntupleOut.root')
)

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

globalTagString = ""
if (options.year == "2016"):
  if (options.isData == 1):
    globalTagString += "94X_dataRun2_v10"
  elif (options.isData == 0):
    globalTagString += "94X_mcRun2_asymptotic_v3"
elif (options.year == "2017"):
  if (options.isData == 1):
    globalTagString += "94X_dataRun2_v11"
  elif (options.isData == 0):
    globalTagString += "94X_mc2017_realistic_v17"
elif (options.year == "2018ABC"):
  if (options.isData == 1):
    globalTagString += "102X_dataRun2_v12"
  elif (options.isData == 0):
    globalTagString += "102X_upgrade2018_realistic_v20"
elif (options.year == "2018D"):
  if (options.isData == 1):
    globalTagString += "102X_dataRun2_Prompt_v15"
  elif (options.isData == 0):
    globalTagString += "102X_upgrade2018_realistic_v20"

process.GlobalTag.globaltag = globalTagString

hltTriggerPath1 = ""
hltTriggerPath2 = ""
hltTriggerPath3 = ""
if (options.year == "2016"):
  hltTriggerPath1 += "HLT_IsoMu24_v"
  hltTriggerPath2 += "HLT_IsoTkMu24_v"
  hltTriggerPath3 += "HLT_Mu27_v"
elif (options.year == "2017"):
  hltTriggerPath1 += "HLT_IsoMu24_v"
  hltTriggerPath2 += "HLT_IsoMu27_v"
  hltTriggerPath3 += "HLT_Mu27_v"
elif (options.year == "2018ABC"):
  hltTriggerPath1 += "HLT_IsoMu24_v"
  hltTriggerPath2 += "HLT_IsoMu27_v"
  hltTriggerPath3 += "HLT_Mu27_v"
elif (options.year == "2018D"):
  hltTriggerPath1 += "HLT_IsoMu24_v"
  hltTriggerPath2 += "HLT_IsoMu27_v"
  hltTriggerPath3 += "HLT_Mu27_v"

metFilterSwitch = ""
if (options.isData == 0):
  metFilterSwitch += "PAT"
elif (options.isData == 1):
  metFilterSwitch += "RECO"

################################################################################
## PRODUCERS
##

# -----

#### AK4 JETS WITH JECS ONLY
# AK4PFchs Jets --
# Updating JECs ONLY
# from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
# updateJetCollection(
#    process,
#    jetSource = cms.InputTag('slimmedJets'),
#    labelName = 'UpdatedJECAK4PFchs',
#    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  
# )
# process.jecSequenceAK4 = cms.Sequence(process.patJetCorrFactorsUpdatedJECAK4PFchs * process.updatedPatJetsUpdatedJECAK4PFchs)

# -----

#### AK4 JETS WITH RE-RUNNING B-TAGGING AND JECS
# AK4PFchs Jets --
# Updating JECs and Getting B-tagging TagInfos
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJets'),  # AK4 PF CHS
   jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None'),  
   btagDiscriminators = [
     'pfCombinedInclusiveSecondaryVertexV2BJetTags',
     'pfJetProbabilityBJetTags',
     'pfCombinedMVAV2BJetTags',
     'pfDeepCSVJetTags:probudsg',
     'pfDeepCSVJetTags:probbb',
     'pfDeepCSVJetTags:probb',
     'pfDeepCSVJetTags:probc'
   ]
)
process.jecSequenceAK4 = cms.Sequence(
  process.patJetCorrFactors * 
  process.updatedPatJets *
  process.pfImpactParameterTagInfos *                   
  process.pfSecondaryVertexTagInfos *                   
  process.pfInclusiveSecondaryVertexFinderTagInfos *   
  process.pfJetProbabilityBJetTags *
  process.pfCombinedInclusiveSecondaryVertexV2BJetTags * 
  process.pfJetProbabilityBJetTags *
  process.softPFMuonsTagInfos *   
  process.softPFElectronsTagInfos *
  process.pfCombinedMVAV2BJetTags *
  process.pfInclusiveSecondaryVertexFinderCvsLTagInfos *
  process.pfDeepCSVTagInfos *
  process.pfDeepCSVJetTags *                       
  process.patJetCorrFactorsTransientCorrected *
  process.updatedPatJetsTransientCorrected
)
process.updatedPatJetsTransientCorrected.addTagInfos = cms.bool(True)
process.pfInclusiveSecondaryVertexFinderCvsLTagInfos.extSVCollection = cms.InputTag("slimmedSecondaryVertices")

# -----

# AK8PFPuppi Jets --
# Re-cluster the jet collection to a lower pT threshold
from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
jetToolbox(
  process, 'ak8', 'jetToolboxAK8PFPuppi', 'noOutput',
  PUMethod='Puppi', 
  dataTier='miniAOD',
  runOnMC= not options.isData,
  bTagDiscriminators=['pfDeepCSVJetTags:probb', 'pfDeepCSVJetTags:probbb'],
  GetJetMCFlavour = True,
  Cut='pt >= 30.0',
  verbosity=2
)

# Updating MET ---
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
if (options.isData == 1): 
  metSwitch = True
else:
  metSwitch = False
runMetCorAndUncFromMiniAOD(
  process,
  isData=metSwitch
)

################################################################################
## MAIN ANALYZER
##

process.tupel = cms.EDAnalyzer("Tupel",
  ##### Switches
  yearToProcess       = cms.untracked.string(options.year),
  doGenInfo           = cms.untracked.int32(options.doGenInfo),
  ##### Vertex information, HLT Trigger Bits
  vertexSrc           = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'), 
  triggerSrc          = cms.InputTag("TriggerResults", "", "HLT"),
  triggerObjectTag    = cms.untracked.InputTag("slimmedPatTrigger"),
  triggerPrescalesTag = cms.untracked.InputTag("patTrigger"),
  ## HLT paths
  muonHLTTriggerPath1 = cms.untracked.string(hltTriggerPath1),
  muonHLTTriggerPath2 = cms.untracked.string(hltTriggerPath2),
  muonHLTTriggerPath3 = cms.untracked.string(hltTriggerPath3),
  ##### Muons, Jets, MET
  muonSrc             = cms.untracked.InputTag("slimmedMuons"),
  # -----
  # jetSrc              = cms.untracked.InputTag("updatedPatJetsUpdatedJECAK4PFchs"), #updated with JECs ONLY
  jetSrc              = cms.untracked.InputTag("updatedPatJetsTransientCorrected"), #updated with JECs, b-tagging TagInfos
  # -----
  jetAK8Src           = cms.untracked.InputTag("selectedPatJetsAK8PFPuppi"), #reclustered with JetToolbox
  metSrc              = cms.untracked.InputTag("slimmedMETs"),
  ##### MET Filters
  noiseFilterTag      = cms.InputTag("TriggerResults","",metFilterSwitch),
  ##### GEN objects, MC truth pileup information
  genInfoSrc          = cms.untracked.InputTag('generator'),
  lheSrc              = cms.untracked.InputTag('externalLHEProducer'),
  puSrc               = cms.untracked.InputTag('slimmedAddPileupInfo'),
  genSrc              = cms.untracked.InputTag("prunedGenParticles"),
  gjetSrc             = cms.untracked.InputTag('slimmedGenJets'),
  gjetAK8Src          = cms.untracked.InputTag('slimmedGenJetsAK8'),
  ##### Other
  mSrcRho             = cms.untracked.InputTag('fixedGridRhoFastjetAll'),
  ##### Verbosity of output
  DJALOG              = cms.untracked.bool(False),
  printLHEWeightsInfo = cms.untracked.bool(False)
)

########################################

process.p = cms.Path(
  process.jecSequenceAK4 * 
  process.fullPatMetSequence *
  process.tupel 
)

