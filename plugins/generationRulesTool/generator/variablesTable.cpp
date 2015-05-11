#include "variablesTable.h"

using namespace generationRules::generator;

VariablesTable::VariablesTable()
	: mHashTable()
{
}

void VariablesTable::addNewVariable(const QString &name, const QString &type, const qReal::IdList &listOfIds)
{
	VariableData *newVariableData = new VariableData(type, listOfIds);
	mHashTable.insert(name, newVariableData);
}

void VariablesTable::removeVariable(const QString &name)
{
	mHashTable.remove(name);
}

void VariablesTable::clear()
{
	mHashTable.clear();
}

QString VariablesTable::textRepresentation() const
{
	QString result = "";

	QHashIterator<QString, VariableData*> iterator(mHashTable);
	while (iterator.hasNext()) {
		iterator.next();
		result += iterator.key() + " " + iterator.value()->stringRepresentation() + "\n";
	}

	return result;
}

bool VariablesTable::containsVariable(const QString &name) const
{
	return mHashTable.contains(name);
}

QString VariablesTable::typeByName(const QString &name) const
{
	return mHashTable.value(name)->type();
}

qReal::Id VariablesTable::currentId(const QString &variableName)
{
	return mHashTable.value(variableName)->currentId();
}

void VariablesTable::movePointer(const QString &variableName)
{
	mHashTable.value(variableName)->moveToNextId();
}

bool VariablesTable::nextIdExists(const QString &variableName)
{
	return mHashTable.value(variableName)->nextExists();
}
