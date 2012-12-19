require('js/coffee-script.js');

var _require = window.require;
window.require = function require(src)
{
  if(/\.coffee$/.test(src))
  {
    console.log('required ' + src);
    var ret = null;
    try
    {
      ret = eval(CoffeeScript.compile(read(src)));
    }
    catch(err)
    {
      console.log(err);
    }
    return ret;
  }
  
  return _require(src);
};

require('main.coffee');
