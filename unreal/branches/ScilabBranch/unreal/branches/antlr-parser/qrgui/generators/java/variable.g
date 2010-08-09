grammar variable;

options 
{
    k		= 2; 
    backtrack	= true; 
    memoize	= true;
    output = AST;
    language	= C;
}

@lexer::preinclude
{
#define ANTLR3_HUGE
}
@parser::preinclude
{
#define	ANTLR3_HUGE
}


@lexer::header
{
#define	ANTLR3_INLINE_INPUT_ASCII
}

@lexer::members 
{
static ANTLR3_BOOLEAN enumIsKeyword = ANTLR3_TRUE;
}

// starting point for parsing a java file// STATEMENTS / BLOCKS
compilationUnit
	: blockStatement
	;


blockStatement
	:	localVariableDeclaration
    |   classOrInterfaceDeclaration
    |   statement
	;

classOrInterfaceDeclaration
	:	modifier* (classDeclaration | interfaceDeclaration)
	;

classDeclaration
	:	normalClassDeclaration
    |   enumDeclaration
	;

normalClassDeclaration
	:	'class' Identifier (typeParameters)?
        ('extends' type)?
        ('implements' typeList)?
        classBody
	;

enumDeclaration
	:	ENUM Identifier ('implements' typeList)? enumBody
	;
	
localVariableDeclaration
	:	('final')* type variableDeclarators ';'
	;
	
variableDeclarators
	:	variableDeclarator (',' variableDeclarator)*
	;

variableDeclarator
	:	Identifier variableDeclaratorRest
	;

variableDeclaratorRest
	:	('[' ']')+ ('=' variableInitializer)?
	|	'=' variableInitializer
	|
	;


variableInitializer
	:    arrayInitializer
        |    expression
	;

expression
	:	conditionalExpression (assignmentOperator expression)?
	;

conditionalExpression
    	:   conditionalOrExpression ( '?' expression ':' expression )?
	;

conditionalOrExpression
	:   conditionalAndExpression ( '||' conditionalAndExpression )*
	;


conditionalAndExpression
    	:   inclusiveOrExpression ( '&&' inclusiveOrExpression )*
	;

inclusiveOrExpression
    	:   exclusiveOrExpression ( '|' exclusiveOrExpression )*
	;
	

exclusiveOrExpression
    	:   andExpression ( '^' andExpression )*
	;

andExpression
    	:   equalityExpression ( '&' equalityExpression )*
	;
	
equalityExpression
    	:   instanceOfExpression ( ('==' | '!=') instanceOfExpression )*
	;

instanceOfExpression
   	:   relationalExpression ('instanceof' type)?
	;
	
relationalExpression
    	:   shiftExpression ( relationalOp shiftExpression )*
	;

shiftExpression
    	:   additiveExpression ( shiftOp additiveExpression )*
	;

additiveExpression
    	:   multiplicativeExpression ( ('+' | '-') multiplicativeExpression )*
	;

multiplicativeExpression
    	:   unaryExpression ( ( '*' | '/' | '%' ) unaryExpression )*
	;

unaryExpression
    :   '+' unaryExpression
    |	'-' unaryExpression
    |   '++' unaryExpression
    |   '--' unaryExpression
    |   unaryExpressionNotPlusMinus
    ;

assignmentOperator
	:	'='
    |   '+='
    |   '-='
    |   '*='
    |   '/='
    |   '&='
    |   '|='
    |   '^='
    |   '%='
    |   '<' '<' '='
    |   '>' '>' '='
    |   '>' '>' '>' '='
	;

relationalOp
	:	('<' '=' | '>' '=' | '<' | '>')
	;
	
shiftOp
	:	('<' '<' | '>' '>' '>' | '>' '>')
	;

unaryExpressionNotPlusMinus
    :   '~' unaryExpression
    | 	'!' unaryExpression
    |   castExpression
    |   primary selector* ('++'|'--')?
    ;
    
selector
	:	'.' Identifier (arguments)?
	|   '.' 'this'
	|   '.' 'super' superSuffix
	|   '.' 'new' (nonWildcardTypeArguments)? innerCreator
	|   '[' expression ']'
	;

innerCreator
	:	Identifier classCreatorRest
	;

classCreatorRest
	:	arguments classBody?
	;

classBody
	:	'{' classBodyDeclaration* '}'
	;

classBodyDeclaration
	:	';'
	|	'static'? block
	|	modifier* memberDecl
	;

