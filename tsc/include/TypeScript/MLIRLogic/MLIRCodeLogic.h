#ifndef MLIR_TYPESCRIPT_MLIRGENLOGIC_MLIRCODELOGIC_H_
#define MLIR_TYPESCRIPT_MLIRGENLOGIC_MLIRCODELOGIC_H_

#include "TypeScript/TypeScriptDialect.h"
#include "TypeScript/TypeScriptOps.h"

#include "TypeScript/MLIRLogic/MLIRDefines.h"
#include "TypeScript/DOM.h"

#include "parser_types.h"

#include <numeric>

#define DEBUG_TYPE "mlir"

using namespace ::typescript;
using namespace ts;
namespace mlir_ts = mlir::typescript;

namespace typescript
{

class MLIRCodeLogic
{
    mlir::MLIRContext *context;

  public:
    MLIRCodeLogic(mlir::MLIRContext *context) : context(context)
    {
    }

    MLIRCodeLogic(mlir::OpBuilder builder) : context(builder.getContext())
    {
    }

    mlir::Attribute ExtractAttr(mlir::Value value, bool removeOpIfSuccess = false)
    {
        auto constOp = dyn_cast_or_null<mlir_ts::ConstantOp>(value.getDefiningOp());
        if (constOp)
        {
            auto val = constOp.valueAttr();
            return val;
        }

        return mlir::Attribute();
    }

    mlir::Value GetReferenceOfLoadOp(mlir::Value value)
    {
        // TODO: sync with Common Logic
        if (auto loadOp = dyn_cast_or_null<mlir_ts::LoadOp>(value.getDefiningOp()))
        {
            // this LoadOp will be removed later as unused
            auto refValue = loadOp.reference();
            return refValue;
        }

        return mlir::Value();
    }

    mlir::Type getEffectiveFunctionTypeForTupleField(mlir::Type elementType)
    {
#ifdef USE_BOUND_FUNCTION_FOR_OBJECTS
        if (auto boundFuncType = elementType.dyn_cast_or_null<mlir_ts::BoundFunctionType>())
        {
            return mlir::FunctionType::get(context, boundFuncType.getInputs(), boundFuncType.getResults());
        }
#endif

        return elementType;
    }

    mlir::Attribute TupleFieldName(StringRef name)
    {
        assert(!name.empty());
        MLIRTypeHelper mth(context);
        return mth.TupleFieldName(name);
    }

    template <typename T>
    std::pair<int, mlir::Type> TupleFieldType(mlir::Location location, T tupleType, mlir::Attribute fieldId, bool indexAccess = false)
    {
        auto result = TupleFieldTypeNoError(location, tupleType, fieldId, indexAccess);
        if (result.first == -1)
        {
            emitError(location, "Tuple member '") << fieldId << "' can't be found";
        }

        return result;
    }

    template <typename T>
    std::pair<int, mlir::Type> TupleFieldTypeNoError(mlir::Location location, T tupleType, mlir::Attribute fieldId,
                                                     bool indexAccess = false)
    {
        auto fieldIndex = tupleType.getIndex(fieldId);
        if (indexAccess && (fieldIndex < 0 || fieldIndex >= tupleType.size()))
        {
            // try to resolve index
            auto intAttr = fieldId.dyn_cast_or_null<mlir::IntegerAttr>();
            if (intAttr)
            {
                fieldIndex = intAttr.getInt();
            }
        }

        if (fieldIndex < 0 || fieldIndex >= tupleType.size())
        {
            LLVM_DEBUG(dbgs() << "@looking for field: " << fieldId << " tuple: " << tupleType << "\n");
            return std::make_pair<>(-1, mlir::Type());
        }

        // type
        auto elementType = tupleType.getType(fieldIndex);

        return std::make_pair(fieldIndex, elementType);
    }

    mlir::Type CaptureTypeStorage(llvm::StringMap<ts::VariableDeclarationDOM::TypePtr> &capturedVars)
    {
        SmallVector<mlir_ts::FieldInfo> fields;
        for (auto &varInfo : capturedVars)
        {
            auto &value = varInfo.getValue();
            fields.push_back(mlir_ts::FieldInfo{TupleFieldName(value->getName()),
                                                value->getReadWriteAccess() ? mlir_ts::RefType::get(value->getType()) : value->getType()});
        }

        auto lambdaType = mlir_ts::TupleType::get(context, fields);
        return lambdaType;
    }

