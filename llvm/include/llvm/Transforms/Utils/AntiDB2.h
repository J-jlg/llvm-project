#ifndef LLVM_TRANSFORMS_UTILS_ANTIDB2
#define LLVM_TRANSFORMS_UTILS_ANTIDB2

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/Local.h"

namespace llvm {

class AntiDB2Pass : public PassInfoMixin<AntiDB2Pass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};



} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_ANTIDB2
