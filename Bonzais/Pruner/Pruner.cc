// Implemantation of the Pruner main class
// Author: Ph. Gras CEA/IRFU Saclay
// Jan.  3, 11. Squeezer code for AnaNaS framework
// Jul. 24, 15. Cloned Squeezer and adapted to the Shears framework

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sys/time.h>

#include "TObjArray.h"
#include "TBranchElement.h"
#include "TFile.h"
#include "TKey.h"
#include "TChain.h"
#include "TLeaf.h"
#include <libgen.h>

#include "Pruner.h"

#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;

bool DJALOG = false;

std::map<std::string, Pruner::ClassRecord> Pruner::daughters_;

void Pruner::listEvents(std::ostream& o, const char* const catalog){
  if(setInput(catalog)){
    listEvents(o);
  }
}

void Pruner::listEvents(std::ostream& o, size_t nInputFiles, const char* const inputFiles[]){
  if(setInput(nInputFiles, inputFiles)){
    listEvents(o);
  }
}

void Pruner::listEvents(std::ostream& o){
  if((eventNum_ == 0) || (runNum_ == 0)){
    std::cerr << "Event and/or run number information was not found in the input "
      "file.\n";
    return;
  }

  int treeNum = -1;
  //For performance concerns, we don't call GetEntries, which requires access
  //to all the files of the chain.
  std::string f_basename;
  for(Long64_t i = 0;; ++i){
    if(treeNum != chain_.LoadTree(i)){
      char* f = strdup(chain_.GetFile()->GetName());
      if(f){
	f_basename = basename(f);
	free(f);
      } else{
	f_basename = "";
      }
    }
    if(0 == chain_.GetEntry(i)) break;
    o << std::setw(8) << *runNum_ << std::setw(8) << "\t" << *eventNum_
      << "\t" << f_basename << endl;
  }
}

void Pruner::listBranchesFromCat(std::ostream& o, const char* catalog){
  if(setInput(catalog)){
    listBranches(o);
  }
}
void Pruner::listBranches(std::ostream& o, const char* inputDataFile){
  if(setInput(1, &inputDataFile)){
    listBranches(o);
  }
}
void Pruner::listBranches(std::ostream& o){
  TObjArray* ls = chain_.GetListOfLeaves();
  TIter it(ls);
  while(it.Next()) o << (*it)->GetName() << endl;
}

void Pruner::fillPerInputSummary(){

  //  std::cerr << "Pruner::fillPerInputSummary(), event " << ievent_
  //<< "\tfile " << chain_.GetFile()->GetName() << "\n";

  struct {
    const char* name;
    TTree** ppTree;
    bool singleEntry;
  } outTrees [] = {{"Header", &outHeaderTree_, false},
		   {"Description", &outDescriptionTree_, true},
		   {"BitFields", &outBitFieldsTree_, true}};

  TFile* fin = chain_.GetFile();
  fin->cd();
  fin->cd("tupel");
  TList* fcontent = gDirectory->GetListOfKeys();
  TIter it(fcontent);
  TKey* key = 0;
  while(it.Next()){
    key = (TKey*) (*it);

    if(strcmp(key->GetClassName(), "TTree") != 0) continue;

    unsigned iTree = 0;
    for(; iTree < sizeof(outTrees)/sizeof(outTrees[0]); ++iTree){
      if(strcmp(key->GetName(), outTrees[iTree].name) == 0) break;
    }
    if(iTree == sizeof(outTrees)/sizeof(outTrees[0])) continue;

    TTree* tree =  (TTree*) key->ReadObj();
    if(tree==0){
      std::cerr << "Warning: Failed to read tree " << key->GetName() << ".\n";
      continue;
    }


    if(verbose_ > 0){
      std::cout << "Copying  ROOT tree " << tree->GetName() << "\n";
    }
    
    fout_->cd();
    TDirectory::CurrentDirectory()->cd("tupel");

    if(*outTrees[iTree].ppTree){
      if(outTrees[iTree].singleEntry) continue; //only tree of 1st file should be copied
      tree->CopyAddresses(*outTrees[iTree].ppTree);
    } else{
      *outTrees[iTree].ppTree = tree->CloneTree(0);
    }
    tree->GetEntry(0);
    (*outTrees[iTree].ppTree)->Fill();
    (*outTrees[iTree].ppTree)->AutoSave();
    fin->cd();
  }
}

