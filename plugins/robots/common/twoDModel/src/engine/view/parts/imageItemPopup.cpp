/* Copyright 2016 Dmitry Mordvinov
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

#include "imageItemPopup.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>

#include <qrutils/widgets/qRealFileDialog.h>
#include <qrutils/graphicsUtils/abstractScene.h>

#include <qrkernel/platformInfo.h>

#include "src/engine/items/imageItem.h"

using namespace twoDModel::view;

ImageItemPopup::ImageItemPopup(graphicsUtils::AbstractScene &scene, QWidget *parent)
	: ItemPopup(scene, parent)
	, mLastIsMemorized(false)
{
	initWidget();
}

ImageItemPopup::~ImageItemPopup()
{
}

bool ImageItemPopup::isMemorized() const
{
	return mLastIsMemorized;
}

QString ImageItemPopup::lastPath() const
{
	return mLastPath;
}

void ImageItemPopup::updateMemorizationToolTip()
{
	mMemorizationPicker->setToolTip(mMemorizationPicker->isChecked()
			? tr("Image will be packed into save file. Warning: this will increase save file size.")
			: tr("Image will not be packed into a save file. Warning: if will use save file on other machine or "
					"rename file this image will disappear from 2D model."));
}

bool ImageItemPopup::suits(QGraphicsItem *item)
{
	return dynamic_cast<items::ImageItem *>(item) != nullptr;
}

bool ImageItemPopup::attachTo(const QList<QGraphicsItem *> &items)
{
	ItemPopup::attachTo(items);

	// Subsequent setting values to editors will cause theese values loss. Saving it here.
	const bool lastIsMemorizedBackup = mLastIsMemorized;
	const QString lastPathBackup = mLastPath;

	blockSignals(true);
	mMemorizationPicker->blockSignals(true);
	mMemorizationPicker->setChecked(dominantPropertyValue("memorize").toBool());

	// Restoring values that really were picked by user.
	mLastIsMemorized = lastIsMemorizedBackup;
	mLastPath = lastPathBackup;

	blockSignals(false);
	mMemorizationPicker->blockSignals(false);

	return true;
}

void ImageItemPopup::initWidget()
{
	QVBoxLayout * const layout = new QVBoxLayout(this);
	layout->addWidget(initMemorizationPicker());
	layout->addWidget(initPathPicker());

	updateDueToLayout();
}

QWidget *ImageItemPopup::initMemorizationPicker()
{
	QCheckBox *box = new QCheckBox(this);
	mMemorizationPicker = box;
	box->setIcon(QIcon(":/icons/2d_save.png"));
	updateMemorizationToolTip();
	box->setFocusPolicy(Qt::NoFocus);
	connect(mMemorizationPicker, &QAbstractButton::toggled, this, &ImageItemPopup::updateMemorizationToolTip);
	connect(mMemorizationPicker, &QAbstractButton::toggled, this, [=](bool memorized){
		setPropertyMassively("memorize", memorized);
		if (mLastIsMemorized != memorized) {
			mLastIsMemorized = memorized;
			emit memorizingChanged(memorized);
		}
	});

	return box;
}

QWidget *ImageItemPopup::initPathPicker()
{
	QPushButton *button = new QPushButton(this);
	button->setFocusPolicy(Qt::NoFocus);
	button->setFlat(true);
	button->setIcon(QIcon(":/icons/2d_open.png"));
	button->setToolTip(tr("Change image..."));
	connect(button, &QPushButton::clicked, this, [=]() {
		// Loads world and robot models simultaneously.
		const QString loadFileName = utils::QRealFileDialog::getOpenFileName("2DSelectImage", mScene.views().first()
				, tr("Select image")
				, qReal::PlatformInfo::invariantSettingsPath("pathToImages") + "/../fields"
				, tr("Graphics (*.*)"));
		if (loadFileName.isEmpty()) {
			return;
		}

		setPropertyMassively("path", loadFileName);
	});

	return button;
}
