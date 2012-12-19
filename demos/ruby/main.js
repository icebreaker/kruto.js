require('js/opal.js');
require('js/opal-parser.js');

var _require = window.require;
window.require = function(src)
{
  if(/\.rb$/.test(src))
  {
    console.log('required ' + src);
    var ret = null;
    try
    {
      ret = eval(Opal.Opal.Parser.$new().$parse(read(src)));
    }
    catch(err)
    {
      console.log(err);
    }
    return ret;
  }
  
  return _require(src);
};

require('main.rb');
