#include "X86.h"
#include "X86InstrBuilder.h"
#include "X86MachineFunctionInfo.h"
#include "X86RegisterInfo.h"
#include "X86Subtarget.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/InitializePasses.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

namespace {

class X86InstructionObfuscation : public MachineFunctionPass {
  MachineFunction *MF = nullptr;
  const X86Subtarget *ST = nullptr;
  const TargetInstrInfo *TII = nullptr;
  MachineRegisterInfo *MRI = nullptr;
  X86MachineFunctionInfo *X86FI = nullptr;
  MachineFrameInfo *MFI = nullptr;
  const TargetRegisterInfo *TRI = nullptr;
  MachineBasicBlock *MBB = nullptr;
  
  IndexedMap<int, VirtReg2IndexFunctor> StackSlotForVirtReg;


  void replaceSub1(std::vector<MachineInstr *> MI, int64_t fpuStore, int64_t al,
                   int64_t aluStack);
  void replaceSub2(std::vector<MachineInstr *> MI, int64_t fpuStore, int64_t al,
                   int64_t aluStack);

  void replaceMov(std::vector<MachineInstr *> MI, int64_t fpuStore, int64_t al,
                  int64_t aluStack);

  void replaceMov2(std::vector<MachineInstr *> MI, int64_t fpuStore, int64_t al,
                   int64_t aluStack);

  void replaceMov3(std::vector<MachineInstr *> MI, int64_t fpuStore, int64_t al,
                   int64_t aluStack);

  void replaceADD(std::vector<MachineInstr *> MI, int64_t fpuStore, int64_t al,
                  int64_t aluStack);


public:
  X86InstructionObfuscation() : MachineFunctionPass(ID), StackSlotForVirtReg(-1) {}

  /// Return the pass name.
  StringRef getPassName() const override {
    return "InstSubst Preconfigure";
  }

  /// Perform tile register configure.
  bool runOnMachineFunction(MachineFunction &MFunc) override;

  static char ID;
};

}

char X86InstructionObfuscation::ID = 0;

INITIALIZE_PASS_BEGIN(X86InstructionObfuscation, "InstSubst",
                      "Fast Tile Register Preconfigure", false, false)
INITIALIZE_PASS_END(X86InstructionObfuscation, "InstSubst",
                    "Fast Tile Register Preconfigure", false, false)

void X86InstructionObfuscation::replaceSub1(std::vector<MachineInstr *> MI,
                                       int64_t fpuStore, int64_t al,
                                       int64_t aluStack) {
  MachineInstr &MIAll = *MI.at(0);
  uint64_t newX = MIAll.getOperand(2).getImm() - 3;
  Register Xmmm = MRI->createVirtualRegister(&X86::VR128RegClass);
  Register r32 = MRI->createVirtualRegister(&X86::GR32RegClass);
  MachineBasicBlock &MBB = *MIAll.getParent();
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(5);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 16)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    8);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)), al,
                    0);
  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSrm), Xmmm), al, 0)
      .addReg(Xmmm);

  for (int cint = 0; cint <= 64; cint += 64) {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSmr)),
                      al, cint)
        .addReg(Xmmm);
  }
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 16)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(2);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(16);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FSAVEm)),
                    fpuStore, 0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(4);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::F2XM1));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRSTORm)),
                    fpuStore, 0);

  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRNDINT));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)), al,
                    8);
  MachineInstr *MIi = MI.at(1);
  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), al, 0);
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::SUB32mr)), al,
                    8)
      .addReg(r32);
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(newX);
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm), r32),
                    al, 0);
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::ADD32mr)), al,
                    8)
      .addReg(r32);
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32mr)),

                    al, 0)
      .addReg(MIAll.getOperand(1).getReg());
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm), r32),
                    al, 8);
  
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::SUB32mr)), al,
                    0)
      .addReg(r32);
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm),
                            MIAll.getOperand(0).getReg()),
                    al, 0);
  MIAll.getOperand(2).setImm(42);
}

