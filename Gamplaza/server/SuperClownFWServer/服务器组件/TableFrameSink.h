#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////

//游戏桌子
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
//组件变量
protected:
																		                    //游戏逻辑
	ITableFrame							* m_pITableFrame;									//框架接口
	const tagGameServiceOption			* m_pGameServiceOption;								//配置参数
	tagGameServiceAttrib *				m_pGameServiceAttrib;								//游戏属性
	tagCustomRule *						m_pGameCustomRule;									//自定规则
//属性变量
protected:
	static const WORD					m_wPlayerCount;										//游戏人数
	static const BYTE					m_GameStartMode;									//开始模式
//游戏变量
	LONGLONG                            m_lUserScore;                                       //用户拥有钱数
	LONGLONG							m_lDefultChip;									    //默认下注大小
	LONGLONG							m_lCurrentChip;									    //实时下注大小
	LONGLONG                            m_lChipPrize;                                       //赌注奖励
	LONGLONG                            m_lFinalPrize;                                      //最大奖
	int                                 m_nPrizeTimes;                                      //奖励倍数
	int                                 m_nStarCount;                                       //下注后猜对次数
	int                                 m_nClownIndex;                                      //小丑索引
	int                                 m_nTimes;                                           //下注倍数
	int                                 m_nAvailableTimes;                                  //玩家剩余可玩次数
	int                                 m_nYear;                                            //对比登录日期刷新每日可玩次数
	int                                 m_nMonth;
	int                                 m_nDay;
	WORD                                m_wChipUser;                                        //当前玩家
	bool                                m_bIsSelected;                                      //是否选中
	bool                                m_bIsRolling;                                       //是否在转动
	bool                                m_bLackChip;                                        //是否不足下注
	int									m_miniRevenue;
	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release();
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//获取游戏内部分数
	virtual	LONG GetInsideScore(WORD wChairID) { return NULL; }

	//管理接口
public:
	//配置桌子
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0; }
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) { return 0; }
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID) { return false; }

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//确定下注量
	void EnsureChip();

	//消息处理
public:
	//开始游戏
	bool OnSubStartGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//托管游戏
	bool OnSubAutoRunGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//停止转动
	bool OnSubStopScroll(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//改变赌注
	bool OnSubChangeBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//判断彩灰小丑
	bool OnSubClownBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//改变猜小丑赌注
	bool OnSubChangePrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//结束游戏
	bool OnSubGetPrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore) {}

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif