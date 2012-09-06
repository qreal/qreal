#include "framePropertyManager.h"

using namespace qReal::widgetsEdit;

FramePropertyManager::FramePropertyManager(Frame *frame)
	: LayoutToolPropertyManager(frame)
{
	mFrame = dynamic_cast<QFrame *>(mTool->widget());
	if (!mFrame)
	{
		return;
	}
	initProperties();
	QListIterator<QPair<QString, int> > *it
			= new QListIterator<QPair<QString, int> >(mProperties);
	while (it->hasNext())
	{
		QPair<QString, int> property = it->next();
		if (property.second != QtVariantPropertyManager::enumTypeId())
			addProperty(property.first, property.second);
		else
			addEnum(property.first);
	}
}

void FramePropertyManager::onPropertyChanged(const QString &name, const QVariant &value)
{
	if (!mFrame || !containsProperty(name))
	{
		LayoutToolPropertyManager::onPropertyChanged(name, value);
		return;
	}
	if (name == "shape")
	{
		mFrame->setFrameShape(shapeTypeToEnum(mShapeTypes[value.toInt()]));
	}
	if (name == "shadow")
	{
		mFrame->setFrameShadow(shadowTypeToEnum(mShadowTypes[value.toInt()]));
	}
	if (name == "lineWidth")
	{
		mFrame->setLineWidth(value.toInt());
	}
	if (name == "midLineWidth")
	{
		mFrame->setMidLineWidth(value.toInt());
	}

}

QVariant FramePropertyManager::propertyValue(const QString &name)
{
	if (!mFrame || !containsProperty(name))
	{
		return LayoutToolPropertyManager::propertyValue(name);
	}
	if (name == "shape")
	{
		return QVariant(mShapeTypes.indexOf(
							shapeEnumToType(mFrame->frameShape())));
	}
	if (name == "shadow")
	{
		return QVariant(mShadowTypes.indexOf(
							shadowEnumToType(mFrame->frameShadow())));
	}
	if (name == "lineWidth")
	{
		return QVariant(mFrame->lineWidth());
	}
	if (name == "midLineWidth")
	{
		return QVariant(mFrame->midLineWidth());
	}
	return QVariant();
}

void FramePropertyManager::initProperties()
{
	mProperties << QPair<QString, int>("shape",
									   QtVariantPropertyManager::enumTypeId());
	mShapeTypes << "NoFrame" << "Box" << "Panel"
				<< "WinPanel" << "HLine" << "VLine" << "StyledPanel";
	mProperties << QPair<QString, int>("shadow",
									   QtVariantPropertyManager::enumTypeId());
	mShadowTypes << "Plain" << "Raised" << "Sunken";
	mProperties << QPair<QString, int>("lineWidth", QVariant::Int);
	mProperties << QPair<QString, int>("midLineWidth", QVariant::Int);
}

bool FramePropertyManager::containsProperty(const QString &name)
{
	QListIterator<QPair<QString, int> > it =
			QListIterator<QPair<QString, int> >(mProperties);
	while (it.hasNext())
	{
		if (it.next().first == name)
		{
			return true;
		}
	}
	return false;
}

void FramePropertyManager::addEnum(const QString &name)
{
	if (name == "shape")
	{
		addEnumProperty(name, mShapeTypes);
	}
	if (name == "shadow")
	{
		addEnumProperty(name, mShadowTypes);
	}
}

QFrame::Shape FramePropertyManager::shapeTypeToEnum(const QString &type)
{
	if (type == "NoFrame")
		return QFrame::NoFrame;
	if (type == "Box")
		return QFrame::Box;
	if (type == "Panel")
		return QFrame::Panel;
	if (type == "WinPanel")
		return QFrame::WinPanel;
	if (type == "HLine")
		return QFrame::HLine;
	if (type == "VLine")
		return QFrame::VLine;
	if (type == "StyledPanel")
		return QFrame::StyledPanel;
	return QFrame::NoFrame;
}

QString FramePropertyManager::shapeEnumToType(QFrame::Shape shape)
{
	switch(shape)
	{
	case QFrame::NoFrame:
		return "NoFrame";
	case QFrame::Box:
		return "Box";
	case QFrame::Panel:
		return "Panel";
	case QFrame::WinPanel:
		return "WinPanel";
	case QFrame::HLine:
		return "HLine";
	case QFrame::VLine:
		return "VLine";
	case QFrame::StyledPanel:
		return "StyledPanel";
	default:
		return "NoFrame";
	}
}

QFrame::Shadow FramePropertyManager::shadowTypeToEnum(const QString &type)
{
	if (type == "Plain")
		return QFrame::Plain;
	if (type == "Raised")
		return QFrame::Raised;
	if (type == "Sunken")
		return QFrame::Sunken;
	return QFrame::Plain;
}

QString FramePropertyManager::shadowEnumToType(QFrame::Shadow shape)
{
	switch(shape)
	{
	case QFrame::Plain:
		return "Plain";
	case QFrame::Raised:
		return "Raised";
	case QFrame::Sunken:
		return "Sunken";
	default:
		return "Plain";
	}
}
