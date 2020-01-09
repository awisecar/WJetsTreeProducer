#### Setup -----
Working on lxplus7 in CMSSW_10_2_20_UL
PATH=$PATH:/afs/cern.ch/work/a/awisecar/WJetsNtuple16_2018/CMSSW_10_2_20_UL/src/WJetsTreeProducer/ntuple_production
voms-proxy-init --voms cms
source /cvmfs/cms.cern.ch/crab3/crab.sh

# To run the test cfg:
# cmsRun simple_run_cfg.py year=2018ABC isData=1 doGenInfo=0

#### I: For Baobab production -----
To create crab config file:

For 2018 data --
simple_grow_boababs Baobabs_DATA_2018ABC_dataset.txt --year=2018ABC --unitsPerJob=60000 --no-submit 
simple_grow_boababs Baobabs_DATA_2018D_dataset.txt   --year=2018D   --unitsPerJob=60000 --no-submit 
For 2018 MC --
simple_grow_boababs Baobabs_MC_2018_dataset.txt      --year=2018ABC --unitsPerJob=3     --no-submit

############################################################################################################
##
## NOTE: for W+Jets MC you will need to manually switch on doGenInfo=1 in the crab cfg file after it's made!
##
############################################################################################################

# Make sure to alter grow_baobabs_cfg.py if needed (this is your cmsRun cfg file)
# To run grow_baobabs_cfg.py for testing you can do...
## e.g. 2018, real data, 10000 events
cmsRun grow_baobabs_cfg.py year=2018ABC isData=1 doGenInfo=0 maxEvents=10000 inputFiles=/store/data/Run2018A/SingleMuon/MINIAOD/17Sep2018-v2/110000/10210DAB-2848-054A-B7A2-BE9B4866EA19.root
cmsRun grow_baobabs_cfg.py year=2018D   isData=1 doGenInfo=0 maxEvents=10000 inputFiles=/store/data/Run2018D/SingleMuon/MINIAOD/22Jan2019-v2/60002/FF86562D-76CB-4B43-90CC-1E76C7D3286C.root
# e.g. 2018, MC, 10000 events
cmsRun grow_baobabs_cfg.py year=2018ABC isData=0 doGenInfo=1 maxEvents=10000 inputFiles=/store/mc/RunIIAutumn18MiniAOD/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/90000/957A0153-D108-6449-A9BF-7B971EE370D5.root

To submit:
crab submit -c crab_SingleMuon_Run2017B-31Mar2018-v1.py --dryrun
crab proceed
-- OR --
crab submit -c crab_SingleMuon_Run2017B-31Mar2018-v1.py

To check status:
crab status -d crab_SingleMuon_Run2017B-31Mar2018-v1

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
# To get int lumi in pb^-1, do (the same normtag works for all of Run 2) --
2016:
brilcalc lumi -i crab_SingleMuon_Run2016B-17Jul2018_ver2-v1/results/processedLumis.json -b "STABLE BEAMS" -u /pb --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json
2017:
brilcalc lumi -i crab_SingleMuon_Run2017B-31Mar2018-v1/results/processedLumis.json -b "STABLE BEAMS" -u /pb --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json

