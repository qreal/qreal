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

// For autocompletion and syntax highlighting
var mainWindow, ui, utils, keyboard, palette, hints, scene, cursor;

lrClicks = function (widget) {
    assert(widget != null);
    expect(utils.isEnabledAndVisible(widget));
    cursor.moveToPoint(utils.topLeftWidgetCorner(widget));
    cursor.leftButtonPress(widget);
    cursor.leftButtonRelease(widget);
    cursor.rightButtonPress(widget);
    cursor.rightButtonRelease(widget);
    api.wait(200);
}

closeContextMenuLater = function(msec){
    invokeLater(utils, "closeContextMenu", msec);
}

function checkDockWidget(dockWidgetName) {
    var dock = ui.widget("QDockWidget", dockWidgetName);
    closeContextMenuLater(500);
    lrClicks(dock);
    api.wait(501);
}

checkDockWidget("logicalModelDock");
checkDockWidget("graphicalModelDock");
checkDockWidget("propertyDock");
checkDockWidget("minimapDock");

var paletteDock = ui.widget("QDockWidget", "paletteDock");
closeContextMenuLater(500);
lrClicks(paletteDock);
api.wait(501);

var errorDock = ui.widget("QDockWidget", "errorDock");
assert(errorDock == null);

var menu_View = ui.getMenu("menu_View");
assert(menu_View != null);

utils.activateMenu(menu_View);
api.wait(200);

var actionMenuPanels = ui.getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(ui.isSubMenuInMenu(menu_View, actionMenuPanels));

var subMenuPanels = ui.getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionErrors = ui.getActionInMenu(subMenuPanels, "Errors");
utils.activateMenuAction(menu_View, actionMenuPanels);
utils.activateMenuAction(subMenuPanels, actionErrors);
api.wait(1000);

errorDock = ui.widget("QDockWidget", "errorDock");
assert(errorDock != null);
assert(utils.isEnabledAndVisible(errorDock));
expect(utils.actionIsChecked(actionErrors));

activateContextMenuActionLater("Errors", 500);
lrClicks(errorDock);
api.wait(501);

api.wait(250);
assert(!utils.actionIsChecked(actionErrors));

activateContextMenuActionLater("Errors", 500);
lrClicks(paletteDock);
api.wait(501);

api.wait(250);
assert(utils.actionIsChecked(actionErrors));

errorDock = ui.widget("QDockWidget", "errorDock");
assert(errorDock != null);
var viewPortOfDock = findViewPort(errorDock);

var errorListWidget = ui.widget("ErrorListWidget", "errorListWidget");
assert(errorListWidget != null);
var errorListWidgetViewPort = ui.viewPort(errorListWidget);
assert(errorListWidgetViewPort != null);

if (errorListWidgetViewPort != viewPortOfDock) {
    add_failure("ViewPorts should be equal");
    if (!utils.isEnabledAndVisible(errorListWidgetViewPort)
            || !utils.isEnabledAndVisible(viewPortOfDock)) {
        fail("ViewPorts should be equal and enabled and visible!!!!");
    }
}

activateContextMenuActionLater("Clear", 400);
rightClick(errorListWidgetViewPort);
api.wait(450);

errorDock = ui.widget("QDockWidget", "errorDock");
assert(errorDock == null);

checkClick = function(objName) {
    var widget = ui.widget("QWidget", objName);
    lrClicks(widget);
    api.wait(10);
}

actionErrors = ui.getActionInMenu(subMenuPanels, "Errors");
utils.activateMenu(menu_View);
utils.activateMenuAction(menu_View, actionMenuPanels);
utils.activateMenuAction(subMenuPanels, actionErrors);
api.wait(50);

var names = ["paletteTree", "paletteTreeWidget", "paletteTreeWidgets",
             "errorListWidget", "minimapView", "propertyEditor",
             "graphicalModelExplorer", "logicalModelExplorer"];

for(var i = 0; i < names.length; ++i) {
   checkClick(names[i]);
}
