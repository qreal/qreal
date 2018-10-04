/* Copyright 2015-2016 QReal Research Group, CyberTech Labs Ltd.
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

// here just common function definitions and global declarations for other scripts

var mainWindow, ui, utils, keyboard, palette, hints, scene, cursor;

// init() called in this file after common vars declarations and function definitions
function init() {
    ui = api.ui();
    utils = api.utils();
    keyboard = api.keyboard();
    palette = api.palette();
    hints = api.hints();
    scene = api.scene();
    cursor = api.cursor();
    mainWindow = ui.mainWindow();
    assert(mainWindow != null);
    api.switchMouseCursorToWindow(mainWindow);
    api.wait(200);
}

function findViewPort(parent) {
    return ui.deepViewPort(parent);
}

function checkAction(action, isEnabledAndVisible, isCheckable, isChecked) {
    assert(action != null);
    expect(utils.isEnabledAndVisible(action) == isEnabledAndVisible);
    expect(utils.actionIsCheckable(action) == isCheckable);
    expect(utils.actionIsChecked(action) == isChecked);
}

function leftClick(widget) {
    cursor.moveTo(widget);
    cursor.leftButtonPress(widget);
    cursor.leftButtonRelease(widget);
}

function rightClick(widget) {
    cursor.moveTo(widget);
    cursor.rightButtonPress(widget);
    cursor.rightButtonRelease(widget);
}

function fillInputWidgetLater(dialogName, lineEditObjectName, text, msec) {
    invokeLater(utils, "fillInputWidget", dialogName, lineEditObjectName, text, msec);
}

function clickButtonLater(dialogName, text, msec) {
    invokeLater(utils, "clickButton", dialogName, text, msec);
}

function chooseComboBoxItemLater(dialogName, comboBoxObjectName, itemName, msec) {
    invokeLater(utils, "chooseComboBoxItem", dialogName, comboBoxObjectName, itemName, msec);
}

function activateContextMenuActionLater(actionName, msec) {
    invokeLater(utils, "activateContextMenuAction", actionName, msec);
}

function quitWithoutSave() {
    var menuFile = ui.findMenu("menu_File");
    assert(menuFile != null);
    utils.activateMenu(menuFile);
    api.wait(200);
    var actionQuit = ui.findActionInMenu(menuFile, "actionQuit");
    checkAction(actionQuit, true, false, false);
    assert(!ui.isSubMenuInMenu(menuFile, actionQuit));
    clickButtonLater("Save", "Discard", 1500);
    utils.activateMenuAction(menuFile, actionQuit);
}

// function quit() // just quit action knowing that save window dialog would not called
// function quitWithSave(fileName)
// function togglePanelsAction(actionName);

//----Start main evaluating!----//
init();
