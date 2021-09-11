#ifndef MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_CODELOGICHELPER_H_
#define MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_CODELOGICHELPER_H_

#include "TypeScript/Config.h"
#include "TypeScript/Defines.h"
#include "TypeScript/Passes.h"
#include "TypeScript/TypeScriptDialect.h"
#include "TypeScript/TypeScriptOps.h"

#include "mlir/IR/PatternMatch.h"

using namespace mlir;
namespace mlir_ts = mlir::typescript;

namespace typescript
{

class CodeLogicHelper
{
    Location loc;
    PatternRewriter &rewriter;

  public:
    CodeLogicHelper(Operation *op, PatternRewriter &rewriter) : loc(op->getLoc()), rewriter(rewriter)
    {
    }

    CodeLogicHelper(Location loc, PatternRewriter &rewriter) : loc(loc), rewriter(rewriter)
    {
    }

    ArrayAttr getStructIndexAttr(int index)
    {
        return rewriter.getI32ArrayAttr(mlir::ArrayRef<int32_t>(index));
    }

    Value createIConstantOf(unsigned width, unsigned value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getIntegerType(width),
                                                 rewriter.getIntegerAttr(rewriter.getIntegerType(width), value));
    }

    Value createFConstantOf(unsigned width, double value)
    {
        auto ftype = rewriter.getF32Type();
        if (width == 16)
        {
            ftype = rewriter.getF16Type();
        }
        else if (width == 64)
        {
            ftype = rewriter.getF64Type();
        }
        else if (width == 128)
        {
            ftype = rewriter.getF128Type();
        }

        return rewriter.create<LLVM::ConstantOp>(loc, ftype, rewriter.getFloatAttr(ftype, value));
    }

    Value createI8ConstantOf(unsigned value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getIntegerType(8),
                                                 rewriter.getIntegerAttr(rewriter.getIntegerType(8), value));
    }

    Value createI32ConstantOf(unsigned value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getIntegerType(32), rewriter.getIntegerAttr(rewriter.getI32Type(), value));
    }

    Value createI64ConstantOf(unsigned value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getIntegerType(64), rewriter.getIntegerAttr(rewriter.getI64Type(), value));
    }

    Value createI1ConstantOf(bool value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getIntegerType(1), rewriter.getIntegerAttr(rewriter.getI1Type(), value));
    }

    Value createF32ConstantOf(float value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getF32Type(), rewriter.getIntegerAttr(rewriter.getF32Type(), value));
    }

    Value createIndexConstantOf(unsigned value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getIntegerType(64), rewriter.getIntegerAttr(rewriter.getI64Type(), value));
    }

    Value createStructIndexConstantOf(unsigned value)
    {
        return rewriter.create<LLVM::ConstantOp>(loc, rewriter.getIntegerType(32), rewriter.getIntegerAttr(rewriter.getI32Type(), value));
    }

    Value castToI8Ptr(mlir::Value value)
    {
        TypeHelper th(rewriter);
        return rewriter.create<LLVM::BitcastOp>(loc, th.getI8PtrType(), value);
    }

    Value castToI8PtrPtr(mlir::Value value)
    {
        TypeHelper th(rewriter);
        return rewriter.create<LLVM::BitcastOp>(loc, th.getI8PtrPtrType(), value);
    }

    Value conditionalExpressionLowering(Type type, Value condition, function_ref<Value(OpBuilder &, Location)> thenBuilder,
                                        function_ref<Value(OpBuilder &, Location)> elseBuilder)
    {
        // Split block
        auto *opBlock = rewriter.getInsertionBlock();
        auto opPosition = rewriter.getInsertionPoint();
        auto *continuationBlock = rewriter.splitBlock(opBlock, opPosition);

        // then block
        auto *thenBlock = rewriter.createBlock(continuationBlock);
        auto thenValue = thenBuilder(rewriter, loc);

        // else block
        auto *elseBlock = rewriter.createBlock(continuationBlock);
        auto elseValue = elseBuilder(rewriter, loc);

        // result block
        auto *resultBlock = rewriter.createBlock(continuationBlock, TypeRange{type});
        rewriter.create<LLVM::BrOp>(loc, ValueRange{}, continuationBlock);

        rewriter.setInsertionPointToEnd(thenBlock);
        rewriter.create<LLVM::BrOp>(loc, ValueRange{thenValue}, resultBlock);

        rewriter.setInsertionPointToEnd(elseBlock);
        rewriter.create<LLVM::BrOp>(loc, ValueRange{elseValue}, resultBlock);

        // Generate assertion test.
        rewriter.setInsertionPointToEnd(opBlock);
        rewriter.create<LLVM::CondBrOp>(loc, condition, thenBlock, elseBlock);

        rewriter.setInsertionPointToStart(continuationBlock);

        return resultBlock->getArguments().front();
    }

    template <typename OpTy> void saveResult(OpTy &op, Value result)
    {
        auto defOp = op.operand1().getDefiningOp();
        // TODO: finish it for field access
        if (auto loadOp = dyn_cast<mlir_ts::LoadOp>(defOp))
        {
            rewriter.create<mlir_ts::StoreOp>(loc, result, loadOp.reference());
        }
        else
        {
            llvm_unreachable("not implemented");
        }
    }
};
} // namespace typescript

#endif // MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_CODELOGICHELPER_H_