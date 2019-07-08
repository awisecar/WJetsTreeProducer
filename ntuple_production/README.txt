#### Setup -----
PATH=$PATH:/afs/cern.ch/work/a/awisecar/WJetsNtuple16/CMSSW_9_4_10/src/shears/ntuple_production
voms-proxy-init --voms cms
source /cvmfs/cms.cern.ch/crab3/crab.sh

#### I: For Baobab production -----
To create crab config file:
simple_grow_boababs Baobabs_DATA_2017_ERA_B_dataset.txt  --no-submit --unitsPerJob=500000

To submit:
crab submit -c crab_SingleMuon_Run2017B-31Mar2018-v1.py --dryrun
crab proceed
-- OR --
crab submit -c crab_SingleMuon_Run2017B-31Mar2018-v1.py

To check status:
crab status --dir=crab_SingleMuon_Run2017B-31Mar2018-v1

Jobs will go here (listed in input datasets.txt file) --
/eos/cms/store/group/phys_smp/AnalysisFramework/Baobab/awisecar/2017/31Mar2018/DATA/v1

Make catalog files, which list the Baobab output files for Bonzai input --
simple_grow_boababs Baobabs_DATA_2017_ERA_B_dataset.txt  --make-catalogs

#### II: For Bonzai production -----
First move to Pruner directory and build Pruner ---
cd ../Bonzais/Pruner
make clean
make

Can run the Pruner manually ---
Either over an individual file:
./pruner --selection VJetPruner --subselection SMu -o ../bonzaiNtupleTest.root -b branch_list.txt ../baobabNtupleTest.root
Or other a catalog:
./pruner --selection VJetPruner --subselection SMu -o outputFile.root -b branchesFile.txt  -c <CATALOG TXT FILE>   
Can also use the option:
--max-events=100000 

This Pruner utility is run within the "grow_bonzais" program -----
To generate the CRAB cfg file first:
grow_bonzais --task-list Bonzai_DATA_2017_ERA_B_tasklist.txt --no-submit
crab submit -c crabBonzai_SingleMuon-VJetPruner-SMu.py

For CRAB status --
crab status --dir=crab_SingleMuon-VJetPruner-SMu

Now make Bonzai catalogs ---
grow_bonzais --make-catalogs Bonzai_DATA_2017_ERA_B_tasklist.txt

We run the analysis code over these Bonzai ntuples (itty bitty trees).


### III: To get int lumi -------
# First use "crab report" to get lumi information for the Baobab tasks you submitted; can consult these links --
https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCrab
https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3Commands#crab_report
# To get list of good lumis for your task, do:
crab report -d crab_SingleMuon_Run2017B-31Mar2018-v1
and then the processed lumi file will be written in
crab_SingleMuon_Run2017B-31Mar2018-v1/results/processedLumis.json

# Now need to run brilcalc on processedLumis.json to get integrated lumi -------
# If on lxplus, source brilcalc directory (and install if do not have it)
# 1) source brilws
export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.1.7/bin:$PATH
# 2) install (only do if you're installing for first time)
pip install --install-option="--prefix=$HOME/.local" brilws
# "brilcalc lumi" command taken from these pages: 
https://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html#brilcalclumi
https://twiki.cern.ch/twiki/bin/view/CMSPublic/LumiPublicResults#Technical_details
# To get int lumi in pb^-1, do (normtag for 2017 specifically?) --
brilcalc lumi -i crab_SingleMuon_Run2017B-31Mar2018-v1/results/processedLumis.json -b "STABLE BEAMS" -u /pb --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json