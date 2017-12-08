#include "GameAudioEngine.h"
#include <SimpleAudioEngine.h>

static GameAudioEngine* instant = nullptr;

GameAudioEngine::GameAudioEngine()
{
	_switchOfMusic = true;
	_switchOfEffect = true;
}

GameAudioEngine* GameAudioEngine::getInstance()
{
	if (nullptr == instant)
	{
		instant = new GameAudioEngine();
	}
	return instant;
}

void GameAudioEngine::destroyInstance()
{
	if (instant)
		CC_SAFE_DELETE(instant);
}

void GameAudioEngine::setSwitchOfMusic(bool isOpen)
{
	_switchOfMusic = isOpen;
    if (isOpen)
    {
        resumeBackgroundMusic();
        setBackgroundMusicVolume(1.0f);
    }else
    {
        pauseBackgroundMusic();
        setBackgroundMusicVolume(0.0f);
    }
}

bool GameAudioEngine::getSwitchOfMusic() const
{
	return _switchOfMusic;
}

void GameAudioEngine::setSwitcjofEffect(bool isOpen)
{
	_switchOfEffect = isOpen;
    if (isOpen)
    {
        setEffectsVolume(1.0f);
    }else
    {
        setEffectsVolume(0.0f);
    }
}

bool GameAudioEngine::getSwitcjofEffect() const
{
	return _switchOfEffect;
}

void GameAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	if (_switchOfMusic)
	{
		if (nullptr != pszFilePath)
		{
			if (0 != _currentBackgroundName.compare(pszFilePath))
			{
				_currentBackgroundName = pszFilePath;
				CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, bLoop);
			}
		}
	}
}

void GameAudioEngine::stopBackgroundMusic(bool bReleaseData)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);
}

void GameAudioEngine::pauseBackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void GameAudioEngine::resumeBackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

}

unsigned int GameAudioEngine::playEffect(const char* pszFilePath, bool bLoop, float pitch, float pan, float gain)
{
	if (_switchOfEffect)
	{
		return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop, pitch, pan, gain);
	}
	return 0;
}

void GameAudioEngine::setEffectsVolume(float volume)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}

float GameAudioEngine::getEffectsVolume() const
{
	return CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume();
}

void GameAudioEngine::setBackgroundMusicVolume(float volume)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

float GameAudioEngine::getBackgroundMusicVolume() const
{
	return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}

