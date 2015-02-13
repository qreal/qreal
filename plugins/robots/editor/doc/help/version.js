var version = "3.0.1";
var year = 2015;

window.onload = function() {
	document.title = document.title.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%year%", year);
}
