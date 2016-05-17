var version = "3.1.3";
var year = 2016;

window.onload = function() {
	document.title = document.title.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%version%", version);
	document.body.innerHTML = document.body.innerHTML.replace("%year%", year);
}