void Pruner::fillGlobalSummary(){
  fout_->cd();
  TDirectory::CurrentDirectory()->cd("tupel");
  TTree* t = new TTree("BonzaiHeader", "BonzaiHeader");
  t->Branch("InEvtCount", &nRead_);
  t->Branch("InEvtWeightSums", &evtWeightSums_);
  t->Branch("EvtWeightSums", &passedEvtWeightSums_);
  t->Branch("npNLOBinnedEvtWeightSums", &npNLOBinnedEvtWeightSums_);
  t->Branch("passednpNLOBinnedEvtWeightSums", &passednpNLOBinnedEvtWeightSums_);
  //t->Branch("hnpNLO","TH1F",&hnpNLO_);
  //t->Branch("hWeightednpNLO","TH1F",&hWeightednpNLO_);
  t->Fill();
  t->Write();
}

int Pruner::run(const char* inputCatalog, const char* outputDataFile,
		 int maxEvents, int skipEvents,
		 int maxFiles, int skipFiles){
  maxEvents_ = maxEvents;
  skipEvents_ = skipEvents;
  if(!setInput(inputCatalog, maxFiles, skipFiles)) return 100;
  if(!init((TChain*) &chain_)){
    std::cerr << "Failed to initialize the event filter. Method "
	      << (className_.size() > 0 ? className_ + "::" : "")
	      << "init(TChain*) returned code false.\n";
    return 200;
  }
  if(!setOutput(outputDataFile)) return 300;
  setBranchAdd();
  run();
  return 0;
}

int Pruner::run(size_t nInputFiles, const char* const inputDataFiles[],
		 const char* outputDataFile, int maxEvents, int skipEvents){
  maxEvents_ = maxEvents;
  skipEvents_ = skipEvents;
  if(!setInput(nInputFiles, inputDataFiles)) return 100;
  if(!init((TChain*) &chain_)){
    std::cerr << "Failed to initialize the event filter. Method "
	      << className_ << "::init(TChain*) returned code false.\n";
    return 200;
  }
  if(!setOutput(outputDataFile)) return 300;
  setBranchAdd();
  run();
  return 0;
}

