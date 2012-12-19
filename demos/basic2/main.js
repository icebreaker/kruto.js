window.title = "Demos: Basic 2";

var canvas = document.getElementById("canvas");
canvas.style.backgroundColor = "#333333";

var ctx = canvas.getContext("2d");

ctx.strokeStyle = "#FF0000";
ctx.fillStyle = "#FFFF00";
ctx.lineWidth = 2;

ctx.beginPath();
ctx.moveTo(175,50);
ctx.lineTo(100,175);
ctx.lineTo(100,25);
ctx.fill();
ctx.stroke();
