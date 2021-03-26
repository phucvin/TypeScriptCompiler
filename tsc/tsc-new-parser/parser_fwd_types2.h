#ifndef NEW_PARSER_FWRD_TYPES_H
#define NEW_PARSER_FWRD_TYPES_H

#include <functional>

#define FORWARD_DECLARATION(x) \
    struct x; \
    using x##Ref = std::reference_wrapper<x>;

#define FORWARD_DECLARATION_T(x) \
    template <typename T> \
    struct x; \
    template <typename T> \
    using x##Ref = std::reference_wrapper<x<T>>;

#define FORWARD_DECLARATION_VAR(x, v) \
    template <v TKind> \
    struct x; \
    template <v TKind> \
    using x##Ref = std::reference_wrapper<x<TKind>>;

#define FORWARD_DECLARATION_VARS(x, v) \
    template <v ... TKind> \
    struct x; \
    template <v ... TKind> \
    using x##Ref = std::reference_wrapper<x<TKind...>>;

#define REF(x) x##Ref

#define NODE_REF(x) \
    using x = Node; \
    using REF(x) = REF(Node);

#define CLASS_REF(x, c) \
    using x = c; \
    using REF(x) = REF(c);

#define TMPL_REF(x, n, t) \
    using x = n<t>; \
    using REF(x) = REF(n)<t>;

#define WRAPPER(x) \
    struct x {  \
        REF(data::x) ref;   \
    };

