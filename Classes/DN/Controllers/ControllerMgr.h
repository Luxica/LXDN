#ifndef __ControllerMgr_H__
#define __ControllerMgr_H__

class ControllerMgr
{
private:
	ControllerMgr();
	~ControllerMgr();

	static ControllerMgr *_instance;

public:
	static ControllerMgr *getInstance();

	static void destroyInstance();

private:
	bool init();
};

#endif