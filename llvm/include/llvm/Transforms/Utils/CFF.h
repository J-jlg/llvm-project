#ifndef LLVM_TRANSFORMS_UTILS_CFF
#define LLVM_TRANSFORMS_UTILS_CFF

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/Local.h"
#include <unordered_set>

namespace llvm {

class SpecialEdgeClass {
public:
  BasicBlock *s1;
  BasicBlock *s2;
};

class PhiReDefClass {
public:
  std::unordered_set<BasicBlock *> *userBBList;
  Instruction *value;
  PHINode *reDef;
};

class PhiBBClass {
public:
  BasicBlock* BB;
  Instruction *instDef;
  PHINode *phi;
};

class PhiBBClassLoop {
public:
  BasicBlock *BB;
  Instruction *instDef;
  PHINode *phiCur;
  PHINode *phiNew;
};



class CFFPass : public PassInfoMixin<CFFPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
  void fixStack(Function *f);
};



} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_CFF