int Pruner::run(){

  bool interactive = isatty(fileno(stdout)) ? true : false;

  if(interactive) std::cout << "Interactive mode" << std::endl;
  else std::cout << "Batch mode" << std::endl;
    
  timeval start;

  gettimeofday(&start, 0);

  if(chain_.GetFile() == 0) return 100;

  chain_.GetFile()->cd(); //Is this cd needed?

  Long64_t nevts;

  nevts = chain_.GetEntries() - skipEvents_;

  if(maxEvents_ >= 0 && nevts > maxEvents_) nevts = maxEvents_;

  if(verbose_ > 0){
    std::cout << "Number of events to read: " << nevts << "\n";
  }

  timeval t;
  time_t smoothed_eat = 0.;

  if(skipEvents_ > 0) ievent_ = skipEvents_ - 1;
  else ievent_ = -1;

  nCopied_ = 0;
  nRead_ = 0;
  //hnpNLO_ = new TH1F("hnpNLO","npNLO histo",4,-0.5,3.5);
  //hWeightednpNLO_ = new TH1F("hWeightednpNLO","npNLO with event weight histo",4,-0.5,3.5);

  for(Long64_t i = 1; i <= nevts; ++i){
    ++nRead_;
    nextEvent();

    if(DJALOG) printf("if(i==1 && evtWeights_){\n");
    if(i==1 && evtWeights_){
      if(evtWeights_->size() > 0){
	evtWeightSums_ = std::vector<double>(evtWeights_->size(), 0);
	passedEvtWeightSums_ = std::vector<double>(evtWeights_->size(), 0);
	npNLOBinnedEvtWeightSums_ = std::vector<float>(NPNLOBINS, 0);
	passednpNLOBinnedEvtWeightSums_ = std::vector<float>(NPNLOBINS, 0);
      }
    }
    
    if(DJALOG) printf("    bool passed = filterEvent();\n");
    bool passed = filterEvent();
    if(DJALOG) printf("if(passed)\n");
    if(passed){
      //printf("Copying\n");
      copyEvent();
      ++nCopied_;
    }

    //Filling npNLO histograms to be used as checks on xsec values. Need to fill them with all the events. Weighted
    //histograms filled just below.
    //if(DJALOG) printf("hnpNLO_->Fill(npNLO_);\n");
    //hnpNLO_->Fill(npNLO_);
    if(evtWeights_){
      if(evtWeights_->size() > 0){
	float eventWeight_0 = 1.0;
	for(unsigned iWeight = 0; iWeight < evtWeights_->size(); ++iWeight){
	  if(iWeight == 0){
	    eventWeight_0 = abs((*evtWeights_)[iWeight]);
	    if(npNLO_ == 0){
	      npNLOBinnedEvtWeightSums_[0] += (*evtWeights_)[iWeight]/eventWeight_0;
	      if(passed) passednpNLOBinnedEvtWeightSums_[0] += (*evtWeights_)[iWeight]/eventWeight_0;
	    }
	    if(npNLO_ == 1){
	      npNLOBinnedEvtWeightSums_[1] += (*evtWeights_)[iWeight]/eventWeight_0;
	      if(passed) passednpNLOBinnedEvtWeightSums_[1] += (*evtWeights_)[iWeight]/eventWeight_0;	      
	    }
	    if(npNLO_ == 2){
	      npNLOBinnedEvtWeightSums_[2] += (*evtWeights_)[iWeight]/eventWeight_0;
	      if(passed) passednpNLOBinnedEvtWeightSums_[2] += (*evtWeights_)[iWeight]/eventWeight_0;
	    }
	  }
	  evtWeightSums_[iWeight] += (*evtWeights_)[iWeight]/eventWeight_0;
	  if(passed) passedEvtWeightSums_[iWeight] += (*evtWeights_)[iWeight]/eventWeight_0;
	}
	//hWeightednpNLO_->Fill(npNLO_,evtWeights_->at(0)/eventWeight_0);
      }
    }
    const static int step = interactive ? 100 : 100000;
    //begin-of-line character: in interactive we stay on same line,
    //when stdout is a file we go to next line
    const static char bol = interactive ? '\r' : '\n';
    
    if(i%step==0 || i == nevts){
      cout << bol << "Read: " << std::setw(8) << i << " Copied: "
	   << std::setw(8) << nCopied_
	   << " Acc.: " << std::setw(5) << int(10000*(nCopied_ / double(i)))/100. << "%"
	   << " Rem.: " << std::setw(8) << (nevts-i)
	   << " Total: " << std::setw(8) << nevts; // << std::flush;
      timeval t0;
      if(i==step) gettimeofday(&t0, 0);
      else if(i >= 2 * step) {
	gettimeofday(&t, 0);
	double remaining = double(nevts - step) / (i - step)
	  * ((t.tv_sec - t0.tv_sec) + 1.e-6 * (t.tv_usec - t0.tv_usec));
	time_t eat = int(t0.tv_sec +  1.e-6 * t0.tv_usec + remaining + 0.5);
	if(smoothed_eat == 0) smoothed_eat = eat;
	if(abs(smoothed_eat - eat) > 0.1 * remaining) smoothed_eat = 0.5 * (smoothed_eat + eat);
	char buf[256];
	strftime(buf, sizeof(buf),  "%a, %d %b %Y %T", localtime(&smoothed_eat));
	cout << " ETA: " << std::setw(16) << buf;
      }
      cout << std::flush;
    }
  } //next event
  cout << "\n";

  fillGlobalSummary();
  if(outEventTree_) outEventTree_->AutoSave();
  fout_->Close();

  gettimeofday(&t, 0);

  double dt = (t.tv_sec - start.tv_sec) + 1.e-6 * (t.tv_usec - start.tv_usec);

  printf("Elapsed time: %.2g sec. %.1f event/sec.\n",  dt, nevts/dt);
  fflush(stdout);
  return 0;
}

