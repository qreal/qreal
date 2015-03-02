#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QScopedPointer>

#include <interpreterBase/readOnly.h>

class QCheckBox;

namespace interpreterCore {
namespace ui {

class SaveAsTaskDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	explicit SaveAsTaskDialog(
			QWidget *parent = nullptr
			);

	~SaveAsTaskDialog() override;

	interpreterBase::ReadOnlyFlags readOnlyFlags() const;

private:
	QScopedPointer<QCheckBox> mWorldReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mSensorsReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mRobotPositionReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mRobotConfigurationReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mSimulationSettingsReadOnlyCheckBox;
};

}
}
