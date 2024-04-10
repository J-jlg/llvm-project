#include "llvm/Transforms/Utils/AntiDB.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"
#include <random>
#include "llvm/IRReader/IRReader.h"
#include "llvm/Transforms/Utils/Linker.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/AntiDBHelper.h"

using namespace llvm;

PreservedAnalyses AntiDBPass::run(Function &F, FunctionAnalysisManager &AM) {
  Module &M = *F.getParent();
  static LLVMContext ctx;
  static IRBuilder<> builder(ctx);
  
  std::string antiDBFctName("_Z6antiDBv");

  SMDiagnostic Diag;
  StringRef ref = StringRef(AntiDBHelper::thread_code.c_str());
  std::unique_ptr<MemoryBuffer> buff = MemoryBuffer::getMemBuffer(ref);
  std::unique_ptr<Module> newModule =
      parseIR(*buff, Diag, M.getContext());

  Function *computeFctExternalModule = M.getFunction(antiDBFctName);
  if (computeFctExternalModule == nullptr) {
      Linker2::linkModules2(M, std::move(newModule));
  }
  
  std::vector<Value *> argsValAntiDB;
  Function *computeFctAntiDBExternalModule = M.getFunction(antiDBFctName);
  builder.SetInsertPoint(F.getEntryBlock().getFirstNonPHIOrDbg());
          Value *ptrCallInstAntiDB =
              builder.CreateCall(computeFctAntiDBExternalModule, argsValAntiDB);
  return PreservedAnalyses::none();
}
