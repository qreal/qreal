#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

class QUrl;
class EditorInterface;

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QUrl>

class EditorManager : public QObject
{
    Q_OBJECT

public:
	EditorManager(QObject *parent = 0);

	QList<QUrl> editors() const;
	QList<QUrl> diagrams(const QUrl &editor) const;
	QList<QUrl> elements(const QUrl &diagram) const;
	
	QString friendlyName(const QUrl &url) const;

public slots:

private slots:

private:
	QStringList pluginsLoaded;
	QMap<QString, QString> pluginFileName;
	QMap<QString, EditorInterface *> pluginIface;
//	parseUrl(QUrl &url);

	QDir pluginsDir;
	QStringList pluginFileNames;

	const QUrl root;
};

#endif
