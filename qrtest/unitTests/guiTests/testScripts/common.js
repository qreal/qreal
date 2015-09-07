/* Copyright 2015 QReal Research Group
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

// just common function implementations and global declarations for other scripts

var mainWindow; // как лучше писать переменные var init init() как сейчас?

var ui;
var utils;
var keyboard;
var palette;
var hints;
var scene;
var cursor;

// init should be called initially before any script if u wanna use common.js vars and methods
// можно бросать исключения, проверять флагом, если некто не увидел про init(). надо ли?
init = function() {
    ui = api.ui();
    utils = api.utils();
    keyboard = api.keyboard();
    palette = api.palette();
    hints = api.hints();
    scene = api.scene();
    cursor = api.cursor();
    mainWindow = ui.mainWindow();
    assert(mainWindow != null);
    api.switchToWindow(mainWindow);
    api.wait(200);
}

findViewPort = function(parent) {
    return ui.deepViewPort(parent);
}

checkAction = function(action, isEnabledAndVisible, isCheckable, isChecked) {
    assert(action != null);
    expect(utils.isEnabledAndVisible(action) == isEnabledAndVisible);
    expect(utils.actionIsCheckable(action) == isCheckable);
    expect(utils.actionIsChecked(action) == isChecked);
}

leftClick = function(widget) {
    cursor.moveTo(widget);
    cursor.leftButtonPress(widget);
    cursor.leftButtonRelease(widget);
}

rightClick = function(widget) {
    cursor.moveTo(widget);
    cursor.rightButtonPress(widget);
    cursor.rightButtonRelease(widget);
}

fillLineEditLater = function(dialogName, lineEditObjectName, text, mces) {
    invokeLater(utils, "fillLineEdit", dialogName, lineEditObjectName, text, mces);
}

clickButtonLater = function(dialogName, type, text, mces) {
    invokeLater(utils, "clickButton", dialogName, type, text, mces);
}

chooseComboBoxItemLater = function(dialogName, comboBoxObjectName, itemName, mces) {
    invokeLater(utils, "chooseComboBoxItem", dialogName, comboBoxObjectName, itemName, mces);
}

activateContextMenuActionLater = function(actionName, mces) {
    invokeLater(utils, "activateContextMenuAction", actionName, mces);
}

quitWithoutSave = function() { // стоит ли везде это писать?
    var menuFile = ui.getMenu("menu_File");
    assert(menuFile != null);
    utils.activateMenu(menuFile);
    api.wait(100);
    var actionQuit = ui.getActionInMenu(menuFile, "actionQuit");
    checkAction(actionQuit, true, false, false);
    assert(!ui.isSubMenuInMenu(menuFile, actionQuit));
    clickButtonLater("Save", "QPushButton", "Discard", 1500);
    reachedEndOfScript();
    utils.activateMenuAction(menuFile, actionQuit);
}

//togglePanelsAction;
