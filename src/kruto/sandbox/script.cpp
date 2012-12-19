#include "kruto/sandbox/script.hpp"
#include "kruto/sandbox/version.hpp"
#include "kruto/sandbox/console.hpp"
#include "kruto/sandbox/canvas.hpp"
#include "kruto/sandbox/image.hpp"
#include "kruto/sandbox/audio.hpp"
#include "kruto/sandbox/application.hpp"
#include "kruto/sandbox/file.hpp"
#include "kruto/sandbox/stdlib.hpp"

#include <string.h>

using namespace v8;

#include "kruto/sandbox/bindings/core.hpp"
#include "kruto/sandbox/bindings/timer.hpp"
#include "kruto/sandbox/bindings/canvas.hpp"
#include "kruto/sandbox/bindings/image.hpp"
#include "kruto/sandbox/bindings/audio.hpp"

namespace kruto {

Script::Script(Application *pApplication) : mApplication(pApplication)
{
	Console::log("[script] created");
}

Script::~Script(void)
{
	Console::log("[script] destroyed");
}

const char *Script::main_js(void) const
{
	if(mApplication->argc() > 1)
	{
		const char *arg0 = mApplication->argv(1);
		if(*arg0 != '-' && File::isTypeOf(arg0, ".js"))
			return arg0;
	}
	return "main.js";
}

bool Script::initialize(void)
{
	Console::log("[script] initialized");

	HandleScope scope;
	TryCatch tryCatch;

	Handle<ObjectTemplate> kruto = ObjectTemplate::New();

	// Functions
	kruto->Set(String::New("log"), FunctionTemplate::New(bindings::core::log));
	kruto->Set(String::New("require"), FunctionTemplate::New(bindings::core::require, External::New(this)));
	kruto->Set(String::New("read"), FunctionTemplate::New(bindings::core::read));
	kruto->Set(String::New("quit"), FunctionTemplate::New(bindings::core::quit, External::New(this)));

	// Accessors: Get and Set
	kruto->SetAccessor(String::New("title"), bindings::core::titleGet, bindings::core::titleSet, External::New(mApplication));

	// Accessors: Get (read-only)
	kruto->SetAccessor(String::New("version"), bindings::core::versionGet);
	kruto->SetAccessor(String::New("name"), bindings::core::nameGet);
	kruto->SetAccessor(String::New("arguments"), bindings::core::argumentsGet, NULL, External::New(mApplication));
	kruto->SetAccessor(String::New("width"), bindings::core::widthGet, NULL, External::New(mApplication));
	kruto->SetAccessor(String::New("height"), bindings::core::heightGet, NULL, External::New(mApplication));

	// Empty Stubs
	kruto->Set(String::New("main")		, FunctionTemplate::New(bindings::core::emptyStub));
	kruto->Set(String::New("keydown")	, FunctionTemplate::New(bindings::core::emptyStub));
	kruto->Set(String::New("keyup")		, FunctionTemplate::New(bindings::core::emptyStub));
	kruto->Set(String::New("mousedown")	, FunctionTemplate::New(bindings::core::emptyStub));
	kruto->Set(String::New("mouseup")	, FunctionTemplate::New(bindings::core::emptyStub));
	kruto->Set(String::New("mousemove")	, FunctionTemplate::New(bindings::core::emptyStub));

	// Canvas
	Handle<ObjectTemplate> canvas = ObjectTemplate::New();
	canvas->Set(String::New("setVideoMode")	, FunctionTemplate::New(bindings::canvas::setVideoMode));
	canvas->Set(String::New("setClearColor"), FunctionTemplate::New(bindings::canvas::setClearColor));

	canvas->Set(String::New("save")			, FunctionTemplate::New(bindings::canvas::save));
	canvas->Set(String::New("restore")		, FunctionTemplate::New(bindings::canvas::restore));
	canvas->Set(String::New("scale")		, FunctionTemplate::New(bindings::canvas::scale));
	canvas->Set(String::New("rotate")		, FunctionTemplate::New(bindings::canvas::rotate));
	canvas->Set(String::New("translate")	, FunctionTemplate::New(bindings::canvas::translate));
	canvas->Set(String::New("transform")	, FunctionTemplate::New(bindings::canvas::transform));
	canvas->Set(String::New("setTransform")	, FunctionTemplate::New(bindings::canvas::setTransform));
	canvas->Set(String::New("fillRect")		, FunctionTemplate::New(bindings::canvas::fillRect));
	canvas->Set(String::New("strokeRect")	, FunctionTemplate::New(bindings::canvas::strokeRect));
	canvas->Set(String::New("clearRect")	, FunctionTemplate::New(bindings::canvas::clearRect));

	canvas->SetAccessor(String::New("strokeStyle")	, NULL, bindings::canvas::setStrokeStyle);
	canvas->SetAccessor(String::New("fillStyle")	, NULL, bindings::canvas::setFillStyle);
	canvas->SetAccessor(String::New("lineWidth")	, NULL, bindings::canvas::setLineWidth);

	canvas->Set(String::New("beginPolygon")	, FunctionTemplate::New(bindings::canvas::beginPolygon));
	canvas->Set(String::New("drawPoint")	, FunctionTemplate::New(bindings::canvas::drawPoint));
	canvas->Set(String::New("endPolygon")	, FunctionTemplate::New(bindings::canvas::endPolygon));

	canvas->Set(String::New("drawImage")	, FunctionTemplate::New(bindings::canvas::drawImage));

	kruto->Set(String::New("canvas"), canvas);

	// Timer
	Handle<ObjectTemplate> timer = ObjectTemplate::New();
	timer->Set(String::New("add"), FunctionTemplate::New(bindings::timer::add, External::New(this)));
	timer->Set(String::New("remove"), FunctionTemplate::New(bindings::timer::remove, External::New(this)));
	timer->Set(String::New("delay"), FunctionTemplate::New(bindings::timer::delay));
	kruto->Set(String::New("timer"), timer);
	
	// Image
	Handle<FunctionTemplate> image = FunctionTemplate::New(bindings::image::create, External::New(this));
	image->InstanceTemplate()->Set(String::New("load"), FunctionTemplate::New(bindings::image::load));
	image->InstanceTemplate()->SetAccessor(String::New("width"), bindings::image::width, NULL);
	image->InstanceTemplate()->SetAccessor(String::New("height"), bindings::image::height, NULL);
	image->InstanceTemplate()->Set(String::New("onload"), FunctionTemplate::New(bindings::core::emptyStub));
	image->InstanceTemplate()->Set(String::New("onerror"), FunctionTemplate::New(bindings::core::emptyStub));
	image->InstanceTemplate()->SetAccessor(String::New("filename"), bindings::image::filenameGet, bindings::image::filenameSet);
	image->InstanceTemplate()->SetInternalFieldCount(1);
	kruto->Set(String::New("image"), image);

#ifndef KR_NO_AUDIO
	// Audio
	Handle<FunctionTemplate> audio = FunctionTemplate::New(bindings::audio::create, External::New(this));
	audio->InstanceTemplate()->Set(String::New("load"), FunctionTemplate::New(bindings::audio::load));
	audio->InstanceTemplate()->Set(String::New("play"), FunctionTemplate::New(bindings::audio::play));
	audio->InstanceTemplate()->Set(String::New("pause"), FunctionTemplate::New(bindings::audio::pause));
	audio->InstanceTemplate()->Set(String::New("canPlayType"), FunctionTemplate::New(bindings::audio::canplaytype));
	audio->InstanceTemplate()->Set(String::New("onload"), FunctionTemplate::New(bindings::core::emptyStub));
	audio->InstanceTemplate()->Set(String::New("onerror"), FunctionTemplate::New(bindings::core::emptyStub));
	audio->InstanceTemplate()->Set(String::New("oncanplaythrough"), FunctionTemplate::New(bindings::core::emptyStub));
	audio->InstanceTemplate()->Set(String::New("onloadedmetadata"), FunctionTemplate::New(bindings::core::emptyStub));
	audio->InstanceTemplate()->SetAccessor(String::New("volume"), bindings::audio::volumeGet, bindings::audio::volumeSet);
	audio->InstanceTemplate()->SetAccessor(String::New("loop"), bindings::audio::loopGet, bindings::audio::loopSet);
	audio->InstanceTemplate()->SetAccessor(String::New("filename"), bindings::audio::filenameGet, bindings::audio::filenameSet);
	audio->InstanceTemplate()->SetInternalFieldCount(1);
	kruto->Set(String::New("audio"), audio);
#endif

	// Kruto
	Handle<ObjectTemplate> global = ObjectTemplate::New();
	global->Set(String::New("kruto"), kruto);

	mContext = Context::New(NULL, global);

	Context::Scope contextScope(mContext);
	mGlobal = Persistent<Object>::New(Handle<Object>::Cast(mContext->Global()->Get(String::New("kruto"))));

	if(!require("stdlib.js"))
		return false;

    if(!require(main_js()))
		return false;

	return true;
}

void Script::uninitialize(void)
{
	mAudios.dispose();
	mAudios.clear();

	mImages.dispose();
	mImages.clear();

	mTimers.dispose();
	mTimers.clear();

	mGlobal.Dispose();
	mGlobal.Clear();

	mContext.Dispose();
	mContext.Clear();

	Console::log("[script] uninitialized");
}

Application *Script::application(void) const
{
	return mApplication;	 
}

bool Script::eval(const char *pScript)
{
	HandleScope scope;
	Context::Scope contextScope(mContext);
	TryCatch tryCatch;

	Handle<v8::Script> script = v8::Script::Compile(String::New(pScript));
	if(script.IsEmpty())
	{
		String::Utf8Value error(tryCatch.Exception());
		Console::log("[script] compilation %s", *error);
		return false;
	}

	Handle<Value> result = script->Run();
	if(result.IsEmpty())
	{
		String::Utf8Value error(tryCatch.Exception());
		Console::log("[script] runtime %s", *error);
		return false;
	}

	return true;
}

bool Script::require(const char *pFileName)
{
	char *buffer = File::read(pFileName);
	if(buffer == NULL)
	{
		if(!strcmp(pFileName, "stdlib.js"))
		{
			int len = strlen(STDLIB);
			buffer = new char[len+1];
			strncpy(buffer, STDLIB, len);
			buffer[len] = '\0';

			Console::log("[script] using built-in %s", pFileName);
		}
		else
		{
			Console::log("[script] no such file to load -- %s", pFileName);
			return false;
		}
	}

	if(eval(buffer))
		Console::log("[script] required %s", pFileName);
	else
		Console::log("[script] require %s -- failed", pFileName);

	delete [] buffer;
	return true;
}

int Script::main(const int pArgc, char **pArgv)
{
	HandleScope scope;
	Context::Scope contextScope(mContext);

	Handle<Array> value = Array::New(pArgc);
	
	for(int i=0; i<pArgc; i++)
		value->Set(Integer::New(i), String::New(pArgv[i]));
	
	Handle<Value> argv[1] = { value };
	Handle<Value> result = call("main", 1, argv);

	return result.IsEmpty() ? EXIT_FAILURE : result->Int32Value();
}

void Script::handleKeyboard(const char *pCallback, const char *pName, const int pKeyCode, const bool pAlt, const bool pShift, const bool pCtrl)
{
	HandleScope scope;
	Context::Scope contextScope(mContext);

	Handle<Value> name = String::New(pName);
	Handle<Value> keycode = Integer::New(pKeyCode);
	Handle<Value> alt = Boolean::New(pAlt);
	Handle<Value> shift = Boolean::New(pShift);
	Handle<Value> ctrl = Boolean::New(pCtrl);
	Handle<Value> argv[5] = { name, keycode, alt, shift, ctrl };
	call(pCallback, 5, argv);
}

void Script::handleMouse(const char *pCallback, const int pButton, const int pX, const int pY)
{
	HandleScope scope;
	Context::Scope contextScope(mContext);

	Handle<Value> button = Integer::New(pButton);
	Handle<Value> x = Integer::New(pX);
	Handle<Value> y = Integer::New(pY);
	Handle<Value> argv[3] = { button, x, y };
	call(pCallback, 3, argv); 
}

int Script::addTimer(Handle<Function> pFunction, const unsigned int pTimeout, const bool pRepeat)
{	
	HandleScope scope;
	Context::Scope contextScope(mContext);

	for(int i=0; i<mTimers.size(); i++)
	{
		Timer *timer = mTimers.at(i);
		if(timer->isStopped())
		{
			timer->setCallback(pFunction);
			timer->start(pTimeout, pRepeat);
			return i + 1;
		}
	}

	Timer *timer = new Timer();
	timer->setCallback(pFunction);
	timer->start(pTimeout, pRepeat);
	return mTimers.add(timer) + 1;
}

bool Script::removeTimer(const int pTimerId)
{
	int id = pTimerId - 1;
	if(id < 0 || id >= mTimers.size())
		return false;

	Timer *timer = mTimers.at(id);
	timer->clear();

	return true;
}

Image *Script::createImage(void)
{
	Image *image = new Image();
	mImages.add(image);
	return image;
}

Audio *Script::createAudio(void)
{
	Audio *audio = new Audio();
	mAudios.add(audio);
	return audio;
}

void Script::update(void)
{
	HandleScope scope;
	Context::Scope contextScope(mContext);

	for(int i=0; i<mTimers.size(); i++)
		mTimers.at(i)->update(mContext);
}

Local<Value> Script::call(const char *pFunction, const int pArgc, Handle<Value> pArgv[])
{
	HandleScope scope;
	Context::Scope contextScope(mContext);
	Handle<Value> result;

	Handle<Value> value = mGlobal->Get(String::New(pFunction));
	if(value->IsFunction())
		result = call(Handle<Function>::Cast(value), pArgc, pArgv);
	else
		Console::log("[script] no function %s(...)", pFunction);	 

	return scope.Close(result);
}

Local<Value> Script::call(Handle<Function> pFunction, const int pArgc, Handle<Value> pArgv[])
{
	HandleScope scope;
	Context::Scope contextScope(mContext);
	TryCatch tryCatch;

	Handle<Value> result = pFunction->Call(mGlobal, pArgc, pArgv);

	if(result.IsEmpty())
	{
		String::Utf8Value error(tryCatch.Exception());
		Console::log("[script] call runtime %s", *error);
	}

	return scope.Close(result);
}

}
