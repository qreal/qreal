#pragma once

#include <QString>

#include "../kernelDeclSpec.h"

namespace qReal {

	class QRKERNEL_EXPORT Exception
	{
	public:
		explicit Exception(QString const &message);
		QString message() const;

	private:
		const QString mMessage;
	};

}
