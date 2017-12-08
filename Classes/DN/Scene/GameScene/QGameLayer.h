#ifndef __QGAME_LAYER_H__
#define __QGAME_LAYER_H__

#include "cocos2d.h"
#include "../../Comm/filter/MsgGameFilter.h"
#include "ui/CocosGUI.h"
#include "bean/PocketSprite.h"
#include "data/QGameBaseData.h"
#include "bean/RoomClock.h"

#include <vector>
using namespace std;
using namespace cocos2d::ui;

using namespace cocos2d;

class DialogLayer;
class PopupLayer;
class GameOverLayer;
class GameVoiceRecordLayer;
class AgreeLayer;
class DismissLayer;
class LookPocker;

class QGameLayer : public Layer
{
public:
	enum GAME_STATE                                         // 游戏状态
	{
		GS_GAME_WAIT,                                       // 游戏等待状态
		GS_SEND_POKER,                                      // 随机发牌状态
		GS_LOOT_DEAL,                                       // 用户抢庄状态
		GS_RAND_DEAL,                                       // 随机庄家状态
		GS_PLAYER_BET,                                      // 闲家下注状态
		GS_OPEN_POKER,                                      // 用户开牌状态
		GS_GAME_RET,                                        // 游戏结算状态
	};
	enum POKER_COW
	{
		PC_NULL = 0,                                        // 无牛
		PC_ONE,                                             // 牛一
		PC_TWO,                                             // 牛二
		PC_THREE,
		PC_FOUR,
		PC_FIVE,
		PC_SIX,
		PC_SEVEN,
		PC_EIGHT,
		PC_NINE,
		PC_COW,                                             // 牛牛
		PC_SANGE,											// 三个
		PC_WUHUA,                                           // 五花牛（金牛）
		PC_WUXIAO,                                          // 五小牛
		PC_ZHADAN,                                          // 炸弹牛
		PC_SILVER,											//银牛
		PC_SHUNZI,                                          // 顺子
		PC_TONGHUA,                                         // 同花
		PC_HULU,                                            // 葫芦
		PC_SHUNJIN,                                         // 顺金  
		PC_MAX
	};
public:
	QGameLayer();
	~QGameLayer();

	virtual bool init();
	void  initListener();
	virtual void update(float delta);
    void hideExitRoom(float delta);
	CREATE_FUNC(QGameLayer);

public:

	void selectedCtorWinEvent(Ref* pSender, CheckBox::EventType type);
	void selectedShowMenuEvent(Ref* pSender, CheckBox::EventType type);
	void touchAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchDisAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchExitRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchContinueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchQueryEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	
	void touchFanCardEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchCuoCardEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchPromptEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchCountEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchChipEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchGrapEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchGrapNoEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchReadyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchStartGameEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchTalkEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchDismissEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchGZBtnEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchCloseGzEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchExitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchSetEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchHeadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchPocketEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void touchInviteFriendEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchCopyRoomNumEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void touchSpeekEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	//自动结算
	void ShowNiu(int nn,int id);
	void ResetLayer();
	void ResetLayerComeBack();
	void startNext(float dt);
	void setGrapButton(bool isFKQ, bool isJDQ=false);

	void updateExitRoom(EventCustom* event);
	void updateEvent(EventCustom* event);
	void updateGameStaues(EventCustom* event);
	void updateReady(EventCustom* event);
	void updateGetPocker(EventCustom* event);
	void updateRobZhuang(EventCustom* event);
	void updateGetZhuang(EventCustom* event);
	void updateGetBet(EventCustom* event);
	void updateOpenCard(EventCustom* event);
	void updateResult(EventCustom* event);
	void updateRoomCancel(EventCustom* event);
	void updateVoteCancel(EventCustom* event);
	void updateRoomOver(EventCustom* event);
	void updateGameOver(EventCustom* event);
	void updateCharFast(EventCustom* event);
    void updateCharVoice(EventCustom* event);

	void updateLoseRoom(EventCustom* event);
	void updateBackRoom(EventCustom* event);
	void updateGameBack(EventCustom* event);

	void playCharVoice(QGameVoiceData *);

	void updateRoomWarn(EventCustom* event);

	void updateBackHall(EventCustom* event);
	void updateSellte(EventCustom* event);

	//*改的*
	void updateNetConnet(float dt);
	void updateSendCard(float dt);
	void lightCallBack(float dt);
	Animate* lightBlink();
	void goldMove(Vector<QRetData*> vecResult);
	//判断是否有牛
	bool isHaveCattle();
	string gettimecurrent();
	std::vector<unsigned char> SortPokerVec(std::vector<unsigned char>& pokerVec);
	
	void updateHead();
	//显示玩家积分
	void showUserScore();
	//获取自己的手牌
	void showMyHand();
	void showOtherHand(QCardData *pCard);

	void showOtherBackPoker();
	//显示自己全部手牌
	void showMyPoker();

	//
	void showAllPocket(Vector<QCardData*> & vecSellte);
	CC_SYNTHESIZE(int, m_gameState, GameState);
    void showMyPaiAgain();
    
    void onInviteCB(int errorCode);

	void ExpressionAction(const Vec2& pos1, const Vec2& pos2, string fileName);

	void updateWife(float dt);				//更新wife信号和电池电量

