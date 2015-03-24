#include "variablesTable.h"

using namespace generationRules::generator;

VariablesTable::VariablesTable()
	: mHashTable()
{
}

void VariablesTable::addNewVariable(const QString &name, const QString &type)
{
	mHashTable.insert(name, type);
}

void VariablesTable::removeVariable(const QString &name)
{
	mHashTable.remove(name);
}

void VariablesTable::clear()
{
	mHashTable.clear();
}

QString VariablesTable::typeByName(const QString &name) const
{
	return mHashTable.value(name);
}
