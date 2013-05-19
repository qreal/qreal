var version = "2.3.0 RC1";
var year = 2013;

window.onload = function() {
	document.title = document.title.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%year%", year);
}