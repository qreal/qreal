#pragma once

#include <QtWidgets/QProgressDialog>

#include "dialogs/progressDialog/progressBar.h"

namespace qReal
{

/// @brief Progress dialog capable with invocation utils. Uses @see ProgressBar
class ProgressDialog : public QProgressDialog
{
	Q_OBJECT

public:
	explicit ProgressDialog(QWidget *parent = 0);

	/// Returns last connected operation
	invocation::LongOperation *operation() const;
	/// Returns if some running operation connected at the moment
	bool isOperationConnected() const;

	/// Connects specified operation to @see ProgressBar instance.
	/// NOTE: when operation started blocks gui thread with
	/// QDialog::exec() call
	void connectOperation(invocation::LongOperation *operation);

private slots:
	void onCanceled();

private:
	ProgressBar *mProgressBar;
};

}