    mlir::Type CaptureType(llvm::StringMap<ts::VariableDeclarationDOM::TypePtr> &capturedVars)
    {
        return mlir_ts::RefType::get(CaptureTypeStorage(capturedVars));
    }
};

class MLIRCustomMethods
{
    mlir::OpBuilder &builder;
    mlir::Location &location;

  public:
    MLIRCustomMethods(mlir::OpBuilder &builder, mlir::Location &location) : builder(builder), location(location)
    {
    }

    mlir::Value callMethod(StringRef functionName, ArrayRef<mlir::Value> operands, const GenContext &genContext)
    {
        // validate params
        for (auto &oper : operands)
        {
            VALIDATE(oper)
        }

        mlir::Value result;
        if (functionName == "print")
        {
            // print - internal command;
            mlir::succeeded(mlirGenPrint(location, operands));
        }
        else if (functionName == "assert")
        {
            // assert - internal command;
            mlir::succeeded(mlirGenAssert(location, operands));
        }
        else if (functionName == "parseInt")
        {
            // assert - internal command;
            result = mlirGenParseInt(location, operands);
        }
        else if (functionName == "parseFloat")
        {
            result = mlirGenParseFloat(location, operands);
        }
        else if (functionName == "sizeof")
        {
            result = mlirGenSizeOf(location, operands);
        }
        else if (functionName == "__array_push")
        {
            result = mlirGenArrayPush(location, operands);
        }
        else if (functionName == "__array_pop")
        {
            result = mlirGenArrayPop(location, operands);
        }
        else if (functionName == "switchstate")
        {
            // switchstate - internal command;
            mlir::succeeded(mlirGenSwitchState(location, operands, genContext));
        }
        /*
        else
        if (functionName == "#_last_field")
        {
            mlir::TypeSwitch<mlir::Type>(operands.front().getType())
                .Case<mlir_ts::ConstTupleType>([&](auto tupleType)
                {
                    result = builder.create<mlir_ts::ConstantOp>(location, builder.getI32Type(),
        builder.getI32IntegerAttr(tupleType.size()));
                })
                .Case<mlir_ts::TupleType>([&](auto tupleType)
                {
                    result = builder.create<mlir_ts::ConstantOp>(location, builder.getI32Type(),
        builder.getI32IntegerAttr(tupleType.size()));
                })
                .Default([&](auto type)
                {
                    llvm_unreachable("not implemented");
                    //result = builder.create<mlir_ts::ConstantOp>(location, builder.getI32Type(), builder.getI32IntegerAttr(-1));
                });

        }
        */
        else if (!genContext.allowPartialResolve)
        {
            emitError(location) << "no defined function found for '" << functionName << "'";
        }

        return result;
    }

  private:
    mlir::LogicalResult mlirGenPrint(const mlir::Location &location, ArrayRef<mlir::Value> operands)
    {
        auto printOp = builder.create<mlir_ts::PrintOp>(location, operands);

        return mlir::success();
    }

    mlir::LogicalResult mlirGenAssert(const mlir::Location &location, ArrayRef<mlir::Value> operands)
    {
        auto msg = StringRef("assert");
        if (operands.size() > 1)
        {
            auto param2 = operands[1];
            auto constantOp = dyn_cast_or_null<mlir_ts::ConstantOp>(param2.getDefiningOp());
            if (constantOp && constantOp.getType().isa<mlir_ts::StringType>())
            {
                msg = constantOp.value().cast<mlir::StringAttr>().getValue();
            }

            param2.getDefiningOp()->erase();
        }

        auto assertOp = builder.create<mlir_ts::AssertOp>(location, operands.front(), mlir::StringAttr::get(builder.getContext(), msg));

        return mlir::success();
    }

    mlir::Value mlirGenParseInt(const mlir::Location &location, ArrayRef<mlir::Value> operands)
    {
        auto op = operands.front();
        if (!op.getType().isa<mlir_ts::StringType>())
        {
            op = builder.create<mlir_ts::CastOp>(location, mlir_ts::StringType::get(builder.getContext()), op);
        }

        auto parseIntOp = builder.create<mlir_ts::ParseIntOp>(location, builder.getI32Type(), op);

        return parseIntOp;
    }

