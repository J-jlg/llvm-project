#ifndef LLVM_TRANSFORMS_UTILS_THREADSOBF
#define LLVM_TRANSFORMS_UTILS_THREADSOBF

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/Local.h"
#include <unordered_set>
#include <random>

namespace llvm {

class ThreadsObfPass : public PassInfoMixin<ThreadsObfPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};


} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_THREADSOBF
