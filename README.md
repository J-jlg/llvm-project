# The LLVM Compiler Infrastructure
## build
How to build:

```console
foo@bar:~$ cmake -S llvm -B build -G Ninja -DLLVM_ENABLE_PROJECTS="clang;lld" -DCMAKE_BUILD_TYPE=Release
foo@bar:~$ ninja -C build
```

Now you can compile your c++ Code with 
```console
foo@bar:~$ /build/bin/clang++ example.cpp -o example
```
Note: The MachineFunctionPass on LLVM M(C)IR /llvm/lib/Target/X86/X86InstructionObfuscation.cpp is enabled by default and you cannot disable it with a command flag (TODO: add flag in /llvm/lib/Target/X86/X86TargetMachine.cpp).


If you want to play yourself with the code modiy the /llvm/lib/Target/X86/X86InstructionObfuscation.cpp file. e.g. insert a ``outs() << "hello";``` inside the runOnFunction Methode.

## Use the Opt (LLVM IR) Pass
To use the CFF and/or Thread-Obfuscation Pass you need to compile your c/cpp/... code into LLVM IR to apply the transformations (they work on the LLVM IR "Middle End"/Opt representation)

```console
foo@bar:~$ /build/bin/clang++ example.cpp -S -emit-llvm
```

This command directs clang to stop the compilation process after transforming example.cpp into LLVM IR. The execution of this command generates the example.ll file. An .ll file represents the textual LLVM IR (There are other LLVM IR storage formats e.g. Bitcode .bc Files, which stores the textual .ll information in a more memory-effient way).

Before you can apply a transformation remove the LLVM IR Metadata (definied by the # symbol) from your Target Function:
e.g. 
```define dso_local noundef i32 @main() #2 {``` -> ```define dso_local noundef i32 @main() {```

You need to do this because ```clang++ example.cpp -S -emit-llvm``` adds the optnone metaattributes to each function by default. You can see this in your example.ll file by searching for the attribute (search for attributes #Number where Number was the removed number e.g. #2 in the above case). 

e.g.
attributes #2 = { mustprogress noinline norecurse ```optnone```

(In production you do not need to do this this -> clang/clang++ only attributes optnone if you use -S -emit-llvm)

To apply the transformations:
```console
foo@bar:~$ /build/bin/opt example.ll -o example_cff.bc -passes=CFF
```
or
```console
foo@bar:~$ /build/bin/opt example.ll -o example_cff.bc -passes=threadsobf
```
or
```console
foo@bar:~$ /build/bin/opt example.ll -o example_cff.bc -passes=antidb
```
or
```console
foo@bar:~$ /build/bin/opt example.ll -o example_cff.bc -passes=antidb2
```

(To find out the pass name see:  llvm/lib/Passes/PassRegistry.def)

To view it use either /build/bin/opt example.ll -o example_cff.bc -passes=CFF -print-changed
or convert the example_cff.bc file into the textual .ll format:
```console
foo@bar:~$ /build/bin/llvm-dis example_cff.bc -o example_cff.ll
```

To now compile the .ll or .bc file use clang++ as the compilation driver: 
```console
foo@bar:~$ /build/bin/clang++ example_cff.bc -o example_cff
```