namespace data
{
    FORWARD_DECLARATION(TextRange)
    FORWARD_DECLARATION(ReadonlyTextRange)
    FORWARD_DECLARATION_T(ReadonlyArray)
    FORWARD_DECLARATION_T(NodeArray)
    FORWARD_DECLARATION(Symbol)
    FORWARD_DECLARATION(Type)
    FORWARD_DECLARATION(Node)
    FORWARD_DECLARATION_VARS(Token, SyntaxKind)
    FORWARD_DECLARATION(JSDocContainer)
    FORWARD_DECLARATION(EndOfFileToken)
    FORWARD_DECLARATION_VAR(PunctuationToken, SyntaxKind)
    FORWARD_DECLARATION_VAR(KeywordToken, SyntaxKind)
    FORWARD_DECLARATION_VAR(ModifierToken, SyntaxKind)
    FORWARD_DECLARATION(QualifiedName)
    FORWARD_DECLARATION(Declaration)
    FORWARD_DECLARATION(NamedDeclaration)
    FORWARD_DECLARATION(DynamicNamedDeclaration)
    FORWARD_DECLARATION(DynamicNamedBinaryExpression)
    FORWARD_DECLARATION(LateBoundDeclaration)
    FORWARD_DECLARATION(LateBoundBinaryExpressionDeclaration)
    FORWARD_DECLARATION(LateBoundElementAccessExpression)
    FORWARD_DECLARATION(DeclarationStatement)
    FORWARD_DECLARATION(ComputedPropertyName)
    FORWARD_DECLARATION(PrivateIdentifier)
    FORWARD_DECLARATION(LateBoundName)
    FORWARD_DECLARATION(Decorator)
    FORWARD_DECLARATION(TypeParameterDeclaration)
    FORWARD_DECLARATION(SignatureDeclarationBase)
    FORWARD_DECLARATION(CallSignatureDeclaration)
    FORWARD_DECLARATION(ConstructSignatureDeclaration)
    FORWARD_DECLARATION(VariableDeclaration)
    FORWARD_DECLARATION(InitializedVariableDeclaration)
    FORWARD_DECLARATION(VariableDeclarationList)
    FORWARD_DECLARATION(ParameterDeclaration)
    FORWARD_DECLARATION(BindingElement)
    FORWARD_DECLARATION(PropertySignature)
    FORWARD_DECLARATION(PropertyDeclaration)
    FORWARD_DECLARATION(PrivateIdentifierPropertyDeclaration)
    FORWARD_DECLARATION(InitializedPropertyDeclaration)
    FORWARD_DECLARATION(ObjectLiteralElement)
    FORWARD_DECLARATION(PropertyAssignment)
    FORWARD_DECLARATION(ShorthandPropertyAssignment)
    FORWARD_DECLARATION(SpreadAssignment)
    FORWARD_DECLARATION(PropertyLikeDeclaration)
    FORWARD_DECLARATION(ObjectBindingPattern)
    FORWARD_DECLARATION(ArrayBindingPattern)
    FORWARD_DECLARATION(FunctionLikeDeclarationBase)
    FORWARD_DECLARATION(FunctionDeclaration)
    FORWARD_DECLARATION(MethodSignature)
    FORWARD_DECLARATION(MethodDeclaration)
    FORWARD_DECLARATION(ConstructorDeclaration)
    FORWARD_DECLARATION(SemicolonClassElement)
    FORWARD_DECLARATION(GetAccessorDeclaration)
    FORWARD_DECLARATION(SetAccessorDeclaration)
    FORWARD_DECLARATION(IndexSignatureDeclaration)
    FORWARD_DECLARATION(TypeNode)
    FORWARD_DECLARATION_VAR(KeywordTypeNode, SyntaxKind)
    FORWARD_DECLARATION(ImportTypeNode)
    FORWARD_DECLARATION(LiteralImportTypeNode)
    FORWARD_DECLARATION(ThisTypeNode)
    FORWARD_DECLARATION(FunctionOrConstructorTypeNodeBase)
    FORWARD_DECLARATION(FunctionTypeNode)
    FORWARD_DECLARATION(ConstructorTypeNode)
    FORWARD_DECLARATION(NodeWithTypeArguments)
    FORWARD_DECLARATION(TypeReferenceNode)
    FORWARD_DECLARATION(TypePredicateNode)
    FORWARD_DECLARATION(TypeQueryNode)
    FORWARD_DECLARATION(TypeLiteralNode)
    FORWARD_DECLARATION(ArrayTypeNode)
    FORWARD_DECLARATION(TupleTypeNode)
    FORWARD_DECLARATION(NamedTupleMember)
    FORWARD_DECLARATION(OptionalTypeNode)
    FORWARD_DECLARATION(RestTypeNode)
    FORWARD_DECLARATION(UnionTypeNode)
    FORWARD_DECLARATION(IntersectionTypeNode)
    FORWARD_DECLARATION(ConditionalTypeNode)
    FORWARD_DECLARATION(InferTypeNode)
    FORWARD_DECLARATION(ParenthesizedTypeNode)
    FORWARD_DECLARATION(TypeOperatorNode)
    FORWARD_DECLARATION(UniqueTypeOperatorNode)
    FORWARD_DECLARATION(IndexedAccessTypeNode)
    FORWARD_DECLARATION(MappedTypeNode)
    FORWARD_DECLARATION(LiteralTypeNode)
    FORWARD_DECLARATION(StringLiteral)
    FORWARD_DECLARATION(TemplateLiteralTypeNode)
    FORWARD_DECLARATION(TemplateLiteralTypeSpan)
    FORWARD_DECLARATION(Expression)
    FORWARD_DECLARATION(OmittedExpression)
    FORWARD_DECLARATION(PartiallyEmittedExpression)
    FORWARD_DECLARATION(UnaryExpression)
    FORWARD_DECLARATION(UpdateExpression)
    FORWARD_DECLARATION(PrefixUnaryExpression)
    FORWARD_DECLARATION(PostfixUnaryExpression)
    FORWARD_DECLARATION(LeftHandSideExpression)
    FORWARD_DECLARATION(MemberExpression)
    FORWARD_DECLARATION(PrimaryExpression)
    FORWARD_DECLARATION(Identifier)
    FORWARD_DECLARATION(TransientIdentifier)
    FORWARD_DECLARATION(GeneratedIdentifier)
    FORWARD_DECLARATION(NullLiteral)
    FORWARD_DECLARATION(TrueLiteral)
    FORWARD_DECLARATION(FalseLiteral)
    FORWARD_DECLARATION(ThisExpression)
    FORWARD_DECLARATION(SuperExpression)
    FORWARD_DECLARATION(ImportExpression)
    FORWARD_DECLARATION(DeleteExpression)
    FORWARD_DECLARATION(TypeOfExpression)
    FORWARD_DECLARATION(VoidExpression)
    FORWARD_DECLARATION(AwaitExpression)
    FORWARD_DECLARATION(YieldExpression)
    FORWARD_DECLARATION(SyntheticExpression)
    FORWARD_DECLARATION(BinaryExpression)
    FORWARD_DECLARATION_T(AssignmentExpression)
    FORWARD_DECLARATION(ObjectDestructuringAssignment)
    FORWARD_DECLARATION(ArrayDestructuringAssignment)
    FORWARD_DECLARATION(ConditionalExpression)
    FORWARD_DECLARATION(FunctionExpression)
    FORWARD_DECLARATION(ArrowFunction)
    FORWARD_DECLARATION(LiteralLikeNode)
    FORWARD_DECLARATION(TemplateLiteralLikeNode)
    FORWARD_DECLARATION(LiteralExpression)
    FORWARD_DECLARATION(RegularExpressionLiteral)
    FORWARD_DECLARATION(NoSubstitutionTemplateLiteral)
    FORWARD_DECLARATION(NumericLiteral)
    FORWARD_DECLARATION(BigIntLiteral)
    FORWARD_DECLARATION(TemplateHead)
    FORWARD_DECLARATION(TemplateMiddle)
    FORWARD_DECLARATION(TemplateTail)
    FORWARD_DECLARATION(TemplateExpression)
    FORWARD_DECLARATION(TemplateSpan)
    FORWARD_DECLARATION(ParenthesizedExpression)
    FORWARD_DECLARATION(ArrayLiteralExpression)
    FORWARD_DECLARATION(SpreadElement)
    FORWARD_DECLARATION_T(ObjectLiteralExpressionBase)
    FORWARD_DECLARATION(ObjectLiteralExpression)
    FORWARD_DECLARATION(PropertyAccessExpression)
    FORWARD_DECLARATION(PrivateIdentifierPropertyAccessExpression)
    FORWARD_DECLARATION(PropertyAccessChain)
    FORWARD_DECLARATION(PropertyAccessChainRoot)
    FORWARD_DECLARATION(SuperPropertyAccessExpression)
    FORWARD_DECLARATION(PropertyAccessEntityNameExpression)
    FORWARD_DECLARATION(ElementAccessExpression)
    FORWARD_DECLARATION(ElementAccessChain)
    FORWARD_DECLARATION(ElementAccessChainRoot)
    FORWARD_DECLARATION(SuperElementAccessExpression)
    FORWARD_DECLARATION(CallExpression)
    FORWARD_DECLARATION(CallChain)
    FORWARD_DECLARATION(CallChainRoot)
    FORWARD_DECLARATION(BindableObjectDefinePropertyCall)
    FORWARD_DECLARATION(LiteralLikeElementAccessExpression)
    FORWARD_DECLARATION(BindableStaticElementAccessExpression)
    FORWARD_DECLARATION(BindableElementAccessExpression)
    FORWARD_DECLARATION(BindableStaticPropertyAssignmentExpression)
    FORWARD_DECLARATION(BindablePropertyAssignmentExpression)
    FORWARD_DECLARATION(SuperCall)
    FORWARD_DECLARATION(ImportCall)
    FORWARD_DECLARATION(ExpressionWithTypeArguments)
    FORWARD_DECLARATION(NewExpression)
    FORWARD_DECLARATION(TaggedTemplateExpression)
    FORWARD_DECLARATION(AsExpression)
    FORWARD_DECLARATION(TypeAssertion)
    FORWARD_DECLARATION(NonNullExpression)
    FORWARD_DECLARATION(NonNullChain)
    FORWARD_DECLARATION(MetaProperty)
    FORWARD_DECLARATION(ImportMetaProperty)
    FORWARD_DECLARATION(JsxElement)
    FORWARD_DECLARATION(JsxTagNamePropertyAccess)
    FORWARD_DECLARATION(JsxAttributes)
    FORWARD_DECLARATION(JsxOpeningElement)
    FORWARD_DECLARATION(JsxSelfClosingElement)
    FORWARD_DECLARATION(JsxFragment)
    FORWARD_DECLARATION(JsxOpeningFragment)
    FORWARD_DECLARATION(JsxClosingFragment)
    FORWARD_DECLARATION(JsxAttribute)
    FORWARD_DECLARATION(JsxSpreadAttribute)
    FORWARD_DECLARATION(JsxClosingElement)
    FORWARD_DECLARATION(JsxExpression)
    FORWARD_DECLARATION(JsxText)
    FORWARD_DECLARATION(Statement)
    FORWARD_DECLARATION(NotEmittedStatement)
    FORWARD_DECLARATION(EndOfDeclarationMarker)
    FORWARD_DECLARATION(CommaListExpression)
    FORWARD_DECLARATION(MergeDeclarationMarker)
    FORWARD_DECLARATION(SyntheticReferenceExpression)
    FORWARD_DECLARATION(EmptyStatement)
    FORWARD_DECLARATION(DebuggerStatement)
    FORWARD_DECLARATION(MissingDeclaration)
    FORWARD_DECLARATION(Block)
    FORWARD_DECLARATION(VariableStatement)
    FORWARD_DECLARATION(ExpressionStatement)
    FORWARD_DECLARATION(PrologueDirective)
    FORWARD_DECLARATION(IfStatement)
    FORWARD_DECLARATION(IterationStatement)
    FORWARD_DECLARATION(DoStatement)
    FORWARD_DECLARATION(WhileStatement)
    FORWARD_DECLARATION(ForStatement)
    FORWARD_DECLARATION(ForInStatement)
    FORWARD_DECLARATION(ForOfStatement)
    FORWARD_DECLARATION(BreakStatement)
    FORWARD_DECLARATION(ContinueStatement)
    FORWARD_DECLARATION(ReturnStatement)
    FORWARD_DECLARATION(WithStatement)
    FORWARD_DECLARATION(SwitchStatement)
    FORWARD_DECLARATION(CaseBlock)
    FORWARD_DECLARATION(CaseClause)
    FORWARD_DECLARATION(DefaultClause)
    FORWARD_DECLARATION(LabeledStatement)
    FORWARD_DECLARATION(ThrowStatement)
    FORWARD_DECLARATION(TryStatement)
    FORWARD_DECLARATION(CatchClause)
    FORWARD_DECLARATION(ClassLikeDeclarationBase)
    FORWARD_DECLARATION(ClassDeclaration)
    FORWARD_DECLARATION(ClassExpression)
    FORWARD_DECLARATION(ClassElement)
    FORWARD_DECLARATION(TypeElement)
    FORWARD_DECLARATION(InterfaceDeclaration)
    FORWARD_DECLARATION(HeritageClause)
    FORWARD_DECLARATION(TypeAliasDeclaration)
    FORWARD_DECLARATION(EnumMember)
    FORWARD_DECLARATION(EnumDeclaration)
    FORWARD_DECLARATION(AmbientModuleDeclaration)
    FORWARD_DECLARATION(ModuleDeclaration)
    FORWARD_DECLARATION(NamespaceDeclaration)
    FORWARD_DECLARATION(JSDocNamespaceDeclaration)
    FORWARD_DECLARATION(ModuleBlock)
    FORWARD_DECLARATION(ImportEqualsDeclaration)
    FORWARD_DECLARATION(ExternalModuleReference)
    FORWARD_DECLARATION(ImportDeclaration)
    FORWARD_DECLARATION(ImportClause)
    FORWARD_DECLARATION(NamespaceImport)
    FORWARD_DECLARATION(NamespaceExport)
    FORWARD_DECLARATION(NamespaceExportDeclaration)
    FORWARD_DECLARATION(ExportDeclaration)
    FORWARD_DECLARATION(NamedImports)
    FORWARD_DECLARATION(NamedExports)
    FORWARD_DECLARATION(ImportSpecifier)
    FORWARD_DECLARATION(ExportSpecifier)
    FORWARD_DECLARATION(ExportAssignment)
    FORWARD_DECLARATION(FileReference)
    FORWARD_DECLARATION(CheckJsDirective)
    FORWARD_DECLARATION(CommentRange)
    FORWARD_DECLARATION(SynthesizedComment)
    FORWARD_DECLARATION(JSDocTypeExpression)
    FORWARD_DECLARATION(JSDocNameReference)
    FORWARD_DECLARATION(JSDocType)
    FORWARD_DECLARATION(JSDocAllType)
    FORWARD_DECLARATION(JSDocUnknownType)
    FORWARD_DECLARATION(JSDocNonNullableType)
    FORWARD_DECLARATION(JSDocNullableType)
    FORWARD_DECLARATION(JSDocOptionalType)
    FORWARD_DECLARATION(JSDocFunctionType)
    FORWARD_DECLARATION(JSDocVariadicType)
    FORWARD_DECLARATION(JSDocNamepathType)
    FORWARD_DECLARATION(JSDoc)
    FORWARD_DECLARATION(JSDocTag)
    FORWARD_DECLARATION(JSDocUnknownTag)
    FORWARD_DECLARATION(JSDocAugmentsTag)
    FORWARD_DECLARATION(JSDocImplementsTag)
    FORWARD_DECLARATION(JSDocAuthorTag)
    FORWARD_DECLARATION(JSDocDeprecatedTag)
    FORWARD_DECLARATION(JSDocClassTag)
    FORWARD_DECLARATION(JSDocPublicTag)
    FORWARD_DECLARATION(JSDocPrivateTag)
    FORWARD_DECLARATION(JSDocProtectedTag)
    FORWARD_DECLARATION(JSDocReadonlyTag)
    FORWARD_DECLARATION(JSDocEnumTag)
    FORWARD_DECLARATION(JSDocThisTag)
    FORWARD_DECLARATION(JSDocTemplateTag)
    FORWARD_DECLARATION(JSDocSeeTag)
    FORWARD_DECLARATION(JSDocReturnTag)
    FORWARD_DECLARATION(JSDocTypeTag)
    FORWARD_DECLARATION(JSDocTypedefTag)
    FORWARD_DECLARATION(JSDocCallbackTag)
    FORWARD_DECLARATION(JSDocSignature)
    FORWARD_DECLARATION(JSDocPropertyLikeTag)
    FORWARD_DECLARATION(JSDocPropertyTag)
    FORWARD_DECLARATION(JSDocParameterTag)
    FORWARD_DECLARATION(JSDocTypeLiteral)
    FORWARD_DECLARATION(AmdDependency)
    FORWARD_DECLARATION(CommentDirective)
    FORWARD_DECLARATION(SourceFileLike)
    FORWARD_DECLARATION(RedirectInfo)
    FORWARD_DECLARATION(DiagnosticMessage)
    FORWARD_DECLARATION(DiagnosticMessageChain)
    FORWARD_DECLARATION(DiagnosticRelatedInformation)
    FORWARD_DECLARATION(Diagnostic)
    FORWARD_DECLARATION(DiagnosticWithLocation)
    FORWARD_DECLARATION(DiagnosticWithDetachedLocation)
    FORWARD_DECLARATION(ResolvedModule)
    FORWARD_DECLARATION(PackageId)
    FORWARD_DECLARATION(ResolvedModuleFull)
    FORWARD_DECLARATION(ResolvedTypeReferenceDirective)
    FORWARD_DECLARATION(PatternAmbientModule)
    FORWARD_DECLARATION(SourceFile)