    mlir::Value mlirGenParseFloat(const mlir::Location &location, ArrayRef<mlir::Value> operands)
    {
        auto op = operands.front();
        if (!op.getType().isa<mlir_ts::StringType>())
        {
            op = builder.create<mlir_ts::CastOp>(location, mlir_ts::StringType::get(builder.getContext()), op);
        }

        auto parseFloatOp = builder.create<mlir_ts::ParseFloatOp>(location, builder.getF32Type(), op);

        return parseFloatOp;
    }

    mlir::Value mlirGenSizeOf(const mlir::Location &location, ArrayRef<mlir::Value> operands)
    {
        auto sizeOfValue =
            builder.create<mlir_ts::SizeOfOp>(location, builder.getI64Type(), mlir::TypeAttr::get(operands.front().getType()));

        return sizeOfValue;
    }

    mlir::Value mlirGenArrayPush(const mlir::Location &location, ArrayRef<mlir::Value> operands)
    {
        MLIRCodeLogic mcl(builder);
        auto thisValue = mcl.GetReferenceOfLoadOp(operands.front());
        assert(thisValue);
        auto sizeOfValue = builder.create<mlir_ts::PushOp>(location, builder.getI64Type(), thisValue, operands.slice(1));

        return sizeOfValue;
    }

    mlir::Value mlirGenArrayPop(const mlir::Location &location, ArrayRef<mlir::Value> operands)
    {
        MLIRCodeLogic mcl(builder);
        auto thisValue = mcl.GetReferenceOfLoadOp(operands.front());
        assert(thisValue);
        auto sizeOfValue =
            builder.create<mlir_ts::PopOp>(location, operands.front().getType().cast<mlir_ts::ArrayType>().getElementType(), thisValue);

        return sizeOfValue;
    }

    mlir::LogicalResult mlirGenSwitchState(const mlir::Location &location, ArrayRef<mlir::Value> operands, const GenContext &genContext)
    {
        auto switchStateOp = builder.create<mlir_ts::SwitchStateOp>(location, operands.front());

        const_cast<GenContext &>(genContext).allocateVarsOutsideOfOperation = true;
        const_cast<GenContext &>(genContext).currentOperation = switchStateOp;

        return mlir::success();
    }
};

class MLIRPropertyAccessCodeLogic
{
    mlir::OpBuilder &builder;
    mlir::Location &location;
    mlir::Value &expression;
    mlir::StringRef name;
    mlir::Attribute fieldId;

  public:
    MLIRPropertyAccessCodeLogic(mlir::OpBuilder &builder, mlir::Location &location, mlir::Value &expression, StringRef name)
        : builder(builder), location(location), expression(expression), name(name)
    {
        MLIRCodeLogic mcl(builder);
        fieldId = mcl.TupleFieldName(name);
    }

    MLIRPropertyAccessCodeLogic(mlir::OpBuilder &builder, mlir::Location &location, mlir::Value &expression, mlir::Attribute fieldId)
        : builder(builder), location(location), expression(expression), fieldId(fieldId)
    {
        if (auto strAttr = fieldId.dyn_cast_or_null<mlir::StringAttr>())
        {
            name = strAttr.getValue();
        }
    }

    mlir::Value Enum(mlir_ts::EnumType enumType)
    {
        auto propName = getName();
        auto dictionaryAttr = getExprConstAttr().cast<mlir::DictionaryAttr>();
        auto valueAttr = dictionaryAttr.get(propName);
        if (!valueAttr)
        {
            emitError(location, "Enum member '") << propName << "' can't be found";
            return mlir::Value();
        }

        return builder.create<mlir_ts::ConstantOp>(location, enumType.getElementType(), valueAttr);
    }

    template <typename T> mlir::Value Tuple(T tupleType, bool indexAccess = false)
    {
        mlir::Value value;

        MLIRTypeHelper mth(builder.getContext());
        MLIRCodeLogic mcl(builder);

        // resolve index
        auto pair = mcl.TupleFieldType(location, tupleType, fieldId, indexAccess);
        auto fieldIndex = pair.first;
        bool isBoundRef = false;
        auto elementTypeForRef = pair.second;
        auto elementType = mth.isBoundReference(pair.second, isBoundRef);

        if (fieldIndex < 0)
        {
            return value;
        }

        auto refValue = getExprLoadRefValue();
        if (isBoundRef && !refValue)
        {
            // allocate in stack
            refValue = builder.create<mlir_ts::VariableOp>(location, mlir_ts::RefType::get(expression.getType()), expression);
        }

        if (refValue)
        {
            auto refType = isBoundRef ? static_cast<mlir::Type>(mlir_ts::BoundRefType::get(elementTypeForRef))
                                      : static_cast<mlir::Type>(mlir_ts::RefType::get(elementTypeForRef));
            auto propRef = builder.create<mlir_ts::PropertyRefOp>(location, refType, refValue, builder.getI32IntegerAttr(fieldIndex));

            return builder.create<mlir_ts::LoadOp>(location, elementType, propRef);
        }

        return builder.create<mlir_ts::ExtractPropertyOp>(location, elementTypeForRef, expression,
                                                          builder.getArrayAttr(mth.getStructIndexAttrValue(fieldIndex)));
    }

