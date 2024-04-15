#include "llvm/Transforms/Utils/ImmediateObfuscation.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include "llvm/IR/Attributes.h"

using namespace llvm;

PreservedAnalyses ImmediateObfuscation::run(Function &F, FunctionAnalysisManager &AM) {

    IRBuilder<> builder(F.getContext());
    
    F.addFnAttr(Attribute::NoInline);
    F.addFnAttr(Attribute::OptimizeNone);

    for(auto& B : F){
        for(auto& I : B){

            if(isa<CallInst>(&I) || isa<PHINode>(&I))
                continue;

            outs() << I << "\n";
            outs() << "Uses: " << I.getNumUses() << "\n";
            outs() << "Terminator? " << I.isTerminator() << "\n";
            outs() << "Number of operands: " << I.getNumOperands() << "\n";
            for (unsigned i = 0; i < I.getNumOperands(); ++i) {
                if (auto* operand = dyn_cast<ConstantInt>(I.getOperand(i))) {
                    outs() << "Operand: " << operand->getValue() << "\n";
                    outs() << "Calc op: " << operand->getValue().getSExtValue() << "\n";
                    outs() << "Operand size: " << operand->getBitWidth() << "\n";
                    unsigned int bitwidth = operand->getBitWidth();

                    // Limit the size of the random number to the bitwidth of the operand
                    uint64_t bitwidth_limiter = (((uint64_t)std::pow(2.0,bitwidth)<<1) - 1);

                    uint64_t random1 = std::rand() & bitwidth_limiter;
                    uint64_t random2 = std::rand() & bitwidth_limiter;
                    uint64_t xor_operand = ((random1 + random2) & bitwidth_limiter)  ^ operand->getValue().getSExtValue();

                    BinaryOperator *add_op = BinaryOperator::CreateAdd(ConstantInt::get(operand->getType(), random1), ConstantInt::get(operand->getType(), (int)random2));
                    BinaryOperator *xor_op = BinaryOperator::CreateXor(add_op, ConstantInt::get(operand->getType(), xor_operand));

                    add_op->insertBefore(&I);
                    xor_op->insertBefore(&I);
                    I.setOperand(i, xor_op);

                }
            }
            outs() << "---------------------\n";
        }
    }

    return PreservedAnalyses::none(); //previously all()

}