#ifndef LLVM_TRANSFORMS_UTILS_ANTIDB
#define LLVM_TRANSFORMS_UTILS_ANTIDB

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/Local.h"

namespace llvm {

class AntiDBPass : public PassInfoMixin<AntiDBPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};



} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_ANTIDB