void X86InstructionObfuscation::replaceSub2(std::vector<MachineInstr *> MI,
                                       int64_t fpuStore, int64_t al,
                                       int64_t aluStack) {
  MachineInstr &MIAll = *MI.at(0);
  uint64_t newX = MIAll.getOperand(2).getImm() - 2;
  MachineBasicBlock &MBB = *MIAll.getParent();
  Register r32 = MRI->createVirtualRegister(&X86::GR32RegClass);

  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FSAVEm)), al,
                    0);

  for (int i = 0; i <= 48; i += 8) {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      fpuStore, i)
        .addImm(0);
  }
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FNSTCW16m)),
                    fpuStore, 16);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::OR64mi32)),
                    fpuStore, 16)
      .addImm(1024);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FLDCW16m)),
                    fpuStore, 16);

  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    fpuStore, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    fpuStore, 16)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    fpuStore, 0)
      .addImm(2);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)),
                    fpuStore, 0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    fpuStore, 0)
      .addImm(16);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)),
                    fpuStore, 0);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRNDINT));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)),
                    fpuStore,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRSTORm)),
                    al, 0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(4);

  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), al,
      8);
  MachineInstr *MIi = MI.at(1);
  addFrameReference(
      BuildMI(MBB, MIAll.getNextNode(), DebugLoc(), TII->get(X86::SUB32mr)),
                    fpuStore,
                    0)
      .addReg(r32);

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(newX);
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm), r32),
                    al, 8);
  
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::ADD32mr)),
                    fpuStore, 0)
      .addReg(r32);

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32mr)),

                    al, 0)
      .addReg(MIAll.getOperand(1).getReg());

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm), r32),
                    fpuStore, 0);

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::SUB32mr)), al,
                    0)
      .addReg(r32);

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm),
                            MIAll.getOperand(0).getReg()),
                    al, 0);
  MIAll.getOperand(2).setImm(13);
}

void X86InstructionObfuscation::replaceMov(std::vector<MachineInstr *> MI,
                                      int64_t fpuStore, int64_t al,
                                      int64_t aluStack) {
  MachineInstr &MIAll = *MI.at(0);
  if (MIAll.getNextNode()->getOpcode() == X86::ADJCALLSTACKDOWN64)
    return;
  Register Xmmm = MRI->createVirtualRegister(&X86::VR128RegClass);
  MachineBasicBlock &MBB = *MIAll.getParent();
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(5);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    8);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)), al,
                    0);
  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSrm), Xmmm), al, 0)
      .addReg(Xmmm);
  for (int cint = 0; cint <= 64; cint += 64) {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSmr)),
                      fpuStore, cint)
        .addReg(Xmmm);
  }
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::PEXTRQmr)),
                    fpuStore, 0)
      .addReg(Xmmm)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::PEXTRQmr)),
                    fpuStore, 8)
      .addReg(Xmmm)
      .addImm(0);
  Register Xmmm2 = MRI->createVirtualRegister(&X86::VR128RegClass);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::AESKEYGENASSIST128rr), Xmmm2)
      .addReg(Xmmm)
      .addImm(25);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::PEXTRQmr)),
                    fpuStore, 0)
      .addReg(Xmmm2)
      .addImm(8);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)),
                    fpuStore, 0);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FLDLG2));
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRNDINT));

  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FSIN));

  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FLDL2T));

  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)),
                    fpuStore, 0);
  int64_t imm = MIAll.getOperand(1).getImm();
  if (imm > 3) {
    Register r32 = MRI->createVirtualRegister(&X86::GR32RegClass);
    addFrameReference(
        BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), fpuStore,
        0);
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      fpuStore, 0)
        .addImm(imm - 3);

    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::ADD32mr)),
                      fpuStore, 0)
        .addReg(r32);
    addFrameReference(BuildMI(MBB, MI.at(1), DebugLoc(), TII->get(X86::MOV32rm),
                              MIAll.getOperand(0).getReg()),
                      fpuStore, 0);
    MIAll.getOperand(1).setImm(42);
  }
}

void X86InstructionObfuscation::replaceMov2(std::vector<MachineInstr *> MI,
                                      int64_t fpuStore, int64_t al,
                                      int64_t aluStack) {
  MachineInstr &MIAll = *MI.at(0);
  uint64_t valDiff = MIAll.getOperand(1).getImm()-4;
  MachineBasicBlock &MBB = *MIAll.getParent();
  Register r32 = MRI->createVirtualRegister(&X86::GR32RegClass);


  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FSAVEm)), al,
                    0);
  for (int i = 0; i <= 48; i+=8) {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      fpuStore, i)
        .addImm(0);
  }

  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FNSTCW16m)),
                    fpuStore, 16);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::OR64mi32)),
                    fpuStore, 16)
      .addImm(2056);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FLDCW16m)),
                    fpuStore, 16);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FLDPI));
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRNDINT));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)),
                    fpuStore, 0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRSTORm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    fpuStore,
                    8)
      .addImm(valDiff);
  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), fpuStore,
      8);

  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::ADD32mr)),
                    fpuStore, 0)
      .addReg(r32);
  addFrameReference(BuildMI(MBB, MI.at(1), DebugLoc(), TII->get(X86::MOV32rm),
                            MIAll.getOperand(0).getReg()),
                    fpuStore, 0);
  MIAll.getOperand(1).setImm(13);
}

