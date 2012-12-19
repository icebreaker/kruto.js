#ifndef KRSCRIPT_HPP
#define KRSCRIPT_HPP

#include "kruto/sandbox/console.hpp"
#include "kruto/sandbox/list.hpp"
#include "kruto/sandbox/timer.hpp"
#include "kruto/sandbox/image.hpp"
#include "kruto/sandbox/audio.hpp"
#include "kruto/sandbox/v8.hpp"

namespace kruto {

class Application;
class Script
{
	friend class Application;

public:
	Script(Application *pApplication);
	~Script(void);

	const char *main_js(void) const;

	bool initialize(void);
	void uninitialize(void);

	Application *application(void) const;

	bool eval(const char *pScript);
	bool require(const char *pFileName);
	int main(const int pArgc, char **pArgv);

	void handleKeyboard(const char *pCallback, const char *pName, const int pKeyCode, const bool pAlt = false, const bool pShift = false, const bool pCtrl = false);
	void handleMouse(const char *pCallback, const int pButton, const int pX, const int pY);

	int addTimer(v8::Handle<v8::Function> pFunction, const unsigned int pTimeout, const bool pRepeat=false);
	bool removeTimer(const int pTimerId);

	Image *createImage(void);
	Audio *createAudio(void);

	void update(void);

protected:
	v8::Local<v8::Value> call(const char *pFunction, const int pArgc = 0, v8::Handle<v8::Value> pArgv[] = NULL);
	v8::Local<v8::Value> call(v8::Handle<v8::Function> pFunction, const int pArgc = 0, v8::Handle<v8::Value> pArgv[] = NULL);

private:
	v8::Persistent<v8::Context> mContext;
	v8::Persistent<v8::Object> mGlobal;

	Application *mApplication;

	List<Image *> mImages;
	List<Audio *> mAudios;
	List<Timer *> mTimers;
};

}

#endif
