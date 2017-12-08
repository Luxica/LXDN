
#ifndef WWMACROS_H_
#define WWMACROS_H_

/******************************************************************************
 *
 *
 *******************************************************************************/

#define C_S_STR const static std::string
#define C_S_INT const static int
#define C_S_FLOAT const static float

//字符串获取宏
#define GetWWString(__KEY__,__SECTION__) WWXMLManager::getWWStringFromXML(__KEY__, __SECTION__)

//静态单例方法 获取代理实例
#ifndef SINGLE_INSTANCE_FUNC
#define SINGLE_INSTANCE_FUNC(_CLASS_) \
static _CLASS_* getInstance() \
{ \
	static _CLASS_ pRet; \
	return &pRet; \
}
#endif //SINGLE_DELEGATE_INSTANCE_FUNC

#define GAME_HALL_BACKGROUND_MUSIC "sound/bkg_music_2.mp3"
#define GAME_GAME_BACKGROUND_MUSIC "sound/bkg_music_3.mp3"
#define GAME_CLICK_BUTTON_EFFECT "sound/button.mp3"
#define GAME_CLICK_CHECKBOX_EFFECT "sound/login.mp3"
static const char* SOUND_VALUE_TEXT = "sound";
static const char* MUSIC_VALUE_TEXT = "music";

#endif /* WWMACROS_H_ */


