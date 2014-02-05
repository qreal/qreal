var version = "2.5.2.1 (TRIK)";
var year = 2013;

window.onload = function() {
	document.title = document.title.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%year%", year);
}
