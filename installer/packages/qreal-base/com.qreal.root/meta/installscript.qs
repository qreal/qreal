function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    var linkExtension = installer.value("os") === "win" ? ".lnk" : "";
    var execExtension = installer.value("os") === "win" ? ".exe" : "";
    var linkName = "@ProductName@" + linkExtension;
    /// @todo: 'trik-studio' must not be here!
    component.addOperation("CreateShortcut", "@TargetDir@/trik-studio" + execExtension, "@StartMenuDir@/" + linkName);
    component.addOperation("CreateShortcut", "@TargetDir@/maintenance" + execExtension, "@StartMenuDir@/Uninstall @ProductName@" + linkExtension);
}
