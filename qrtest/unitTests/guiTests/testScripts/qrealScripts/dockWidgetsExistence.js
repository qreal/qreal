// тут можно проверить все элементы типа paletteDock, paletteTreeWidget и т.д.
init();

lrClicks = function (widget) {
    assert(widget != null);
    assert(api.ui().isEnabledAndVisible(widget));
    api.cursor().moveToPoint(api.ui().topLeftWidgetCorner(widget));
    api.cursor().leftButtonPress(widget);
    api.cursor().leftButtonRelease(widget);
    api.cursor().rightButtonPress(widget);
    api.cursor().rightButtonRelease(widget);
    api.wait(500);
}

var logicalModelDock = api.ui().widget("QDockWidget", "logicalModelDock");
lrClicks(logicalModelDock);
closeContextMenu(100);
api.wait(100);

var graphicalModelDock = api.ui().widget("QDockWidget", "graphicalModelDock");
lrClicks(graphicalModelDock);
closeContextMenu(100);
api.wait(100);

var propertyDock = api.ui().widget("QDockWidget", "propertyDock");
lrClicks(propertyDock);
closeContextMenu(100);
api.wait(100);

var minimapDock = api.ui().widget("QDockWidget", "minimapDock");
lrClicks(minimapDock);
closeContextMenu(100);
api.wait(100);

var paletteDock = api.ui().widget("QDockWidget", "paletteDock");
lrClicks(paletteDock);
closeContextMenu(100);
api.wait(100);

var errorDock = api.ui().widget("QDockWidget", "errorDock");
assert(errorDock == null);

var menu_View = api.ui().getMenu("menu_View");
assert(menu_View != null);

api.ui().activateMenu(menu_View);
api.wait(200);

var actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

var subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
api.ui().activateMenuAction(menu_View, actionMenuPanels);
api.ui().activateMenuAction(subMenuPanels, actionErrors);
api.wait(50);

errorDock = api.ui().widget("QDockWidget", "errorDock");
assert(errorDock != null);
assert(api.ui().isEnabledAndVisible(errorDock));
assert(api.ui().actionIsChecked(actionErrors));

lrClicks(errorDock);
api.wait(200);
activateContextMenuAction("Errors", 200);
api.wait(250);
assert(!api.ui().actionIsChecked(actionErrors));

lrClicks(paletteDock);
api.wait(200);
activateContextMenuAction("Errors", 200);
api.wait(250);
assert(api.ui().actionIsChecked(actionErrors));

errorDock = api.ui().widget("QDockWidget", "errorDock");
assert(errorDock != null);

var errorListWidget = api.ui().widget("ErrorListWidget", "errorListWidget");
assert(errorListWidget != null);
var errorListWidgetViewPort = api.ui().viewPort(errorListWidget);
assert(errorListWidgetViewPort != null);
rightClick(errorListWidgetViewPort);

activateContextMenuAction("Clear", 200);
api.wait(250);
errorDock = api.ui().widget("QDockWidget", "errorDock");
assert(errorDock == null);

checkClick = function(objName) {
    var widget = api.ui().widget("QWidget", objName);
    lrClicks(widget);
    api.wait(10);
}

actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
api.ui().activateMenu(menu_View);
api.ui().activateMenuAction(menu_View, actionMenuPanels);
api.ui().activateMenuAction(subMenuPanels, actionErrors);
api.wait(50);

var names = ["paletteTree", "paletteTreeWidget", "paletteTreeWidgets",
             "errorListWidget", "minimapView", "propertyEditor",
             "graphicalModelExplorer", "logicalModelExplorer"];

for(var i = 0; i < names.length; ++i) {
   checkClick(names[i]);
}