    template <typename T> mlir::Value TupleNoError(T tupleType, bool indexAccess = false)
    {
        mlir::Value value;

        MLIRTypeHelper mth(builder.getContext());
        MLIRCodeLogic mcl(builder);

        // resolve index
        auto pair = mcl.TupleFieldTypeNoError(location, tupleType, fieldId, indexAccess);
        auto fieldIndex = pair.first;
        bool isBoundRef = false;
        auto elementTypeForRef = pair.second;
        auto elementType = mth.isBoundReference(pair.second, isBoundRef);

        if (fieldIndex < 0)
        {
            return value;
        }

        auto refValue = getExprLoadRefValue();
        if (isBoundRef && !refValue)
        {
            // allocate in stack
            refValue = builder.create<mlir_ts::VariableOp>(location, mlir_ts::RefType::get(expression.getType()), expression);
        }

        if (refValue)
        {
            auto refType = isBoundRef ? static_cast<mlir::Type>(mlir_ts::BoundRefType::get(elementTypeForRef))
                                      : static_cast<mlir::Type>(mlir_ts::RefType::get(elementTypeForRef));
            auto propRef = builder.create<mlir_ts::PropertyRefOp>(location, refType, refValue, builder.getI32IntegerAttr(fieldIndex));

            return builder.create<mlir_ts::LoadOp>(location, elementType, propRef);
        }

        return builder.create<mlir_ts::ExtractPropertyOp>(location, elementTypeForRef, expression,
                                                          builder.getArrayAttr(mth.getStructIndexAttrValue(fieldIndex)));
    }

    mlir::Value Bool(mlir_ts::BooleanType intType)
    {
        auto propName = getName();
        if (propName == "toString")
        {
            return builder.create<mlir_ts::CastOp>(location, mlir_ts::StringType::get(builder.getContext()), expression);
        }

        return mlir::Value();
    }

    mlir::Value Int(mlir::IntegerType intType)
    {
        auto propName = getName();
        if (propName == "toString")
        {
            return builder.create<mlir_ts::CastOp>(location, mlir_ts::StringType::get(builder.getContext()), expression);
        }

        return mlir::Value();
    }

    mlir::Value Float(mlir::FloatType floatType)
    {
        auto propName = getName();
        if (propName == "toString")
        {
            return builder.create<mlir_ts::CastOp>(location, mlir_ts::StringType::get(builder.getContext()), expression);
        }

        return mlir::Value();
    }

    mlir::Value Number(mlir_ts::NumberType numberType)
    {
        auto propName = getName();
        if (propName == "toString")
        {
            return builder.create<mlir_ts::CastOp>(location, mlir_ts::StringType::get(builder.getContext()), expression);
        }

        return mlir::Value();
    }

    mlir::Value String(mlir_ts::StringType stringType)
    {
        auto propName = getName();
        if (propName == "length")
        {
            return builder.create<mlir_ts::StringLengthOp>(location, builder.getI32Type(), expression);
        }

        return mlir::Value();
    }

    template <typename T> mlir::Value Array(T arrayType)
    {
        auto propName = getName();
        if (propName == "length")
        {
            if (expression.getType().isa<mlir_ts::ConstArrayType>())
            {
                auto size = getExprConstAttr().cast<mlir::ArrayAttr>().size();
                return builder.create<mlir_ts::ConstantOp>(location, builder.getI32Type(), builder.getI32IntegerAttr(size));
            }
            else if (expression.getType().isa<mlir_ts::ArrayType>())
            {
                auto sizeValue = builder.create<mlir_ts::LengthOfOp>(location, builder.getI32Type(), expression);
                return sizeValue;
            }

            return mlir::Value();
        }
        else if (propName == "push")
        {
            if (expression.getType().isa<mlir_ts::ArrayType>())
            {
                auto symbOp = builder.create<mlir_ts::ThisSymbolRefOp>(location, builder.getNoneType(), expression,
                                                                       mlir::FlatSymbolRefAttr::get(builder.getContext(), "__array_push"));
                return symbOp;
            }

            return mlir::Value();
        }
        else if (propName == "pop")
        {
            if (expression.getType().isa<mlir_ts::ArrayType>())
            {
                auto symbOp = builder.create<mlir_ts::ThisSymbolRefOp>(location, builder.getNoneType(), expression,
                                                                       mlir::FlatSymbolRefAttr::get(builder.getContext(), "__array_pop"));
                return symbOp;
            }

            return mlir::Value();
        }

        return mlir::Value();
    }

