class App
  def initialize(title, color="#00FF00")
    @color = color
    `window.title = #{title};`
  end

  def render
    %x{
      canvas = document.getElementById("canvas");
      ctx = canvas.getContext("2d");
      ctx.fillStyle = #{@color};
      ctx.fillRect(100, 100, 200, 200);
    }
  end
end

App.new("Demos: Ruby", "#FF0000").render
