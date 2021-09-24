#ifndef MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_MLIRRTTIHELPERVCLINUX_H_
#define MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_MLIRRTTIHELPERVCLINUX_H_

#include "TypeScript/Config.h"
#include "TypeScript/Defines.h"
#include "TypeScript/Passes.h"
#include "TypeScript/TypeScriptDialect.h"
#include "TypeScript/TypeScriptOps.h"

#include "TypeScript/MLIRLogic/MLIRTypeHelper.h"
#include "TypeScript/MLIRLogic/MLIRCodeLogic.h"
#include "TypeScript/LowerToLLVM/LLVMRTTIHelperVCLinuxConst.h"

#include "mlir/Transforms/DialectConversion.h"
#include "llvm/ADT/TypeSwitch.h"

#include <sstream>
#include <functional>

using namespace ::typescript;
using namespace ts;
namespace mlir_ts = mlir::typescript;

namespace typescript
{

enum class TypeInfo
{
    Value,
    ClassTypeInfo,
    SingleInheritance_ClassTypeInfo,
    Pointer_TypeInfo
};

struct TypeNames
{
    std::string typeName;
    TypeInfo infoType;
};

class MLIRRTTIHelperVCLinux
{
    mlir::OpBuilder &rewriter;
    mlir::ModuleOp &parentModule;
    MLIRTypeHelper mth;
    MLIRLogicHelper mlh;

    SmallVector<TypeNames> types;

  public:
    MLIRRTTIHelperVCLinux(mlir::OpBuilder &rewriter, mlir::ModuleOp &parentModule)
        : rewriter(rewriter), parentModule(parentModule), mth(rewriter.getContext()), mlh()
    {
        // setI32AsCatchType();
    }

    void setF32AsCatchType()
    {
        types.push_back({F32Type::typeName, TypeInfo::Value});
    }

    void setI32AsCatchType()
    {
        types.push_back({I32Type::typeName, TypeInfo::Value});
    }

    void setStringTypeAsCatchType()
    {
        types.push_back({StringType::typeName, TypeInfo::Value});
    }

    void setI8PtrAsCatchType()
    {
        types.push_back({I8PtrType::typeName, TypeInfo::Value});
    }

    void setClassTypeAsCatchType(ArrayRef<StringRef> names)
    {
        auto first = true;
        auto countM1 = names.size() - 1;
        auto index = 0;
        for (auto name : names)
        {
            types.push_back({name.str(), index < countM1 ? TypeInfo::SingleInheritance_ClassTypeInfo : TypeInfo::ClassTypeInfo});
            if (first)
            {
                types.push_back({name.str(), TypeInfo::Pointer_TypeInfo});
            }

            first = false;
            index++;
        }
    }

    void setClassTypeAsCatchType(StringRef name)
    {
        types.push_back({name.str(), TypeInfo::ClassTypeInfo});
        types.push_back({name.str(), TypeInfo::Pointer_TypeInfo});
    }

    void setType(mlir::Type type, std::function<ClassInfo::TypePtr(StringRef fullClassName)> resolveClassInfo)
    {
        llvm::TypeSwitch<mlir::Type>(type)
            .Case<mlir::IntegerType>([&](auto intType) {
                if (intType.getIntOrFloatBitWidth() == 32)
                {
                    setI32AsCatchType();
                }
                else
                {
                    llvm_unreachable("not implemented");
                }
            })
            .Case<mlir::FloatType>([&](auto floatType) {
                if (floatType.getIntOrFloatBitWidth() == 32)
                {
                    setF32AsCatchType();
                }
                else
                {
                    llvm_unreachable("not implemented");
                }
            })
            .Case<mlir_ts::NumberType>([&](auto numberType) { setF32AsCatchType(); })
            .Case<mlir_ts::StringType>([&](auto stringType) { setStringTypeAsCatchType(); })
            .Case<mlir_ts::ClassType>([&](auto classType) {
                // we need all bases as well
                auto classInfo = resolveClassInfo(classType.getName().getValue());

                SmallVector<StringRef> classAndBases;
                classInfo->getBasesWithRoot(classAndBases);

                setClassTypeAsCatchType(classAndBases);
            })
            .Case<mlir_ts::AnyType>([&](auto anyType) { setI8PtrAsCatchType(); })
            .Default([&](auto type) { llvm_unreachable("not implemented"); });
    }

    void seekLast(mlir::Block *block)
    {
        // find last string
        auto lastUse = [&](mlir::Operation *op) {
            if (auto globalOp = dyn_cast_or_null<mlir_ts::GlobalOp>(op))
            {
                rewriter.setInsertionPointAfter(globalOp);
            }
        };

        block->walk(lastUse);
    }

