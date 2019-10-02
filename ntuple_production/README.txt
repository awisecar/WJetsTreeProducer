#### Setup -----
Working on lxplus6 in CMSSW_9_4_10
PATH=$PATH:/afs/cern.ch/work/a/awisecar/WJetsNtuple16_lxplus6_PanicMode/CMSSW_9_4_10/src/WJetsTreeProducer/ntuple_production
voms-proxy-init --voms cms
source /cvmfs/cms.cern.ch/crab3/crab.sh

# To run the test cfg:
# cmsRun simple_run_cfg.py year=2017 isData=1

#### I: For Baobab production -----
To create crab config file:

simple_grow_boababs Baobabs_DATA_2017_dataset.txt  --no-submit --unitsPerJob=60000
or maybe
simple_grow_boababs Baobabs_DATA_2017_dataset.txt  --no-submit --unitsPerJob=80000

simple_grow_boababs Baobabs_MC_2017_dataset.txt --no-submit --unitsPerJob=3
or maybe
simple_grow_boababs Baobabs_MC_2017_dataset.txt --no-submit --unitsPerJob=5

# Make sure to alter grow_baobabs_cfg.py if needed (this is your cmsRun cfg file)
# To run grow_baobabs_cfg.py for testing you can do, e.g.
cmsRun grow_baobabs_cfg.py maxEvents=10000 isData=1 year=2017 inputFiles=/store/data/Run2017B/SingleMuon/MINIAOD/31Mar2018-v1/90000/FEC62083-1E39-E811-B2A1-0CC47A4D75F8.root
# meaning 2017, real data, 10000 events
cmsRun grow_baobabs_cfg.py maxEvents=10000 isData=0 year=2017 inputFiles=/store/mc/RunIIFall17MiniAODv2/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/100000/7A8364F3-A394-E811-8419-0CC47A78A418.root
# meaning 2017, MC, 10000 events

To submit:
crab submit -c crab_SingleMuon_Run2017B-31Mar2018-v1.py --dryrun
crab proceed
-- OR --
crab submit -c crab_SingleMuon_Run2017B-31Mar2018-v1.py

To check status:
crab status -d crab_SingleMuon_Run2017B-31Mar2018-v1

Jobs will go here (listed in input datasets.txt file) --
/eos/cms/store/group/phys_smp/AnalysisFramework/Baobab/awisecar/2017/31Mar2018/DATA/<CRAB JOBS VERSION NUMBER>

Make catalog files, which list the Baobab output files for Bonzai input --

simple_grow_boababs Baobabs_DATA_2017_dataset.txt --make-catalogs
simple_grow_boababs Baobabs_MC_2017_dataset.txt --make-catalogs

(simple_grow_baobabs does not discriminate among Eras for Data, so all Eras go in one file)


#### II: For Bonzai production -----
First move to Pruner directory and build Pruner for use in Bonzai scripts ---
cd ../Bonzais/Pruner
make clean
make
# To edit (add or remove branches), change the following files:
# branch_list.txt, EventTree.h, VJetPruner.cc

Can also run the Pruner manually ---
Either over an individual file:
./pruner --selection VJetPruner --subselection SMu -o ../bonzaiNtupleTest.root -b branch_list.txt ../baobabNtupleTest.root
Or other a catalog:
./pruner --selection VJetPruner --subselection SMu -o outputFile.root -b branchesFile.txt  -c <CATALOG TXT FILE>   
Can also use the option:
--max-events=100000 

This Pruner utility is run within the "grow_bonzais" program -----
To generate the CRAB cfg file first:

grow_bonzais --task-list Bonzai_DATA_2017_tasklist.txt --no-submit
grow_bonzais --task-list Bonzai_MC_2017_tasklist.txt --no-submit

Then submit:
crab submit -c crabBonzai_SingleMuon-VJetPruner-SMu.py

For CRAB status --
crab status -d crab_SingleMuon-VJetPruner-SMu

Now make Bonzai catalogs ---
grow_bonzais --make-catalogs Bonzai_DATA_2017_tasklist.txt
grow_bonzais --make-catalogs Bonzai_MC_2017_tasklist.txt

We run the analysis code over these Bonzai ntuples (these itty bitty trees).
Our analysis code does all of the event selection/histogram filling.


### III: To get int lumi -------
# First use "crab report" to get lumi information for the Baobab tasks you submitted; can consult these links --
https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCrab
https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3Commands#crab_report
# To get list of good lumis for your task, do:
crab report -d crab_SingleMuon_Run2017B-31Mar2018-v1
and then the processed lumi file will be written in
crab_SingleMuon_Run2017B-31Mar2018-v1/results/processedLumis.json
(need to do this separately for every Era)

# Now need to run brilcalc on processedLumis.json to get integrated lumi -------
# If on lxplus, source brilcalc directory (and install if do not have it)
# 1) source brilws
export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.1.7/bin:$PATH
# 2) install (only do if you're installing for first time)
pip install --install-option="--prefix=$HOME/.local" brilws
# 2b) if something goes wrong, try updating brilcalc
pip install --user --upgrade brilws
# "brilcalc lumi" command taken from these pages: 
https://twiki.cern.ch/twiki/bin/viewauth/CMS/BrilcalcQuickStart
https://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html#brilcalclumi
https://twiki.cern.ch/twiki/bin/view/CMSPublic/LumiPublicResults#Technical_details
# To get int lumi in pb^-1, do (normtag works for at least 2017) --
brilcalc lumi -i crab_SingleMuon_Run2017B-31Mar2018-v1/results/processedLumis.json -b "STABLE BEAMS" -u /pb --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json