    NODE_REF(EntityName)
    NODE_REF(PropertyName)
    NODE_REF(MemberName)
    NODE_REF(DeclarationName)

    NODE_REF(EntityNameExpression)
    NODE_REF(EntityNameOrEntityNameExpression)
    NODE_REF(AccessExpression)

    NODE_REF(BindingName)
    NODE_REF(SignatureDeclaration)
    NODE_REF(BindingPattern)
    NODE_REF(ArrayBindingElement)
    NODE_REF(ClassLikeDeclaration)

    CLASS_REF(FunctionBody, Block)
    NODE_REF(ConciseBody)

    NODE_REF(ObjectTypeDeclaration)
    NODE_REF(DeclarationWithTypeParameters)
    NODE_REF(DeclarationWithTypeParameterChildren)
    
    TMPL_REF(DotToken, PunctuationToken, SyntaxKind::DotToken)
    TMPL_REF(DotDotDotToken, PunctuationToken, SyntaxKind::DotDotDotToken)
    TMPL_REF(QuestionToken, PunctuationToken, SyntaxKind::QuestionToken)
    TMPL_REF(ExclamationToken, PunctuationToken, SyntaxKind::ExclamationToken)
    TMPL_REF(ColonToken, PunctuationToken, SyntaxKind::ColonToken)
    TMPL_REF(EqualsToken, PunctuationToken, SyntaxKind::EqualsToken)
    TMPL_REF(AsteriskToken, PunctuationToken, SyntaxKind::AsteriskToken)
    TMPL_REF(EqualsGreaterThanToken, PunctuationToken, SyntaxKind::EqualsGreaterThanToken)
    TMPL_REF(PlusToken, PunctuationToken, SyntaxKind::PlusToken)
    TMPL_REF(MinusToken, PunctuationToken, SyntaxKind::MinusToken)
    TMPL_REF(QuestionDotToken, PunctuationToken, SyntaxKind::QuestionDotToken)