    void setRTTIForType(mlir::Location loc, mlir::Type type, std::function<ClassInfo::TypePtr(StringRef fullClassName)> resolveClassInfo)
    {
        setType(type, resolveClassInfo);

        mlir::OpBuilder::InsertionGuard guard(rewriter);

        rewriter.setInsertionPointToStart(parentModule.getBody());
        seekLast(parentModule.getBody());

        // _ZTId
        auto index = 0;
        for (auto type : types)
        {
            switch (type.infoType)
            {
            case TypeInfo::ClassTypeInfo:
            case TypeInfo::Pointer_TypeInfo:
                typeInfoClass(loc, type.typeName, type.infoType);
                break;
            case TypeInfo::SingleInheritance_ClassTypeInfo:
                typeInfoSingleInheritanceClass(loc, type.typeName, type.infoType, types[index + 1].typeName, types[index + 1].infoType);
                break;
            default:
                typeInfoValue(loc, type.typeName);
                break;
            }

            index++;
        }
    }

    mlir::LogicalResult typeInfoValue(mlir::Location loc, StringRef name)
    {
        if (parentModule.lookupSymbol<mlir_ts::GlobalOp>(name))
        {
            return mlir::failure();
        }

        SmallVector<mlir::NamedAttribute> attrs;
        attrs.push_back({IDENT("Linkage"), ATTR("External")});

        rewriter.create<mlir_ts::GlobalOp>(loc, mth.getOpaqueType(), true, /*LLVM::Linkage::External,*/ name, mlir::Attribute{}, attrs);
        return mlir::success();
    }

    std::string join(StringRef name, const char *prefix, int nameLength, const char *suffix)
    {
        std::stringstream ss;
        ss << prefix;
        ss << nameLength;
        ss << name.str();
        ss << suffix;
        return ss.str();
    }

    std::string join(StringRef name, const char *prefix, const char *suffix)
    {
        std::stringstream ss;
        ss << prefix;
        ss << name.str();
        ss << suffix;
        return ss.str();
    }

    mlir::Type getStringConstType(int size)
    {
        return mth.getConstArrayValueType(mth.getI8Type(), size);
    }

    const char *prefixLabel(TypeInfo ti)
    {
        switch (ti)
        {
        case TypeInfo::Pointer_TypeInfo:
            return "P";
        default:
            return "";
        }
    }

    std::string labelValue(StringRef className, TypeInfo ti)
    {
        assert(className.size());
        auto label = join(className, prefixLabel(ti), className.size(), "");
        return label;
    }

    std::string stringConstRefName(StringRef className, TypeInfo ti)
    {
        assert(className.size());
        auto name = join(labelValue(className, ti), "_ZTS", "");
        return name;
    }

    std::string typeInfoRefName(StringRef className, TypeInfo ti)
    {
        assert(className.size());
        auto name = join(labelValue(className, ti), "_ZTI", "");
        return name;
    }

    mlir::Type stringConstType(StringRef className, TypeInfo ti)
    {
        auto label = labelValue(className, ti);
        return getStringConstType(label.size() + 1);
    }

    mlir::LogicalResult stringConst(mlir::Location loc, StringRef className, TypeInfo ti)
    {
        auto label = labelValue(className, ti);
        auto name = stringConstRefName(className, ti);
        if (parentModule.lookupSymbol<mlir_ts::GlobalOp>(name))
        {
            return mlir::failure();
        }

        SmallVector<mlir::NamedAttribute> attrs;
        attrs.push_back({IDENT("Linkage"), ATTR("LinkonceODR")});

        rewriter.create<mlir_ts::GlobalOp>(loc, stringConstType(className, ti), true,
                                           /*LLVM::Linkage::LinkonceODR,*/ name, rewriter.getStringAttr(label), attrs);

        return mlir::success();
    }

    mlir::Type getTIType(TypeInfo ti)
    {
        switch (ti)
        {
        case TypeInfo::SingleInheritance_ClassTypeInfo:
            return mth.getTupleType({mth.getOpaqueType(), mth.getOpaqueType(), mth.getOpaqueType()});
        case TypeInfo::Pointer_TypeInfo:
            return mth.getTupleType({mth.getOpaqueType(), mth.getOpaqueType(), mth.getI32Type(), mth.getOpaqueType()});
        default:
            return mth.getTupleType({mth.getOpaqueType(), mth.getOpaqueType()});
        }
    }

    const char *getClassInfoName(TypeInfo ti)
    {
        switch (ti)
        {
        case TypeInfo::SingleInheritance_ClassTypeInfo:
            return ClassType::singleInheritanceClassTypeInfoName;
        case TypeInfo::Pointer_TypeInfo:
            return ClassType::pointerTypeInfoName;
        case TypeInfo::ClassTypeInfo:
            return ClassType::classTypeInfoName;
        default:
            llvm_unreachable("not implemented");
        }
    }

