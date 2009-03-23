#pragma once

#include <QString>

namespace qReal {

	class Exception
	{
	public:
		explicit Exception(QString const &message) : mMessage(message) {};
		QString message();

	private:
		const QString mMessage;
	};

}