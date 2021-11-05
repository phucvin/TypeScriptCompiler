#define DEBUG_TYPE "pass"

#include "mlir/Pass/Pass.h"

#include "TypeScript/TypeScriptDialect.h"
#include "TypeScript/TypeScriptOps.h"
#include "TypeScript/TypeScriptFunctionPass.h"
#include "TypeScript/Passes.h"

#include "TypeScript/LowerToLLVMLogic.h"

#include "mlir/Dialect/LLVMIR/LLVMDialect.h"

#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

namespace mlir_ts = mlir::typescript;

namespace
{

class RelocateConstantAndAllocsPass : public mlir::PassWrapper<RelocateConstantAndAllocsPass, TypeScriptFunctionPass>
{
  public:
    void runOnFunction() override
    {
        auto f = getFunction();

        SmallPtrSet<Operation *, 16> workSetConst;
        // SmallPtrSet<Operation *, 16> workSetVars;

        getOps<mlir_ts::ConstantOp>(f, workSetConst);
        // getOps<mlir_ts::VariableOp>(f, workSetVars);

        relocateConst(f, workSetConst);
        // relocateAllocs(f, workSetVars);

        LLVM_DEBUG(llvm::dbgs() << "\n!! AFTER FUNC DUMP: \n" << *getFunction() << "\n";);
    }

    Operation *seekFirstNonConstOp(mlir_ts::FuncOp &f)
    {
        return seekFirstNonOp(f, [](Operation *op) {
            if (auto constantOp = dyn_cast_or_null<mlir_ts::ConstantOp>(op))
                return true;
            if (auto constOp = dyn_cast_or_null<mlir::ConstantOp>(op))
                return true;
            return false;
        });
    }

    void relocateConst(mlir_ts::FuncOp &f, SmallPtrSet<Operation *, 16> &workSet)
    {
        // find fist non-constant op
        auto firstNonConstOp = seekFirstNonConstOp(f);
        if (firstNonConstOp)
        {
            ConversionPatternRewriter rewriter(f.getContext());
            rewriter.setInsertionPoint(firstNonConstOp);

            LLVM_DEBUG(llvm::dbgs() << "\nInsert const at: \n" << *firstNonConstOp << "\n";);

            for (auto op : workSet)
            {
                auto constantOp = cast<mlir_ts::ConstantOp>(op);

                LLVM_DEBUG(llvm::dbgs() << "\nconst to insert: \n" << constantOp << "\n";);

                auto newOp = rewriter.create<mlir_ts::ConstantOp>(constantOp->getLoc(), constantOp.getType(), constantOp.value());
                constantOp->replaceAllUsesWith(newOp);

                rewriter.eraseOp(constantOp);
            }
        }
    }

    Operation *seekFirstNonConstAndNonAllocOp(mlir_ts::FuncOp &f)
    {
        // find fist non-constant op
        return seekFirstNonOp(f, [](Operation *op) {
            if (auto constantOp = dyn_cast_or_null<mlir_ts::ConstantOp>(op))
                return true;
            if (auto constOp = dyn_cast_or_null<mlir::ConstantOp>(op))
                return true;
            if (auto vartOp = dyn_cast_or_null<mlir_ts::VariableOp>(op))
                return true;
            return false;
        });
    }

    void relocateAllocs(mlir_ts::FuncOp &f, SmallPtrSet<Operation *, 16> &workSet)
    {
        // find fist non-constant op
        auto firstNonConstAndNonAllocOp = seekFirstNonConstAndNonAllocOp(f);
        if (firstNonConstAndNonAllocOp)
        {
            ConversionPatternRewriter rewriter(f.getContext());
            rewriter.setInsertionPoint(firstNonConstAndNonAllocOp);

            LLVM_DEBUG(llvm::dbgs() << "\nInsert variable at: \n" << *firstNonConstAndNonAllocOp << "\n";);

            for (auto op : workSet)
            {
                auto varOp = cast<mlir_ts::VariableOp>(op);

                LLVM_DEBUG(llvm::dbgs() << "\nvariable to insert: \n" << varOp << "\n";);

                if (varOp.initializer())
                {
                    // split save and alloc
                    auto newVar =
                        rewriter.create<mlir_ts::VariableOp>(varOp->getLoc(), varOp.getType(), mlir::Value(), varOp.capturedAttr());
                    varOp->replaceAllUsesWith(newVar);

                    OpBuilder::InsertionGuard guard(rewriter);
                    rewriter.setInsertionPoint(op);
                    // varOp.initializer()
                    rewriter.create<mlir_ts::StoreOp>(varOp->getLoc(), varOp.initializer(), newVar);
                    rewriter.eraseOp(varOp);
                }
                else
                {
                    // just relocate
                    auto newVar =
                        rewriter.create<mlir_ts::VariableOp>(varOp->getLoc(), varOp.getType(), varOp.initializer(), varOp.capturedAttr());
                    varOp->replaceAllUsesWith(newVar);

                    rewriter.eraseOp(varOp);
                }
            }
        }
    }

    template <typename T> void getOps(mlir_ts::FuncOp &f, SmallPtrSet<Operation *, 16> &workSet)
    {
        auto skipFirstConsts = true;
        f.walk([&](mlir::Operation *op) {
            if (auto constantOp = dyn_cast_or_null<T>(op))
            {
                if (!skipFirstConsts)
                {
                    // if const is not child of function but nested block - ignore it
                    if (constantOp->getParentOp() == f)
                    {
                        // select only those consts which are not at the beginning
                        workSet.insert(constantOp);
                    }
                }
            }
            else
            {
                skipFirstConsts = false;
            }
        });
    }

    Operation *seekFirstNonOp(mlir_ts::FuncOp funcOp, std::function<bool(Operation *)> filter)
    {
        auto allowSkipConsts = true;
        auto found = false;
        Operation *foundOp = nullptr;
        // find last string
        auto lastUse = [&](Operation *op) {
            if (found)
            {
                return;
            }

            // we need only first level
            if (op->getParentOp() != funcOp)
            {
                // it is not top anymore
                found = true;
                return;
            }

            if (filter(op))
            {
                if (allowSkipConsts)
                {
                    return;
                }
            }

            if (allowSkipConsts)
            {
                allowSkipConsts = false;
                found = true;
                foundOp = op;
                return;
            }
        };

        funcOp.walk(lastUse);

        return foundOp;
    }
};
} // end anonymous namespace

/// Create pass.
std::unique_ptr<mlir::Pass> mlir_ts::createRelocateConstantAndAllocsPass()
{
    return std::make_unique<RelocateConstantAndAllocsPass>();
}
