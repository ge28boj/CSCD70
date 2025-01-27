#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {

class FunctionInfoPass final : public PassInfoMixin<FunctionInfoPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &) {
    
    /// @todo(CSCD70) Please complete this method.
    for (auto& F : M) {
      if (F.isIntrinsic() || !F.hasUWTable())  continue;
      outs() << "Function Name: " << F.getName() << "\n";

      if (F.isVarArg()) {
        outs() << "Number of Arguments: " << F.arg_size() << "+*" << "\n";
      } else {
        outs() << "Number of Arguments: " << F.arg_size() << "\n";
      }
      outs() << "Number of Calls: " << F.getNumUses() << "\n";

      int BB_size = 0;
      for (auto& BB : F) {
        BB_size++;
      }
      outs() << "Number of BBs: " << BB_size << "\n";

      outs() << "Number of Instructions: " << F.getInstructionCount() << "\n";
    }

    return PreservedAnalyses::all();
  }
}; // class FunctionInfoPass

} // anonymous namespace

extern "C" PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
      .APIVersion = LLVM_PLUGIN_API_VERSION,
      .PluginName = "FunctionInfo",
      .PluginVersion = LLVM_VERSION_STRING,
      .RegisterPassBuilderCallbacks =
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) -> bool {
                  if (Name == "function-info") {
                    MPM.addPass(FunctionInfoPass());
                    return true;
                  }
                  return false;
                });
          } // RegisterPassBuilderCallbacks
  };        // struct PassPluginLibraryInfo
}