    template <typename T> mlir::Value Ref(T refType)
    {
        if (auto constTupleType = refType.getElementType().template dyn_cast_or_null<mlir_ts::ConstTupleType>())
        {
            return RefLogic(constTupleType);
        }
        else if (auto tupleType = refType.getElementType().template dyn_cast_or_null<mlir_ts::TupleType>())
        {
            return RefLogic(tupleType);
        }
        else
        {
            llvm_unreachable("not implemented");
        }
    }

    mlir::Value Object(mlir_ts::ObjectType objectType)
    {
        if (auto constTupleType = objectType.getStorageType().template dyn_cast_or_null<mlir_ts::ConstTupleType>())
        {
            return RefLogic(constTupleType);
        }
        else if (auto tupleType = objectType.getStorageType().template dyn_cast_or_null<mlir_ts::TupleType>())
        {
            return RefLogic(tupleType);
        }
        else
        {
            llvm_unreachable("not implemented");
        }
    }

    template <typename T> mlir::Value RefLogic(T tupleType)
    {
        MLIRTypeHelper mth(builder.getContext());
        MLIRCodeLogic mcl(builder);

        // resolve index
        auto pair = mcl.TupleFieldType(location, tupleType, fieldId);
        auto fieldIndex = pair.first;
        bool isBoundRef = false;
        auto elementTypeForRef = pair.second;
        auto elementType = mth.isBoundReference(pair.second, isBoundRef);

        if (fieldIndex < 0)
        {
            return mlir::Value();
        }

        auto refType = isBoundRef ? static_cast<mlir::Type>(mlir_ts::BoundRefType::get(elementTypeForRef))
                                  : static_cast<mlir::Type>(mlir_ts::RefType::get(elementTypeForRef));
        auto propRef = builder.create<mlir_ts::PropertyRefOp>(location, refType, expression, builder.getI32IntegerAttr(fieldIndex));

        return builder.create<mlir_ts::LoadOp>(location, elementType, propRef);
    }

    mlir::Value Class(mlir_ts::ClassType classType)
    {
        if (auto classStorageType = classType.getStorageType().template dyn_cast_or_null<mlir_ts::ClassStorageType>())
        {
            return Class(classStorageType);
        }
        else
        {
            llvm_unreachable("not implemented");
        }
    }

    mlir::Value Class(mlir_ts::ClassStorageType classStorageType)
    {
        MLIRCodeLogic mcl(builder);

        // resolve index
        auto pair = mcl.TupleFieldTypeNoError(location, classStorageType, fieldId);
        auto fieldIndex = pair.first;
        bool isBoundRef = false;
        auto elementTypeForRef = pair.second;
        auto elementType = elementTypeForRef;
        /* as this is class, we do not take reference to class as for object */
        // auto elementType = mcl.isBoundReference(pair.second,
        // isBoundRef);

        if (fieldIndex < 0)
        {
            return mlir::Value();
        }

        auto refType = isBoundRef ? static_cast<mlir::Type>(mlir_ts::BoundRefType::get(elementTypeForRef))
                                  : static_cast<mlir::Type>(mlir_ts::RefType::get(elementTypeForRef));
        auto propRef = builder.create<mlir_ts::PropertyRefOp>(location, refType, expression, builder.getI32IntegerAttr(fieldIndex));

        return builder.create<mlir_ts::LoadOp>(location, elementType, propRef);
    }

    StringRef getName()
    {
        return name;
    }

    mlir::Attribute getAttribute()
    {
        return fieldId;
    }

  private:
    mlir::Attribute getExprConstAttr()
    {
        MLIRCodeLogic mcl(builder);

        auto value = mcl.ExtractAttr(expression);
        if (!value)
        {
            llvm_unreachable("not implemented");
        }

        return value;
    }

