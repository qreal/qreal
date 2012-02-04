#pragma once
#include "../../qrrepo/logicalRepoApi.h"

namespace qReal {

class ConstraintsPluginInterface {
public:
	virtual QPair<bool, QPair<QString, QString> > check(IdList const &elements, qrRepo::LogicalRepoApi const &logicalApi) = 0;
	virtual QString languageName() const = 0;

	bool isCorrectLanguageName(IdList const &elements) const//asd //?? //т.е. совпадает хотя бы с одним элементом
	{
		foreach (Id const &element, elements) {
			if (languageName() == element.editor()) {
				return true;
			}
		}
		return false;
	}
};

}

Q_DECLARE_INTERFACE(qReal::ConstraintsPluginInterface, "ru.qreal.QReal.ConstraintsPluginInterface/0.1")
