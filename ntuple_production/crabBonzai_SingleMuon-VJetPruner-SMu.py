from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SingleMuon-VJetPruner-SMu'
config.section_('JobType')
config.JobType.outputFiles = ['Bonzai-SingleMuon-VJetPruner-SMu.root']
config.JobType.scriptArgs = ['sel=VJetPruner', 'subsel=SMu', 'output_file=Bonzai-SingleMuon-VJetPruner-SMu.root', 'input_catalog=crabBonzai_SingleMuon-VJetPruner-SMu_inputs.txt', 'files_per_job=20']
config.JobType.scriptExe = 'job_bonzai_prod.sh'
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'donothing_cfg.py'
config.JobType.inputFiles = ['crabBonzai_SingleMuon-VJetPruner-SMu_inputs.txt', '/afs/cern.ch/work/a/awisecar/WJetsNtuple16/CMSSW_9_4_10/src/shears/Bonzais/Pruner/pruner']
config.JobType.maxJobRuntimeMin = 1400
config.section_('Data')
config.Data.unitsPerJob = 20
config.Data.totalUnits = 254
config.Data.publication = False
config.Data.splitting = 'EventBased'
config.Data.outLFNDirBase = '/store/group/phys_smp/AnalysisFramework/Baobab/awisecar/2017/31Mar2018/Bonzai/v1/Data'
config.section_('User')
config.section_('Site')
config.Site.blacklist = ['T3_TW_NTU_HEP', 'T3_GR_IASA', 'T2_GR_Ioannina', 'T3_MX_Cinvestav', 'T2_DE_RWTH', 'T2_UK_SGrid_RALPP', 'T3_RU_FIAN', 'T2_FI_HIP', 'T2_BR_SPRACE', 'T2_ES_CIEMAT', 'T2_EE_Estonia']
config.Site.storageSite = 'T2_CH_CERN'