void X86InstructionObfuscation::replaceMov3(std::vector<MachineInstr *> MI,
                                       int64_t fpuStore, int64_t al,
                                       int64_t aluStack) {
  MachineInstr &MIAll = *MI.at(0);
  uint64_t valDiff = MIAll.getOperand(1).getImm();
  if (valDiff > 100000)
    return;
  MachineBasicBlock &MBB = *MIAll.getParent();
  Register r32 = MRI->createVirtualRegister(&X86::GR32RegClass);

  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FSAVEm)), al,
                    0);
  for (int i = 0; i <= 48; i += 8) {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      fpuStore, i)
        .addImm(0);
  }

  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FNSTCW16m)),
                    fpuStore, 16);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::OR64mi32)),
                    fpuStore, 16)
      .addImm(2056);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FLDCW16m)),
                    fpuStore, 16);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)),
                    fpuStore, 0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRSTORm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(2);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(16);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FRNDINT));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::OR64mi32)),
                    al, 0)
      .addImm(8);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::AND64mi32)),
                    al, 0)
      .addImm(24);
  if (valDiff >= 8) {
    uint64_t diff = valDiff - 8;
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      al, 8)
        .addImm(diff);
    addFrameReference(
        BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), al, 8);

    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::ADD32mr)),
                      al, 0)
        .addReg(r32);
  } else {
    uint64_t diff = 8-valDiff;
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      al, 8)
        .addImm(diff);
    addFrameReference(
        BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), al,
        8);

    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::SUB32mr)),
                      al, 0);
  }
  addFrameReference(BuildMI(MBB, MI.at(1), DebugLoc(), TII->get(X86::MOV32rm),
                            MIAll.getOperand(0).getReg()),
                    al, 0);

  MIAll.getOperand(1).setImm(13);
}


void X86InstructionObfuscation::replaceADD(std::vector<MachineInstr *> MI,
                                       int64_t fpuStore, int64_t al,
                                       int64_t aluStack) {
  MachineInstr &MIAll = *MI.at(0);
  Register Xmmm = MRI->createVirtualRegister(&X86::VR128RegClass);
  Register Xmmm2 = MRI->createVirtualRegister(&X86::VR128RegClass);
  MachineBasicBlock &MBB = *MIAll.getParent();
  Register r32 = MRI->createVirtualRegister(&X86::GR32RegClass);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(5);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 16)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    8);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)), al,
                    0);
  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSrm), Xmmm), al, 0)
      .addReg(Xmmm);

  for (int cint = 0; cint <= 64; cint += 64) {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSmr)),
                      al, cint)
        .addReg(Xmmm);
  }
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(45);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(2);
 
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBLDm)), al,
                    0);
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FYL2X));
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::FBSTPm)), al,
                    0);
  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSrm), Xmmm), al, 0)
      .addReg(Xmmm);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 0)
      .addImm(1245);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                    al, 8)
      .addImm(0);
  addFrameReference(
      BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::VMOVUPSrm), Xmmm2), al, 0)
      .addReg(Xmmm2);
  
  BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::GF2P8MULBrr), Xmmm)
      .addReg(Xmmm).addReg(Xmmm2);
  addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::PEXTRQmr)),
                    al, 0)
      .addReg(Xmmm)
      .addImm(0);
  MachineInstr *MIi = MI.at(1);
  uint64_t val = MIAll.getOperand(2).getImm();
  if (val >= 253) {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      al, 8)
        .addImm(val - 253);
    addFrameReference(
        BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), al,
        8);
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::ADD32mr)),
                      al, 0)
        .addReg(r32);
  } else {
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV64mi32)),
                      al, 8)
        .addImm(253-val);
    addFrameReference(
        BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::MOV32rm), r32), al, 8);
    addFrameReference(BuildMI(MBB, MIAll, DebugLoc(), TII->get(X86::SUB32mr)),
                      al,
                      0)
        .addReg(r32);
  }

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32mr)),

                    al, 8)
      .addReg(MIAll.getOperand(1).getReg());
  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm), r32),
                    al, 8);

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::ADD32mr)),
                    al, 0)
      .addReg(r32);

  addFrameReference(BuildMI(MBB, MIi, DebugLoc(), TII->get(X86::MOV32rm),
                            MIAll.getOperand(0).getReg()),
                    al, 0);
  MIAll.getOperand(2).setImm(2);

}