void Pruner::readEventList(const char* fileName){
  allEvent_ = false;
  FILE* f = fopen(fileName, "r");
  if(f==0) { cout << "Failed to open event list file " << fileName << endl; abort(); }
  eventList_.clear();
  int nerr = 0;
  while(!feof(f)){
    //      while(0==fscanf(f, " # ")) {/*NOP*/}
    int run;
    int event;

    char buffer[256];
    int n = fscanf(f, " %d %d %255s\n", &run, &event, buffer);
    if(n==2){
      buffer[0] = 0;
      n = 3;
    }
    if(n!=3){
      ++nerr;
    } else{
      eventList_.push_back(EventRcd(run, event, std::string(buffer)));
    }
  }
  sort(eventList_.begin(), eventList_.end());
  if(nerr!=0) cout << nerr << " lines of " << fileName << " were skipped\n";
  if(verbose_>1){
    cout << "Event to select: \n";
    for(size_t i = 0; i < eventList_.size(); ++i){
      cout << "Run " << eventList_[i].event << " event "
	   << eventList_[i].run << "\n";
    }
  }
}

void Pruner::readBranchList(const char* fileName){
  FILE* f = fopen(fileName, "r");
  if(f==0) {
    cout << "Failed to open exluded tree list file "
	 << fileName << endl; abort();
  }
  branchList_.clear();
  int nerr = 0;
  while(!feof(f)){
    //      while(0==fscanf(f, " # ")) {/*NOP*/}
    char branchName[256];
    //TODO use +/- sign to include/exclude a branch
    int n = fscanf(f, "%255s ", branchName);
    if(n!=1){
      ++nerr;
    } else{
      branchList_.push_back(std::string(branchName));
    }
  }

  sort(branchList_.begin(), branchList_.end());
  if(nerr!=0) cout << nerr << " lines of " << fileName << " were skipped\n";
  if(verbose_>1){
    cout << "Branches to include in the tree copy: \n";
    for(size_t i = 0; i < branchList_.size(); ++i){
      cout << branchList_[i] << "\n";
    }
  }
}

/** Initialize input and output files and trees
 */
bool Pruner::setInput(size_t nInputFiles, const char* const inputDataFiles[]){
  bool rc = true;
  for(unsigned i = 0; i < nInputFiles; ++i){
    chain_.Add(TString(inputDataFiles[i]));
  }
  if(chain_.GetNtrees() == 0){
    printf("chain is empty after setting input\n");
    rc = false;
  }
  //  setBranchAdd();
  return rc;
}

void Pruner::setBranchAdd(){
  //sets runNum_ and eventNum_ pointers
  TBranch* br = (TBranch*)chain_.GetBranch("EvtNum");
  TLeaf* leaf;
  if(br && (leaf = (TLeaf*)br->GetListOfLeaves()->At(0))){
    eventNum_ = (UInt_t*) leaf->GetValuePointer();
  }
  br = (TBranch*)chain_.GetBranch("EvtRunNum");
  if(br && (leaf = (TLeaf*)br->GetListOfLeaves()->At(0))){
    runNum_ = (UInt_t*) leaf->GetValuePointer();
  }

  evtWeights_ = 0;    
  if(chain_.GetBranch("EvtWeights")){
    chain_.SetBranchAddress("EvtWeights", &evtWeights_);
  }
  
  npNLO_ = 0;    
  if(chain_.GetBranch("npNLO")){
    chain_.SetBranchAddress("npNLO", &npNLO_);
  }

  LHEZPx_ = 0;    
  if(chain_.GetBranch("LHEZPx")){
    chain_.SetBranchAddress("LHEZPx", &LHEZPx_);
  }
  

  LHEZPy_ = 0;    
  if(chain_.GetBranch("LHEZPy")){
    chain_.SetBranchAddress("LHEZPy", &LHEZPy_);
  }
}

