#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//游戏属性
#define KIND_ID						127									//游戏 I D
#define GAME_PLAYER					1							        //游戏人数
#define GAME_NAME					TEXT("超级小丑")					//游戏名字
#define GAME_GENRE				   (GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型
#define SCROLL_PICTURE_COUNT		3									//显示区域图片数量

//组件属性
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//////////////////////////////////////////////////////////////////////////////////
//状态定义
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_GO				GAME_STATUS_PLAY					//点击GO
#define GAME_SCENE_AUTORUN          (GAME_STATUS_PLAY+7)                //托管游戏
#define GAME_SCENE_PICTURES_ROLL    (GAME_STATUS_PLAY+1)                //图片转动
#define GAME_SCENE_CHANGE_BET       (GAME_STATUS_PLAY+2)                //改变赌注
#define GAME_SCENE_STOP				(GAME_STATUS_PLAY+3)				//点击STOP
#define GAME_SCENE_CLOWN_BET        (GAME_STATUS_PLAY+4)                //赌小丑
#define GAME_SCENE_CHANGE_PRIZE     (GAME_STATUS_PLAY+5)                //改变猜小丑注数
#define GAME_SCENE_END				(GAME_STATUS_PLAY+6)  				//游戏结束

//图像索引
#define SPRITE_YELLOW_DRAGON			    0							//黄龙
#define SPRITE_BLUA_DRAGON					1							//绿龙
#define SPRITE_COLOR_CLOWN					2							//彩小丑
#define SPRITE_GRAY_CLOWN					3							//灰小丑
#define SPRITE_BEAUTI_GIRL					4							//美女
#define SPRITE_BOX				        	5							//宝箱
#define SPRITE_DIAMOND					    6							//钻石
#define SPRITE_SHELL						7							//珍珠
#define SPRITE_COIN					        8							//金币
#define SPRITE_SILVER					    9							//银币
#define SPRITE_MAX                          10                          //精灵总数

////////////////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_READY                  115                           //准备开始游戏
#define SUB_S_START_GAME                  116                           //开始游戏
#define SUB_S_AUTORUN_GAME                125                           //托管游戏
#define SUB_S_SEND_RESLUT                 117                           //发送结果
#define SUB_S_PICTURES_ROLL               118                           //图片转动
#define SUB_S_CHANGE_BET                  119                           //改变赌注
#define SUB_S_STOP_SCROLL                 120                           //停止转动
#define SUB_S_CLOWN_BET                   122                           //下注小丑
#define SUB_S_CHANGE_PRIZE                123                           //改变猜小丑注数
#define SUB_S_GET_PRIZE                   124                           //结束本轮游戏