bool X86InstructionObfuscation::runOnMachineFunction(MachineFunction &MFunc) {
  if (!MFunc.getName().equals("main")) { // messes up the timing of the threads (pseudo race condition -> real race condition)
    outs() << "skipped";
    return false;
  }
    
  MF = &MFunc;
  MRI = &MFunc.getRegInfo();
  ST = &MFunc.getSubtarget<X86Subtarget>();
  TII = ST->getInstrInfo();
  X86FI = MFunc.getInfo<X86MachineFunctionInfo>();
  MFI = &MFunc.getFrameInfo();
  TRI = ST->getRegisterInfo();

  int64_t fpuStore = MFI->CreateStackObject(128, Align(8), true);
  int64_t al = MFI->CreateStackObject(128, Align(8), true);
  int64_t aluStack = MFI->CreateStackObject(128, Align(8), true);

  //fuehrt sonst zu kollisionen -> falls mov() gefolgt von einer sub() Instruktion. -> sub Instr Substitution greift auf stack von mov() zu!
  //Achtung: rm geht nicht bei add??
  //bei subst Rechenoperationen nach dem sub32ri machen!
  int64_t fpuStoreSUB = MFI->CreateStackObject(128, Align(8), true);
  int64_t alSUB = MFI->CreateStackObject(128, Align(8), true);
  int64_t aluStackSUB = MFI->CreateStackObject(128, Align(8), true);

  int64_t fpuStoreADD = MFI->CreateStackObject(128, Align(8), true);
  int64_t alADD = MFI->CreateStackObject(128, Align(8), true);
  int64_t aluStackADD = MFI->CreateStackObject(128, Align(8), true);

  SmallVector<std::vector<MachineInstr *>> WorkListMOV32;
  SmallVector<MachineInstr *> WorkListMOV8;
  SmallVector<std::vector<MachineInstr *>> WorkListSUB32;
  SmallVector<std::vector<MachineInstr *>> WorkListADD32;
  // int64_t alCW = MFI->CreateStackObject(2, Align(8), true);
  for (MachineBasicBlock &MBB : MFunc) {
    for (MachineInstr &MIAll : MBB) {
      // Probleme mit MOV32ri -> crashed
      if (MIAll.getOpcode() == X86::MOV32ri64 ||
          MIAll.getOpcode() == X86::MOV32ri) {
        if (MIAll.getOperand(1).getImm() > 3) { // TOOD: support negative values
          std::vector<MachineInstr *> list;
          list.push_back(&MIAll);
          list.push_back(MIAll.getNextNode());
          WorkListMOV32.push_back(list);
        }
      } else if (MIAll.getOpcode() == X86::MOV8ri) {
        int64_t Imm;
        Register Reg;
        for (int cntl = 0; cntl < MIAll.getNumOperands(); cntl++) {
          if (MIAll.getOperand(cntl).isImm()) {
            Imm = MIAll.getOperand(cntl).getImm();
          } else if (MIAll.getOperand(cntl).isReg()) {
            Reg = MIAll.getOperand(cntl).getReg();
          }
        }
      } else if (MIAll.getOpcode() == X86::SUB32ri8 ||
                 MIAll.getOpcode() == X86::SUB32ri) {

        if (!MIAll.getOperand(2).isImm())
          continue;
        if (MIAll.getOperand(2).getImm() < 3)
          continue;
        std::vector<MachineInstr *> list;
        list.push_back(&MIAll);
        list.push_back(MIAll.getNextNode());
        WorkListSUB32.push_back(list);
      } else if (MIAll.getOpcode() == X86::ADD32ri8 ||
                 MIAll.getOpcode() == X86::ADD32ri) {
        int64_t Imm;
        Register Reg;
        if (!MIAll.getOperand(2).isImm())
          continue;
        if (MIAll.getOperand(2).getImm() < 3)
          continue;
        std::vector<MachineInstr *> list;
        list.push_back(&MIAll);
        list.push_back(MIAll.getNextNode());
        WorkListADD32.push_back(list);
      }
    }
  }

  for (auto MI : WorkListSUB32) {
  //TODO: use mt19937 -> but compiler error because of ??
    replaceSub2(MI, fpuStoreSUB, alSUB, aluStackSUB);
  }

  for (auto MI : WorkListADD32) {
    //replaceADD(MI, fpuStoreADD, alADD, aluStackADD);
  }

  for (auto MI : WorkListMOV32) {
  //TODO: use mt19937 -> but compiler error because of ??
    replaceMov3(MI, fpuStore, al, aluStack);
    
  }
  MFunc.print(outs());
  return true;
}

FunctionPass *llvm::createX86InstructionObfuscationPass() {
  return new X86InstructionObfuscation();
}
