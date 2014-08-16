function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    var linkName = "@ProductName@" + (installer.value("os") === "win" ? ".lnk" : "");
    /// @todo: 'trik-studio' must not be here!
    component.addOperation("CreateShortcut", "@TargetDir@/trik-studio", "@DesktopDir@/" + linkName);
}