	void setLookLayer(){ m_lookPocker = nullptr; };
	void showLastCard();


	void BetChip(int score, Layout* headPos[], int index);
	Action* ChipMove();
	void ChipToWinner(int seatId);							//筹码移动到赢家位置

	void PlayWinAction();

	void CloseDismissLayer();

private:
	void PromptCow();
    ///判断牛
    int GetCow(vector<int>& vecCard);
    bool IsZhaDan(vector<int>& vecCard);     // 炸弹
    bool IsWuXiao(vector<int>& vecCard);     // 五小牛
    bool IsWuHua(vector<int>& vecCard);      // 五花牛
    int GetValue(int u08Card);
    int GetValue(vector<int>& vecCard);
    /**
     * 检测能否说话
     */
    bool isCanSpeek();

	void resetTime(float dt);
	void reStartTime(float dt);

public:
	void showZhuang();
    /**
     * 设置上次说话时间
     */
    void setSpeekTime();
    
private:

	__Dictionary* m_Dictionary;			//xml文档
	int m_gameNum;						//记录局数
	Text* m_countNum;					//局数显示
	Text* m_systemTime;					//显示时间
	Layout* m_speakPos[8];				//语音播放位置

	std::queue<QGameVoiceData *>		_queueCharVoice;
	bool								_charVoicePlay;

	Button* m_dissolveButton;			//解散按钮
	ImageView* m_showBg;				

	Layout* m_readyLayout;				//准备
	Layout* m_grapLayout;				//抢庄
	int m_ZID;							//庄家位置
	Layout* m_chipLayout;				//下注
	vector<Button*> m_chipButtonVec;	//下注按钮

	Layout* m_countLayout;				//计算
	static int COUNT;					//记录点击了三张牌
	int CoutSum;						//记录点击的三张牌的和值
	PocketSprite* m_coutPockNum[3];		//计算牌面值

	Layout* m_showResult;				//显示玩家的牛层
	vector<Sprite*> m_spriteNiu;		//牛的显示位置
	vector<Sprite*> m_niuBg;			//牛的背景显示位置
	Layout* m_result;					//结算弹窗
	ListView* m_listView_1;				//小结列表容器1
	ListView* m_listView_2;				//小结列表容器2
	Layout* m_resultPanel;				//小结个人信息
	Layout* m_running;					//中途加入游戏等待窗口 

	RoomClock* m_RoomClock;				//倒计时
	//当前玩家
	vector<unsigned char> m_pocketVec;	//创建玩家牌精灵(发给玩家的牌)
	Vector<ImageView*> m_pocketUser;	//当前玩家的牌

	GameOverLayer* m_gameOverlayer;		//大结弹窗
	GameVoiceRecordLayer* m_voiceLayer;			//语音窗口
	bool isGameOver;					//是否隐藏大结

	//玩家位置显示
	QUserData* m_player[8];				//存放视图位置的玩家
	vector<Layout*> m_playerPanelVec;   //显示玩家头像
	vector<Layout*> m_verUserHead;		//玩家头像位置
	int m_scoreSum[8];					//每个人的累积积分
	vector<Layout*> m_palyerPockerVec;	//显示玩家牌位置
	vector<Layout*> m_imageview;		//点击
	//开牌创建其他玩家的牌
	vector<unsigned char>m_spritePockerVec[7];	//其他玩家的牌

	vector<int> m_grapVeiw[4];			//疯狂抢庄灯（按倍数确认）

	Layout* m_PocketPos;				//存放玩家牌的层
	Layout* m_WarnLayer;				//警告
	Layout* m_lightLayout;
	vector<Sprite*> m_vecLight;			//存放抢庄的人面前的灯
	Sprite* m_ligthDealer;				//确定庄家之后灯闪

	bool m_bIsStart;					//是否一局结算完成
	bool m_pIsSendPocke;				//是否发牌
	DialogLayer* m_exitRoom;			//发起解散游戏弹窗
	AgreeLayer* m_agreeMent;
	DismissLayer* m_dismiss;

	PopupLayer* m_popupLayer;			//游戏弹窗

	LookPocker* m_lookPocker;			//搓牌

	bool m_bIsGameBack;					//是否是断线重连回来
	Sprite* m_loseRoom[8];				//断线图片显示
    
    bool m_bCanSendSpeek;				//是否可以发送快捷语或则语音
   	struct timeval	m_tvSpeekTime;      //当时讲话的时间

	int m_isPlay;

	bool m_ctorWin;						//是否控制赢	

	int m_exitTime;						//关闭退出弹窗倒计时
	int m_reStartTime;

	Layout* m_HeadPos[8];
	
	CheckBox* chipCheckBox;				//下注托管选择
	int m_checkChip;					//托管注数

	vector<Sprite*> m_goldVec[8];		//金币容器

	ImageView* m_Wife;					//wife信号
	LoadingBar* m_Power;				//电池电量

	ImageView* m_ZBet;					//抢庄倍数

	static int sendIndex;
	static int indexCount;

	int m_curNum;						//当前房间人数

	Button* m_btnQuery;					//查询战绩
	Button* m_btnContinue;				//继续游戏

	vector<Sprite*> m_vecChip;

	ImageView* m_startNext;
};

#endif
