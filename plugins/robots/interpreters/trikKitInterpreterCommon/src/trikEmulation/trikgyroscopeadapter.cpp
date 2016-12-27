#include <trikKitInterpreterCommon/trikEmulation/trikgyroscopeadapter.h>

#include <twoDModel/engine/twoDModelEngineInterface.h>

#include <cmath>


template <typename T>
static T getPitch(const QQuaternion &q)
{
	return std::atan2(2 * q.y()*q.z() + 2 * q.scalar() * q.x()
			, 1 - 2 * q.x() * q.x() - 2 * q.y() * q.y());
}

template <typename T>
static T getRoll(const QQuaternion &q)
{
	return std::asin(2 * q.scalar() * q.y() - 2 * q.x() * q.y());
}

template <typename T>
static T getYaw(const QQuaternion &q)
{
	return std::atan2(2 * q.x() * q.y() + 2 * q.scalar() * q.z()
			, 1 - 2 * q.y() * q.y() - 2 * q.z() * q.z());
}

static int getTimeVal(trik::robotModel::twoD::TrikTwoDRobotModel * const model) {
	return static_cast<int>(model->timeline().timestamp() * 1000 / 256);
}


TrikGyroscopeAdapter::TrikGyroscopeAdapter(kitBase::robotModel::robotParts::GyroscopeSensor *gyro
		, const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model)
	: mGyro(gyro), mModel(model), mResult({0, 0, 0, getTimeVal(model.data()), 0, 0, 0})
{
	connect(gyro, SIGNAL(newData(QVector<int>)), this, SLOT(countTilt(QVector<int>)));
}


QVector<int> TrikGyroscopeAdapter::read() const
{
	return mResult;

	// rework properly later, when it'll return value without gyrscope COnstant "-1600"
	// replace with QMetaObject::invokeMethod call?
}

void TrikGyroscopeAdapter::calibrate(int msec)
{
	Q_UNUSED(msec); // Don't wait for now. 2D model calibrates immedeatly with zero bias (at least I hope so)
}

bool TrikGyroscopeAdapter::isCalibrated() const
{
	return true;
}

QVector<int> TrikGyroscopeAdapter::readRawData() const
{
	QVector<int> res = mModel->engine()->readGyroscopeSensor();
	res.append(getTimeVal(mModel.data())); // 256 because in trikRuntime timeVal granularity is 256 microsec
	return res;
}

void TrikGyroscopeAdapter::countTilt(QVector<int> oldFormat)
{
	constexpr int gyroConstant = -1600 / 100;
	constexpr auto pi = 3.14159265358979323846;
	constexpr auto RAD_TO_MDEG = 1000 * 180 / pi;
	//QVector<int> oldFormat = mModel->engine()->readGyroscopeSensor();
	int timeStamp = getTimeVal(mModel.data());
	//qDebug() << mModel->timeline().timestamp() << ", " << timeStamp;

	static bool timeInited = false;
	if (!timeInited) {
		timeInited = true;
		mLastUpdate = timeStamp;
	} else {

		mResult[0] = oldFormat[0] * 1000 / gyroConstant;
		mResult[1] = oldFormat[1] * 1000 / gyroConstant;
		mResult[2] = oldFormat[2] * 1000 / gyroConstant;
		mResult[3] = timeStamp;

		constexpr auto deltaConst = pi / 180 / 1000 / 1000000;
		const auto dt = (timeStamp - mLastUpdate) * 256 * deltaConst;

		const auto x = mResult[0] * dt;
		const auto y = mResult[1] * dt;
		const auto z = mResult[2] * dt;

		const auto c1 = std::cos(x / 2);
		const auto s1 = std::sin(x / 2);
		const auto c2 = std::cos(y / 2);
		const auto s2 = std::sin(y / 2);
		const auto c3 = std::cos(z / 2);
		const auto s3 = std::sin(z / 2);

		QQuaternion deltaQ;
		deltaQ.setScalar(c1 * c2 * c3 + s1 * s2 * s3);
		deltaQ.setX(s1 * c2 * c3 - c1 * s2 * s3);
		deltaQ.setY(c1 * s2 * c3 + s1 * c2 * s3);
		deltaQ.setZ(c1 * c2 * s3 - s1 * s2 * c3);

		mQ *= deltaQ;
		mQ.normalize();

		mLastUpdate = timeStamp;

		mResult[4] = getPitch<float>(mQ) * RAD_TO_MDEG;
		mResult[5] = getRoll<float>(mQ) * RAD_TO_MDEG; // probably not needed
		mResult[6] = getYaw<float>(mQ) * RAD_TO_MDEG;
		//emit newData(mResult, t);
	}
}