bool Pruner::setInput(const char* catalog, int maxFiles, int skipFiles){
  bool rc = chain_.setCatalog(catalog, maxFiles, skipFiles);
  if(chain_.GetNtrees() == 0){
    printf("chain is empty after setting input\n");
    rc = false;
  }
  //  setBranchAdd();
  return rc;
}

//bool Pruner::setInput(const char* catalog, int maxFiles, int skipFiles){
//  std::ifstream f(catalog);
//  if(!f.good()){
//    std::cerr << "Failed to open file "<< catalog << "!\n";
//    return false;
//  }
//
//  int iline = 0;
//  int nfiles = 0;
//  while(f.good()){
//    ++iline;
//    std::string l;
//    std::string::size_type p;
//
//    std::getline(f, l);
//
//    //trim white spaces:
//    p = l.find_first_not_of(" \t");
//    if(p!=std::string::npos) l.erase(0, p);
//    p = l.find_last_not_of(" \t\n\r");
//    if(p!=std::string::npos) l.erase(p + 1);
//    else l.clear();
//
//    //skip empty lines and comment lines:
//    if (!l.size() || l[0] == '#') continue;
//
//    //extract first column (file name):
//    p = l.find_first_of(" \t");
//    if(p!=std::string::npos) l.erase(p);
//
//    //sanity check:
//    const char ext[] = ".root";
//
//    if(l.size() < sizeof(ext) || l.substr(l.size() - sizeof(ext) + 1) != ext){
//      std::cerr << "Line " << iline << " of catalog file " << catalog << " was skipped.\n";
//      continue;
//    }
//
//    //Solves EOS paths:
//    if(l.substr(0,7) == "/store/"){
//      //A CMS EOS path
//      l.insert(0, "root://eoscms.cern.ch//eos/cms");
//    }
//
//    //Adds path to tree within the ROOT file structure:
//    //l.append("/tupel/EventTree");
//
//    if(verbosity_>0){
//      std::cout << "Add file " << l.c_str() << " to the list of input files.\n";
//    }
//    
//    if(skipFiles <= 0){
//      ++nfiles;
//      if(maxFiles > 0 &&  nfiles > maxFiles) break;
//      chain_.Add(l.c_str());
//    } else{
//      --skipFiles;
//    }
//  }
//
//  //sets runNum_ and eventNum_ pointers
//  TBranch* br = chain_.GetBranch("EvtNum");
//  TLeaf* leaf;
//  if(br && (leaf = (TLeaf*)br->GetListOfLeaves()->At(0))){
//    eventNum_ = (UInt_t*) leaf->GetValuePointer();
//  }
//  br = chain_.GetBranch("EvtRunNum");
//  if(br && (leaf = (TLeaf*)br->GetListOfLeaves()->At(0))){
//    runNum_ = (UInt_t*) leaf->GetValuePointer();
//  }
//
//  return true;
//}

