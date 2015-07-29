/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "methodsTesterForQrxcAndInterpreter.h"
#include "convertingMethods.h"
#include "defs.h"
#include "qrkernel/exception/exception.h"
#include <QElapsedTimer>
#include <iostream>
#include <QtCore/QDebug>
#include <cstdint>
#include <QtCore/qmath.h>

using namespace std;

using namespace editorPluginTestingFramework;
using namespace qReal;

MethodsTesterForQrxcAndInterpreter::MethodsTesterForQrxcAndInterpreter(
		EditorManager* qrxcGeneratedPlugin
		, InterpreterEditorManager* interpreterGeneratedPlugin
		)
{
	mQrxcGeneratedPlugin = qrxcGeneratedPlugin;
	mInterpreterGeneratedPlugin = interpreterGeneratedPlugin;
}

class MethodsTesterForQrxcAndInterpreter::StringGenerator : public AbstractStringGenerator
{
public:
	void init(EditorManagerInterface *editorManagerInterface)
	{
		mEditorManagerInterface = editorManagerInterface;
	}

	virtual QString generateString() const
	{
		QString resultStr = "";
		for (const QString &elementOfList : generateList(mEditorManagerInterface)) {
			resultStr += elementOfList + " ";
		}

		return resultStr;
	}

	virtual QString generateStringTime()
	{
		QString resultStr = "";
		resultStr = mListOfTime.join(' ');
		return resultStr;
	}

	virtual QPair<qint64, double> callFunction(std::function<void()> functionToCall) const
	{
		QElapsedTimer timer;
		qint64 array[20];
		qint64 time = 0;
		qint64 expectedValue = 0;
		qint64 variance = 0;

		for (int i = 0; i < 20; ++i) {
			timer.start();
			for (int i = 0; i < 20; ++i) {
				functionToCall();
			}

			array[i] = timer.nsecsElapsed();
			time += array[i];
		}

		expectedValue = time / 20;

		for (int t = 0; t < 20; ++t) {
			variance += qPow((array[t] - expectedValue), 2);
		}

		variance = variance / 20;
		double sqrtVariance = sqrt(variance);

		QPair<qint64, double> result = qMakePair(expectedValue, sqrtVariance);
		return result;
	}

protected:
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const = 0;

	virtual QPair<qint64, double> dataOfTime() const = 0;

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId = Id::rootId()
			, const Id &diagramId = Id::rootId()
			, const Id &elementId = Id::rootId()
			, const QString &propertyName = ""
			) const = 0;

	QStringList mutable mListOfTime;

private:
	EditorManagerInterface *mEditorManagerInterface;
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;

		for (const Id &editor: editorManagerInterface->editors()) {
			const QString &additionalString = ConvertingMethods::transformateOutput(
					callMethod(editorManagerInterface, editor), editor);
			const QPair<qint64, double> additional = dataOfTime();
			mListOfTime.append(QString::number(additional.first));
			mListOfTime.append(" ");
			mListOfTime.append(QString::number(additional.second));
			mListOfTime.append("|");
			resultList.append(additionalString);
			resultList.append("|");
		}
		return resultList;
	}
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;
		for (const Id &editor : editorManagerInterface->editors()) {
			for (const Id &diagram : editorManagerInterface->diagrams(editor)) {
				const QString additionalString = ConvertingMethods::transformateOutput(
						callMethod(editorManagerInterface, editor, diagram), diagram);
				const QPair<qint64, double> additional = dataOfTime();
				mListOfTime.append(QString::number(additional.first));
				mListOfTime.append(" ");
				mListOfTime.append(QString::number(additional.second));
				mListOfTime.append("|");
				resultList.append(additionalString);
				resultList.append("|");
			}
		}

		return resultList;
	}
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;
		for (const Id &editor : editorManagerInterface->editors()) {
			for (const Id &diagram : editorManagerInterface->diagrams(editor)) {
				for (const Id &element : editorManagerInterface->elements(diagram)) {
					const QString additionalString = ConvertingMethods::transformateOutput(
							callMethod(editorManagerInterface, editor, diagram, element), element);
					const QPair<qint64, double> additional = dataOfTime();
					mListOfTime.append(QString::number(additional.first));
					mListOfTime.append(" ");
					mListOfTime.append(QString::number(additional.second));
					mListOfTime.append("|");
					resultList.append(additionalString);
					resultList.append("|");
				}
			}
		}

		return resultList;
	}
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
		: public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const
	{
		QStringList resultList;
		for (const Id &editor : editorManagerInterface->editors()) {
			for (const Id &diagram : editorManagerInterface->diagrams(editor)) {
				for (const Id &element : editorManagerInterface->elements(diagram)) {
					for (const QString &property : editorManagerInterface->propertyNames(element)) {
						const QString additionalString = ConvertingMethods::transformateOutput(
								callMethod(editorManagerInterface, editor, diagram, element, property)
								, Id::rootId(), property + "(" + element.element() + ")");
						const QPair<qint64, double> additional = dataOfTime();
						mListOfTime.append(QString::number(additional.first));
						mListOfTime.append(" ");
						mListOfTime.append(QString::number(additional.second));
						mListOfTime.append("|");
						resultList.append(additionalString);
						resultList.append("|");
					}
				}
			}
		}

		return resultList;
	}
};

