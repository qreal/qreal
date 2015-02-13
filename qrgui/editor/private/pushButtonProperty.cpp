#include "pushButtonProperty.h"

PushButtonPropertyManager::PushButtonPropertyManager(QObject *parent)
	: QtAbstractPropertyManager(parent)
{
	d_ptr = nullptr;
}

PushButtonPropertyManager::~PushButtonPropertyManager()
{
	clear();
	delete d_ptr;
}

bool PushButtonPropertyManager::value(const QtProperty *) const
{
	return false;
}

QString PushButtonPropertyManager::valueText(const QtProperty *) const
{
	return tr("Click to choose");
}

QIcon PushButtonPropertyManager::valueIcon(const QtProperty *) const
{
	return QIcon();
}

PushButtonFactory::PushButtonFactory(QObject *parent)
	: QtAbstractEditorFactory<PushButtonPropertyManager>(parent)
{
	d_ptr = nullptr; // new PushButtonFactoryPrivate();
}

PushButtonFactory::~PushButtonFactory()
{
}

void PushButtonFactory::connectPropertyManager(PushButtonPropertyManager *)
{
}

QWidget *PushButtonFactory::createEditor(PushButtonPropertyManager *manager, QtProperty *property, QWidget *)
{
	emit manager->buttonClicked(property);
	return nullptr;
}

void PushButtonFactory::disconnectPropertyManager(PushButtonPropertyManager *)
{
}
