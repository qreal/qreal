#pragma once
#include "../../qrrepo/logicalRepoApi.h"

namespace qReal {

class ConstraintsPluginInterface {
public:
	virtual bool check(IdList const &elements, qrRepo::LogicalRepoApi const &logicalApi) = 0;
	virtual QString languageName() const = 0;

	bool isCorrectLanguageName(IdList const &elements)//asd //т.е. совпадает хотя бы с одним элементом
	{
		foreach (Id element, elements) {
			if (languageName() == element.editor()) {
				return true;
			}
		}
		return false;
	}
};

}

Q_DECLARE_INTERFACE(qReal::ConstraintsPluginInterface, "ru.qreal.QReal.ConstraintsPluginInterface/0.1")
