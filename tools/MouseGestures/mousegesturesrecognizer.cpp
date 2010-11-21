//#include "mousegesturesrecognizer.h"

//MouseGesturesRecognizer::MouseGesturesRecognizer(EntityVector const & idealGestures)
//{
//	foreach(Entity gestureObject, idealGestures)
//	{
//		PathVector gesture = gestureObject.second;
//		PointVector path = createPath(gesture);
//		TKey idealKey  = getKey(path);
//		IdealGesture idealGesture(gestureObject.first, idealKey);
//		this->mIdealKeys.push_back(idealGesture);
//	}
//}

//MouseGesturesRecognizer::getObject(const QList<QPoint> &path)
//{
//	float min = mMaxKeyDistance;
//	TKey key = getKey(path);
//	QString object;
//	if (key.isEmpty())
//		return object;
//	foreach (IdealGesture item, this->mIdealKeys)
//	{
//		if (!item.second.isEmpty())
//		{
//			float distance = getDistance(item.second, key);
//			if (distance < min && distance < mMaxKeyDistance)
//			{
//				min = distance;
//				object = item.first;
//			}
//		}
//	}
//	return object;
//}