    mlir::Value getExprLoadRefValue()
    {
        MLIRCodeLogic mcl(builder);
        auto value = mcl.GetReferenceOfLoadOp(expression);
        return value;
    }
};

class MLIRCodeLogicHelper
{
    mlir::OpBuilder &builder;
    mlir::Location &location;

  public:
    MLIRCodeLogicHelper(mlir::OpBuilder &builder, mlir::Location &location) : builder(builder), location(location)
    {
    }

    mlir::Value conditionalExpression(mlir::Type type, mlir::Value condition,
                                      mlir::function_ref<mlir::Value(mlir::OpBuilder &, mlir::Location)> thenBuilder,
                                      mlir::function_ref<mlir::Value(mlir::OpBuilder &, mlir::Location)> elseBuilder)
    {
        // ts.if
        auto ifOp = builder.create<mlir_ts::IfOp>(location, type, condition, true);

        // then block
        auto &thenRegion = ifOp.thenRegion();

        builder.setInsertionPointToStart(&thenRegion.back());

        mlir::Value value = thenBuilder(builder, location);
        builder.create<mlir_ts::ResultOp>(location, value);

        // else block
        auto &elseRegion = ifOp.elseRegion();

        builder.setInsertionPointToStart(&elseRegion.back());

        mlir::Value elseValue = elseBuilder(builder, location);
        builder.create<mlir_ts::ResultOp>(location, elseValue);

        builder.setInsertionPointAfter(ifOp);

        return ifOp.results().front();
    }
};

class MLIRLogicHelper
{
  public:
    static bool isNeededToSaveData(SyntaxKind &opCode)
    {
        switch (opCode)
        {
        case SyntaxKind::PlusEqualsToken:
            opCode = SyntaxKind::PlusToken;
            break;
        case SyntaxKind::MinusEqualsToken:
            opCode = SyntaxKind::MinusToken;
            break;
        case SyntaxKind::AsteriskEqualsToken:
            opCode = SyntaxKind::AsteriskToken;
            break;
        case SyntaxKind::AsteriskAsteriskEqualsToken:
            opCode = SyntaxKind::AsteriskAsteriskToken;
            break;
        case SyntaxKind::SlashEqualsToken:
            opCode = SyntaxKind::SlashToken;
            break;
        case SyntaxKind::PercentEqualsToken:
            opCode = SyntaxKind::PercentToken;
            break;
        case SyntaxKind::LessThanLessThanEqualsToken:
            opCode = SyntaxKind::LessThanLessThanToken;
            break;
        case SyntaxKind::GreaterThanGreaterThanEqualsToken:
            opCode = SyntaxKind::GreaterThanGreaterThanToken;
            break;
        case SyntaxKind::GreaterThanGreaterThanGreaterThanEqualsToken:
            opCode = SyntaxKind::GreaterThanGreaterThanGreaterThanToken;
            break;
        case SyntaxKind::AmpersandEqualsToken:
            opCode = SyntaxKind::AmpersandToken;
            break;
        case SyntaxKind::BarEqualsToken:
            opCode = SyntaxKind::BarToken;
            break;
        case SyntaxKind::BarBarEqualsToken:
            opCode = SyntaxKind::BarBarToken;
            break;
        case SyntaxKind::AmpersandAmpersandEqualsToken:
            opCode = SyntaxKind::AmpersandAmpersandToken;
            break;
        case SyntaxKind::QuestionQuestionEqualsToken:
            opCode = SyntaxKind::QuestionQuestionToken;
            break;
        case SyntaxKind::CaretEqualsToken:
            opCode = SyntaxKind::CaretToken;
            break;
        default:
            return false;
            break;
        }

        return true;
    }

    static bool isLogicOp(SyntaxKind opCode)
    {
        switch (opCode)
        {
        case SyntaxKind::EqualsEqualsToken:
        case SyntaxKind::EqualsEqualsEqualsToken:
        case SyntaxKind::ExclamationEqualsToken:
        case SyntaxKind::ExclamationEqualsEqualsToken:
        case SyntaxKind::GreaterThanToken:
        case SyntaxKind::GreaterThanEqualsToken:
        case SyntaxKind::LessThanToken:
        case SyntaxKind::LessThanEqualsToken:
            return true;
        }

        return false;
    }
};
} // namespace typescript

#endif // MLIR_TYPESCRIPT_MLIRGENLOGIC_MLIRCODELOGIC_H_