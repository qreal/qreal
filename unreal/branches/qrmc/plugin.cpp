#include "plugin.h"

Plugin::Plugin(qReal::Id id, QString const &name) : mMetamodelId(id), mName(name), mHeader(name), mSource(name)
{
	qDebug() << "created plugin " << name;
}

Plugin::~Plugin()
{
}

void Plugin::init(qrRepo::RepoApi &repo)
{
	mHeader.init(repo);
	mSource.init(repo, mMetamodelId);
}

void Plugin::generate(QString const &headerTemplate, QString const &sourceTemplate, QMap<QString, QString> const &utils)
{

	qDebug() << "generating plugin " << mName;
	mHeader.generate(headerTemplate);
	mSource.generate(sourceTemplate, utils);
}
