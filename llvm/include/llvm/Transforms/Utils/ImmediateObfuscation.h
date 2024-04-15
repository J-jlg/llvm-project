#ifndef LLVM_TRANSFORMS_UTILS_ImmediateObfuscation_H
#define LLVM_TRANSFORMS_UTILS_ImmediateObfuscation_H

#include "llvm/IR/PassManager.h"

namespace llvm {

    class ImmediateObfuscation : public PassInfoMixin<ImmediateObfuscation> {

        public:
            PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

    };

  

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_ImmediateObfuscation_H