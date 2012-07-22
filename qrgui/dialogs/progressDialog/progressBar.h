#pragma once

#include <QtGui/QProgressBar>

#include "../../../qrutils/invocationUtils/longOperation.h"

namespace qReal
{

/// @brief Progress dialog capable with invocation utils
class ProgressBar : public QProgressBar
{
	Q_OBJECT

public:
	explicit ProgressBar(QWidget *parent = 0);

	/// Returns last connected operation
	invocation::LongOperation *operation() const;
	/// Returns if some running operation connected at the moment
	bool isOperationConnected() const;

	/// Binds current progress bar with some operation from
	/// invocation utils. If operation provides progress info
	/// progress bar shows that progress. Else progress bar
	/// just shows that something is invocing at the moment
	void connectOperation(invocation::LongOperation *operation);
	/// Reinits progress bar
	void reset();

private slots:
	void onBeforeStart();
	void onOperationComplete(invocation::InvocationState result);

private:
	void connectOperation();
	void disconnectOperation();

	void onOperationFinishedNormally();
	void onOperationCanceled();

	invocation::LongOperation* mOperation;
};

}
