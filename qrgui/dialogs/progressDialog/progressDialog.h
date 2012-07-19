#pragma once

#include "progressBar.h"

#include <QtGui/QProgressDialog>

namespace qReal
{

/// @brief Progress dialog capable with invokation utils. Uses @see ProgressBar
class ProgressDialog : public QProgressDialog
{
	Q_OBJECT

public:
	explicit ProgressDialog(QWidget *parent = 0);

	/// Returns last connected operation
	invokation::LongOperation *operation() const;
	/// Returns if some running operation connected at the moment
	bool isOperationConnected() const;

	/// Connects specified operation to @see ProgressBar instance.
	/// NOTE: when operation started blocks gui thread with
	/// QDialog::exec() call
	void connectOperation(invokation::LongOperation *operation);

private slots:
	void onCanceled();

private:
	ProgressBar *mProgressBar;
};

}