//////////////////////////////////////////////////////////////////////////////////
//准备游戏
struct CMD_S_GAME_READY
{
	WORD			wUserID;							//玩家椅号
	LONGLONG		lUserScore;							//玩家金币数量
	LONGLONG        lGameNeed;                          //游戏所需金币
	LONGLONG        lDefultChip;                        //游戏默认所需金币
	LONGLONG        lFinalPrize;                        //最大奖励
	int             nAvailableTimes;                    //今日可玩次数
	int             nTimes;                             //赌小丑倍数
	int             nStarCount;                         //猜对小丑次数
	bool            bIsSelected;                        //是否选中
	bool            bIsRolling;                         //是否在转动
	bool            bLackChip;                          //是否不足下注
};
//开始游戏
struct CMD_S_GAME_START
{
	LONGLONG        lGameNeed;                          //游戏所需金币
	LONGLONG		lUserScore;							//玩家金币数量
	LONGLONG        lDefultChip;                        //游戏默认所需金币
};
//托管游戏
struct CMD_S_AUTORUN_GAME
{
	bool            bIsSelected;                       //是否选中
	bool            bIsRolling;                        //是否在转动
};
//发送结果
struct CMD_S_SEND_RESULT
{
	int			    bResultData[SCROLL_PICTURE_COUNT];	//开奖结果
	int             nPrizeTimes;                        //奖励倍数
	int             nStarCount;                         //猜对次数
	bool            bPictureIsAllSame;                  //显示图片是否全相同
	bool            bLackChip;                          //是否不足下注
	LONGLONG        lChipPrize;                         //相同后赌注奖励
	LONGLONG        lCurrentChip;                       //如果输了刷新当前下注数
	LONGLONG        lDefultChip;                        //游戏默认所需金币
};
//图像转动
struct CMD_S_PICTURES_ROLL
{
	LONGLONG        lGameNeed;                          //游戏所需金币
	LONGLONG		lUserScore;							//玩家金币数量
	LONGLONG	    lDefultChip;					    //默认消耗金币量
	int             nAvailableTimes;                    //剩余可玩游戏次数
	bool            bIsRolling;                         //是否在转动
};
//改变赌注
struct CMD_S_CHANGE_BET
{
	LONGLONG         lGameNeed;                          //当前赌注
	LONGLONG		 lUserScore;					   	 //玩家金币数量
	LONGLONG		 lDefultChip;					     //默认消耗金币量
};
//停止转动
struct CMD_S_STOP_SCROLL
{
	bool            bIsRolling;                         //是否在转动
};
//赢了之后开始下注
struct CMD_S_CLOWN_BET
{
	int              nStarCount;                         //猜对小丑次数星级
	int              nClownIndex;                        //小丑索引
	bool             bChooseRight;                       //是否猜对小丑
	LONGLONG         lChipPrize;                         //猜小丑赌注
	LONGLONG		 lUserScore;					   	 //玩家金币数量
	int              nTimes;                             //猜小丑赌注倍数
	bool             bLackChip;                          //是否不足下注
};
//改变猜小丑下注倍数
struct CMD_S_CHANGE_PRIZE
{
	LONGLONG         lChipPrize;                        //猜小丑赌注
	LONGLONG		 lUserScore;						//玩家金币数量
	int              nTimes;                            //下注倍数
};
//结束本轮游戏
struct CMD_S_GET_PRIZE
{
	WORD			 wUserID;							//玩家椅号
	LONGLONG         lChipPrize;                        //赌注奖励
	LONGLONG         lFinalPrize;                       //最大奖励
	LONGLONG		 lUserScore;						//玩家金币数量
	LONGLONG         lDefultChip;                        //游戏默认所需金币
	LONGLONG         lCurrentChip;                      //当前下注
	BYTE			 bEndMode;						    //结束方式
	int              nPrizeTimes;                       //奖励倍数
	int              nStarCount;                        //猜对小丑次数
	int              nTimes;                            //下注倍数
	bool             bLackChip;                         //是否不足下注
};
//准备状态
struct CMD_S_READY_STATUS
{
	WORD			wUserID;							//玩家椅号
	LONGLONG		lUserScore;							//玩家金币数量
	LONGLONG        lGameNeed;                          //游戏所需金币
	LONGLONG        lDefultChip;                        //游戏默认所需金币
	LONGLONG        lFinalPrize;                        //最大奖励
	int             nAvailableTimes;                    //今日可玩次数
	int             nTimes;                             //赌小丑倍数
	int             nStarCount;                         //猜对小丑次数
	bool            bIsSelected;                        //是否选中
	bool            bIsRolling;                         //是否在转动
	bool            bLackChip;                          //是否不足下注
};
//开始状态
struct CMD_S_START_STATUS
{
	LONGLONG        lGameNeed;                          //游戏所需金币
	LONGLONG		lUserScore;							//玩家金币数量
	LONGLONG        lDefultChip;                        //游戏默认所需金币
};
//托管状态
struct CMD_S_AOTURUN_STATUS
{
	bool            bIsSelected;                       //是否选中
	bool            bIsRolling;                        //是否在转动
};
//转动状态
struct CMD_S_PICROLL_STATUS
{
	LONGLONG        lGameNeed;                          //游戏所需金币
	LONGLONG		lUserScore;							//玩家金币数量
	LONGLONG		lDefultChip;					    //默认消耗金币量
	int             nAvailableTimes;                    //剩余可玩游戏次数
	bool            bIsRolling;                         //是否在转动
};
//改变下注
struct CMD_S_CHANGEBET_STATUS
{
	LONGLONG        lGameNeed;                          //当前赌注
	LONGLONG		lUserScore;						    //玩家金币数量
	LONGLONG		lDefultChip;						//默认消耗金币量
};
//停止状态
struct CMD_S_STOP_STATUS
{
	bool            bIsRolling;                         //是否在转动
};
//赌小丑状态
struct CMD_S_CLOWNBET_STATUS
{
	int              nStarCount;                         //猜对小丑次数星级
	int              nClownIndex;                        //小丑索引
	bool             bChooseRight;                       //是否猜对小丑
	LONGLONG         lChipPrize;                         //当前赌注
	LONGLONG		 lUserScore;						 //玩家金币数量
	int              nTimes;                             //猜小丑赌注倍数
	bool             bLackChip;                          //是否不足下注
};
//改变倍数
struct CMD_S_CHANGEPRIZE_STATUS
{
	LONGLONG         lGameNeed;                         //当前赌注
	LONGLONG		 lUserScore;						//玩家金币数量
	int              nTimes;                            //下注倍数
};
//结束游戏
struct CMD_S_GETPRIZE_STATUS
{
	WORD			 wUserID;							 //玩家椅号
	LONGLONG         lChipPrize;                         //当前赌注
	LONGLONG         lFinalPrize;                        //最大奖励
	LONGLONG		 lUserScore;					   	 //玩家金币数量
	LONGLONG         lDefultChip;                        //游戏默认所需金币
	LONGLONG         lCurrentChip;                       //当前下注
	BYTE			 bEndMode;						     //结束方式
	int              nPrizeTimes;                        //奖励倍数
	int              nStarCount;                         //猜对小丑次数
	int              nTimes;                             //下注倍数
	bool             bLackChip;                          //是否不足下注
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//客户端命令结构
#define SUB_C_GO_GAME                   300               //开始命令
#define SUB_C_AUTORUN_GAME              306               //托管游戏
#define SUB_C_CHANGE_BET                301               //改变赌注
#define SUB_C_STOP_GAME                 302               //停止命令
#define SUB_C_CLOWN_BET                 303               //判断小丑
#define SUB_C_CHANGE_PRIZE              304               //改变猜小丑奖励
#define SUB_C_GET_PRIZE                 305               //结束游戏

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//开始游戏
struct CMD_C_START_GAME
{
	LONGLONG        lGameNeed;                          //游戏所需金币
	LONGLONG		lUserScore;							//玩家金币数量
	LONGLONG        lDefultChip;                        //游戏默认所需金币
	int             nAvailableTimes;                   //剩余可玩游戏次数
};
//托管游戏
struct CMD_C_AUTORUN_GAME
{
	bool            bIsSelected;                       //是否选中
	bool            bIsRolling;                        //是否在转动
};
//改变下注
struct CMD_C_CHANGE_BET
{
	bool            bPlusChip;                          //增加下注
	bool            bMinusChip;                         //减少下注
	LONGLONG        lGameNeed;                          //当前赌注
	LONGLONG		lUserScore;						    //玩家金币数量
	LONGLONG		lDefultChip;						//默认消耗金币量
};
//停止游戏
struct CMD_C_STOP_SCROLL
{
	bool            bIsRolling;                        //是否在转动
};
//判断小丑
struct CMD_C_CLOWN_BET
{
	int              nStarCount;                         //猜对小丑次数星级
	int              nClownIndex;                        //小丑索引
	bool             bChooseRight;                       //是否猜对小丑
	LONGLONG         lChipPrize;                         //猜小丑赌注
	LONGLONG		 lUserScore;					     //玩家金币数量
	int              nTimes;                             //猜小丑赌注倍数
	bool             bLackChip;                          //是否不足下注
};
//改变猜小丑赌注
struct CMD_C_CHANGE_PRIZE
{
	bool            bTwiceChip;                         //双倍赌注
	bool            bThreeTimesChip;                    //三倍赌注
	bool            bFiveTimesChip;                     //五倍赌注
	LONGLONG        lChipPrize;                         //猜小丑赌注
	LONGLONG		lUserScore;						    //玩家金币数量
	int             nTimes;                             //下注倍数
};
//结束本轮游戏
struct CMD_C_GET_PRIZE
{
	WORD			 wUserID;							//玩家椅号
	LONGLONG         lChipPrize;                        //猜小丑赌注
	LONGLONG         lFinalPrize;                       //最大奖励
	LONGLONG		 lUserScore;						//玩家金币数量
	LONGLONG         lDefultChip;                       //游戏默认所需金币
	LONGLONG         lCurrentChip;                      //当前下注
	BYTE			 bEndMode;						    //结束方式
	int              nPrizeTimes;                       //奖励倍数
	int              nStarCount;                        //猜对小丑次数
	int              nTimes;                            //下注倍数
	bool             bLackChip;                         //是否不足下注
};
////////////////////////////////////////////////////////////////////////////////
#endif