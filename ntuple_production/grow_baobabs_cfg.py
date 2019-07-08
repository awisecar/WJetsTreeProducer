import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import re

process = cms.Process("GrowBaobabs")

# setup 'analysis'  options
opt = VarParsing.VarParsing ('analysis')
# Addition options.
# Note: if you add an option, update the code which write the values 
# in the configuration dump, you can find at the end of this file.
opt.register('minRun', 1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Gives an indication on the minimum run number included in the input samples.')
opt.register('maxRun', 999999, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Gives an indication on the maximum run number include in the input samples.')
opt.register('prodEra', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Production run era. Label used to identify a run period whose data are processed together.')
opt.register('recoTag', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Tag of the recontruction.')
opt.register('dataTier', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'Data tier of input dataset, typically AOD, AODSIM, MINIAOD or MINIAODSIM')
opt.register('isMC',    -1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Flag indicating if the input samples are from MC (1) or from the detector (0).')
opt.register('makeEdm', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, 'Switch for EDM output production. Use 0 (default) to disable it, 1 to enable it.')

#input files. Can be changed on the command line with the option inputFiles=...
opt.inputFiles = [
  #Data
  "root://xrootd-cms.infn.it//store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root"
]

#max number of events. #input files. Can be changed on the command line with the option maxEvents=...
opt.maxEvents = -1
#opt.maxEvents = 50000

opt.parseArguments()

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
#process.options.allowUnscheduled = cms.untracked.bool(True)

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.source = cms.Source("PoolSource",
                            fileNames =  cms.untracked.vstring(opt.inputFiles))

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(opt.maxEvents))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('ntupleOut.root' )
)

if opt.isMC < 0 and len(process.source.fileNames) > 0:
  if re.match(r'.*/(MINI)?AODSIM/.*', process.source.fileNames[0]):
    print "MC dataset detected."
    opt.isMC = 1
  elif re.match(r'.*/(MINI)?AOD/.*', process.source.fileNames[0]):
    print "Real data dataset detected."
    opt.isMC = 0
  #endif
#endif

if opt.isMC < 0:
  raise Exception("Failed to detect data type. Data type need to be specify with the isMC cmsRun command line option")
#endif

dataGlobalTag ='94X_dataRun2_v11'
mcGlobalTag = '94X_mc2017_realistic_v17'

#Condition DB tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
if opt.isMC == 1:
  process.GlobalTag.globaltag = mcGlobalTag
else:
  process.GlobalTag.globaltag = dataGlobalTag


#process.prefiringweight = cms.EDProducer("L1ECALPrefiringWeightProducer",
#    ThePhotons = cms.InputTag("slimmedPhotons"),
#    TheJets = cms.InputTag("slimmedJets"),
#    L1Maps = cms.string("L1PrefiringMaps_new.root"),
#    DataEra = cms.string("2016BtoH"), #Use 2016BtoH for 2016
#    UseJetEMPt = cms.bool(False), #can be set to true to use jet prefiring maps parametrized vs pt(em) instead of pt
#    PrefiringRateSystematicUncty = cms.double(0.2) #Minimum relative prefiring uncty per object
#)


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

process.p = cms.Path()
#process.p += process.prefiringweight
process.p += process.tupel

if opt.makeEdm:
  process.out = cms.OutputModule("PoolOutputModule",
      fileName = cms.untracked.string('edm_out.root'),
      outputCommands = cms.untracked.vstring('keep *')
  )

  process.outpath = cms.EndPath(process.out)
#endif makeEdm

iFileName = "configDump_cfg.py"
file = open(iFileName,'w')
file.write(str(process.dumpPython()))
file.write('''
opt.minRun   = %d
opt.maxRun   = %d
opt.prodEra  = %s
opt.recoTag  = %s
opt.isMC     = %d
''' % (opt.minRun, opt.maxRun, opt.prodEra, opt.recoTag, opt.isMC))
file.close()

