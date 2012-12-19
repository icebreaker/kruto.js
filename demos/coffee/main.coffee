class App
  constructor: (title) ->
    window.title = title
    canvas = document.getElementById("canvas")
    ctx = canvas.getContext("2d")
    ctx.fillStyle = "#FF0000"
    ctx.fillRect(100, 100, 200, 200)

new App("Demos: CoffeeScript")
