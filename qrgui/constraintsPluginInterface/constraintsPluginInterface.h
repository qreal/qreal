#pragma once
#include "../pluginManager/editorManagerInterface.h"
#include "../../qrrepo/logicalRepoApi.h"
#include "../../qrrepo/graphicalRepoApi.h"
#include "checkStatus.h"

namespace qReal {

const QString keywordForAllMetamodels = "AllMetamodels";//qwerty_asd_copypast

class ConstraintsPluginInterface {
public:
	virtual QList<CheckStatus> check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManagerInterface const &editorManager)
	{
		Q_UNUSED(element)
		Q_UNUSED(logicalApi)
		Q_UNUSED(editorManager)
		return CheckStatus::defaultCheckStatusAsList();
	}
	virtual QList<CheckStatus> check(Id const &element, qrRepo::GraphicalRepoApi const &graphicalApi, EditorManagerInterface const &editorManager) //qwerty_harcode
	{
		Q_UNUSED(element)
		Q_UNUSED(graphicalApi)
		Q_UNUSED(editorManager)
		return CheckStatus::defaultCheckStatusAsList();
	}

	virtual QString metamodelName() const = 0;
	virtual QString id() const = 0;

	bool isCorrectMetamodelName(Id const &element) const
	{
		QString myMetamodelName = metamodelName();
		if (myMetamodelName == element.editor() || myMetamodelName == keywordForAllMetamodels) {
			return true;
		}
		return false;
	}
};

}

Q_DECLARE_INTERFACE(qReal::ConstraintsPluginInterface, "ru.qreal.QReal.ConstraintsPluginInterface/0.1")
