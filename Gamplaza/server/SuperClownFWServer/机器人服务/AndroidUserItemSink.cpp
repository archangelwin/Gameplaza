#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS						1									//最少时间

//游戏时间


#define TIME_CHECK_BANKER				30									//摊牌时间

//游戏时间
#define IDI_START_GAME					(100)									//开始定时器
#define IDI_CALL_BANKER					(101)									//叫庄定时器
#define IDI_USER_ADD_SCORE				(102)									//下注定时器
#define IDI_OPEN_CARD					(103)									//开牌定时器

#define IDI_CHECK_BANKER_OPERATE        (104)									//检查定时器

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_nRobotBankStorageMul=0;
	m_lRobotBankGetScore=0;
	m_lRobotBankGetScoreBanker=0;
	ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));

	//接口变量
	m_pIAndroidUserItem=NULL;
	srand((unsigned)time(NULL));   

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	printf("接口查询\n");
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	ReadConfigInformation();

	//检查银行
	UINT nElapse=TIME_CHECK_BANKER+rand()%100;
	m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER_OPERATE,nElapse);

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	printf("重置接口 %d\n",__LINE__);
	printf("------------------------------------------\n");

	//检查银行
	UINT nElapse=TIME_CHECK_BANKER+rand()%100;
	m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER_OPERATE,nElapse);

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER_OPERATE://检查银行
		{

			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			if(pUserItem->GetUserStatus()<US_SIT)
			{
				ReadConfigInformation();
				BankOperate(1);
			}
			//检查银行
			UINT nElapse=TIME_CHECK_BANKER+rand()%100;
			m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER_OPERATE,nElapse);

			return true;
		}
	}

	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//switch (wSubCmdID)
	//{
	//case SUB_S_CALL_BANKER:	//用户叫庄
	//	{
	//		//消息处理
	//		return OnSubCallBanker(pData,wDataSize);
	//	}
	//case SUB_S_GAME_START:	//游戏开始
	//	{
	//		//消息处理
	//		return OnSubGameStart(pData,wDataSize);
	//	}
	//case SUB_S_ADD_SCORE:	//用户下注
	//	{
	//		//消息处理
	//		return OnSubAddScore(pData,wDataSize);
	//	}
	//case SUB_S_SEND_CARD:	//发牌消息
	//	{
	//		//消息处理
	//		return OnSubSendCard(pData,wDataSize);
	//	}
	//case SUB_S_OPEN_CARD:	//用户摊牌
	//	{
	//		//消息处理
	//		return OnSubOpenCard(pData,wDataSize);
	//	}
	//case SUB_S_PLAYER_EXIT:	//用户强退
	//	{
	//		//消息处理
	//		return OnSubPlayerExit(pData,wDataSize);
	//	}
	//case SUB_S_GAME_END:	//游戏结束
	//	{
	//		//消息处理
	//		return OnSubGameEnd(pData,wDataSize);
	//	}
	//case SUB_S_ALL_CARD:
	//	{
	//		return true;
	//	}
	//case SUB_S_BANKER_OPERATE:
	//	{
	//		//BankOperate();
	//		return true;
	//	}
	//}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
printf("Android 游戏消息 %d\n",__LINE__);
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	printf("Android 场景消息 %d\n",__LINE__);
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_READY_STATUS)) return false;

			//消息处理
			CMD_S_READY_STATUS * pStatusFree=(CMD_S_READY_STATUS *)pData;

			//memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation();

			BankOperate(2);

			//开始时间
			UINT nElapse=rand()%(15)+3;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	printf("用户进入\n");
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	printf("用户离开\n");
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户叫庄
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{


	return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//用户摊牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{

	return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//银行操作
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pUserItem->GetUserStatus()>=US_SIT)
	{
		if(cbType==1)
		{
			CString strInfo;
			strInfo.Format(TEXT("大厅：状态不对，不执行存取款"));
		}
		return;
	}

	//变量定义
	LONGLONG lRobotScore = pUserItem->GetUserScore();

	CString strInfo;
	strInfo.Format(TEXT("配置信息：取款条件(%I64d)，存款条件(%I64d),机器人分数(%I64d)"),m_lRobotScoreRange[0],m_lRobotScoreRange[1],lRobotScore);
}

//读取配置
void CAndroidUserItemSink::ReadConfigInformation()
{
	printf("读取配置 %d\n",__LINE__);
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	myprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\OxConfig.ini"), szPath);

	//分数限制
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMin"), _T("100000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[0]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMax"), _T("10000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[1]);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//提款数额

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGet"), _T("1000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScore);

	//提款数额 (庄家)
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGetBanker"), _T("500000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScoreBanker);

	//存款倍数
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

	if (m_nRobotBankStorageMul<0||m_nRobotBankStorageMul>100) m_nRobotBankStorageMul =20;
}


//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

