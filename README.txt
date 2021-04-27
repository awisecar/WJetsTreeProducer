### WJets Ntupelizer
### For making "Baobab" and "Bonzai" trees for analysis-level code

mkdir WJetsNtuple16_2018
cd WJetsNtuple16_2018
cmsrel CMSSW_10_2_20_UL
cd CMSSW_10_2_20_UL/src
cmsenv

Check out the JetToolbox module (for CMSSW_10_2_X):
git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_102X_v3
scram b -j 16

Then pull repository:
git clone -b 2018 git@github.com:awisecar/WJetsTreeProducer.git

For further instructions, see the README in the ntuple_production directory...
