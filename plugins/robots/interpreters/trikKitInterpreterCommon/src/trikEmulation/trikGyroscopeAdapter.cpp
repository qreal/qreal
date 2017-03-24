#include <QtCore/QtMath>

#include <trikKitInterpreterCommon/trikEmulation/trikGyroscopeAdapter.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>
#include <twoDModel/engine/model/constants.h>


template <typename T>
static T getPitch(const QQuaternion &q)
{
	return qAtan2(2 * q.y()*q.z() + 2 * q.scalar() * q.x()
			, 1 - 2 * q.x() * q.x() - 2 * q.y() * q.y());
}

template <typename T>
static T getRoll(const QQuaternion &q)
{
	return qAsin(2 * q.scalar() * q.y() - 2 * q.x() * q.z());
}

template <typename T>
static T getYaw(const QQuaternion &q)
{
	return qAtan2(2 * q.x() * q.y() + 2 * q.scalar() * q.z()
			, 1 - 2 * q.y() * q.y() - 2 * q.z() * q.z());
}

static quint64 getTimeValue(trik::robotModel::twoD::TrikTwoDRobotModel * const model) {
	return model->timeline().timestamp();
}


TrikGyroscopeAdapter::TrikGyroscopeAdapter(kitBase::robotModel::robotParts::GyroscopeSensor *gyro
		, const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model)
	: mTimeInited(false)
	, mGyro(gyro)
	, mModel(model)
	, mResult(7, 0)
	, mQuaternion(1, 0, 0, 0)
	, mStartTime(getTimeValue(model.data()))
	, mLastUpdateTimeStamp(getTimeValue(model.data()))
{
	connect(gyro, SIGNAL(newData(QVector<int>)), this, SLOT(countTilt(QVector<int>)));
}

QVector<int> TrikGyroscopeAdapter::read() const
{
	return mResult;
}

void TrikGyroscopeAdapter::calibrate(int msec)
{
	Q_UNUSED(msec); // Don't wait for now. 2D model calibrates immedeatly with zero bias (at least I hope so)
	mTimeInited = false;
}

bool TrikGyroscopeAdapter::isCalibrated() const
{
	return !mTimeInited;
}

QVector<int> TrikGyroscopeAdapter::readRawData() const
{
	QVector<int> result = mModel->engine()->readGyroscopeSensor();
	result.append(convertToTrikRuntimeTime(getTimeValue(mModel.data())));
	return result;
}

void TrikGyroscopeAdapter::countTilt(const QVector<int> &oldFormat)
{
	const quint64 timeStamp = getTimeValue(mModel.data());

	if (!mTimeInited) {
		mTimeInited = true;
		mLastUpdateTimeStamp = timeStamp;
	} else {
		mResult[0] = oldFormat[0];
		mResult[1] = oldFormat[1];
		mResult[2] = oldFormat[2];
		mResult[3] = convertToTrikRuntimeTime(timeStamp);

		const qreal scale = static_cast<qreal>(timeStamp - mLastUpdateTimeStamp) / twoDModel::timeQuant;
		const qreal x = static_cast<qreal>(mResult[0]) / twoDModel::gyroscopeConstant * scale;
		const qreal y = static_cast<qreal>(mResult[1]) / twoDModel::gyroscopeConstant * scale;
		const qreal z = static_cast<qreal>(mResult[2]) / twoDModel::gyroscopeConstant * scale;

		mLastUpdateTimeStamp = timeStamp;

		const float c1 = static_cast<float>(qCos(x / 2));
		const float s1 = static_cast<float>(qSin(x / 2));
		const float c2 = static_cast<float>(qCos(y / 2));
		const float s2 = static_cast<float>(qSin(y / 2));
		const float c3 = static_cast<float>(qCos(z / 2));
		const float s3 = static_cast<float>(qSin(z / 2));

		QQuaternion deltaQuaternion;
		deltaQuaternion.setScalar(c1 * c2 * c3 + s1 * s2 * s3);
		deltaQuaternion.setX(s1 * c2 * c3 - c1 * s2 * s3);
		deltaQuaternion.setY(c1 * s2 * c3 + s1 * c2 * s3);
		deltaQuaternion.setZ(c1 * c2 * s3 - s1 * s2 * c3);

		mQuaternion *= deltaQuaternion;
		mQuaternion.normalize();

		mResult[4] = static_cast<int>(degreeToMilidegree(qRadiansToDegrees(getPitch<qreal>(mQuaternion))));
		mResult[5] = static_cast<int>(degreeToMilidegree(qRadiansToDegrees(getRoll<qreal>(mQuaternion))));
		mResult[6] = static_cast<int>(degreeToMilidegree(qRadiansToDegrees(getYaw<qreal>(mQuaternion))));
	}
}

qreal TrikGyroscopeAdapter::degreeToMilidegree(qreal value)
{
	return value * 1000;
}

int TrikGyroscopeAdapter::convertToTrikRuntimeTime(quint64 time) const
{
	return static_cast<int>(time * 1000 / 256);
}

