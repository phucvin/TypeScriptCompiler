#include "TypeScript/Defines.h"
#include "TypeScript/AliasPass.h"

#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/GlobalAlias.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;
using namespace PatternMatch;

#define DEBUG_TYPE "pass"

struct AliasPassCode
{
    bool isWasm;
    size_t intSize;

    AliasPassCode(bool isWasm, size_t intSize) : isWasm(isWasm), intSize(intSize)
    {
    }

    bool runOnFunction(Function &F)
    {
        auto MadeChange = false;

        LLVM_DEBUG(llvm::dbgs() << "\nEXPORT Function: " << F.getName());
        LLVM_DEBUG(llvm::dbgs() << "\nEXPORT Dump Before: ...\n" << F << "\n";);

        if (isWasm && F.getName() == "main") {
            if (!F.isDeclaration() 
                && F.arg_size() == 0 
                && !F.isVarArg() 
                && F.getReturnType()->isIntegerTy(intSize)) {
                auto *GA = llvm::GlobalAlias::create("__main_void", &F);
                GA->setVisibility(llvm::GlobalValue::HiddenVisibility);

                MadeChange = true;
            }
        }

        if (MadeChange)
        {
            LLVM_DEBUG(llvm::dbgs() << "\n!! EXPORT Change: " << MadeChange;);
            LLVM_DEBUG(llvm::dbgs() << "\n!! EXPORT Dump After: ...\n" << F << "\n";);
        }

        return MadeChange;
    } 
};

namespace ts
{
    llvm::PreservedAnalyses AliasPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM)
    {
        AliasPassCode LPF{isWasm, intSize};
        bool MadeChange = false;

        for (auto &F : M)
        {
            MadeChange |= LPF.runOnFunction(F);
        }

        return MadeChange ? llvm::PreservedAnalyses::none() : llvm::PreservedAnalyses::all();
    }
}