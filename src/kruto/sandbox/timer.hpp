#ifndef KRTIMER_HPP
#define KRTIMER_HPP

#include "kruto/sandbox/v8.hpp"

namespace kruto {

class Timer
{
public:
	Timer(void);
	~Timer(void);

	void start(const unsigned int pTimeout, const bool pRepeat=false);
	void clear(void);

	bool isRepeat(void) const;
	bool isSingle(void) const;

	bool isActive(void) const;
	bool isStopped(void) const;

	void setCallback(v8::Handle<v8::Function> pCallback);

	void update(v8::Handle<v8::Context> pContext);

public:
	static void delay(const unsigned int pDelay);

private:
	unsigned int mTimeout;
	unsigned int mStarted;
	bool mRepeat;
	v8::Persistent<v8::Function> mCallback;
};

}

#endif
