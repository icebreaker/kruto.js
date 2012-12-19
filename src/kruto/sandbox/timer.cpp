#include "kruto/sandbox/timer.hpp"
#include "kruto/sandbox/console.hpp"

#include <SDL/SDL.h>

using namespace v8;

namespace kruto {

Timer::Timer(void) : mTimeout(0), mStarted(0), mRepeat(false)
{
	// empty
}

Timer::~Timer(void)
{
	// empty
}

void Timer::start(const unsigned int pTimeout, const bool pRepeat)
{
	mStarted = SDL_GetTicks();
	mTimeout = pTimeout;
	mRepeat = pRepeat;
}

void Timer::clear(void)
{
	mCallback.Dispose();
	mCallback.Clear();

	mStarted = 0;
	mTimeout = 0;
	mRepeat = false;
}

bool Timer::isRepeat(void) const
{
	return mRepeat;
}

bool Timer::isSingle(void) const
{
	return mRepeat == false;
}

bool Timer::isActive(void) const
{
	return mTimeout > 0;
}

bool Timer::isStopped(void) const
{
	return mTimeout == 0;
}

void Timer::setCallback(Handle<Function> pCallback)
{
	HandleScope scope;
	mCallback = Persistent<Function>::New(pCallback);	 
}

void Timer::update(Handle<Context> pContext)
{
	HandleScope scope;
	TryCatch tryCatch;

	if(mTimeout == 0 || SDL_GetTicks() < (mStarted + mTimeout))
		return;

	Handle<Value> result = mCallback->Call(pContext->Global(), 0, NULL);
	if(result.IsEmpty())
	{
		String::Utf8Value error(tryCatch.Exception());
		Console::log("[timer] call runtime %s", *error);
	}

	if(mRepeat)
		mStarted = SDL_GetTicks();	 
	else
		clear();
}

void Timer::delay(const unsigned int pDelay)
{
	SDL_Delay(pDelay);	 
}

}
