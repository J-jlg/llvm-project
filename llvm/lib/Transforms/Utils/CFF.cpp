#include "llvm/Transforms/Utils/CFF.h"
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

#define DEBUG_TYPE "cff"

using namespace llvm;

cl::opt<int> prcApply("percCFF",
                      cl::desc("Specify the target BCF Function by Name"),
                      cl::value_desc("prcApply"));

// Shamefully borrowed from ../Scalar/RegToMem.cpp :(
bool valueEscapes(Instruction *Inst) {
  BasicBlock *BB = Inst->getParent();
  for (Value::use_iterator UI = Inst->use_begin(), E = Inst->use_end(); UI != E;
       ++UI) {
    Instruction *I = cast<Instruction>(*UI);
    if (I->getParent() != BB || isa<PHINode>(I)) {
      return true;
    }
  }
  return false;
}

// Shamefully borrowed from ../Scalar/RegToMem.cpp :(
void CFFPass::fixStack(Function *f) {
  // Try to remove phi node and demote reg to stack
  std::vector<PHINode *> tmpPhi;
  std::vector<Instruction *> tmpReg;
  BasicBlock *bbEntry = &*f->begin();

  do {
    tmpPhi.clear();
    tmpReg.clear();

    for (Function::iterator i = f->begin(); i != f->end(); ++i) {

      for (BasicBlock::iterator j = i->begin(); j != i->end(); ++j) {

        if (isa<PHINode>(j)) {
          PHINode *phi = cast<PHINode>(j);
          tmpPhi.push_back(phi);
          continue;
        }
        if (!(isa<AllocaInst>(j) && j->getParent() == bbEntry) &&
            (valueEscapes(&*j) || j->isUsedOutsideOfBlock(&*i))) {
          tmpReg.push_back(&*j);
          continue;
        }
      }
    }
    for (unsigned int i = 0; i != tmpReg.size(); ++i) {
      DemoteRegToStack(*tmpReg.at(i));
    }

    for (unsigned int i = 0; i != tmpPhi.size(); ++i) {
      DemotePHIToStack(tmpPhi.at(i));
    }

  } while (tmpReg.size() != 0 || tmpPhi.size() != 0);
}

PreservedAnalyses CFFPass::run(Function &F, FunctionAnalysisManager &AM) {
  Module *module = F.getParent();
  static LLVMContext ctx;
  static IRBuilder<> Builder(ctx);
  time_t t;
  srand((unsigned)time(&t));
  IntegerType *intType = Type::getInt32Ty(F.getContext());
  SmallVector<BasicBlock *> WorkList;
  std::unordered_set<BasicBlock *> reachable;

  int anzCn = 0;
  BasicBlock *entry = &F.getEntryBlock();

  std::map<BasicBlock *, SmallVector<SpecialEdgeClass *>> potentialUserList;

  if (F.hasPersonalityFn() || F.hasOptNone()) {
    return PreservedAnalyses::all();
  }
  Instruction *InstStart = F.getEntryBlock().getFirstNonPHI();

  WorkList.push_back(InstStart->getParent());
  reachable.insert(InstStart->getParent());

  while (!WorkList.empty()) {
    BasicBlock *front = WorkList.back();
    WorkList.pop_back();
    for (BasicBlock *succ : successors(front)) {
      if (succ->isEHPad() || succ->isLandingPad()) {
        return PreservedAnalyses::all();
      }
      if (reachable.count(succ) == 0) {
        WorkList.push_back(succ);
        reachable.insert(succ);
      }
    }
  }

   std::vector<SpecialEdgeClass *> l1;

   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> distr(0, 100 - 1);
   int random = (prcApply.isDefaultOption())?100:prcApply.getValue();

  for (BasicBlock *BB : reachable) {
    for (BasicBlock *succ : successors(BB)) {
      if (distr(gen) > random-1)
        continue;
      BasicBlock *inserted = SplitEdge(BB, succ);
      BasicBlock *after = SplitEdge(inserted, succ);
      SpecialEdgeClass *sec = new SpecialEdgeClass;
      sec->s1 = inserted;
      sec->s2 = after;
      l1.push_back(sec);
    }
  }
  //avoid SSA-Form violation
  fixStack(&F);

  BasicBlock *phiCentral;
  PHINode *phiNodeCe;
  SwitchInst *switchNodeCe;
  int nmbrCr = 0;
  bool created = false;
  for (SpecialEdgeClass *sec : l1) {
    if (!created) {
      phiCentral = BasicBlock::Create(ctx, "", & F);
      phiNodeCe = PHINode::Create(intType, 0, "");
      Builder.SetInsertPoint(phiCentral);
      switchNodeCe = Builder.CreateSwitch(phiNodeCe, sec->s2, 0);
      phiNodeCe->insertBefore(switchNodeCe);
      created = true;
    }

    BranchInst *toPhiInst = BranchInst::Create(phiCentral);
    ReplaceInstWithInst(sec->s1->getTerminator(),
                        toPhiInst); // setSuccessor, da Unique!

    phiNodeCe->addIncoming(ConstantInt::get(intType, nmbrCr), sec->s1);
    switchNodeCe->addCase(ConstantInt::get(intType, nmbrCr), sec->s2);

    nmbrCr++;
  }
  return PreservedAnalyses::none();
}