    TMPL_REF(AssertsKeyword, KeywordToken, SyntaxKind::AssertsKeyword)
    TMPL_REF(AwaitKeyword, KeywordToken, SyntaxKind::AwaitKeyword)

    CLASS_REF(AwaitKeywordToken, AwaitKeyword)
    CLASS_REF(AssertsToken, AssertsKeyword)

    TMPL_REF(AbstractKeyword, ModifierToken, SyntaxKind::AbstractKeyword)
    TMPL_REF(AsyncKeyword,ModifierToken, SyntaxKind::AsyncKeyword)
    TMPL_REF(ConstKeyword, ModifierToken, SyntaxKind::ConstKeyword)
    TMPL_REF(DeclareKeyword, ModifierToken, SyntaxKind::DeclareKeyword)
    TMPL_REF(DefaultKeyword, ModifierToken, SyntaxKind::DefaultKeyword)
    TMPL_REF(ExportKeyword, ModifierToken, SyntaxKind::ExportKeyword)
    TMPL_REF(PrivateKeyword, ModifierToken, SyntaxKind::PrivateKeyword)
    TMPL_REF(ProtectedKeyword, ModifierToken, SyntaxKind::ProtectedKeyword)
    TMPL_REF(PublicKeyword, ModifierToken, SyntaxKind::PublicKeyword)
    TMPL_REF(ReadonlyKeyword, ModifierToken, SyntaxKind::ReadonlyKeyword)
    TMPL_REF(StaticKeyword, ModifierToken, SyntaxKind::StaticKeyword)

