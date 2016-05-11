var version = "1.0";
var year = 2016;

window.onload = function() {
    document.title = document.title.replace("%version%", version);
    document.body.innerHTML = document.body.innerHTML.replace("%version%", version);
    document.body.innerHTML = document.body.innerHTML.replace("%year%", year);
}