memberDecl
	:	genericMethodOrConstructorDecl
	|	methodDeclaration
	|	fieldDeclaration
	|	'void' Identifier voidMethodDeclaratorRest
	|	Identifier constructorDeclaratorRest
	|	interfaceDeclaration
	|	classDeclaration
	;

constructorDeclaratorRest
	:	formalParameters ('throws' qualifiedNameList)? methodBody
	;

genericMethodOrConstructorDecl
	:	typeParameters genericMethodOrConstructorRest
	;

methodDeclaration
	:	type Identifier methodDeclaratorRest
	;

fieldDeclaration
	:	type variableDeclarators ';'
	;

voidMethodDeclaratorRest
	:	formalParameters ('throws' qualifiedNameList)?
        (   methodBody
        |   ';'
        )
	;
	
modifier
    :   'public'
    |   'protected'
    |   'private'
    |   'static'
    |   'abstract'
    |   'final'
    |   'native'
    |   'synchronized'
    |   'transient'
    |   'volatile'
    |   'strictfp'
    ;

block
	:	'{' blockStatement* '}'
	;

arguments
	:	'(' expressionList? ')'
	;

expressionList
    :   expression (',' expression)*
    ;

superSuffix
	:	arguments
	|   '.' Identifier (arguments)?
    ;
    
primary
    :	parExpression
    |   nonWildcardTypeArguments
        (explicitGenericInvocationSuffix | 'this' arguments)
    |   'this' ('.' Identifier)* (identifierSuffix)?
    |   'super' superSuffix
    |   literal
    |   'new' creator
    |   Identifier ('.' Identifier)* (identifierSuffix)?
    |   primitiveType ('[' ']')* '.' 'class'
    |   'void' '.' 'class'
    ;

creator
	:	nonWildcardTypeArguments? createdName
        (arrayCreatorRest | classCreatorRest)
	;

createdName
	:	Identifier typeArguments?
        ('.' Identifier typeArguments?)*
    |	primitiveType
	;

arrayCreatorRest
	:	'['
        (   ']' ('[' ']')* arrayInitializer
        |   expression ']' ('[' expression ']')* ('[' ']')*
        )
	;

explicitGenericInvocation
	:	nonWildcardTypeArguments explicitGenericInvocationSuffix
	;
   
explicitGenericInvocationSuffix
	:	'super' superSuffix
	|   Identifier arguments
	;

identifierSuffix
	:	('[' ']')+ '.' 'class'
	|	('[' expression ']')+ // can also be matched by selector, but do here
    |   arguments
    |   '.' 'class'
    |   '.' explicitGenericInvocation
    |   '.' 'this'
    |   '.' 'super' arguments
    |   '.' 'new' (nonWildcardTypeArguments)? innerCreator
	;

literal	
	:   integerLiteral
    |   FloatingPointLiteral
    |   CharacterLiteral
    |   StringLiteral
    |   booleanLiteral
    |   'null'
	;

integerLiteral
    :   HexLiteral
    |   OctalLiteral
    |   DecimalLiteral
    ;

booleanLiteral
    :   'true'
    |   'false'
    ;

parExpression
	:	'(' expression ')'
	;

nonWildcardTypeArguments
	:	'<' typeList '>'
	;

typeList
	:	type (',' type)*
	;

castExpression
    :  '(' primitiveType ')' unaryExpression
    |  '(' (type | expression) ')' unaryExpressionNotPlusMinus
    ;

arrayInitializer
	:	'{' (variableInitializer (',' variableInitializer)* (',')? )? '}'
	;

type
	:	Identifier (typeArguments)? ('.' Identifier (typeArguments)? )* ('[' ']')*
	|	primitiveType ('[' ']')*
	;

primitiveType
    :   'boolean'
    |	'char'
    |	'byte'
    |	'short'
    |	'int'
    |	'long'
    |	'float'
    |	'double'
    ;

methodBody
	:	block
	;

formalParameters
	:	'(' formalParameterDecls? ')'
	;
	
formalParameterDecls
	:	variableModifier* type formalParameterDeclsRest?
	;

formalParameterDeclsRest
	:	variableDeclaratorId (',' formalParameterDecls)?
	|   '...' variableDeclaratorId
	;

	
genericMethodOrConstructorRest
	:	(type | 'void') Identifier methodDeclaratorRest
	|	Identifier constructorDeclaratorRest
	;

variableDeclaratorId
	:	Identifier ('[' ']')*
	;

qualifiedNameList
	:	qualifiedName (',' qualifiedName)*
	;

