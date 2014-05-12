scriptAPI.addHint("Hello, World", 7000);
scriptAPI.moveVirtualCursor("Action", "actionNew_Diagram", 1000);
scriptAPI.virtualCursorLeftClick("Action", "actionNew_Diagram");
const fromid = scriptAPI.dragPaletteElement("qrm:/RobotsMetamodel/RobotsDiagram/InitialNode", 5000, 100, 100);
const toid = scriptAPI.dragPaletteElement("qrm:/RobotsMetamodel/RobotsDiagram/FinalNode", 5000, 300, 300);
scriptAPI.addHint("Bye, world!", 5000);
scriptAPI.drawLink(fromid, toid, 5000);
const pluginWidget = robotsGuiFacade.d2ModelWidget();
scriptAPI.open2dModel(pluginWidget);