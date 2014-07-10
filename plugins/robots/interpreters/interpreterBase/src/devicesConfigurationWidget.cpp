#include "interpreterBase/devicesConfigurationWidget.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>

#include "interpreterBase/robotModel/robotModelInterface.h"

using namespace interpreterBase;
using namespace robotModel;

DevicesConfigurationWidget::DevicesConfigurationWidget(QWidget *parent, bool autosaveMode, bool compactMode)
	: QScrollArea(parent)
	, mAutosaveMode(autosaveMode)
	, mCompactMode(compactMode)
	, mSaving(false)
	, mRefreshing(false)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumHeight(125);
	setFrameShape(QFrame::NoFrame);

	setWidgetResizable(true);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Background, Qt::transparent);
	setPalette(palette);
}

void DevicesConfigurationWidget::loadRobotModels(QList<RobotModelInterface *> const &models)
{
	for (RobotModelInterface * const model : models) {
		QString const name = model->name();
		mRobotModels[name] = model;
		QWidget * const configurer = configurerForRobotModel(*model);
		mRobotModelConfigurers[name] = configurer;
	}
}

void DevicesConfigurationWidget::selectRobotModel(RobotModelInterface &robotModel)
{
	QString const robotModelId = robotModel.name();
	mCurrentModel = robotModelId;
	takeWidget();
	if (mRobotModels.contains(robotModelId)) {
		setWidget(mRobotModelConfigurers[robotModelId]);
		refresh();
	}
}

QWidget *DevicesConfigurationWidget::configurerForRobotModel(RobotModelInterface &robotModel)
{
	/// @todo: What if robot model has no configurable sensors?
	QWidget *result = new QWidget;
	QPalette palette = result->palette();
	palette.setColor(QPalette::Background, Qt::transparent);
	result->setPalette(palette);
	QVBoxLayout * const layout = new QVBoxLayout;
	result->setLayout(layout);
	QList<PortInfo> const configurablePorts = robotModel.configurablePorts();
	for (PortInfo const &port : configurablePorts) {
		layout->addLayout(initPort(robotModel.name(), port, robotModel.allowedDevices(port)));
	}

	return result;
}

QLayout *DevicesConfigurationWidget::initPort(QString const &robotModel
		, PortInfo const &port, QList<DeviceInfo> const &sensors)
{
	QString const labelText = mCompactMode ? tr("%1:") : tr("Port %1:");
	QLabel * const portLabel = new QLabel(labelText.arg(port.name()), this);
	QComboBox * const comboBox = new QComboBox(this);
	comboBox->setProperty("robotModel", robotModel);
	comboBox->setProperty("port", QVariant::fromValue(port));
	mConfigurers << comboBox;
	comboBox->addItem(tr("Unused"), QVariant::fromValue(DeviceInfo()));
	for (DeviceInfo const &sensor : sensors) {
		comboBox->addItem(sensor.friendlyName(), QVariant::fromValue(sensor));
	}

	if (mAutosaveMode) {
		connect(comboBox, &QComboBox::currentTextChanged, this, &DevicesConfigurationWidget::save);
	}

	QHBoxLayout * const layout = new QHBoxLayout;
	layout->addWidget(portLabel);
	layout->addWidget(comboBox);
	layout->setStretch(0, 0);
	layout->setStretch(1, 1);

	return layout;
}


void DevicesConfigurationWidget::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)

	// This method can be called when we did not accomplish processing all combo boxes during saving.
	// So ignoring such case.
	if (!mSaving) {
		refresh();
	}
}


void DevicesConfigurationWidget::refresh()
{
	mRefreshing = true;
	for (QComboBox * const box : mConfigurers) {
		PortInfo const port = box->property("port").value<PortInfo>();
		DeviceInfo const device = currentConfiguration(mCurrentModel, port);
		if (device.isNull()) {
			box->setCurrentIndex(0);
		} else {
			for (int index = 0; index < box->count(); ++index) {
				if (box->itemData(index).value<DeviceInfo>().isA(device)) {
					box->setCurrentIndex(index);
					break;
				}
			}
		}
	}

	mRefreshing = false;
}

void DevicesConfigurationWidget::save()
{
	// Refreshing may affect combobox current index. No saving is needed then.
	if (mRefreshing) {
		return;
	}

	mSaving = true;
	for (QComboBox * const box : mConfigurers) {
		if (!box->isVisible()) {
			continue;
		}

		QString const robotModel = box->property("robotModel").toString();
		PortInfo const port = box->property("port").value<PortInfo>();
		DeviceInfo const device = box->itemData(box->currentIndex()).value<DeviceInfo>();
		if (currentConfiguration(robotModel, port) != device) {
			propagateChanges(port, device);
		}
	}

	mSaving = false;
}

void DevicesConfigurationWidget::propagateChanges(PortInfo const &port, DeviceInfo const &sensor)
{
	for (QString const &robotModelId : mRobotModels.keys()) {
		RobotModelInterface const *robotModel = mRobotModels[robotModelId];
		for (PortInfo const &otherPort : robotModel->configurablePorts()) {
			if (areConvertible(port, otherPort)) {
				if (sensor.isNull()) {
					deviceConfigurationChanged(robotModelId, otherPort, DeviceInfo());
				} else {
					DeviceInfo const otherDevice = convertibleDevice(robotModel, otherPort, sensor);
					if (!otherDevice.isNull()) {
						deviceConfigurationChanged(robotModelId, otherPort, otherDevice);
					}
				}
			}
		}
	}
}

bool DevicesConfigurationWidget::areConvertible(PortInfo const &port1, PortInfo const &port2) const
{
	return (port1.name() == port2.name()
			|| port1.nameAliases().contains(port2.name())
			|| port2.nameAliases().contains(port1.name()))
			&& port1.direction() == port2.direction();
}

DeviceInfo DevicesConfigurationWidget::convertibleDevice(RobotModelInterface const *robotModel
		, PortInfo const &port, DeviceInfo const &device) const
{
	QList<DeviceInfo> const convertibleBases = robotModel->convertibleBases();
	for (DeviceInfo const &allowedDevice : robotModel->allowedDevices(port)) {
		for (DeviceInfo const &convertibleBase : convertibleBases) {
			if (device.isA(convertibleBase) && allowedDevice.isA(convertibleBase)) {
				return allowedDevice;
			}
		}
	}

	return DeviceInfo();
}
