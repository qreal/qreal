var version = "0.4.0";
var year = 2013;

window.onload = function() {
    document.title = document.title.replace("%version%", version);
    document.body.innerHTML = document.body.innerHTML.replace("%version%", version);
    document.body.innerHTML = document.body.innerHTML.replace("%year%", year);
}
