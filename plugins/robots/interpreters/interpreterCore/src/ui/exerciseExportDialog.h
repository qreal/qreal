#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QScopedPointer>

#include <kitBase/readOnly.h>

class QCheckBox;

namespace interpreterCore {
namespace ui {

class ExerciseExportDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	explicit ExerciseExportDialog(
			QWidget *parent = nullptr
			);

	~ExerciseExportDialog() override;

	kitBase::ReadOnlyFlags readOnlyFlags() const;

private:
	QScopedPointer<QCheckBox> mWorldReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mSensorsReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mRobotPositionReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mRobotSetupReadOnlyCheckBox;
	QScopedPointer<QCheckBox> mSimulationSettingsReadOnlyCheckBox;
};

}
}
