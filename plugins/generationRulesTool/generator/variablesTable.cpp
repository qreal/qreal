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

QString VariablesTable::textRepresentation() const
{
	QString result = "";

	QHashIterator<QString, QString> iterator(mHashTable);
	while (iterator.hasNext()) {
		iterator.next();
		result += iterator.key() + " " + iterator.value() + "\n";
	}

	return result;
}

bool VariablesTable::containsVariable(const QString &name) const
{
	return mHashTable.contains(name);
}

QString VariablesTable::typeByName(const QString &name) const
{
	return mHashTable.value(name);
}