    NODE_REF(BindableStaticNameExpression)
    NODE_REF(BindableStaticAccessExpression)
    NODE_REF(BindableAccessExpression)
    NODE_REF(CallLikeExpression)
    
    NODE_REF(TemplateLiteral)

    NODE_REF(JsxChild)
    NODE_REF(JsxOpeningLikeElement)
    NODE_REF(JsxAttributeLike)
    NODE_REF(JsxTagNameExpression)

    NODE_REF(ForInitializer)
    NODE_REF(BreakOrContinueStatement)
    NODE_REF(CaseOrDefaultClause)

    NODE_REF(ModuleName)
    NODE_REF(ModuleBody)
    NODE_REF(ModuleReference)
    NODE_REF(NamespaceBody)
    NODE_REF(JSDocNamespaceBody)

    NODE_REF(NamedImportBindings)
    NODE_REF(NamedExportBindings)

    NODE_REF(ImportOrExportSpecifier)
    NODE_REF(TypeOnlyCompatibleAliasDeclaration)

    NODE_REF(JSDocTypeReferencingNode)
    NODE_REF(HasJSDoc)
    NODE_REF(DestructuringPattern)
    NODE_REF(BindingElementGrandparent)

    NODE_REF(ObjectLiteralElementLike)
    NODE_REF(StringLiteralLike)
    
} // namespace data

