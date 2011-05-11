#pragma once

#include <QDialog>
#include <QLabel>
#include <QGridLayout>

namespace qReal
{
	namespace gui
	{
		class ExecutionIndicator : public QDialog
		{
			Q_OBJECT

		public:
			ExecutionIndicator(QWidget *parent, QString const &message);

		private:
			QLabel *mLabel;
			QGridLayout *mLayout;
		};
	}
}
