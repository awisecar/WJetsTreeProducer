import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

####################

options = VarParsing.VarParsing()
options.register('year', '2016', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Year of data/MC to process.')
options.register('isData', '1', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Switch to run on real data (isData=1) or MC (isData=0).')
options.parseArguments()

# Print out all options
print("\n>>>>>>> Options:")
print("year = "+str(options.year))
print("isData = "+str(options.isData))

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

inputFilename = ""
# ------- 2016 samples -------
if (options.year == "2016"):
  if (options.isData == 1):
    # 2016 17Jul2018 legacy rereco, SingleMuon dataset
    inputFilename += "/store/data/Run2016D/SingleMuon/MINIAOD/17Jul2018-v1/50000/F2A9EE0F-0990-E811-8A4B-44A84225C629.root"
  elif (options.isData == 0):
    # RunIISummer16MiniAODv3 campaign
    ### WJets, 0J, FXFX sample
    inputFilename += "/store/mc/RunIISummer16MiniAODv3/WToLNu_0J_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/90000/F6D52575-F828-E911-BB5C-5065F38162B1.root"
  else:
    print("Pick a sensible value for isData.")
# ------- 2017 samples -------
elif (options.year == "2017"):
  if (options.isData == 1):
    # 2017 31Mar2018 reMiniAOD, SingleMuon dataset
    inputFilename += "/store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root"
  elif (options.isData == 0):
    # RunIIFall17MiniAODv2 campaign
    ### WJets MLM sample
    inputFilename += "/store/mc/RunIIFall17MiniAODv2/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/70000/FE956420-925A-E911-8872-0025905A48FC.root"
    # ### WJets, 0J, FXFX sample
    # inputFilename += "/store/mc/RunIIFall17MiniAODv2/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/410000/F2509DBD-37A4-E811-8876-0242AC1C0506.root"
    # ### Single top, s-channel
    # inputFilename += "/store/mc/RunIIFall17MiniAODv2/ST_s-channel_4f_leptonDecays_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/710000/04B18578-BE44-E811-80F1-0CC47A1DF7FE.root"
    # ### ttBar, 2L2Nu
    # inputFilename += "/store/mc/RunIIFall17MiniAODv2/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/FECD59BD-1842-E811-96D7-0242AC130002.root"
    # ### WZ inclusive
    # inputFilename += "/store/mc/RunIIFall17MiniAODv2/WZ_TuneCP5_13TeV-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/40000/FEA1F528-1A42-E811-85AC-6CC2173D6B10.root"
  else:
    print("Pick a sensible value for isData.")
# ------- 2018 samples -------
else:
  print("No listed samples for 2018 yet!")

print("inputFilename = "+inputFilename)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(inputFilename)
)

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5) )

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

print("outputFilename = "+outputFilename)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string(outputFilename)
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

print("globalTagString = "+globalTagString)
print("")

# #Attempting to update the JECs associated with my input GT (rather than a txt file or db file)
# #Doing this because assuming miniAOD made for a different set of JECs associated with an older GT
# from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
# # AK4 PF CHS
# updateJetCollection(
#    process,
#    jetSource = cms.InputTag('slimmedJets'),
#    labelName = 'UpdatedJECAK4PFchs',
#    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  
#    # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
# )
# # new jet collection will be called: updatedPatJetsUpdatedJECAK4PFchs
# # because new jet colleciton takes the form "updatedPatJets+labelName+postfix"

# # AK8 PF PUPPI
# updateJetCollection(
#    process,
#    jetSource = cms.InputTag('slimmedJetsAK8'),
#    labelName = 'UpdatedJECAK8PFPuppi',
#    jetCorrections = ('AK8PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  
#    # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
# )
# # new jet collection will be called: updatedPatJetsUpdatedJECAK8PFPuppi
# # because new jet colleciton takes the form "updatedPatJets+labelName+postfix"

