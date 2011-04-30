#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

namespace qReal
{
	namespace gui
	{
		class ExecutionIndicator : public QWidget
		{
			Q_OBJECT

		public:
			ExecutionIndicator(QString const &message);

		private:
			QLabel *mLabel;
			QGridLayout *mLayout;
		};
	}
}
