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

var mainWindow;

init = function() {
    mainWindow = api.ui().mainWindow();
    assert(mainWindow != null);
    api.switchToWindow(mainWindow);
    api.wait(500);
}

//getWidgetScrollView = function(widget) {
//   return api.ui().widgetV(widget, "qt_scrollarea_viewport");
//}

//checkAction?checkAppropriateActionProperties
checkAction = function(action, isEnabledAndVisible, isCheckable, isChecked) {
    assert(action != null);
    assert(api.ui().isEnabledAndVisible(action) == isEnabledAndVisible);
    assert(api.ui().actionIsCheckable(action) == isCheckable);
    assert(api.ui().actionIsChecked(action) == isChecked);
}

leftClick = function(widget) {
    api.cursor().moveTo(widget);
    api.cursor().leftButtonPress(widget);
    api.cursor().leftButtonRelease(widget);
}

rightClick = function(widget) {
    api.cursor().moveTo(widget);
    api.cursor().rightButtonPress(widget);
    api.cursor().rightButtonRelease(widget);
}

clickButtonLater = function(type, text, time) {
    clickButtonInExpectedTopLevelDialog(mainWindow, type, text, time);
}

clickDialogButtonLater = function(dialogName, type, text, time) {
    clickButtonInExpectedDialog(mainWindow, dialogName, type, text, time);
}

quitWithoutSave = function() {
    var menuFile = api.ui().getMenu("menu_File");
    assert(menuFile != null);
    api.ui().activateMenu(menuFile);
    api.wait(100);
    var actionQuit = api.ui().getActionInMenu(menuFile, "actionQuit");
    checkAction(actionQuit, true, false, false);
    assert(!api.ui().isSubMenuInMenu(menuFile, actionQuit));
    clickDialogButtonLater("Save", "QPushButton", "Discard", 1500);
    api.ui().activateMenuAction(menuFile, actionQuit);
}

