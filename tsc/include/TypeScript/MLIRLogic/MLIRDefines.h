
#ifndef MLIR_TYPESCRIPT_MLIRGENLOGIC_MLIRDEFINES_H_
#define MLIR_TYPESCRIPT_MLIRGENLOGIC_MLIRDEFINES_H_

#include "llvm/ADT/ScopedHashTable.h"

#define VALIDATE(value)                                                                                                                    \
    if (!value)                                                                                                                            \
    {                                                                                                                                      \
        if (!genContext.allowPartialResolve)                                                                                               \
        {                                                                                                                                  \
            emitError(value.getDefiningOp()->getLoc(), "expression has no result");                                                        \
        }                                                                                                                                  \
                                                                                                                                           \
        return mlir::Value();                                                                                                              \
    }                                                                                                                                      \
                                                                                                                                           \
    if (auto unresolved = dyn_cast_or_null<mlir_ts::UnresolvedSymbolRefOp>(value.getDefiningOp()))                                         \
    {                                                                                                                                      \
        if (!genContext.allowPartialResolve)                                                                                               \
        {                                                                                                                                  \
            emitError(value.getDefiningOp()->getLoc(), "can't find variable: ") << unresolved.identifier();                                \
        }                                                                                                                                  \
                                                                                                                                           \
        return mlir::Value();                                                                                                              \
    }

#define VALIDATE_LOGIC(value)                                                                                                              \
    if (!value)                                                                                                                            \
    {                                                                                                                                      \
        if (!genContext.allowPartialResolve)                                                                                               \
        {                                                                                                                                  \
            emitError(value.getDefiningOp()->getLoc(), "expression has no result");                                                        \
        }                                                                                                                                  \
                                                                                                                                           \
        return mlir::failure();                                                                                                            \
    }                                                                                                                                      \
                                                                                                                                           \
    if (auto unresolved = dyn_cast_or_null<mlir_ts::UnresolvedSymbolRefOp>(value.getDefiningOp()))                                         \
    {                                                                                                                                      \
        if (!genContext.allowPartialResolve)                                                                                               \
        {                                                                                                                                  \
            emitError(value.getDefiningOp()->getLoc(), "can't find variable: ") << unresolved.identifier();                                \
        }                                                                                                                                  \
                                                                                                                                           \
        return mlir::failure();                                                                                                            \
    }

using VariablePairT = std::pair<mlir::Value, ts::VariableDeclarationDOM::TypePtr>;
using SymbolTableScopeT = llvm::ScopedHashTableScope<StringRef, VariablePairT>;

#endif // MLIR_TYPESCRIPT_MLIRGENLOGIC_MLIRDEFINES_H_