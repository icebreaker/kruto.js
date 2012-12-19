//
// Kruto.js Standard Library
//
// EDIT THIS FILE and run 'premake4 stdlib'
//
// For more information please consult the documentation.
//
window = this;

(function(window)
{
  window.console = { log: kruto.log };
  window.alert = kruto.log;
  window.require = kruto.require;
  window.read = kruto.read;
  window.close = kruto.quit;

  window.__defineGetter__("title", function() { return kruto.title; });
  window.__defineSetter__("title", function(title) { kruto.title = title; });

  window.navigator = {};
  window.navigator.__defineGetter__("userAgent", function() { return kruto.version });
  window.navigator.__defineGetter__("appVersion", function() { return kruto.version });

  function _urlFromArgs(url, args)
  {
    var params = [];
    for(var i=0; i<args.length; i++)
    {
      var arg = args[i];
      if(arg.indexOf("=") != -1)
         params.push(arg.replace(/^(-+)/, ""));
    }

    if(params.length > 0)
      return url + "?" + params.join("&");
    else
      return url;
  }

  var _url = _urlFromArgs("http://" + kruto.name.toLowerCase() +"/", kruto.arguments);
  console.log(_url);

  window.location = {};
  window.location.__defineGetter__("href", function() { return _url; });

  window.setInterval = function(callback, interval) 
  {
    return kruto.timer.add(callback, interval, true);
  };
  window.clearInterval = function(intervalId)
  {
    return kruto.timer.remove(intervalId);
  };
  window.setTimeout = function(callback, timeout) 
  { 
    return kruto.timer.add(callback, timeout, false);
  };
  window.clearTimeout = function(timeoutId)
  {
    return kruto.timer.remove(intervalId);
  };
  window.requestAnimationFrame = function(callback) { return setTimeout(callback, 1000 / 60); };

  function Event(type)
  {
    this.type = type;
  }
  Event.prototype = { type: null };
  Event.prototype.toString = function() { return "Event " + JSON.stringify(this); };

  function KeyboardEvent(type, name, key, alt, shift, ctrl)
  {
    this.type = type;
    this.keyIdentifier = name;
    this.keyCode = key;
    this.altKey = alt;
    this.shiftKey = shift;
    this.ctrlKey = ctrl;
  }
  KeyboardEvent.prototype = { type: null, keyIdentifier: "", keyCode: 0, altKey: false, shiftKey: false, ctrlKey: false };
  KeyboardEvent.prototype.toString = function() { return "KeyboardEvent " + JSON.stringify(this); };

  function MouseEvent(type, button, x, y)
  {
    this.type = type;
    this.which = button;
    this.button = button > 0 ? button - 1 : 0;
    this.offsetX = x;
    this.clientX = x;
    this.offsetY = y;
    this.clientY = y;
  }
  MouseEvent.prototype = { type: null, which: 0, button: 0, offsetX: 0, clientX: 0, offsetY: 0, clientY: 0 };
  MouseEvent.prototype.toString = function() { return "MouseEvent " + JSON.stringify(this); };

  var _eventListeners = 
  {
    load: [],
    keydown: [],
    keyup: [],
    mousedown: [],
    mouseup: [],
    mousemove: [],
    oncontextmenu: []
  };
  var _callEventListeners = function(type, e)
  {
    if(typeof window["on" + type] == "function")
      window["on" + type].call(this, e);

    if(typeof _eventListeners[type] == "undefined" || _eventListeners[type] == null)
      return;

    for(var i=0; i<_eventListeners[type].length; i++)
    {
      if(typeof _eventListeners[type][i] == "function")
        _eventListeners[type][i].call(this, e); 
    }
  };
  window.addEventListener = function(type, callback)
  {
    if(typeof _eventListeners[type] == "undefined" || _eventListeners[type] == null)
      _eventListeners[type] = [];

    _eventListeners[type].push(callback);
  };
  window.removeEventListener = function(type, callback)
  {
    if(typeof _eventListeners[type] == "undefined" || _eventListeners[type] == null)
      return;

    _eventListeners[type].splice(_eventListeners[type].indexOf(callback), 1);
  };

  kruto.main = function(argv)
  {
    _callEventListeners("load", new Event("load"));
  };
  kruto.keydown = function(name, key, alt, shift, ctrl)
  {
    _callEventListeners("keydown", new KeyboardEvent("keydown", name, key, alt, shift, ctrl));
  };
  kruto.keyup = function(name, key, alt, shift, ctrl)
  {
    _callEventListeners("keyup", new KeyboardEvent("keyup", name, key, alt, shift, ctrl));
  };
  kruto.mousedown = function(button, x, y)
  {
    if(button == 2)
      _callEventListeners("contextmenu", new MouseEvent("contextmenu", button, x, y));

    _callEventListeners("mousedown", new MouseEvent("mousedown", button, x, y));
  };
  kruto.mouseup = function(button, x, y)
  {
    _callEventListeners("mouseup", new MouseEvent("mouseup", button, x, y));
  };
  kruto.mousemove = function(button, x, y)
  {
    _callEventListeners("mousemove", new MouseEvent("mousemove", button, x, y));
  };

  function HTMLElementList()
  {
    var _length = 0;
    this.append = function(element) { this[_length++] = element; return element; }
    this.__defineGetter__("length", function() { return _length; });
  }

  function HTMLElement(type, id)
  {
    this.type = type || 'div';
    this.id = id || null;
    this.childNodes = new HTMLElementList();
  }
  HTMLElement.prototype = 
  { 
    id: null,
    type: 'div',
    childNodes: null,
    appendChild: function(element) { return this.childNodes.append(element); },
    requestFullscreen: function() { /* empty */ },
    isFullscreen: function() { /* empty */ },
    style: {}
  };
  HTMLElement.prototype.toString = function() { return "HTMLElement " + JSON.stringify(this); };

  window.Image = kruto.image;
  window.Image.prototype.__defineGetter__("src", function()
  {
    return this.filename;
  });
  window.Image.prototype.__defineSetter__("src", function(value)
  {
      this.filename = value.split("?")[0];

      var _self = this;
      setTimeout(function()
      {
        if(_self.load())
          _self.onload();
        else
          _self.onerror(); 
      }, 100);
  });
  window.Image.prototype.addEventListener = function(type, callback)
  {
    this["on" + type] = callback;
  };

  window.Audio = kruto.audio || function()
  {
    this.canPlayType = function() { return false; };
    this.play = function() { /* empty */ };
    this.pause = function() { /* empty */  };
    this.load = function() { return false; };
    this.onload = function() { /* empty */ };
    this.onerror = function() { /* empty */ };
    this.onloadedmetadata = function() { /* empty */ };
    this.oncanplaythrough = function() { /* empty */ };
  };
  window.Audio.prototype.__defineGetter__("src", function()
  {
    return this.filename;
  });
  window.Audio.prototype.__defineSetter__("src", function(value)
  {
      this.filename = value.split("?")[0];

      var _self = this;
      setTimeout(function()
      {
        if(_self.load())
        {
          _self.onloadedmetadata();
          _self.oncanplaythrough();
          _self.onload();
        }
        else
        {
          _self.onerror();  
        }
      }, 200);
  });
  window.Audio.prototype.addEventListener = function(type, callback)
  {
    this["on" + type] = callback;
  };

  function Context(canvas)
  {
    this.canvas = canvas;
    this._path = [];
  }
  Context.prototype = 
  {
    save: kruto.canvas.save,
    restore: kruto.canvas.restore,
    scale: kruto.canvas.scale,
    rotate: kruto.canvas.rotate,
    translate: kruto.canvas.translate,
    transform: kruto.canvas.transform,
    setTransform: kruto.canvas.setTransform,
    fillRect: kruto.canvas.fillRect,
    strokeRect: kruto.canvas.strokeRect,
    clearRect: kruto.canvas.clearRect,
    drawImage: kruto.canvas.drawImage,
    beginPath: function()
    {
      this._path = [];
    },
    closePath: function()
    {
      // empty
    },
    arc: function()
    {
      // TODO
    },
    moveTo: function()
    {
      this._path.push(arguments);
    },
    lineTo: function()
    {
      this._path.push(arguments);
    },
    stroke: function()
    {
      if(this._strokeStyle)
        kruto.canvas.strokeStyle = this._strokeStyle;

      kruto.canvas.beginPolygon(false);
      for(var i=0; i<this._path.length; i++)
      {
        var p = this._path[i];
        kruto.canvas.drawPoint(p[0], p[1]);
      }
      kruto.canvas.endPolygon();
    },
    fill: function()
    {
      if(this._fillStyle)
        kruto.canvas.fillStyle = this._fillStyle;

      kruto.canvas.beginPolygon(true);
      for(var i=0; i<this._path.length; i++)
      {
        var p = this._path[i];
        kruto.canvas.drawPoint(p[0], p[1]);
      }
      kruto.canvas.endPolygon();
    },
    get strokeStyle()
    {
      return this._strokeStyle;
    },
    set strokeStyle(value)
    {
      kruto.canvas.strokeStyle = value;
      this._strokeStyle = value;
    },
    get fillStyle()
    {
      return this._fillStyle;
    },
    set fillStyle(value)
    {
      kruto.canvas.fillStyle = value;
      this._fillStyle = value;
    },
    get lineWidth()
    {
      return this._lineWidth;  
    },
    set lineWidth(value)
    {
      kruto.canvas.lineWidth = value;
      this._lineWidth = value;
    }
  };

  var _fullscreen = null;

  function Canvas()
  {
    var _width = 800;
    var _height = 600;
    var _context = null;
    var _color = "#FFFFFF";
    
    this.style = {};
    this.style.__defineGetter__("backgroundColor", function()
    {
      return _color;
    });
    this.style.__defineSetter__("backgroundColor", function(value)
    {
      _color = value;
      kruto.canvas.setClearColor(value);
    });

    this.getContext = function(type) 
    {
      if(_context == null)
      {
        if(kruto.canvas.setVideoMode(_width, _height, false, _color))
          _context = new Context(this);
        else
          kruto.quit();
      }
      return _context; 
    };
    this.requestFullscreen = function()
    {
        _fullscreen = (_fullscreen == null && 
                       _context != null && 
                       kruto.canvas.setVideoMode(_width, _height, true, _color)) ? this : null;
    };
    this.isFullscreen = function()
    {
        return _fullscreen != null;
    };

    this.__defineGetter__("width", function() { return _width; });
    this.__defineSetter__("width", function(value) { _width = value; });

    this.__defineGetter__("height", function() { return _height; });
    this.__defineSetter__("height", function(value) { _height = value; });

    this.__defineGetter__("onmousedown", function() { return window["onmousedown"]; });
    this.__defineSetter__("onmousedown", function(value) { window["onmousedown"] = value; });

    this.__defineGetter__("onmouseup", function() { return window["onmouseup"]; });
    this.__defineSetter__("onmouseup", function(value) { window["onmouseup"] = value; });

    this.__defineGetter__("onmousemove", function() { return window["onmousemove"]; });
    this.__defineSetter__("onmousemove", function(value) { window["onmousemove"] = value; });

    this.__defineGetter__("oncontextmenu", function() { return window["oncontextmenu"]; });
    this.__defineSetter__("oncontextmenu", function(value) { window["oncontextmenu"] = value; });
  }
  Canvas.prototype.toString = function() { return "Canvas " + JSON.stringify(this); };

  window.__defineGetter__("innerWidth", function() { return kruto.width; });
  window.__defineGetter__("innerHeight", function() { return kruto.height; });

  window.document = {};
  window.document.addEventListener = window.addEventListener;
  window.document.removeEventListener = window.removeEventListener;
  window.document.exitFullscreen = function()
  {
    _fullscreen = (_fullscreen != null && 
                   kruto.canvas.setVideoMode(_fullscreen.width, _fullscreen.height, false)) ? null : _fullscreen;
  };
  window.document.createElement = function(type)
  {
    switch(type)
    {
      case "canvas":
        return new Canvas();

      case "image":
        return new Image();

      case "audio":
        return new Audio();
    }

    return new HTMLElement(type);
  };
  window.document.createTextNode = function(data)
  {
    return null; 
  };
  window.document.canvas = new Canvas();
  window.document.getElementById = function(id)
  {
    switch(id)
    {
      case "canvas":
        return window.document.canvas;
    }

    return null;
  };
  window.document.getElementsByTagName = function() { return []; };
  window.document.head = new HTMLElement('head');
  window.document.body = new HTMLElement('body');

})(window);
