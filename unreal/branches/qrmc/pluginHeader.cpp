#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "defs.h"
#include "pluginHeader.h"

PluginHeader::PluginHeader(const QString &pluginName) : mName(pluginName)
{
}

PluginHeader::~PluginHeader()
{
}

void PluginHeader::init(qrRepo::RepoApi &repo)
{
	Q_UNUSED(repo);
}

bool PluginHeader::generate(QString const &hdrTemplate)
{
	QString headerTemplate = hdrTemplate;
	qDebug() << "generating plugin header for " << mName;
	QDir dir;
	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);
	if (!dir.exists(mName))
		dir.mkdir(mName);
	dir.cd(mName);


	QString fileName = dir.absoluteFilePath(pluginHeaderName);
	QFile pluginHeaderFile(fileName);
	if (!pluginHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	headerTemplate.replace(metamodelNameTag, mName); // header requires just plugin name customization
	QTextStream out(&pluginHeaderFile);
	out << headerTemplate;
	pluginHeaderFile.close();

	return true;
}
