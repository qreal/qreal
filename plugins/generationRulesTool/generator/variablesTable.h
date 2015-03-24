#pragma once

#include <QtCore/QHash>
#include <QtCore/QString>

namespace generationRules {
namespace generator {

class VariablesTable
{
public:
	VariablesTable();

	void addNewVariable(const QString &name, const QString &type);
	void removeVariable(const QString &name);

	void clear();

	QString typeByName(const QString &name) const;

private:
	QHash<QString, QString> mHashTable;
};

}
}
