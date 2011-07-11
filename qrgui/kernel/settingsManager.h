#pragma once
#include <QtCore/QSettings>
#include <QString>
#include <QHash>
#include <QVariant>
#include <QMutex>

class SettingsManager
{
public:
	static QVariant value(QString key, QVariant defaultValue = QVariant());
	static void setValue(QString name, QVariant value = QVariant());
	void saveData();
	void load();

	static SettingsManager* instance();

private:
	void set(QString name, QVariant value);
	QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;

	static SettingsManager* mInstance;
	SettingsManager();
	QString mDirectory;
	QString mName;
	QHash<QString, QVariant> mData;
	QSettings mSettings;

};


