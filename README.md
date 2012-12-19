Kruto.js
========
Kruto.js is an HTML5 *sandbox* inspired by [Ejecta](https://github.com/phoboslab/Ejecta).

> Transform your HTML5 games/apps into first class citizens of the desktop.

Please *note* that only a small subset of features is implemented.

You should consider it *alpha quality*; take everything with a grain of salt and 
pepper for that matter.

> No shaders were harmed in the making of Kruto.js.

Getting Started
---------------
In order to get started you need to clone this repository and fetch the
bundled submodules by opening up a Terminal window and issuing the following
commands.

```bash
git clone git://github.com/icebreaker/kruto.js.git
cd kruto.js
git submodule init
git submodule update
```

Before you start make sure that you installed all the required dependencies.

**Note**: Windows and Mac OSX users you are on your for now, but everything
**should** work just fine without *any* changes.

#### Requirements
* [premake4](http://industriousone.com/premake)
* [SDL 1.2](http://www.libsdl.org/)
* [SDL Image 1.2](http://www.libsdl.org/projects/SDL_image/)
* [SDL Mixer 1.2](http://www.libsdl.org/projects/SDL_mixer/)
* [V8](https://developers.google.com/v8/)

##### Premake
Download premake from [industriousone.com](http://industriousone.com/premake/download) 
and copy it to a location in your `PATH`.

```bash
sudo cp premake4 /usr/bin
```

##### SDL
You can install SDL from *source* or via your GNU/Linux distribution's favorite
package manager. Odds are you already have it installed.

Fedora

```bash
sudo yum install SDL-devel SDL_image-devel SDL_mixer-devel SDL_ttf-devel
```

Debian

```bash
sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev
```

openSUSE

```bash
sudo zypper install libSDL-devel libSDL_image-devel libSDL_mixer-devel
```

##### V8
V8 is bundled as a `git submodule` and you can compile it by issuing the
following commands in the `kruto.js` directory.

```bash
cd extern/v8
make dependencies
make native
```

#### Build
Now that you have all the dependencies run the following commands in the
`kruto.js` directory.

```bash
premake4 gmake
make config=release
```

If all goes well and the birds sing in your yard there should be an executable called 
`kruto.js` in the `build` directory.

Basic Example
-------------
![](https://raw.github.com/icebreaker/kruto.js/master/screenshots/basic.png)

```bash
cd demos/basic
../../build/kruto.js
```

Advanced Example
----------------
![](https://raw.github.com/icebreaker/kruto.js/master/screenshots/paperballoon.png)

```bash
cd demos/paperballoon
../../build/kruto.js src/paperballoon.js
```

Play it online [here](http://szabster.net/paperballoon).

That's all. Enjoy :)

Available API
-------------
Kruto.js implements **only** a small subset of HTML5 meaning that there's
**no real DOM, CSS, WebGL or other exotic stuff.**.

#### console
* log(message, ...)

#### Canvas
* save()
* restore()
* scale(x, y)
* rotate(angle)
* translate(x, y)
* transform(m11, m12, m21, m22, dx, dy)
* setTransform(m11, m12, m21, m22, dx, dy)
* width (property)
* height (property)
* drawImage(image, dx, dy)
* drawImage(image, dx, dy, dw, dh)
* drawImage(image, sx, sy, sw, sh, dx, dy, dw, dh)
* clearRect(x, y, w, h)
* fillRect(x, y, w, h)
* strokeRect(x, y, w, h)
* beginPath()
* closePath()
* moveTo(x, y)
* lineTo(x, y)
* fill()
* stroke()
* strokeStyle (property)
* fillStyle (property)
* lineWidth (property)
* requestFullscreen()
* isFullscreen()

#### Image
* src (property)
* width (read-only property)
* height (read-only property)
* load (event)
* error (event)

```javascript
var image = new Image();
image.onload = function() 
{
  ctx.drawImage(image, 100, 100);
};
image.src = 'panda.png';
```

#### Audio
* play()
* pause()
* canPlayType(type) (supports `audio/wav` and `audio/ogg`)
* volume (property) (supports `0.0f` to `1.0f`)
* error(event)
* canplaythrough(event)
* loop (property)
* src (property)

```javascript
var audio = new Audio();
audio.canplaythrough = function()
{
  audio.play();
};
audio.src = 'panda.ogg';
```

#### document
* title (property)
* createElement(tagName)
* getElementById(id)
* keydown (event)
* keyup (event)
* mousedown (event)
* mouseup (event)
* mousemove (event)

#### navigator
* userAgent (read-only property) returns `Kruto.js/1.0.1337 Chrome/24.0.1305.3`
* appVersion (read-only property) returns `Kruto.js/1.0.1337 Chrome/24.0.1305.3`

#### window
* innerWidth (read-only property)
* innerHeight (read-only property)
* setTimeout(callback, timeout)
* clearTimeout(timeoutId)
* setInterval(callback, interval)
* clearInterval(intervalId)
* requestAnimationFrame(callback)
* exitFullscreen()

#### location
* href (read-only property) returns `http://kruto.js/?carg1=carg2&carg3=carg4`

#### this
* require(file)

```javascript
// loads a script file synchronously (blocking)
require('js/game.js')
```

Extending the API
-----------------
There are two possible ways to extend `kruto.js` as outlined below.

#### Native Code - C++
New *features* can be added by implementing and compiling them into the
`kruto.js` binary. More often than not this will also require adding some
*glue* or *syntactic sugar* into the `Standard Library`.

For instance TTF font support could be added by using the `SDL_ttf` library in
a similar fashion to the existing `Image` and `Audio` support.

All native *methods* must be exposed as part of the *kruto* namespace/object.

#### Standard Library Code - JS
However, some features are `easier` to emulate from JS and that's where the
*Standard Library* comes into play.

The Standard Library resides in `src/kruto/sandbox/stdlib.js`, it is
compiled into the `kruto.js` executable and used to bootstrap the environment
before loading `main.js`.

`kruto.js` has to be recompiled in order to pick up the changes by running 
the following commands.

```bash
premake4 stdlib
make
```

If there's a local copy of `stdlib.js` in the same directory with `main.js` it
will take precedence and will be loaded instead of the embedded one.

This behavior can be used while testing without recompiling `kruto.js` after
each change to `stdlib.js`.

Feel free to take a sneak peek into `src/kruto/sandbox/stdlib.js` in order to
make an idea about what is available at this point.

Notes
-----
There are a couple of important things to consider when building with `kruto.js` 
in mind.

* You **must** set the `width` and `height` of the canvas before calling `getContext`

```javascript
var canvas = document.getElementById("canvas");

// setting `width` and `height` before
canvas.width = 1280;
canvas.height = 720;

var ctx = canvas.getContext("2d");
```

* You **can** set `style.backgroundColor` to change the `clear color` (background) of 
the canvas

```javascript
var canvas = document.getElementById("canvas");
canvas.style.backgroundColor = "#CACACA";
```

#### Command Line

```bash
kruto.js [main.js] [--key=value --key=value]
```

By default `kruto.js` will look in the current directory for the `main.js`
file, if not found it will terminate immediately.

It is possible to specify an alternate file to be executed by providing its
name as the first argument when running `kruto.js`. 

Please **note** that this file must be *relative* to the current directory.

Contribute
----------
* Fork the project.
* Make your feature addition or bug fix.
* Do **not** bump the version number.
* Send me a pull request. Bonus points for topic branches.

License
-------
Copyright (c) 2012, Mihail Szabolcs

Kruto.js is provided **as-is** under the **MIT** license. For more information see
LICENSE.
