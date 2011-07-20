#pragma once
#include <QPushButton>

#include "../thirdparty/qtpropertybrowser/src/qtvariantproperty.h"
#include "../thirdparty/qtpropertybrowser/src/qteditorfactory.h"

class PushButtonPropertyManager : public QtAbstractPropertyManager
{
	friend class PushButtonFactory;
	Q_OBJECT
public:
	PushButtonPropertyManager(QObject *parent = 0);
	~PushButtonPropertyManager();

	bool value(const QtProperty *property) const;

public Q_SLOTS:
	void setValue(QtProperty *, bool){}
Q_SIGNALS:
	void valueChanged(QtProperty *property, bool val);
	void buttonClicked(QtProperty *property);

protected:
	QString valueText(const QtProperty *property) const;
	QIcon valueIcon(const QtProperty *property) const;
	virtual void initializeProperty(QtProperty *){}
	virtual void uninitializeProperty(QtProperty *){}
private:
	class {} *d_ptr;
	Q_DISABLE_COPY(PushButtonPropertyManager)
};


class PushButtonFactory : public QtAbstractEditorFactory<PushButtonPropertyManager>
{
	Q_OBJECT

public:
	PushButtonFactory(QObject *parent = 0);
	~PushButtonFactory();

protected:
	void connectPropertyManager(PushButtonPropertyManager *manager);
	QWidget *createEditor(PushButtonPropertyManager *manager, QtProperty *property,
				QWidget *parent);
	void disconnectPropertyManager(PushButtonPropertyManager *manager);

private:
	class {} *d_ptr;
	Q_DISABLE_COPY(PushButtonFactory)
};
