#include "llvm/Transforms/Utils/ThreadsObf.h"
#include "llvm/Transforms/Utils/ThreadHelper.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Transforms/Utils/Linker.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include <random>
#include <utility>
#include <vector>
#include <algorithm>
#include <thread>
#include <iostream>
#include <map>
#include <mutex>


int globalVar1 = 0;
int globalVar2 = 0;
int globalVar3 = 0;

// Mutexe für kritische Abschnitte
std::mutex mutex1;
std::mutex mutex2;
std::mutex mutex3;

void executeFunction(int functionIndex) {
  // Temporäre Variablen für zusätzliche Rechenoperationen
  int temp1 = 0, temp2 = 0, temp3 = 0;

  // Dispatcher für die Funktionen basierend auf dem übergebenen Index
  switch (functionIndex) {
  case 1: {
    for (int i = 0; i < 40; ++i) {

      if (globalVar1 == 0) {
        globalVar1 += 5;
      } else if (globalVar2 > 10) {
        globalVar1 *= 2;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += globalVar1;
      globalVar3 = globalVar1 + globalVar2;
      globalVar1 *= globalVar3;
      globalVar2 -= globalVar3;
      globalVar3 /= 2;
    }
    if (globalVar1 < 20 && globalVar2 > 5) {
      globalVar1 += 10;
    } else {
      globalVar1 -= 5;
    }
    break;
  }
  case 2: {
    // Zusätzliche Rechenoperationen am Anfang (längere Ausführungsdauer)
    for (int i = 0; i < 30; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      // int temp = i * 2;
      // for (int i = 0; i < 10000000; ++i) {
      //	temp = ((i*2)%globalVar2);
      // }
      // std::this_thread::sleep_for(std::chrono::milliseconds(20));
      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 3: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    if (globalVar1 == 123) {
      globalVar1++;
    }
    // Mutex erst später sperren
    for (int i = 0; i < 40; ++i) {
      if (globalVar3 < 10) {
        globalVar3 += 10;
      } else {
        globalVar3 -= 7;
      }
      globalVar1 -= globalVar3;
      globalVar2 += globalVar3;
      globalVar3 *= 3;
      globalVar2 -= 5;
    }
    break;
  }
  case 4: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    int count3 = 0, temp5 = 0;
    while (count3 < 12) {
      temp5 ^= globalVar2;
      temp5 |= globalVar1;
      ++count3;
    }

    int temp4 = temp5 % 2;

    for (int i = 0; i < 20; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
    }
    break;
  }
  case 5: {
    for (int i = 0; i < 10; ++i) {
      if (globalVar1 == 0) {
        globalVar1 += 5;
      } else if (globalVar2 > 10) {
        globalVar1 *= 2;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += globalVar1;
      globalVar3 = globalVar1 + globalVar2;
      globalVar1 *= globalVar3;
      globalVar2 -= globalVar3;
    }
    if (globalVar1 < 20 && globalVar2 > 5) {
      globalVar1 += 10;
    } else {
      globalVar1 -= 5;
    }
    break;
  }
  case 6: {
    for (int i = 0; i < 30; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp = 0;
      for (int i = 0; i < 2; ++i) {
        temp = ((i * 2) + (globalVar2 + 1));
      }

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 7: {
    for (int i = 0; i < 40; ++i) {
      if (globalVar3 < 10) {
        globalVar3 += 10;
      } else {
        globalVar3 -= 7;
      }
      globalVar1 -= globalVar3;
      globalVar2 += globalVar3;
      globalVar3 *= 3;
      globalVar1 /= globalVar3;
      globalVar2 -= 5;
    }
    break;
  }
  case 8: {
    int count3 = 0, temp3 = 0;
    while (count3 < 100) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }
    int temp4 = temp3 % 2;
    // Verzögerung hinzufügen
    // Mutex erst später sperren
    for (int i = 0; i < 20; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    break;
  }
  case 9: { // Mutex erst später sperren
    for (int i = 0; i < 10; i++) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp5 = 0;
      for (int i = 0; i < 3; i++) {
        temp5 = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 1));
      }
      // std::this_thread::sleep_for(std::chrono::milliseconds(11));
      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 10: {
    for (int i = 0; i < 40; ++i) {
      if (globalVar1 == 0) {
        globalVar1 += 5;
      } else if (globalVar2 > 10) {
        globalVar1 *= 2;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += globalVar1;
      globalVar3 = globalVar1 + globalVar2;
      globalVar1 *= globalVar3;
      globalVar2 -= globalVar3;
      globalVar3 /= 2;
    }
    if (globalVar1 < 20 && globalVar2 > 5) {
      globalVar1 += 10;
    } else {
      globalVar1 -= 5;
    }
    break;
  }
  case 11: {
    for (int i = 0; i < 30; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp = 0;
      for (int i = 0; i < 2; ++i) {
        temp = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 1));
      }

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 12: {
    for (int i = 0; i < 1; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }

      // std::this_thread::sleep_for(std::chrono::milliseconds(8));
      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 13: {
    for (int i = 0; i < 20; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp = i * 2;

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      for (int i = 0; i < 1; ++i) {
        temp = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 1));
      }
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 14: {
    for (int i = 0; i < 30; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      int temp = i * 2;
      for (int i = 0; i < 4; ++i) {
        temp = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 4));
      }
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 15: {
    for (int i = 0; i < 20; i++) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      int count3 = 0, temp3 = 0;
      while (count3 < 23) {
        temp3 ^= globalVar2;
        temp3 |= globalVar1;
        ++count3;
      }

      int temp4 = temp3 % 2;

      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    break;
  }
  case 16: {
    for (int i = 0; i < 20; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 12) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }

    int temp4 = temp3 % 2;
    
    break;
  }
  case 17: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    for (int i = 0; i < 20; ++i) {
      
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 31) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }
    int temp4 = temp3 % 2;

    
    break;
  }
  case 18: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    for (int i = 0; i < 20; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 6) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }

    int temp4 = temp3 % 2;

    
    break;
  }
  case 0: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    for (int i = 0; i < 20; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 1) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }
    
    break;
  }

  default:
    std::cerr << "Ungültiger Funktionindex!" << std::endl;
  }
}