WRAPPER(TextRange)
WRAPPER(ReadonlyTextRange)
WRAPPER(Symbol)
WRAPPER(Type)
WRAPPER(Node)
WRAPPER(JSDocContainer)
WRAPPER(EndOfFileToken)
WRAPPER(QualifiedName)
WRAPPER(Declaration)
WRAPPER(NamedDeclaration)
WRAPPER(DynamicNamedDeclaration)
WRAPPER(DynamicNamedBinaryExpression)
WRAPPER(LateBoundDeclaration)
WRAPPER(LateBoundBinaryExpressionDeclaration)
WRAPPER(LateBoundElementAccessExpression)
WRAPPER(DeclarationStatement)
WRAPPER(ComputedPropertyName)
WRAPPER(PrivateIdentifier)
WRAPPER(LateBoundName)
WRAPPER(Decorator)
WRAPPER(TypeParameterDeclaration)
WRAPPER(SignatureDeclarationBase)
WRAPPER(CallSignatureDeclaration)
WRAPPER(ConstructSignatureDeclaration)
WRAPPER(VariableDeclaration)
WRAPPER(InitializedVariableDeclaration)
WRAPPER(VariableDeclarationList)
WRAPPER(ParameterDeclaration)
WRAPPER(BindingElement)
WRAPPER(PropertySignature)
WRAPPER(PropertyDeclaration)
WRAPPER(PrivateIdentifierPropertyDeclaration)
WRAPPER(InitializedPropertyDeclaration)
WRAPPER(ObjectLiteralElement)
WRAPPER(PropertyAssignment)
WRAPPER(ShorthandPropertyAssignment)
WRAPPER(SpreadAssignment)
WRAPPER(PropertyLikeDeclaration)
WRAPPER(ObjectBindingPattern)
WRAPPER(ArrayBindingPattern)
WRAPPER(FunctionLikeDeclarationBase)
WRAPPER(FunctionDeclaration)
WRAPPER(MethodSignature)
WRAPPER(MethodDeclaration)
WRAPPER(ConstructorDeclaration)
WRAPPER(SemicolonClassElement)
WRAPPER(GetAccessorDeclaration)
WRAPPER(SetAccessorDeclaration)
WRAPPER(IndexSignatureDeclaration)
WRAPPER(TypeNode)
WRAPPER(ImportTypeNode)
WRAPPER(LiteralImportTypeNode)
WRAPPER(ThisTypeNode)
WRAPPER(FunctionOrConstructorTypeNodeBase)
WRAPPER(FunctionTypeNode)
WRAPPER(ConstructorTypeNode)
WRAPPER(NodeWithTypeArguments)
WRAPPER(TypeReferenceNode)
WRAPPER(TypePredicateNode)
WRAPPER(TypeQueryNode)
WRAPPER(TypeLiteralNode)
WRAPPER(ArrayTypeNode)
WRAPPER(TupleTypeNode)
WRAPPER(NamedTupleMember)
WRAPPER(OptionalTypeNode)
WRAPPER(RestTypeNode)
WRAPPER(UnionTypeNode)
WRAPPER(IntersectionTypeNode)
WRAPPER(ConditionalTypeNode)
WRAPPER(InferTypeNode)
WRAPPER(ParenthesizedTypeNode)
WRAPPER(TypeOperatorNode)
WRAPPER(UniqueTypeOperatorNode)
WRAPPER(IndexedAccessTypeNode)
WRAPPER(MappedTypeNode)
WRAPPER(LiteralTypeNode)
WRAPPER(StringLiteral)
WRAPPER(TemplateLiteralTypeNode)
WRAPPER(TemplateLiteralTypeSpan)
WRAPPER(Expression)
WRAPPER(OmittedExpression)
WRAPPER(PartiallyEmittedExpression)
WRAPPER(UnaryExpression)
WRAPPER(UpdateExpression)
WRAPPER(PrefixUnaryExpression)
WRAPPER(PostfixUnaryExpression)
WRAPPER(LeftHandSideExpression)
WRAPPER(MemberExpression)
WRAPPER(PrimaryExpression)
WRAPPER(Identifier)
WRAPPER(TransientIdentifier)
WRAPPER(GeneratedIdentifier)
WRAPPER(NullLiteral)
WRAPPER(TrueLiteral)
WRAPPER(FalseLiteral)
WRAPPER(ThisExpression)
WRAPPER(SuperExpression)
WRAPPER(ImportExpression)
WRAPPER(DeleteExpression)
WRAPPER(TypeOfExpression)
WRAPPER(VoidExpression)
WRAPPER(AwaitExpression)
WRAPPER(YieldExpression)
WRAPPER(SyntheticExpression)
WRAPPER(BinaryExpression)
WRAPPER(ObjectDestructuringAssignment)
WRAPPER(ArrayDestructuringAssignment)
WRAPPER(ConditionalExpression)
WRAPPER(FunctionExpression)
WRAPPER(ArrowFunction)
WRAPPER(LiteralLikeNode)
WRAPPER(TemplateLiteralLikeNode)
WRAPPER(LiteralExpression)
WRAPPER(RegularExpressionLiteral)
WRAPPER(NoSubstitutionTemplateLiteral)
WRAPPER(NumericLiteral)
WRAPPER(BigIntLiteral)
WRAPPER(TemplateHead)
WRAPPER(TemplateMiddle)
WRAPPER(TemplateTail)
WRAPPER(TemplateExpression)
WRAPPER(TemplateSpan)
WRAPPER(ParenthesizedExpression)
WRAPPER(ArrayLiteralExpression)
WRAPPER(SpreadElement)
WRAPPER(ObjectLiteralExpression)
WRAPPER(PropertyAccessExpression)
WRAPPER(PrivateIdentifierPropertyAccessExpression)
WRAPPER(PropertyAccessChain)
WRAPPER(PropertyAccessChainRoot)
WRAPPER(SuperPropertyAccessExpression)
WRAPPER(PropertyAccessEntityNameExpression)
WRAPPER(ElementAccessExpression)
WRAPPER(ElementAccessChain)
WRAPPER(ElementAccessChainRoot)
WRAPPER(SuperElementAccessExpression)
WRAPPER(CallExpression)
WRAPPER(CallChain)
WRAPPER(CallChainRoot)
WRAPPER(BindableObjectDefinePropertyCall)
WRAPPER(LiteralLikeElementAccessExpression)
WRAPPER(BindableStaticElementAccessExpression)
WRAPPER(BindableElementAccessExpression)
WRAPPER(BindableStaticPropertyAssignmentExpression)
WRAPPER(BindablePropertyAssignmentExpression)
WRAPPER(SuperCall)
WRAPPER(ImportCall)
WRAPPER(ExpressionWithTypeArguments)
WRAPPER(NewExpression)
WRAPPER(TaggedTemplateExpression)
WRAPPER(AsExpression)
WRAPPER(TypeAssertion)
WRAPPER(NonNullExpression)
WRAPPER(NonNullChain)
WRAPPER(MetaProperty)
WRAPPER(ImportMetaProperty)
WRAPPER(JsxElement)
WRAPPER(JsxTagNamePropertyAccess)
WRAPPER(JsxAttributes)
WRAPPER(JsxOpeningElement)
WRAPPER(JsxSelfClosingElement)
WRAPPER(JsxFragment)
WRAPPER(JsxOpeningFragment)
WRAPPER(JsxClosingFragment)
WRAPPER(JsxAttribute)
WRAPPER(JsxSpreadAttribute)
WRAPPER(JsxClosingElement)
WRAPPER(JsxExpression)
WRAPPER(JsxText)
WRAPPER(Statement)
WRAPPER(NotEmittedStatement)
WRAPPER(EndOfDeclarationMarker)
WRAPPER(CommaListExpression)
WRAPPER(MergeDeclarationMarker)
WRAPPER(SyntheticReferenceExpression)
WRAPPER(EmptyStatement)
WRAPPER(DebuggerStatement)
WRAPPER(MissingDeclaration)
WRAPPER(Block)
WRAPPER(VariableStatement)
WRAPPER(ExpressionStatement)
WRAPPER(PrologueDirective)
WRAPPER(IfStatement)
WRAPPER(IterationStatement)
WRAPPER(DoStatement)
WRAPPER(WhileStatement)
WRAPPER(ForStatement)
WRAPPER(ForInStatement)
WRAPPER(ForOfStatement)
WRAPPER(BreakStatement)
WRAPPER(ContinueStatement)
WRAPPER(ReturnStatement)
WRAPPER(WithStatement)
WRAPPER(SwitchStatement)
WRAPPER(CaseBlock)
WRAPPER(CaseClause)
WRAPPER(DefaultClause)
WRAPPER(LabeledStatement)
WRAPPER(ThrowStatement)
WRAPPER(TryStatement)
WRAPPER(CatchClause)
WRAPPER(ClassLikeDeclarationBase)
WRAPPER(ClassDeclaration)
WRAPPER(ClassExpression)
WRAPPER(ClassElement)
WRAPPER(TypeElement)
WRAPPER(InterfaceDeclaration)
WRAPPER(HeritageClause)
WRAPPER(TypeAliasDeclaration)
WRAPPER(EnumMember)
WRAPPER(EnumDeclaration)
WRAPPER(AmbientModuleDeclaration)
WRAPPER(ModuleDeclaration)
WRAPPER(NamespaceDeclaration)
WRAPPER(JSDocNamespaceDeclaration)
WRAPPER(ModuleBlock)
WRAPPER(ImportEqualsDeclaration)
WRAPPER(ExternalModuleReference)
WRAPPER(ImportDeclaration)
WRAPPER(ImportClause)
WRAPPER(NamespaceImport)
WRAPPER(NamespaceExport)
WRAPPER(NamespaceExportDeclaration)
WRAPPER(ExportDeclaration)
WRAPPER(NamedImports)
WRAPPER(NamedExports)
WRAPPER(ImportSpecifier)
WRAPPER(ExportSpecifier)
WRAPPER(ExportAssignment)
WRAPPER(FileReference)
WRAPPER(CheckJsDirective)
WRAPPER(CommentRange)
WRAPPER(SynthesizedComment)
WRAPPER(JSDocTypeExpression)
WRAPPER(JSDocNameReference)
WRAPPER(JSDocType)
WRAPPER(JSDocAllType)
WRAPPER(JSDocUnknownType)
WRAPPER(JSDocNonNullableType)
WRAPPER(JSDocNullableType)
WRAPPER(JSDocOptionalType)
WRAPPER(JSDocFunctionType)
WRAPPER(JSDocVariadicType)
WRAPPER(JSDocNamepathType)
WRAPPER(JSDoc)
WRAPPER(JSDocTag)
WRAPPER(JSDocUnknownTag)
WRAPPER(JSDocAugmentsTag)
WRAPPER(JSDocImplementsTag)
WRAPPER(JSDocAuthorTag)
WRAPPER(JSDocDeprecatedTag)
WRAPPER(JSDocClassTag)
WRAPPER(JSDocPublicTag)
WRAPPER(JSDocPrivateTag)
WRAPPER(JSDocProtectedTag)
WRAPPER(JSDocReadonlyTag)
WRAPPER(JSDocEnumTag)
WRAPPER(JSDocThisTag)
WRAPPER(JSDocTemplateTag)
WRAPPER(JSDocSeeTag)
WRAPPER(JSDocReturnTag)
WRAPPER(JSDocTypeTag)
WRAPPER(JSDocTypedefTag)
WRAPPER(JSDocCallbackTag)
WRAPPER(JSDocSignature)
WRAPPER(JSDocPropertyLikeTag)
WRAPPER(JSDocPropertyTag)
WRAPPER(JSDocParameterTag)
WRAPPER(JSDocTypeLiteral)
WRAPPER(AmdDependency)
WRAPPER(CommentDirective)
WRAPPER(SourceFileLike)
WRAPPER(RedirectInfo)
WRAPPER(DiagnosticMessage)
WRAPPER(DiagnosticMessageChain)
WRAPPER(DiagnosticRelatedInformation)
WRAPPER(Diagnostic)
WRAPPER(DiagnosticWithLocation)
WRAPPER(DiagnosticWithDetachedLocation)
WRAPPER(ResolvedModule)
WRAPPER(PackageId)
WRAPPER(ResolvedModuleFull)
WRAPPER(ResolvedTypeReferenceDirective)
WRAPPER(PatternAmbientModule)
WRAPPER(SourceFile)