qualifiedName
	:	Identifier ('.' Identifier)*
	;
	
typeParameters
	:	'<' typeParameter (',' typeParameter)* '>'
	;

typeParameter
	:	Identifier ('extends' bound)?
	;

	
interfaceDeclaration
	:	normalInterfaceDeclaration
	;

normalInterfaceDeclaration
	:	'interface' Identifier typeParameters? ('extends' typeList)? interfaceBody
	;

interfaceBody
	:	'{' interfaceBodyDeclaration* '}'
	;

interfaceBodyDeclaration
	:	modifier* interfaceMemberDecl
	|   ';'
	;


interfaceMemberDecl
	:	interfaceMethodOrFieldDecl
	|   interfaceGenericMethodDecl
    |   'void' Identifier voidInterfaceMethodDeclaratorRest
    |   interfaceDeclaration
    |   classDeclaration
	;

interfaceMethodOrFieldDecl
	:	type Identifier interfaceMethodOrFieldRest
	;

bound
	:	type ('&' type)*
	;

methodDeclaratorRest
	:	formalParameters ('[' ']')*
        ('throws' qualifiedNameList)?
        (   methodBody
        |   ';'
        )
	;

variableModifier
	:	'final'
	;


typeArguments
	:	'<' typeArgument (',' typeArgument)* '>'
	;
	
typeArgument
	:	type
	|	'?' (('extends' | 'super') type)?
	;

Identifier 
    :   Letter (Letter|JavaIDDigit)*
    ;
    
fragment
Letter
    :  '\u0024' |
       '\u0041'..'\u005a' |
       '\u005f' |
       '\u0061'..'\u007a' |
       '\u00c0'..'\u00d6' |
       '\u00d8'..'\u00f6' |
       '\u00f8'..'\u00ff' |
       '\u0100'..'\u1fff' |
       '\u3040'..'\u318f' |
       '\u3300'..'\u337f' |
       '\u3400'..'\u3d2d' |
       '\u4e00'..'\u9fff' |
       '\uf900'..'\ufaff'
    ;

fragment
JavaIDDigit
    :  '\u0030'..'\u0039' |
       '\u0660'..'\u0669' |
       '\u06f0'..'\u06f9' |
       '\u0966'..'\u096f' |
       '\u09e6'..'\u09ef' |
       '\u0a66'..'\u0a6f' |
       '\u0ae6'..'\u0aef' |
       '\u0b66'..'\u0b6f' |
       '\u0be7'..'\u0bef' |
       '\u0c66'..'\u0c6f' |
       '\u0ce6'..'\u0cef' |
       '\u0d66'..'\u0d6f' |
       '\u0e50'..'\u0e59' |
       '\u0ed0'..'\u0ed9' |
       '\u1040'..'\u1049'
   ;

FloatingPointLiteral
    :   ('0'..'9')+ '.' ('0'..'9')* Exponent? FloatTypeSuffix?
    |   '.' ('0'..'9')+ Exponent? FloatTypeSuffix?
    |   ('0'..'9')+ (	  Exponent FloatTypeSuffix?
						| FloatTypeSuffix
					)
	;


CharacterLiteral
    :   '\'' ( EscapeSequence | ~('\''|'\\') ) '\''
    ;

fragment
EscapeSequence
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   UnicodeEscape
    |   OctalEscape
    ;
   
fragment
UnicodeEscape
    :   '\\' 'u' HexDigit HexDigit HexDigit HexDigit
    ;
   
fragment
HexDigit : ('0'..'9'|'a'..'f'|'A'..'F') ;

HexLiteral : '0' ('x'|'X') HexDigit+ IntegerTypeSuffix? ;

fragment
IntegerTypeSuffix : ('l'|'L') ;

fragment
Exponent : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
FloatTypeSuffix : ('f'|'F'|'d'|'D') ;

OctalLiteral : '0' ('0'..'7')+ IntegerTypeSuffix? ;

DecimalLiteral : ('0' | '1'..'9' '0'..'9'*) IntegerTypeSuffix? ;

StringLiteral
    :  '"' ( EscapeSequence | ~('\\'|'"') )* '"'
    ;

ENUM:	'enum' {if ( !enumIsKeyword ) $type=Identifier;}
	;
	

fragment
OctalEscape
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

WS  :  (' '|'\r'|'\t'|'\u000C'|'\n') {$channel=HIDDEN;}
    ;

COMMENT
    :   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

LINE_COMMENT
    : '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    ;
