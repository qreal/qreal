#include "textLanguageParser/types/generalizationsTable.h"

#include "textLanguageParser/types/any.h"
#include "textLanguageParser/types/boolean.h"
#include "textLanguageParser/types/float.h"
#include "textLanguageParser/types/function.h"
#include "textLanguageParser/types/integer.h"
#include "textLanguageParser/types/nil.h"
#include "textLanguageParser/types/number.h"
#include "textLanguageParser/types/string.h"
#include "textLanguageParser/types/table.h"

using namespace textLanguageParser::types;

GeneralizationsTable::GeneralizationsTable()
{
}

bool GeneralizationsTable::isGeneralization(QSharedPointer<TypeExpression> const &specific
		, QSharedPointer<TypeExpression> const &general) const
{
	if (specific->is<Integer>() && general->is<Float>()) {
		return true;
	} else {
		return isStructurallyEquivalent(specific, general);
	}
}

bool GeneralizationsTable::isStructurallyEquivalent(QSharedPointer<TypeExpression> const &type1
		, QSharedPointer<TypeExpression> const &type2) const
{
	if (type1->is<Any>() && type2->is<Any>()) {
		return true;
	} if (type1->is<Boolean>() && type2->is<Boolean>()) {
		return true;
	} if (type1->is<Float>() && type2->is<Float>()) {
		return true;
	} if (type1->is<Integer>() && type2->is<Integer>()) {
		return true;
	} if (type1->is<Nil>() && type2->is<Nil>()) {
		return true;
	} if (type1->is<String>() && type2->is<String>()) {
		return true;
	}

	return false;
}

