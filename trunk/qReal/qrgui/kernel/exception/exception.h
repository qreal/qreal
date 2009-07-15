#pragma once

#include <QString>

namespace qReal {

	class Exception
	{
	public:
		explicit Exception(QString const &message);
		QString message() const;

	private:
		const QString mMessage;
	};

}
