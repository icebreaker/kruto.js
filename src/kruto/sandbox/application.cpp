#include "kruto/sandbox/application.hpp"
#include "kruto/sandbox/console.hpp"
#include "kruto/sandbox/file.hpp"
#include "kruto/sandbox/canvas.hpp"
#include "kruto/sandbox/script.hpp"
#include "kruto/sandbox/timer.hpp"
#include "kruto/sandbox/image.hpp"
#include "kruto/sandbox/audio.hpp"

#include <SDL/SDL.h>
#include <time.h>

namespace kruto {

Application::Application(const int pArgc, char **pArgv) : 
	mArgc(pArgc), 
	mArgv(pArgv),
	mWidth(0),
	mHeight(0),
	mRunning(true),
	mScript(NULL)
{
	Console::log("[app] created");
}

Application::~Application(void)
{
	Console::log("[app] destroyed");
}

bool Application::initialize(void)
{
	Console::log("[app] initialized");

	srand(time(NULL));

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		Console::log("[app] Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	mWidth = info->current_w;
	mHeight = info->current_h;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
//	SDL_WM_GrabInput(SDL_GRAB_ON);

	setTitle("Kruto.js");

	if(!Image::initialize())
		return false;

#ifndef KR_NO_AUDIO
	if(!Audio::initialize())
		return false;
#endif

	mScript = new Script(this);
	if(!mScript->initialize())
		return false;

	return true;
}

void Application::uninitialize(void)
{
	if(mScript != NULL)
	{
		mScript->uninitialize();
		delete mScript;
		mScript = NULL;
	}

#ifndef KR_NO_AUDIO
	Audio::uninitialize();
#endif

	Image::uninitialize();

	SDL_Quit();
	Console::log("[app] unintialized");
}

int Application::translateKey(const int pKey) const
{
	switch(pKey)
	{
		case SDLK_LEFT:
			return 37;
		case SDLK_UP:
			return 38;
		case SDLK_RIGHT:
			return 39;
		case SDLK_DOWN:
			return 40;
	}

	return pKey;
}

#define KR_KM(e, k) ((e.key.keysym.mod & KMOD_L##k) || (e.key.keysym.mod & KMOD_R##k))
#define KR_MB(e) ((e.motion.state & SDL_BUTTON(1)) ? 1 : (e.motion.state & SDL_BUTTON(2)) ? 2 : (e.motion.state & SDL_BUTTON(3)) ? 3 : 0)
void Application::processEvents(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				mScript->handleKeyboard("keydown", SDL_GetKeyName(event.key.keysym.sym), translateKey(event.key.keysym.sym), KR_KM(event, ALT), KR_KM(event, SHIFT), KR_KM(event, CTRL));
				break;

			case SDL_KEYUP:
				mScript->handleKeyboard("keyup", SDL_GetKeyName(event.key.keysym.sym), translateKey(event.key.keysym.sym), KR_KM(event, ALT), KR_KM(event, SHIFT), KR_KM(event, CTRL));
				break;

			case SDL_MOUSEBUTTONDOWN:
				mScript->handleMouse("mousedown", event.button.button, event.button.x, event.button.y);
				break;

			case SDL_MOUSEBUTTONUP:
				mScript->handleMouse("mouseup", event.button.button, event.button.x, event.button.y);
				break;

			case SDL_MOUSEMOTION:
				mScript->handleMouse("mousemove", KR_MB(event), event.motion.x, event.motion.y);
				break;

			case SDL_QUIT:
				mRunning = false;
				break;
		}
	}
}
#undef KR_KM
#undef KR_MB

int Application::exec(void)
{
	if(!initialize() || mScript->main(mArgc-1, mArgv+1) == EXIT_FAILURE || !Canvas::isInitialized())
	{
		uninitialize();
		return EXIT_FAILURE;
	}

	while(mRunning)
	{
		processEvents();
		Canvas::begin();
		mScript->update();
		Canvas::end();
	}

	uninitialize();
	return EXIT_SUCCESS;
}

void Application::setTitle(const char *pTitle)
{
	SDL_WM_SetCaption(pTitle, pTitle);
}

const char *Application::title(void) const
{
	char *title;
	SDL_WM_GetCaption(&title, NULL);
	return title;
}

int Application::argc(void) const
{
	return mArgc;	 
}

char **Application::argv(void) const
{
	return mArgv;	 
}

const char *Application::argv(const unsigned int pIndex)
{
	return mArgv[pIndex];	 
}

unsigned int Application::width(void) const
{
	return mWidth;
}

unsigned int Application::height(void) const
{
	return mHeight;	 
}

unsigned int Application::ticks(void) const
{
	return SDL_GetTicks();
}

void Application::quit(void)
{
	mRunning = false;	 
}

}