// oder void und direkt globale variable auslesen? Hilft vielleicht für das
// inlinen, falls nur void geht
int execFunctionsGlobal(int k1, int k2, int k3, int k4) {
  // 10mal ausfüren
  std::map<int, int> counters;

  for (int i = 0; i < 20; i++) {
    globalVar1 = 0;
    globalVar2 = 0;
    globalVar3 = 0;

    std::thread t1(executeFunction, k1);
    int temp = globalVar1 +5;
    temp %= 2;
    std::this_thread::sleep_for(std::chrono::nanoseconds(5));
    std::thread t2(executeFunction, k2);
    temp = globalVar1 + 5;
    temp *= 3;
    temp %= 2;
    std::this_thread::sleep_for(std::chrono::nanoseconds(5));
    std::thread t3(executeFunction, k3);
    temp = globalVar1 + 5;
    temp *= 3;
    temp %= 2;
    std::this_thread::sleep_for(std::chrono::nanoseconds(5));
    std::thread t4(executeFunction, k4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    if (globalVar1 < 0)
      globalVar1 = globalVar1 * (-1);
    counters[globalVar1 % 600] = counters[globalVar1 % 600] + 1;
  }
  std::map<int, int>::iterator best = std::max_element(
      counters.begin(), counters.end(),
      [](const std::pair<int, int> &a, const std::pair<int, int> &b) -> bool {
        return a.second < b.second;
      });
  return best->first;
}

using namespace llvm;

PreservedAnalyses ThreadsObfPass::run(Function &F, FunctionAnalysisManager &AM) {
  Module &M = *F.getParent();
  IRBuilder<> builder(F.getContext());
  SmallVector<ICmpInst *> WorkList;
  SmallVector<CallInst *> WorkListCall;
  SmallVector<Module::global_iterator> WorkGV;
  std::random_device rdev;
  std::mt19937 randomng(rdev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 200);
  std::uniform_int_distribution<std::mt19937::result_type> dist5(1, 5);

  std::uniform_int_distribution<std::mt19937::result_type> dist20(1, 20);

  //TODO: hardcode the thread_code.ll code into a std::string raw textblock. std::string s = R"""(.ll code)""";
  std::string fctName("_Z19execFunctionsGlobaliiii");

  SMDiagnostic Diag;
  StringRef ref = StringRef(ThreadHelper::thread_code.c_str());
  std::unique_ptr<MemoryBuffer> buff = MemoryBuffer::getMemBuffer(ref);
  std::unique_ptr<Module> newModule =
      parseIR(*buff, Diag, M.getContext());

  Function *computeFctExternalModule = M.getFunction(fctName);
  if (computeFctExternalModule == nullptr) {
      for (Module::global_iterator J2 = M.global_begin(); J2 != M.global_end();
           ++J2) {
        if (J2->isConstant()) {
          WorkGV.push_back(J2);
        }
      }
      //TODO: parseIRFile -> parseFile to use raw string instead of hardcoded file?!
      Linker2::linkModules2(M, std::move(newModule));
  }
  
  Function *computeFctOwn = M.getFunction(fctName);

  for (Instruction &I : instructions(F)) {
    if (auto cmp = dyn_cast<ICmpInst>(&I)) {
      if (auto constant = dyn_cast<ConstantInt>(cmp->getOperand(1))) {
        outs() << constant->getIntegerType()->getBitWidth();
          if (constant->isNegative() || constant->getIntegerType()->getBitWidth()!=32)
            continue;
        WorkList.push_back(cmp);
	outs() << "applied thread obf";
      }
    } else if (auto call = dyn_cast<CallInst>(&I)) {
      if (call->getCalledFunction()->getName().equals("strncpy")) {
        WorkListCall.push_back(call);
      }
    }
  }

  for (auto J2 : WorkGV) {
    auto constValue = J2->getInitializer();

    auto con = dyn_cast<ConstantDataArray>(constValue);
    if (!con)
      continue;
    if (con) {
        //for easy crackme challenge
      if (con->getNumElements()<1 || con->getElementAsInteger(0) == 80) {
        continue;
      }

      char *str = new char[con->getNumElements()];
      char *strKey = new char[con->getNumElements()];
      for (int elemIdx = 0; elemIdx < con->getNumElements() - 1; elemIdx++) {
        int offsetRand = dist20(randomng);
        str[elemIdx] = con->getElementAsInteger(elemIdx) ^ offsetRand;
        strKey[elemIdx] = 0x00 + offsetRand;
      }
      str[con->getNumElements() - 1] = 0x00;
      strKey[con->getNumElements() - 1] = 0x00;
      auto a4 = dyn_cast<GlobalVariable>(J2);
      auto contest = ConstantDataArray::getString(M.getContext(), str, true);
      auto contestKey =
          ConstantDataArray::getString(M.getContext(), strKey, true);
      auto GV = new GlobalVariable(
          M,
          ArrayType::get(IntegerType::getInt8Ty(M.getContext()),
                         con->getNumElements()),
          true, J2->getLinkage(), contestKey, "affe", a4);
      GV->setInitializer(contestKey);
      J2->setInitializer(contest);
      std::string fctName("_Z10decodeCharPcS_i");
      Function *computeFctExternalModule = M.getFunction(fctName);
      std::vector<Value *> argsVal;

      auto aK = dyn_cast<GlobalVariable>(GV);
      argsVal.push_back(a4);
      argsVal.push_back(aK);
      argsVal.push_back(ConstantInt::get(
          IntegerType::getInt32Ty(F.getContext()), con->getNumElements()));
      for (auto &use : J2->uses()) {
        if (auto calllInst = dyn_cast<CallInst>(use.getUser())) {
          builder.SetInsertPoint(calllInst);
          Value *ptrCallInst =
              builder.CreateCall(computeFctExternalModule, argsVal);
          calllInst->setArgOperand(use.getOperandNo(), ptrCallInst);
          break;
        }
      }
    }
  }

  for (CallInst *call : WorkListCall) {
    builder.SetInsertPoint(call->getNextNode());
    auto GV = dyn_cast<GlobalVariable>(call->getArgOperand(1));

    auto constValue = GV->getInitializer();

    auto con = dyn_cast<ConstantDataArray>(constValue);

    int64_t *key = new int64_t[con->getNumElements()];
   
    for (int elemIdx = 0; elemIdx < con->getNumElements(); elemIdx++) {

      // create target random value
      
      uint64_t valueDest = dist(randomng);

      key[elemIdx] = con->getElementAsInteger(elemIdx) - valueDest;

      Value *ConstEncoded =
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), valueDest);
      std::vector<Value *> argsVal;

      int64_t in1Sub = dist5(randomng);
      int64_t in2Sub;
      int64_t in3Sub;
      int64_t in4Sub;
      BinaryOperator *in1SubBO = BinaryOperator::Create(
          BinaryOperator::Sub,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           con->getElementAsInteger(elemIdx)),
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in1Sub),
          "in1SubCall");
      in1SubBO->insertAfter(call);

      BinaryOperator *in1SubRem = BinaryOperator::Create(
          BinaryOperator::URem, in1SubBO,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in1Call");
      in1SubRem->insertAfter(in1SubBO);

      argsVal.push_back(in1SubRem);

      // 2nd Value
      in2Sub = dist5(randomng);

      BinaryOperator *in2SubBO = BinaryOperator::Create(
          BinaryOperator::Sub,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           con->getElementAsInteger(elemIdx)),
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in2Sub),
          "in2SubCall");
      in2SubBO->insertAfter(call);

      BinaryOperator *in2SubRem = BinaryOperator::Create(
          BinaryOperator::URem, in2SubBO,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in2Call");
      in2SubRem->insertAfter(in2SubBO);

      argsVal.push_back(in2SubRem);

      // 3rd +4rd + call
      in3Sub = dist5(randomng);

      BinaryOperator *in3SubBO = BinaryOperator::Create(
          BinaryOperator::Sub,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           con->getElementAsInteger(elemIdx)),
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in3Sub),
          "in3SubCall");
      in3SubBO->insertAfter(call);

      BinaryOperator *in3SubRem = BinaryOperator::Create(
          BinaryOperator::URem, in3SubBO,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in3");
      in3SubRem->insertAfter(in3SubBO);

      argsVal.push_back(in3SubRem);

      in4Sub = con->getElementAsInteger(elemIdx) - in1Sub - in2Sub - in3Sub;
      BinaryOperator *in4SubBO = BinaryOperator::Create(
          BinaryOperator::Sub,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           con->getElementAsInteger(elemIdx)),
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in4Sub),
          "in4SubCall");
      in4SubBO->insertAfter(call);

      BinaryOperator *in4SubRem = BinaryOperator::Create(
          BinaryOperator::URem, in4SubBO,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in4Call");
      in4SubRem->insertAfter(in4SubBO);

      argsVal.push_back(in4SubRem);


      ArrayRef<Value *> args(argsVal);
      CallInst *callThreads = builder.CreateCall(computeFctOwn, args);
      int64_t execFunResult = execFunctionsGlobal(
          (con->getElementAsInteger(elemIdx) - in1Sub) % 17,
          (con->getElementAsInteger(elemIdx) - in2Sub) % 17,
          (con->getElementAsInteger(elemIdx) - in3Sub) % 17,
          (con->getElementAsInteger(elemIdx) - in4Sub) % 17);
      BinaryOperator *diffSub;
      if (execFunResult < valueDest) {
        int differenceInts = valueDest - execFunResult;
        if (execFunResult < 0) {
          differenceInts = valueDest + execFunResult;
        }
        diffSub = BinaryOperator::Create(
            BinaryOperator::Add, callThreads,
            ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                             differenceInts),
            "diffAddCall");
      } else {
        int differenceInts = execFunResult - valueDest;
        diffSub = BinaryOperator::Create(
            BinaryOperator::Sub, callThreads,
            ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                             differenceInts),
            "diffSubCall");
      }
      diffSub->insertAfter(callThreads);
      CastInst* trunc = TruncInst::CreateIntegerCast(
          diffSub, IntegerType::getInt8Ty(F.getContext()), false, "hihiCall");
      trunc->insertAfter(diffSub);
      auto load = dyn_cast<LoadInst>(call->getArgOperand(0));
      std::vector<Value *> idxVectorGEP;

      idxVectorGEP.push_back(
          ConstantInt::get(IntegerType::getInt64Ty(F.getContext()), elemIdx));
      ArrayRef<Value *> valsIdxVector(idxVectorGEP);
      auto gep = builder.CreateGEP(IntegerType::getInt8Ty(F.getContext()), load,
                                   valsIdxVector);
      builder.CreateStore(trunc, gep);

      BinaryOperator *restoreInst = BinaryOperator::Create(
          BinaryOperator::Add, diffSub,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           key[elemIdx]),
          "restoreddCall");
      restoreInst->insertAfter(diffSub);

      builder.CreateStore(restoreInst
              ,
          gep);
      // I->setOperand(0, diffSub);

    }
  }

  for (ICmpInst *I : WorkList) {
    auto constantOperandDest = dyn_cast<ConstantInt>(I->getOperand(1));
    int64_t constantDest = constantOperandDest->getValue().getZExtValue();

    uint64_t valueDest = dist(randomng);
    Value *ConstEncoded =
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), valueDest);
    I->setOperand(1, ConstEncoded);

    builder.SetInsertPoint(I);

    if (constantDest <= 25) {
      // TODO: Impl. edge casr
    }

    std::vector<Value *> argsVal;

    int64_t in1Sub = dist5(randomng);
    int64_t in2Sub;
    int64_t in3Sub;
    int64_t in4Sub;
    BinaryOperator *in1SubBO = BinaryOperator::Create(
        BinaryOperator::Sub, I->getOperand(0),
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in1Sub),
        "in1Sub");
    in1SubBO->insertBefore(I);

    BinaryOperator *in1SubRem = BinaryOperator::Create(
        BinaryOperator::URem, in1SubBO,
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in1");
    in1SubRem->insertBefore(I);

    argsVal.push_back(in1SubRem);

    if (constantDest - in1Sub <= 25) {

      in2Sub = constantDest - in1Sub;
      BinaryOperator *in2SubBO = BinaryOperator::Create(
          BinaryOperator::Sub, I->getOperand(0),
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in2Sub),
          "in2Sub");
      in2SubBO->insertBefore(I);

      BinaryOperator *in2SubRem = BinaryOperator::Create(
          BinaryOperator::URem, in2SubBO,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in2");
      in2SubRem->insertBefore(I);

      argsVal.push_back(in2SubRem);

      for (int i = 0; i < 2; i++) {
        BinaryOperator *in34SubBO = BinaryOperator::Create(
            BinaryOperator::Sub, I->getOperand(0),
            ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 0));
        in34SubBO->insertBefore(I);

        BinaryOperator *in34SubRem = BinaryOperator::Create(
            BinaryOperator::URem, in34SubBO,
            ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17));
        in34SubRem->insertBefore(I);

        argsVal.push_back(in34SubRem);
      }

      ArrayRef<Value *> args(argsVal);
      CallInst *callThreads = builder.CreateCall(computeFctOwn, args);

      int64_t differenceInts = execFunctionsGlobal((constantDest - in1Sub) % 17,
                                                   (constantDest - in2Sub) % 17,
                                                   (constantDest - 0) % 17,
                                                   (constantDest - 0) % 17) -
                               valueDest;

      BinaryOperator *diffSub = BinaryOperator::Create(
          BinaryOperator::Sub, callThreads,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           differenceInts),
          "diffSub");
      diffSub->insertBefore(I);
      I->setOperand(0, diffSub);
      continue;
    }

    in2Sub = dist5(randomng);

    BinaryOperator *in2SubBO = BinaryOperator::Create(
        BinaryOperator::Sub, I->getOperand(0),
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in2Sub),
        "in2Sub");
    in2SubBO->insertBefore(I);

    BinaryOperator *in2SubRem = BinaryOperator::Create(
        BinaryOperator::URem, in2SubBO,
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in2");
    in2SubRem->insertBefore(I);

    argsVal.push_back(in2SubRem);

    if ((constantDest - in1Sub - in2Sub) <= 25) {

      in3Sub = constantDest - in1Sub - in2Sub;
      BinaryOperator *in3SubBO = BinaryOperator::Create(
          BinaryOperator::Sub, I->getOperand(0),
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in3Sub),
          "in3Sub");
      in3SubBO->insertBefore(I);

      BinaryOperator *in3SubRem = BinaryOperator::Create(
          BinaryOperator::URem, in3SubBO,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in3");
      in3SubRem->insertBefore(I);

      argsVal.push_back(in3SubRem);

      for (int i = 0; i < 1; i++) {
        BinaryOperator *in4SubBO = BinaryOperator::Create(
            BinaryOperator::Sub, I->getOperand(0),
            ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 0));
        in4SubBO->insertBefore(I);

        BinaryOperator *in4SubRem = BinaryOperator::Create(
            BinaryOperator::URem, in4SubBO,
            ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17));
        in4SubRem->insertBefore(I);

        argsVal.push_back(in4SubRem);
      }
      
      ArrayRef<Value *> args(argsVal);
      CallInst *callThreads = builder.CreateCall(computeFctOwn, args);

      int64_t differenceInts = execFunctionsGlobal((constantDest - in1Sub) % 17,
                                                   (constantDest - in2Sub) % 17,
                                                   (constantDest - in3Sub) % 17,
                                                   (constantDest - 0) % 17) -
                               valueDest;
      BinaryOperator *diffSub = BinaryOperator::Create(
          BinaryOperator::Sub, callThreads,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           differenceInts),
          "diffSub");
      diffSub->insertBefore(I);
      I->setOperand(0, diffSub);

      continue;
    }

    in3Sub = dist5(randomng);

    BinaryOperator *in3SubBO = BinaryOperator::Create(
        BinaryOperator::Sub, I->getOperand(0),
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in3Sub),
        "in3Sub");
    in3SubBO->insertBefore(I);

    BinaryOperator *in3SubRem = BinaryOperator::Create(
        BinaryOperator::URem, in3SubBO,
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in3");
    in3SubRem->insertBefore(I);

    argsVal.push_back(in3SubRem);

    in4Sub = constantDest - in1Sub - in2Sub - in3Sub;
    BinaryOperator *in4SubBO = BinaryOperator::Create(
        BinaryOperator::Sub, I->getOperand(0),
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), in4Sub),
        "in4Sub");
    in4SubBO->insertBefore(I);

    BinaryOperator *in4SubRem = BinaryOperator::Create(
        BinaryOperator::URem, in4SubBO,
        ConstantInt::get(IntegerType::getInt32Ty(F.getContext()), 17), "in4");
    in4SubRem->insertBefore(I);

    argsVal.push_back(in4SubRem);
    
    ArrayRef<Value *> args(argsVal);
    CallInst *callThreads = builder.CreateCall(computeFctOwn, args);
    int64_t execFunResult = execFunctionsGlobal(
        (constantDest - in1Sub) % 17, (constantDest - in2Sub) % 17,
        (constantDest - in3Sub) % 17, (constantDest - in4Sub) % 17);
    BinaryOperator *diffSub;
    if (execFunResult < valueDest) {
      int differenceInts = valueDest - execFunResult;
      if (execFunResult < 0) {
        differenceInts = valueDest + execFunResult;
      }
      diffSub = BinaryOperator::Create(
          BinaryOperator::Add, callThreads,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           differenceInts),
          "diffAdd");
    } else {
      int differenceInts = execFunResult - valueDest;
      diffSub = BinaryOperator::Create(
          BinaryOperator::Sub, callThreads,
          ConstantInt::get(IntegerType::getInt32Ty(F.getContext()),
                           differenceInts),
          "diffSub");
    }
    diffSub->insertBefore(I);
    I->setOperand(0, diffSub);
  }

  return PreservedAnalyses::none();
}
