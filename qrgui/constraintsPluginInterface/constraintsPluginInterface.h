#pragma once
#include "../pluginManager/editorManagerInterface.h"
#include "../../qrrepo/logicalRepoApi.h"
#include "checkStatus.h"

namespace qReal {

const QString keywordForAllLanguages = "AllLanguages";//qwerty_asd_copypast

class ConstraintsPluginInterface {
public:
	virtual QList<CheckStatus> check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManagerInterface const &editorManager) = 0;
	virtual QString metamodelName() const = 0;
	virtual QString id() const = 0;

	bool isCorrectLanguageName(Id const &element) const
	{
		QString myMetamodelName = metamodelName();
		if (myMetamodelName == element.editor() || myMetamodelName == keywordForAllLanguages) {
			return true;
		}
		return false;
	}
};

}

Q_DECLARE_INTERFACE(qReal::ConstraintsPluginInterface, "ru.qreal.QReal.ConstraintsPluginInterface/0.1")
