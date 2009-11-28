#pragma once

namespace utils {

	class OutFile
	{
	public:
		explicit OutFile(QString const &fileName)
				: mFile(fileName)
		{
			mFile.open(QIODevice::WriteOnly | QIODevice::Text);
			mOut.setDevice(&mFile);
		}

		~OutFile()
		{
			mFile.close();
		}

		QTextStream& operator()()
		{
			return mOut;
		}
	private:
		QTextStream mOut;
		QFile mFile;
	};

}