class MethodsTesterForQrxcAndInterpreter::StringGeneratorForGroups : public MethodsTesterForQrxcAndInterpreter::StringGenerator
{
	virtual QStringList generateList(EditorManagerInterface *editorManagerInterface) const {
		QStringList resultList;
		for (const Id &editor : editorManagerInterface->editors()) {
			for (const Id &diagram : editorManagerInterface->diagrams(editor)) {
				for (const QString &group : editorManagerInterface->paletteGroups(editor, diagram)) {
					const QString additionalString = ConvertingMethods::transformateOutput(
							callMethod(editorManagerInterface, editor, diagram, diagram, group), Id::rootId(), group);
					const QPair<qint64, double> additional = dataOfTime();
					mListOfTime.append(QString::number(additional.first));
					mListOfTime.append(" ");
					mListOfTime.append(QString::number(additional.second));
					mListOfTime.append("|");
					resultList.append(additionalString);
					resultList.append("|");
				}
			}
		}

		return resultList;
	}
};

class MethodsTesterForQrxcAndInterpreter::EditorsListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
{
	virtual QString methodName() const
	{
		return "Editors";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		Q_UNUSED(editorManagerInterface);

		mResult = callFunction([editorId]() { return editorId; });
		return ConvertingMethods::convertIdIntoStringList(editorId);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EditorsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::DiagramsListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
{
	virtual QString methodName() const
	{
		return "Diagrams";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, editorId]()
				{return editorManagerInterface->diagrams(editorId); });

		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->diagrams(editorId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::ElementsListGeneratorWithIdParameter
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Elements(const Id &diagram)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, diagramId]()
				{ return editorManagerInterface->elements(diagramId); });

		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->elements(diagramId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsListGeneratorWithIdParameter();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:

	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::ElementsListGeneratorWithQStringParameters
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Elements(const QString &editor, const QString &diagram)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		const QString &editorName = editorId.editor();
		const QString &diagramName = diagramId.diagram();

		mResult = callFunction([editorManagerInterface, editorName, diagramName]()
				{ return editorManagerInterface->elements(editorName, diagramName); });

		return editorManagerInterface->elements(editorName, diagramName);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ElementsListGeneratorWithQStringParameters();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::MouseGesturesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Mouse gestures";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->mouseGesture(elementId); });

		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->mouseGesture(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new MouseGesturesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::IsParentOfGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is Parent Of";
	}

	virtual void callIsParent(EditorManagerInterface *editorManagerInterface, const Id &editorId, const Id &diagramId
			, const Id &elementId) const
	{
		for (const Id &parentDiagram : editorManagerInterface->diagrams(editorId)) {
			for (const Id &parentElement : editorManagerInterface->elements(diagramId)) {
				editorManagerInterface->isParentOf(elementId, parentElement);
			}
		}
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(propertyName);
		mResult = callFunction([this, editorManagerInterface, editorId, diagramId, elementId]()
				{ return callIsParent(editorManagerInterface, editorId, diagramId, elementId); });

		QStringList result;
		for (const Id &parentDiagram : editorManagerInterface->diagrams(editorId)) {
			for (const Id &parentElement : editorManagerInterface->elements(diagramId)) {
				bool isParent = editorManagerInterface->isParentOf(elementId, parentElement);
				if (isParent) {
					result << parentElement.toString();
					result <<  " is parent of ";
					result << elementId.toString();
				}
			}
		}

		return result;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsParentOfGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};


class MethodsTesterForQrxcAndInterpreter::FriendlyNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Friendly name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->friendlyName(elementId); });
		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->friendlyName(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new FriendlyNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::DescriptionListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Description";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->description(elementId); });

		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->description(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DescriptionListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PropertyDescriptionListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property description";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);

		mResult = callFunction([editorManagerInterface, elementId, propertyName]()
				{ return editorManagerInterface->propertyDescription(elementId, propertyName); });

		QStringList resultList;
		try {
			resultList = ConvertingMethods::convertStringIntoStringList(
					editorManagerInterface->propertyDescription(elementId, propertyName));
		} catch (Exception e) {
			resultList.append("method failed");
		}
		return resultList;
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDescriptionListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PropertyDisplayedNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property displayed name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		mResult = callFunction([editorManagerInterface, elementId, propertyName]()
				{ return editorManagerInterface->propertyDisplayedName(elementId, propertyName); });

		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->propertyDisplayedName(elementId,propertyName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyDisplayedNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::ContainedTypesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Contained types";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->containedTypes(elementId); });

		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->containedTypes(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ContainedTypesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::ExplosionsListGenerator
: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Explosions";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->explosions(elementId); });

		return ConvertingMethods::convertExplosionListIntoStringList(editorManagerInterface->explosions(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ExplosionsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::EnumValuesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Enum values";
	}

	virtual void callEnum(EditorManagerInterface *editorManagerInterface, const Id &elementId) const
	{
		const QStringList nameList =  editorManagerInterface->propertyNames(elementId);
		for (const QString &name : nameList) {
			editorManagerInterface->enumValues(elementId, name);
		}
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([this, editorManagerInterface, elementId]()
				{ return callEnum(editorManagerInterface, elementId); });

		const QStringList nameList =  editorManagerInterface->propertyNames(elementId);

		QList<QPair<QString, QString>> result;
		for (const QString &name : nameList) {
			result.append(editorManagerInterface->enumValues(elementId, name));
		}

		return ConvertingMethods::convertingQPairListIntoStringList(result);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new EnumValuesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::TypeNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Type name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		const QString &name = elementId.element();
		mResult = callFunction([editorManagerInterface, elementId, name]()
				{ return editorManagerInterface->typeName(elementId, name); });

		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->typeName(elementId, name));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new TypeNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::ReferencePropertiesGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Reference Properties";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->referenceProperties(elementId); });

		return (editorManagerInterface->referenceProperties(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ReferencePropertiesGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::AllChildrenTypesOfListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "All children types of";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ editorManagerInterface->allChildrenTypesOf(elementId); });

		return (editorManagerInterface->allChildrenTypesOf(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new AllChildrenTypesOfListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::IsEditorListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForEditors
{
	virtual QString methodName() const
	{
		return "Is editor (for editors)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, editorId]()
				{ return editorManagerInterface->isEditor(editorId); });

		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isEditor(editorId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsEditorListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::IsDiagramListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const {
		return "Is diagram (for diagrams)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, diagramId]()
				{ return editorManagerInterface->isDiagram(diagramId); });

		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isDiagram(diagramId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsDiagramListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::IsElementListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is element (for elements)";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->isElement(elementId); });

		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->isElement(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsElementListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PropertyNamesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Property names";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->propertyNames(elementId); });

		return (editorManagerInterface->propertyNames(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertyNamesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PortTypesListGenerator
: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Port types";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->portTypes(elementId); });

		return editorManagerInterface->portTypes(elementId);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PortTypesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::DefaultPropertyValuesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Property default value";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);

		mResult = callFunction([editorManagerInterface, elementId, propertyName]()
				{ return editorManagerInterface->defaultPropertyValue(elementId, propertyName); });

		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->defaultPropertyValue(elementId, propertyName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DefaultPropertyValuesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PropertiesWithDefaultValuesListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Properties with default values";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->propertiesWithDefaultValues(elementId); });

		return (editorManagerInterface->propertiesWithDefaultValues(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PropertiesWithDefaultValuesListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::HasElementListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Has element";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);

		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->hasElement(elementId); });

		return ConvertingMethods::convertBoolIntoStringList(editorManagerInterface->hasElement(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new HasElementListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::FindElementByTypeListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Find element by type";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		const QString &type = elementId.element();
		mResult = callFunction([editorManagerInterface, type]()
				{ return editorManagerInterface->findElementByType(type); });

		return ConvertingMethods::convertIdIntoStringList(editorManagerInterface->findElementByType(type));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new FindElementByTypeListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::IsGraphicalElementListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is graphical element";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->isGraphicalElementNode(elementId); });

		return ConvertingMethods::convertBoolIntoStringList(
				editorManagerInterface->isGraphicalElementNode(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsGraphicalElementListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::IsNodeOrEdgeListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Is node or edge";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		const QString &editorName = editorId.editor();
		const QString &elementName = elementId.element();
		mResult = callFunction([editorManagerInterface, editorName, elementName]()
				{ return editorManagerInterface->isNodeOrEdge(editorName, elementName); });

		return ConvertingMethods::convertIntIntoStringList(
				editorManagerInterface->isNodeOrEdge(editorName, elementName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsNodeOrEdgeListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::DiagramNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		const QString &editorName = editorId.editor();
		const QString &diagramName = diagramId.diagram();
		mResult = callFunction([editorManagerInterface, editorName, diagramName]()
				{ return editorManagerInterface->diagramName(editorName, diagramName); });

		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->diagramName(editorName, diagramName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::DiagramNodeNameListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Diagram node name";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(elementId);
		Q_UNUSED(propertyName);
		const QString &editorName = editorId.editor();
		const QString &diagramName = diagramId.diagram();

		mResult = callFunction([editorManagerInterface, editorName, diagramName]()
				{ return editorManagerInterface->diagramNodeName(editorName, diagramName); });

		return ConvertingMethods::convertStringIntoStringList(
				editorManagerInterface->diagramNodeName(editorName, diagramName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new DiagramNodeNameListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::IsParentPropertyListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForProperties
{
	virtual QString methodName() const
	{
		return "Is parent property";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		mResult = callFunction([editorManagerInterface, elementId, propertyName]()
				{ return editorManagerInterface->isParentProperty(elementId, propertyName); });

		return ConvertingMethods::convertBoolIntoStringList(
				editorManagerInterface->isParentProperty(elementId, propertyName));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new IsParentPropertyListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::ChildrenListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Children";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->children(elementId); });

		return ConvertingMethods::convertIdListIntoStringList(editorManagerInterface->children(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ChildrenListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::ShapeListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForElements
{
	virtual QString methodName() const
	{
		return "Shape";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(editorId);
		Q_UNUSED(diagramId);
		Q_UNUSED(propertyName);
		mResult = callFunction([editorManagerInterface, elementId]()
				{ return editorManagerInterface->shape(elementId); });

		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->shape(elementId));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new ShapeListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PaletteGroupsGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForDiagrams
{
	virtual QString methodName() const
	{
		return "Palette Groups";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &propertyName
			) const
	{
		Q_UNUSED(propertyName);
		Q_UNUSED(elementId);
		mResult = callFunction([editorManagerInterface, editorId, diagramId]()
				{ return editorManagerInterface->paletteGroups(editorId, diagramId); });

		return editorManagerInterface->paletteGroups(editorId, diagramId);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupsGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PaletteGroupsListGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Palette Groups List";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &group
			) const
	{
		Q_UNUSED(elementId);

		mResult = callFunction([editorManagerInterface, editorId, diagramId, group]()
				{ return editorManagerInterface->paletteGroupList(editorId, diagramId, group); });

		return editorManagerInterface->paletteGroupList(editorId, diagramId, group);
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupsListGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

class MethodsTesterForQrxcAndInterpreter::PaletteGroupDescriptionGenerator
		: public MethodsTesterForQrxcAndInterpreter::StringGeneratorForGroups
{
	virtual QString methodName() const
	{
		return "Palette Groups Description";
	}

	virtual QStringList callMethod(
			EditorManagerInterface *editorManagerInterface
			, const Id &editorId
			, const Id &diagramId
			, const Id &elementId
			, const QString &group
			) const
	{
		Q_UNUSED(elementId);

		mResult = callFunction([editorManagerInterface, editorId, diagramId, group]()
				{ return editorManagerInterface->paletteGroupDescription(editorId, diagramId, group); });

		return ConvertingMethods::convertStringIntoStringList(editorManagerInterface->paletteGroupDescription(editorId, diagramId, group));
	}

	virtual AbstractStringGenerator* clone() const
	{
		AbstractStringGenerator* clonedGenerator = new PaletteGroupDescriptionGenerator();
		return clonedGenerator;
	}

	virtual QPair<qint64, double> dataOfTime() const
	{
		return mResult;
	}

private:
	mutable QPair<qint64, double> mResult = qMakePair(0, 0.0);
};

void MethodsTesterForQrxcAndInterpreter::testMethods()
{
	fillMethodsToTestList(pathToQrxcInterpreterMethodsToTest);
	mGeneratedList.append(testMethodIfExistsInList(DiagramsListGenerator(), "diagrams"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsListGeneratorWithIdParameter(), "elements(Id)"));
	mGeneratedList.append(testMethodIfExistsInList(ElementsListGeneratorWithQStringParameters()
			, "elements(QString, QString)"));
	mGeneratedList.append(testMethodIfExistsInList(MouseGesturesListGenerator(), "mouseGestures"));
	mGeneratedList.append(testMethodIfExistsInList(FriendlyNameListGenerator(), "friendlyName"));
	mGeneratedList.append(testMethodIfExistsInList(DescriptionListGenerator(), "description"));
	mGeneratedList.append(testMethodIfExistsInList(PropertyDisplayedNameListGenerator(), "propertyDisplayedName"));

	mGeneratedList.append(testMethodIfExistsInList(EnumValuesListGenerator(), "enumValues"));
	mGeneratedList.append(testMethodIfExistsInList(PropertyNamesListGenerator(), "propertyNames"));
	mGeneratedList.append(testMethodIfExistsInList(PortTypesListGenerator(), "portTypes"));
	mGeneratedList.append(testMethodIfExistsInList(DefaultPropertyValuesListGenerator(), "defaultPropertyValues"));
	mGeneratedList.append(testMethodIfExistsInList(PropertiesWithDefaultValuesListGenerator()
			, "propertiesWithDefaultValues"));
	mGeneratedList.append(testMethodIfExistsInList(IsNodeOrEdgeListGenerator(), "isNodeOrEdge"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNameListGenerator(), "diagramName"));
	mGeneratedList.append(testMethodIfExistsInList(DiagramNodeNameListGenerator(), "diagramNodeName"));
	mGeneratedList.append(testMethodIfExistsInList(IsParentOfGenerator(), "isParentOf"));
}

QList<MethodsTester::ResultOfGenerating> MethodsTesterForQrxcAndInterpreter::generatedResult()
{
	testMethods();
	return mGeneratedList;
}

AbstractStringGenerator * MethodsTesterForQrxcAndInterpreter::initGeneratorWithFirstInterface(
		const AbstractStringGenerator &generator) const
{
	StringGenerator *clonedGenerator = dynamic_cast<StringGenerator *>(generator.clone());

	clonedGenerator->init(mQrxcGeneratedPlugin);
	return clonedGenerator;
}

AbstractStringGenerator * MethodsTesterForQrxcAndInterpreter::initGeneratorWithSecondInterface(
		const AbstractStringGenerator &generator) const
{
	StringGenerator *clonedGenerator = dynamic_cast<StringGenerator *>(generator.clone());

	clonedGenerator->init(mInterpreterGeneratedPlugin);
	return clonedGenerator;
}
