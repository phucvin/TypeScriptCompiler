#ifndef MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_TYPEHELPER_H_
#define MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_TYPEHELPER_H_

#include "TypeScript/Config.h"
#include "TypeScript/Defines.h"
#include "TypeScript/Passes.h"
#include "TypeScript/TypeScriptDialect.h"
#include "TypeScript/TypeScriptOps.h"

#include "mlir/IR/PatternMatch.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"

using namespace mlir;
namespace mlir_ts = mlir::typescript;

namespace typescript
{

class TypeHelper
{
    MLIRContext *context;

  public:
    TypeHelper(PatternRewriter &rewriter) : context(rewriter.getContext())
    {
    }

    TypeHelper(MLIRContext *context) : context(context)
    {
    }

    Type getBooleanType()
    {
        return mlir_ts::BooleanType::get(context);
    }

    Type getI8Type()
    {
        return IntegerType::get(context, 8);
    }

    Type getI32Type()
    {
        return IntegerType::get(context, 32);
    }

    Type getI64Type()
    {
        return IntegerType::get(context, 64);
    }

    Type getF32Type()
    {
        return FloatType::getF32(context);
    }

    IntegerAttr getStructIndexAttrValue(int32_t value)
    {
        return IntegerAttr::get(getI32Type(), APInt(32, value));
    }

    Type getIndexType()
    {
        return getI64Type();
    }

    IntegerAttr getIndexAttrValue(int64_t value)
    {
        return IntegerAttr::get(getIndexType(), APInt(64, value));
    }

    Type getLLVMBoolType()
    {
        return IntegerType::get(context, 1 /*, IntegerType::SignednessSemantics::Unsigned*/);
    }

    LLVM::LLVMVoidType getVoidType()
    {
        return LLVM::LLVMVoidType::get(context);
    }

    LLVM::LLVMPointerType getI8PtrType()
    {
        return LLVM::LLVMPointerType::get(getI8Type());
    }

    LLVM::LLVMPointerType getI8PtrPtrType()
    {
        return LLVM::LLVMPointerType::get(LLVM::LLVMPointerType::get(getI8Type()));
    }

    LLVM::LLVMPointerType getI8PtrPtrPtrType()
    {
        return LLVM::LLVMPointerType::get(LLVM::LLVMPointerType::get(LLVM::LLVMPointerType::get(getI8Type())));
    }

    LLVM::LLVMArrayType getI8Array(unsigned size)
    {
        return LLVM::LLVMArrayType::get(getI8Type(), size);
    }

    LLVM::LLVMArrayType getI32Array(unsigned size)
    {
        return LLVM::LLVMArrayType::get(getI32Type(), size);
    }

    LLVM::LLVMPointerType getPointerType(Type elementType)
    {
        return LLVM::LLVMPointerType::get(elementType);
    }

    LLVM::LLVMArrayType getArrayType(Type elementType, size_t size)
    {
        return LLVM::LLVMArrayType::get(elementType, size);
    }

    LLVM::LLVMFunctionType getFunctionType(Type result, ArrayRef<Type> arguments, bool isVarArg = false)
    {
        return LLVM::LLVMFunctionType::get(result, arguments, isVarArg);
    }

    LLVM::LLVMFunctionType getFunctionType(ArrayRef<Type> arguments, bool isVarArg = false)
    {
        return LLVM::LLVMFunctionType::get(getVoidType(), arguments, isVarArg);
    }
};

} // namespace typescript

#endif // MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_TYPEHELPER_H_