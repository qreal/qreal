#pragma once

#include <QtGui/QProgressBar>

#include "../../../qrutils/invokationUtils/longOperation.h"

namespace qReal
{

/// @brief Progress dialog capable with invokation utils
class ProgressBar : public QProgressBar
{
	Q_OBJECT

public:
	explicit ProgressBar(QWidget *parent = 0);

	/// Returns last connected operation
	invokation::LongOperation *operation() const;
	/// Returns if some running operation connected at the moment
	bool isOperationConnected() const;

	/// Binds current progress bar with some operation from
	/// invokation utils. If operation provides progress info
	/// progress bar shows that progress. Else progress bar
	/// just shows that something is invoking at the moment
	void connectOperation(invokation::LongOperation *operation);
	/// Reinits progress bar
	void reset();

private slots:
	void onBeforeStart();
	void onOperationComplete(invokation::InvokationState result);

private:
	void connectOperation();
	void disconnectOperation();

	void onOperationFinishedNormally();
	void onOperationCanceled();

	invokation::LongOperation* mOperation;
};

}
