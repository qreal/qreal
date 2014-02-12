#pragma once

#include <QtCore/QVariant>
#include <QtCore/QMap>

#include <qrgui/umllib/nodeElement.h>
#include <qrgui/editorPluginInterface/propertyEditorInterface.h>

namespace qReal
{

class PropertyProxyBase : public QObject
{
	Q_OBJECT

public:
	explicit PropertyProxyBase(QWidget *widget);

	QMap<QString, QString> outerBindings() const;
	void setOuterBindings(QMap<QString, QString> const &bindings);

	void generateBinders(QList<PropertyEditorInterface *> &editors);

protected:
	NodeElement *getNode() const;
	void onPropertyChanged(QString const &name, QVariant const &value);

signals:
	void propertyChanged(QString const &name, QVariant const &value);

private slots:
	void setValueInRepo(QString const &property, QVariant const &value);

private:
	QWidget *mWidget;
	QMap<QString, QString> mOuterBindings;
};

}
