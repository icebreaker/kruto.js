#ifndef KRAPPLICATION_HPP
#define KRAPPLICATION_HPP

namespace kruto {

class Script;
class Application
{
public:

public:
	Application(const int pArgc, char **pArgv);
	~Application(void);

	bool initialize(void);
	void uninitialize(void);

	int translateKey(const int pKey) const;
	void processEvents(void);

	int exec(void);

	void setTitle(const char *pTitle);
	const char *title(void) const;

	int argc(void) const;
	char **argv(void) const;
	const char *argv(const unsigned int pIndex);

	unsigned int width(void) const;
	unsigned int height(void) const;

	unsigned int ticks(void) const;

	void quit(void);

private:
	int mArgc;
	char **mArgv;
	int mWidth;
	int mHeight;
	bool mRunning;
	Script *mScript;
};

}

#endif
