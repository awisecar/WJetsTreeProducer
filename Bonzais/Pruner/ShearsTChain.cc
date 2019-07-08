#include "ShearsTChain.h"
#include "TFile.h"

//ClassImp(ShearsTChain);

bool ShearsTChain::setCatalog(const char* catalog, int maxFiles, int skipFiles){
  std::ifstream f(catalog);
  if(!f.good()){
    std::cerr << "Failed to open file "<< catalog << "!\n";
    return false;
  }
    
  //Remove previously added files:
  Reset();

  int iline = 0;
  int nfiles = 0;
  firstFile_ = "";
  while(f.good()){
    ++iline;
    std::string l;
    std::string::size_type p;
      
    std::getline(f, l);

    //trim white spaces:
    p = l.find_first_not_of(" \t");
    if(p!=std::string::npos) l.erase(0, p);
    p = l.find_last_not_of(" \t\n\r");
    if(p!=std::string::npos) l.erase(p + 1);
    else l.clear();
      
    //skip empty lines and comment lines:
    if (!l.size() || l[0] == '#') continue;
      
    if (!l.size() || l[0] == '*') continue;
      
    //extract first column (file name):
    p = l.find_first_of(" \t");
    if(p!=std::string::npos) l.erase(p);
      
    //sanity check:
    const char ext[6] = ".root";
      
    if(l.size() < sizeof(ext) || l.substr(l.size() - sizeof(ext) + 1) != ext){
      std::cerr << "Line " << iline << " of catalog file " << catalog << " was skipped.\n";
      continue;
    }
      
    //Solves EOS paths:
    std::string store("/store/");
    if(l.substr(0,7) == store){
      //A CMS EOS path
      //l.insert(0, "/eos/uscms");
      // l.insert(0, "root://cmsxrootd.fnal.gov//");
      l.insert(0, "root://eoscms.cern.ch//eos/cms");
    }
      
    if(skipFiles <= 0){
      ++nfiles;
      if((maxFiles > 0) &&  (nfiles > maxFiles)) break;
      if(verbosity_>0){
	std::cout << "Add file " << l.c_str() << " to the list of input files.\n";
      }
      if(Add(l.c_str(),0) == 0){
	printf("Didnt find the file, exiting\n");
	return false;
      }
      if(firstFile_.size()==0) firstFile_ = l;
    } else{
      --skipFiles;
    }      
  }
  
  return true;
}

void ShearsTChain::branchHelp(const char* branchName){
  TFile* firstFile = (TFile*) GetListOfFiles()->First();
  if(firstFile_.empty()){
    std::cerr << "No ntuple file found. Did you set the catalog using the setCatalog method?\n";
    return;
  }
  TFile* f =  TFile::Open(firstFile_.c_str());
  if(!f || f->IsZombie()){
    std::cerr << "Failed to open file " << firstFile_ << "\n";
    return;
  }
  TTree* t;
  f->GetObject("tupel/Description", t);
  if(!t){
    std::cerr << "The branch description tree tupel/Description was not found in file "
	      << firstFile->GetName() << ".  Please check that the file is a boabab or bonzai file.\n";
    return;
  }
  if(t->GetEntries()<1){
    std::cerr << "The tree tupel/Description found in file "
	      << firstFile->GetName() << " is empty!\n";
    return;
  }

  TLeaf* l = t->FindLeaf(branchName);

  if(!l){
    std::cerr << "No description was found for branch " << branchName << " in file "
	      << firstFile->GetName() << "\n";
    return;
  }
  
  size_t len = l->GetLen();

  std::vector<char> buffer(len);
  
  l->SetAddress(&buffer[0]);
  
  t->GetEntry(0);

  std::cout << &buffer[0] << "\n";
}

#if defined(__ROOTCLING__)
#pragma link C++ class ShearsTChain;
#endif

