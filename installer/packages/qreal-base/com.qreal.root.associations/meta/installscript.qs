function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (installer.value("os") === "win") {
        component.addOperation("RegisterFileType",
                               "qrs",
                               /// @todo: 'trik-studio' must not be here!
                               "@TargetDir@/trik-studio '%1'",
                               "@ProductName@ Project",
                               "application/octet-stream");
    }
}