WRAPPER(EntityName)
WRAPPER(PropertyName)
WRAPPER(MemberName)
WRAPPER(DeclarationName)

WRAPPER(EntityNameExpression)
WRAPPER(EntityNameOrEntityNameExpression)
WRAPPER(AccessExpression)

WRAPPER(BindingName)
WRAPPER(SignatureDeclaration)
WRAPPER(BindingPattern)
WRAPPER(ArrayBindingElement)
WRAPPER(ClassLikeDeclaration)

WRAPPER(FunctionBody)
WRAPPER(ConciseBody)

WRAPPER(ObjectTypeDeclaration)
WRAPPER(DeclarationWithTypeParameters)
WRAPPER(DeclarationWithTypeParameterChildren)

WRAPPER(DotToken)
WRAPPER(DotDotDotToken)
WRAPPER(QuestionToken)
WRAPPER(ExclamationToken)
WRAPPER(ColonToken)
WRAPPER(EqualsToken)
WRAPPER(AsteriskToken)
WRAPPER(EqualsGreaterThanToken)
WRAPPER(PlusToken)
WRAPPER(MinusToken)
WRAPPER(QuestionDotToken)

WRAPPER(AssertsKeyword)
WRAPPER(AwaitKeyword)

WRAPPER(AwaitKeywordToken)
WRAPPER(AssertsToken)

