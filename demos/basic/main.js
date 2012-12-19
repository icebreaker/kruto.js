var dt = 0;
var pX = 100 + (Math.random() * 100);
var pY = 120 + (Math.random() * 120);

window.title = "Demos: Basic";
window.onkeydown = function(e) 
{ 
  if(e.keyCode == 27) // ESC
  {
    window.close();
    return;
  }

  if(e.keyCode == 37) // LEFT
    pX -= dt * 200 + 10;
  else if(e.keyCode == 39) // RIGHT
    pX += dt * 200 + 10;

  if(e.keyCode == 38) // UP
    pY -= dt * 200 + 10;
  else if(e.keyCode == 40) // DOWN
    pY += dt * 200 + 10;
}

var canvas = document.getElementById("canvas");
var ctx = canvas.getContext("2d");

var grass = new Image();
grass.loaded = false;
grass.onload = function() { grass.loaded = true; }
grass.src = "data/gfx/greenground1.png";

var building = new Image();
building.loaded = false;
building.onload = function() { building.loaded = true; }
building.src = "data/gfx/building11.png";

var tree = new Image();
tree.loaded = false;
tree.onload = function() { tree.loaded = true; }
tree.src = "data/gfx/fullpufftree.png";

var soldier = new Image();
soldier.loaded = false;
soldier.onload = function() { soldier.loaded = true; }
soldier.src = "data/gfx/malesoldiernormal.png";

var music = new Audio();
music.addEventListener("canplaythrough", function() 
{
  music.loop = true;
  music.play();
});
music.src = "data/sfx/calm_town.ogg";

var last = new Date();
function tick()
{
  var now = new Date();
  dt = (now - last) * 0.001;
  last = now;

  if(grass.loaded)
    ctx.drawImage(grass, 0, 0);

  if(building.loaded)
    ctx.drawImage(building, 20, 20);

  if(soldier.loaded)
    ctx.drawImage(soldier, 0, 128, 64, 64, pX, pY, 64, 64);  

  if(tree.loaded)
  {
    ctx.drawImage(tree, 400, 100);
    ctx.drawImage(tree, 300, 300);
  }

  requestAnimationFrame(tick);
}

tick();
