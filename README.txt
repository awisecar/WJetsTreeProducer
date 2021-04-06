### WJets Ntupelizer
### For making "Baobab" and "Bonzai" trees for analysis-level code

mkdir WJetsNtuple16_2016_2017
cd WJetsNtuple16_2016_2017
cmsrel CMSSW_9_4_14_UL_patch1
cd CMSSW_9_4_14_UL_patch1/src
cmsenv

Check out the JetToolbox module (for CMSSW_9_4_X):
git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_94X_v4
scram b -j 16

Then pull repository:
git clone -b 2016_2017 git@github.com:awisecar/WJetsTreeProducer.git

For further instructions, see the README in the ntuple_production directory...
