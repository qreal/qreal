#pragma once

#include <QtCore/QString>

namespace editorPluginTestingFramework {

class AbstractStringGenerator
{
public:
	/// clones StringGenerator and inits cloned generator
	virtual AbstractStringGenerator* clone() const = 0;

	/// generates string of results for one method of one interface
	virtual QString generateString() const = 0;

	/// generates string of results for one method of one interface
	virtual QString generateStringTime()
	{
		return "";
	}
//	virtual QList<QPair<qint64, int>> generateListTime(EditorManagerInterface *editorManagerInterface) = 0;





	/// generates name of tested method
	virtual QString methodName() const = 0;

	virtual QStringList ololo()
	{
		QStringList result;
		return result;
	}
};

}
