#pragma once

#include <QtWidgets/QDialog>

namespace utils {

/// A base class for all system dilogs. Provides such tools like automatic
/// serialization of dialog`s position and size
class QRealDialog : public QDialog
{
	Q_OBJECT

public:
	/// @param id The name of this dialog. Used for storing dialog`s parameters
	///in settings so should be unique.
	explicit QRealDialog(QString const &id, QWidget *parent = 0);

protected:
	virtual void showEvent(QShowEvent *);
	virtual void closeEvent(QCloseEvent *);

	void serializeParameters();
	void deserializeParameters();

	/// Returns a key string used for storing maximized parameter in registry
	virtual QString maximizedKey() const;

	/// Returns a key string used for storing position parameter in registry
	virtual QString positionKey() const;

	/// Returns a key string used for storing size parameter in registry
	virtual QString sizeKey() const;

private:
	QString const mId;
};

}
