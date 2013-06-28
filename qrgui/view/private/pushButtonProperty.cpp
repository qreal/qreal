#include "pushButtonProperty.h"

PushButtonPropertyManager::PushButtonPropertyManager(QObject *parent)
	: QtAbstractPropertyManager(parent)
{
	d_ptr = NULL;
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
	d_ptr = NULL; // new PushButtonFactoryPrivate();
}

PushButtonFactory::~PushButtonFactory()
{
}

void PushButtonFactory::connectPropertyManager(PushButtonPropertyManager *)
{
//	connect(manager, SIGNAL(valueChanged(QtProperty *, bool)),
//				this, SLOT(slotPropertyChanged(QtProperty *, bool)));
}

QWidget *PushButtonFactory::createEditor(PushButtonPropertyManager *manager, QtProperty *property,
		QWidget *)
{
	emit manager->buttonClicked(property);
	return NULL;
}

void PushButtonFactory::disconnectPropertyManager(PushButtonPropertyManager *)
{
//	disconnect(manager, SIGNAL(valueChanged(QtProperty *, bool)),
//				this, SLOT(slotPropertyChanged(QtProperty *, bool)));
}
