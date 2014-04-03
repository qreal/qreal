var version = "2.5.3";
var year = 2014;

window.onload = function() {
	document.title = document.title.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%year%", year);
}