# # add these sequences to the path below
# process.jecSequenceAK4 = cms.Sequence(process.patJetCorrFactorsUpdatedJECAK4PFchs * process.updatedPatJetsUpdatedJECAK4PFchs)
# process.jecSequenceAK8 = cms.Sequence(process.patJetCorrFactorsUpdatedJECAK8PFPuppi * process.updatedPatJetsUpdatedJECAK8PFPuppi)

# # Need to recorrect MET since we updated JECs
# from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
# runMetCorAndUncFromMiniAOD(
#   process,
#   isData=True
#   # isData=options.isData #doesn't work
# )
# # Need also to correct Puppi MET for AK8 jets
# # Also to run over PUPPI MET?

# # 2017 EE noise MET fix
# # from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD   #already included above
# runMetCorAndUncFromMiniAOD(
#   process,
#   isData = True, # false for MC
#   # isData = options.isData, #doesnt work
#   fixEE2017 = True,
#   fixEE2017Params = {'userawPt': True, 'ptThreshold':50.0, 'minEtaThreshold':2.65, 'maxEtaThreshold': 3.139} ,
#   postfix = "ModifiedMET"
# )

process.GlobalTag.globaltag = globalTagString

#--------------------------------------------

# Set HLT trigger paths of interest by year
hltTriggerPath1 = ""
hltTriggerPath2 = ""
hltTriggerPath3 = ""
if (options.year == "2016"):
  hltTriggerPath1 += "HLT_IsoMu24_v"
  hltTriggerPath2 += "HLT_IsoTkMu24_v"
  hltTriggerPath3 += "blank"
elif (options.year == "2017"):
  hltTriggerPath1 += "HLT_IsoMu24_v"
  hltTriggerPath2 += "HLT_IsoMu27_v"
  hltTriggerPath3 += "HLT_Mu27_v"
else:
  hltTriggerPath1 += "HLT_IsoMu24"
  hltTriggerPath2 += "blank"
  hltTriggerPath3 += "blank"

#--------------------------------------------                                                                       

process.tupel = cms.EDAnalyzer("Tupel",
  yearToProcess = cms.untracked.string(options.year),
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
  jetSrc              = cms.untracked.InputTag("slimmedJets"), #default ak4 chs jet colleciton in miniAOD
  jetAK8Src           = cms.untracked.InputTag("slimmedJetsAK8"), #default ak8 puppi jet colleciton in miniAOD
  # jetSrc       = cms.untracked.InputTag("updatedPatJetsUpdatedJECAK4PFchs"), #updated with JECs
  # jetAK8Src    = cms.untracked.InputTag("updatedPatJetsUpdatedJECAK8PFPuppi"), #updated with JECs
  metSrc              = cms.untracked.InputTag("slimmedMETs"),
  # metSrc       = cms.untracked.InputTag("slimmedMETsPuppi"),
  ##### MET Filters
  noiseFilterTag      = cms.InputTag("TriggerResults","","PAT"),
  ##### GEN objects, MC truth pileup information
  genInfoSrc          = cms.untracked.InputTag('generator'),            # GenEventInfoProduct
  lheSrc              = cms.untracked.InputTag('externalLHEProducer'),  # LHEEventProduct, LHERunInfoProduct
  puSrc               = cms.untracked.InputTag('slimmedAddPileupInfo'), # PileupSummaryInfo
  genSrc              = cms.untracked.InputTag("prunedGenParticles"),
  gjetSrc             = cms.untracked.InputTag('slimmedGenJets'),
  gjetAK8Src          = cms.untracked.InputTag('slimmedGenJetsAK8'),
  ##### Other stuff
  mSrcRho             = cms.untracked.InputTag('fixedGridRhoFastjetAll'),
  ##### Extra printout statements
  DJALOG              = cms.untracked.bool(True), #prints out info about gen weight structure, HLT trigger paths, etc.
  printLHEWeightsInfo = cms.untracked.bool(False) #prints out info about weights from LHERunInfoProduct
)

process.p = cms.Path(
  process.tupel 
)