    void setGlobalOpWritingPoint(mlir_ts::GlobalOp globalOp)
    {
        auto &region = globalOp.getInitializerRegion();
        auto *block = rewriter.createBlock(&region);

        rewriter.setInsertionPoint(block, block->begin());
    }

    mlir::LogicalResult setStructValue(mlir::Location loc, mlir::Value &tupleValue, mlir::Value value, int index)
    {
        auto tpl = tupleValue.getType();
        assert(tpl.isa<mlir_ts::TupleType>() || tpl.isa<mlir_ts::ConstTupleType>() || tpl.isa<mlir_ts::ConstArrayValueType>());
        tupleValue = rewriter.create<mlir_ts::InsertPropertyOp>(loc, tpl, value, tupleValue, rewriter.getI64ArrayAttr(index));
        return mlir::success();
    }

    mlir::LogicalResult typeInfoRef(mlir::Location loc, StringRef className, TypeInfo ti, StringRef baseName = "",
                                    TypeInfo baseti = TypeInfo::Value)
    {
        auto name = typeInfoRefName(className, ti);
        if (parentModule.lookupSymbol<mlir_ts::GlobalOp>(name))
        {
            return mlir::failure();
        }

        SmallVector<mlir::NamedAttribute> attrs;
        attrs.push_back({IDENT("Linkage"), ATTR("LinkonceODR")});

        auto typeInfoType = getTIType(ti);

        auto globalOp = rewriter.create<mlir_ts::GlobalOp>(loc, typeInfoType, true,
                                                           /*LLVM::Linkage::LinkonceODR,*/ name, mlir::Attribute{}, attrs);

        {
            setGlobalOpWritingPoint(globalOp);

            // begin
            mlir::Value structVal = rewriter.create<mlir_ts::UndefOp>(loc, typeInfoType);

            auto itemValue1 = rewriter.create<mlir_ts::AddressOfOp>(
                loc, mth.getRefType(mth.getOpaqueType()), mlir::FlatSymbolRefAttr::get(rewriter.getContext(), getClassInfoName(ti)),
                mlir::IntegerAttr::get(mth.getI32Type(), 2));
            setStructValue(loc, structVal, itemValue1, 0);

            auto itemValue2 = rewriter.create<mlir_ts::ConstantOp>(
                loc, stringConstType(className, ti),
                mlir::FlatSymbolRefAttr::get(rewriter.getContext(), stringConstRefName(className, ti)));

            setStructValue(loc, structVal, itemValue2, 1);

            if (ti == TypeInfo::Pointer_TypeInfo)
            {
                auto itemValueI32 = rewriter.create<mlir_ts::ConstantOp>(loc, mth.getI32Type(), mth.getI32AttrValue(0));
                setStructValue(loc, structVal, itemValueI32, 2);

                // add base class name
                auto itemValue3 = rewriter.create<mlir_ts::ConstantOp>(
                    loc, mlir_ts::RefType::get(getTIType(ti)),
                    mlir::FlatSymbolRefAttr::get(rewriter.getContext(), typeInfoRefName(className, ti)));

                setStructValue(loc, structVal, itemValue3, 3);
            }
            else if (ti == TypeInfo::SingleInheritance_ClassTypeInfo)
            {
                // add base class name
                auto itemValue2 = rewriter.create<mlir_ts::ConstantOp>(
                    loc, mlir_ts::RefType::get(getTIType(baseti)),
                    mlir::FlatSymbolRefAttr::get(rewriter.getContext(), typeInfoRefName(baseName, baseti)));

                setStructValue(loc, structVal, itemValue2, 2);
            }

            // end
            rewriter.create<mlir_ts::GlobalResultOp>(loc, mlir::ValueRange{structVal});

            rewriter.setInsertionPointAfter(globalOp);
        }

        return mlir::success();
    }

    mlir::LogicalResult typeInfoClass(mlir::Location loc, StringRef name, TypeInfo ti)
    {
        typeInfoValue(loc, getClassInfoName(ti));
        stringConst(loc, name, ti);
        typeInfoRef(loc, name, ti);
        return mlir::success();
    }

    mlir::LogicalResult typeInfoSingleInheritanceClass(mlir::Location loc, StringRef name, TypeInfo ti, StringRef baseName, TypeInfo baseti)
    {
        typeInfoValue(loc, getClassInfoName(ti));
        stringConst(loc, name, ti);
        typeInfoRef(loc, name, ti, baseName, baseti);
        return mlir::success();
    }
};
} // namespace typescript

#endif // MLIR_TYPESCRIPT_LOWERTOLLVMLOGIC_MLIRRTTIHELPERVCLINUX_H_