bool Pruner::setOutput(const char* outputDataFile){
  if(outputDataFile){
    std::string filePath;
    if(strncmp(outputDataFile, "/store/", strlen("/store/"))==0){
      filePath = std::string("root://eoscms.cern.ch//eos/cms") + outputDataFile;
    } else{
      filePath = outputDataFile;
    }

    //fout_ = std::auto_ptr<TFile>(TFile::Open(filePath.c_str(), "RECREATE"));
    fout_ = TFile::Open(filePath.c_str(), "RECREATE");
    //fout_ = new TFile(filePath.c_str(), "RECREATE");

    if(fout_->IsZombie()){
      cout << "Failed to open output ntuple file " << outputDataFile << endl;
      return false;
    }

    foutDir_ =  fout_->mkdir("tupel");
    if(foutDir_ == 0){
      cerr << "Failed to create directory tupel in output ROOT file " << outputDataFile << endl;
      return false;
    }
  }

  //  TObjArray* bs = treeRcd->tree->GetListOfBranches();
  TObjArray* bs = chain_.GetListOfBranches();
  TIter itBranches(bs);
  while(itBranches.Next()){
    TBranch* b = (TBranch*)(*itBranches);
    //printf("%s\n",b->GetName());
    //    if(strcmp(b->GetName(), "EvtNum") == 0)
    //disable copy of filered-out branches:
    if(strcmp(b->GetName(), "EvtNum") != 0
       && strcmp(b->GetName(), "EvtRunNum") != 0
       && strcmp(b->GetName(), "EvtWeights") != 0
       && !filterBranch(b->GetName())){
      if(verbose_) 
	cout << "Disable branch " << b->GetName() << "\n";
      //      treeRcd->tree->SetBranchStatus(b->GetName(), 0);
      chain_.SetBranchStatus(b->GetName(), 0);
    }
  }

  //if(fout_.get()){
  if(fout_){
    fout_->cd();
    TDirectory::CurrentDirectory()->cd("tupel");
    outEventTree_ = chain_.CloneTree(0);
    chain_.GetFile()->cd(); //Is this cd needed?
  }

  return true;
}

void Pruner::copyEvent(){
  if(verbose_ > 1) cout << "Copying event " << (*eventNum_) << " of run " << (*runNum_) << endl;
  fout_->cd();
  outEventTree_->Fill();
}

bool Pruner::nextEvent(){
  ++ievent_;
  Long64_t entryInTree = chain_.LoadTree(ievent_);
  switch(entryInTree){
  case -1: //The chain is empty.
    std::cerr << "No event found in input file(s)!\n";
    return false;
  case -2: //The requested entry number is negative or is too large for the chain,
    //       or too large for the large TTree (?).
    return false;
  case -3: //The file corresponding to the entry could not be correctly open
    std::cerr << "Failed to open file containing the event #" << ievent_ << "\n";
    return false;
  case -4: //The TChainElement corresponding to the entry is missing or
    //       the TTree is missing from the file.
    std::cerr << "EventTree was not found in file ";
    if(chain_.GetFile()) std::cerr << chain_.GetFile()->GetName();
    std::cerr << "\n";
    return false;
  }

  //  std::cerr << ">>>>> chain_.GetTreeNumber()" << chain_.GetTreeNumber() << std::endl;

  if (chain_.GetTreeNumber() != treeNum_) {
    treeNum_ = chain_.GetTreeNumber();
    char* f = strdup(chain_.GetFile()->GetName());
    if(f){
      fileBaseName_ = *basename(f);
      free(f);
    } else{
      fileBaseName_ = "";
    }
    fillPerInputSummary();
  }
  return chain_.GetEntry(ievent_);
}

void Pruner::listSelections(std::ostream& o){
  o << "\nList of available selections (--selection) and subselections (--subselections)\n\n";
  for(std::map<std::string, ClassRecord>::const_iterator it = daughters_.begin();
      it != daughters_.end(); ++it){
    const ClassRecord& rcd = it->second;
    o << rcd.className;
    if(rcd.description.size() > 0){
      o << "\t" << rcd.description;
    }
    o << "\n";
    rcd.instance->declareSubSelections();
    std::vector<SubSelection>& subSels = rcd.instance->subSelections_;
    if(subSels.size() > 0){
      o << "\n\t"<< rcd.className << " Subselections:\n\n";
      for(unsigned i = 0; i < subSels.size(); ++i){
	o << "\t" << subSels[i].tag << "\t"
	  << subSels[i].description << "\n";
      }
    }
  }
  o << std::endl;
}
