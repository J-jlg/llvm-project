//===- Inliner.h - Inliner pass utilities -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This header file declares utility structures for the inliner pass.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_TRANSFORMS_INLINER_H
#define MLIR_TRANSFORMS_INLINER_H

#include "mlir/Analysis/CallGraph.h"
#include "mlir/Interfaces/CallInterfaces.h"
#include "mlir/Pass/AnalysisManager.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Support/LogicalResult.h"
#include "llvm/ADT/StringMap.h"

namespace mlir {
class OpPassManager;
class Operation;

class InlinerConfig {
public:
  using DefaultPipelineTy = std::function<void(OpPassManager &)>;
  using OpPipelinesTy = llvm::StringMap<OpPassManager>;

  InlinerConfig() = default;
  InlinerConfig(DefaultPipelineTy defaultPipeline,
                unsigned maxInliningIterations)
      : defaultPipeline(std::move(defaultPipeline)),
        maxInliningIterations(maxInliningIterations) {}

  const DefaultPipelineTy &getDefaultPipeline() const {
    return defaultPipeline;
  }
  const OpPipelinesTy &getOpPipelines() const { return opPipelines; }
  unsigned getMaxInliningIterations() const { return maxInliningIterations; }
  void setDefaultPipeline(DefaultPipelineTy pipeline) {
    defaultPipeline = std::move(pipeline);
  }
  void setOpPipelines(OpPipelinesTy pipelines) {
    opPipelines = std::move(pipelines);
  }
  void setMaxInliningIterations(unsigned max) { maxInliningIterations = max; }

private:
  /// An optional function that constructs an optimization pipeline for
  /// a given operation. This optimization pipeline is applied
  /// only to those callable operations that do not have dedicated
  /// optimization pipeline in opPipelines (based on the operation name).
  DefaultPipelineTy defaultPipeline;
  /// A map of operation names to pass pipelines to use when optimizing
  /// callable operations of these types. This provides a specialized pipeline
  /// instead of the one produced by defaultPipeline.
  OpPipelinesTy opPipelines;
  /// For SCC-based inlining algorithms, specifies maximum number of iterations
  /// when inlining within an SCC.
  unsigned maxInliningIterations{0};
};

/// This is an implementation of the inliner
/// that operates bottom up over the Strongly Connected Components(SCCs)
/// of the CallGraph. This enables a more incremental propagation
/// of inlining decisions from the leafs to the roots of the callgraph.
class Inliner {
public:
  using RunPipelineHelperTy = std::function<LogicalResult(
      Pass &pass, OpPassManager &pipeline, Operation *op)>;

  Inliner(Operation *op, CallGraph &cg, Pass &pass, AnalysisManager am,
          RunPipelineHelperTy runPipelineHelper, const InlinerConfig &config)
      : op(op), cg(cg), pass(pass), am(am),
        runPipelineHelper(std::move(runPipelineHelper)), config(config) {}
  Inliner(Inliner &) = delete;
  void operator=(const Inliner &) = delete;

  /// Perform inlining on a OpTrait::SymbolTable operation.
  LogicalResult doInlining();

  /// This struct represents a resolved call to a given callgraph node. Given
  /// that the call does not actually contain a direct reference to the
  /// Region(CallGraphNode) that it is dispatching to, we need to resolve them
  /// explicitly.
  struct ResolvedCall {
    ResolvedCall(CallOpInterface call, CallGraphNode *sourceNode,
                 CallGraphNode *targetNode)
        : call(call), sourceNode(sourceNode), targetNode(targetNode) {}
    CallOpInterface call;
    CallGraphNode *sourceNode, *targetNode;
  };

protected:
  /// An OpTrait::SymbolTable operation to run the inlining on.
  Operation *op;
  /// A CallGraph analysis for the given operation.
  CallGraph &cg;
  /// A reference to the pass using this inliner.
  Pass &pass;
  /// Analysis manager for the given operation instance.
  AnalysisManager am;
  /// A callback for running a nested pass pipeline on the operation
  /// contained within the main operation.
  const RunPipelineHelperTy runPipelineHelper;
  /// The inliner configuration parameters.
  const InlinerConfig &config;

private:
  /// Forward declaration of the class providing the actual implementation.
  class Impl;

public:
};
} // namespace mlir

#endif // MLIR_TRANSFORMS_INLINER_H
