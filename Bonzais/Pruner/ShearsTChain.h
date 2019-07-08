#include "TChain.h"
#include "TCollection.h"
#include "TLeaf.h"
#include <fstream>
#include <iostream>

#ifndef ShearsTChain_h
#define ShearsTChain_h

class ShearsTChain: public TChain{
public:

  //ShearsTChain(const char* name, const char* title = "", const char* catalog = 0): TChain(name, title) {
  //  if(catalog) setCatalog(catalog);
  //}
  
  ShearsTChain(): TChain("tupel/EventTree", "ShearsTChain"), verbosity_(0) {}
  
  ShearsTChain(const char* name, const char* title = "ShearsTChain"): TChain(name, title) {}
    
  virtual ~ShearsTChain() {}

  void setVerbosity(int val){ verbosity_ = val; }
  
  int getVerbosity() const { return verbosity_; }

  bool setCatalog(const char* catalog, int maxFiles = -1, int skipFiles = 0);
  
  /** Displays the list of Event Tree leaves
   */
  void lsLeaves(){
    TIter it(GetListOfLeaves());
    while(it.Next()){
      std::cout << ((TLeaf*)(*it))->GetTypeName() << "\t" << ((*it))->GetName() << std::endl;
    }
  }

  /** Display the description of a branch of the event tree
   * @param branchName the name of the branch to get the help on
   */
  void branchHelp(const char* branchName);
  
protected:
  int verbosity_;

  std::string firstFile_;
  
  //  ClassDef(ShearsTChain, 1);
};
#endif
