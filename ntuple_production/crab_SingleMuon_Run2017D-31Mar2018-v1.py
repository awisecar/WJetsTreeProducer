from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SingleMuon_Run2017D-31Mar2018-v1'
config.section_('JobType')
config.JobType.psetName = 'grow_baobabs_TEST_cfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.pyCfgParams = ['maxEvents=-1', 'year=2017', 'isMC=0']
config.JobType.maxMemoryMB = 2500
config.section_('Data')
config.Data.inputDataset = '/SingleMuon/Run2017D-31Mar2018-v1/MINIAOD'
config.Data.publication = False
config.Data.unitsPerJob = 60000
config.Data.splitting = 'EventAwareLumiBased'
config.Data.lumiMask = '/afs/cern.ch/work/a/awisecar/WJetsNtuple16/CMSSW_9_4_10/src/shears/ntuple_production/datacards/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt'
config.Data.outLFNDirBase = '/store/group/phys_smp/AnalysisFramework/Baobab/awisecar/2017/31Mar2018/DATA/v2'
config.section_('User')
config.section_('Site')
config.Site.blacklist = ['T3_TW_NTU_HEP', 'T3_GR_IASA', 'T2_GR_Ioannina', 'T3_MX_Cinvestav', 'T2_DE_RWTH', 'T2_UK_SGrid_RALPP', 'T3_RU_FIAN', 'T2_FI_HIP', 'T2_BR_SPRACE', 'T2_ES_CIEMAT', 'T2_EE_Estonia']
config.Site.storageSite = 'T2_CH_CERN'
