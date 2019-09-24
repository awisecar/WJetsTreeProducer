### WJets Ntupel-izer
### For making "Baobab" and "Bonzai" trees for analysis-level code

mkdir WJetsNtuple16_lxplus6_PanicMode
cd WJetsNtuple16_lxplus6_PanicMode
cmsrel CMSSW_9_4_10
cd CMSSW_9_4_10/src
cmsenv

### NOTE: This MET EE noise fix not verified yet!!!
First grab CMSSW packages needed for ad-hoc fixes:
# For 2017 EE noise MET fix --
git cms-merge-topic cms-met:METFixEE2017_949_v2

Then pull repository:
git clone -b master git@github.com:awisecar/WJetsTreeProducer.git

For further instructions, see the README in the ntuple_production directory...