WRAPPER(AbstractKeyword)
WRAPPER(AsyncKeyword)
WRAPPER(ConstKeyword)
WRAPPER(DeclareKeyword)
WRAPPER(DefaultKeyword)
WRAPPER(ExportKeyword)
WRAPPER(PrivateKeyword)
WRAPPER(ProtectedKeyword)
WRAPPER(PublicKeyword)
WRAPPER(ReadonlyKeyword)
WRAPPER(StaticKeyword)

WRAPPER(BindableStaticNameExpression)
WRAPPER(BindableStaticAccessExpression)
WRAPPER(BindableAccessExpression)
WRAPPER(CallLikeExpression)

WRAPPER(TemplateLiteral)

WRAPPER(JsxChild)
WRAPPER(JsxOpeningLikeElement)
WRAPPER(JsxAttributeLike)
WRAPPER(JsxTagNameExpression)

WRAPPER(ForInitializer)
WRAPPER(BreakOrContinueStatement)
WRAPPER(CaseOrDefaultClause)

WRAPPER(ModuleName)
WRAPPER(ModuleBody)
WRAPPER(ModuleReference)
WRAPPER(NamespaceBody)
WRAPPER(JSDocNamespaceBody)

WRAPPER(NamedImportBindings)
WRAPPER(NamedExportBindings)

WRAPPER(ImportOrExportSpecifier)
WRAPPER(TypeOnlyCompatibleAliasDeclaration)

WRAPPER(JSDocTypeReferencingNode)
WRAPPER(HasJSDoc)
WRAPPER(DestructuringPattern)
WRAPPER(BindingElementGrandparent)

WRAPPER(ObjectLiteralElementLike)
WRAPPER(StringLiteralLike)

#endif // NEW_PARSER_FWRD_TYPES_H