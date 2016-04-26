#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ�����
#define CHECK_CHIP                  200                                   //ʱ�̼���Һ���ע����
#define GO_TIME_OVER                201                                   //��ʼ5s���Զ�ֹͣ
#define AUTO_TIME_OVER              202                                   //�й���Ϸʱ��
#define GETPRIZE_OVER               203                                   //�Զ���ȡ����

//���캯��
CTableFrameSink::CTableFrameSink()
{
AllocConsole();
freopen("CONOUT$","w+t",stdout);

	srand((unsigned)time(NULL));
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameCustomRule=NULL;
	
	m_nAvailableTimes = 999;
	m_nTimes = 1;
	m_nClownIndex = 0;
	m_nPrizeTimes = 0;
	m_nStarCount = 0;
	m_lUserScore = 0;
	m_lDefultChip = 0;
	m_lCurrentChip = 0;
	m_lChipPrize = 0;
	m_lFinalPrize = 0;
	m_nYear = 2014;
	m_nMonth = 11;
	m_nDay = 1;
	m_bIsRolling = false;
	m_bIsSelected = false;
	m_bLackChip = false;
	m_wChipUser = INVALID_CHAIR;
	m_miniRevenue = 0;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

//�ͷŶ���
VOID  CTableFrameSink::Release()
{
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
#ifdef __SPECIAL___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;

	return NULL;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);

	//��ȡ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);


	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\LiarsDiceConfig.ini"),szPath);

	TCHAR szRoomName[32] = {};
	memcpy(szRoomName, m_pGameServiceOption->szServerName, sizeof(m_pGameServiceOption->szServerName));

	//�Զ�����
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	m_wChipUser = INVALID_CHAIR;

	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
printf("%s, %d\n", __FUNCTION__, __LINE__);
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NETWORK_ERROR:		//�����ж�
	case GER_USER_LEAVE:		//�û��뿪
		{
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	}
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:
		{
			//��������
			CMD_S_GAME_READY gameReady = {0};
			gameReady.lFinalPrize = m_lFinalPrize;
			gameReady.lGameNeed = m_lCurrentChip;
			gameReady.lUserScore = m_lUserScore;
			gameReady.lDefultChip = m_lDefultChip;
			gameReady.wUserID = m_wChipUser;
			gameReady.nAvailableTimes = m_nAvailableTimes;
			gameReady.nTimes = m_nTimes;
			m_nStarCount = 0;
			gameReady.nStarCount = m_nStarCount;
			gameReady.bIsRolling = m_bIsRolling;
			gameReady.bIsSelected = m_bIsSelected;
			gameReady.bLackChip = m_bLackChip;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &gameReady,sizeof(gameReady));
		}
		break;
	case GAME_SCENE_GO:
		{
			CMD_S_GAME_START chipStart = {0};
			chipStart.lGameNeed = m_lCurrentChip;
			chipStart.lUserScore = m_lUserScore;
			chipStart.lDefultChip = m_lDefultChip;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &chipStart, sizeof(chipStart));
		}
		break;
	case GAME_SCENE_AUTORUN:
		{
			CMD_S_AUTORUN_GAME autoRun = {0};
			autoRun.bIsRolling = m_bIsRolling;
			autoRun.bIsSelected = m_bIsSelected;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &autoRun, sizeof(autoRun));
		}
		break;
	case GAME_SCENE_PICTURES_ROLL:
		{
			CMD_S_PICTURES_ROLL picsRoll = {0};
			picsRoll.lGameNeed = m_lCurrentChip;
			picsRoll.lUserScore = m_lUserScore;
			picsRoll.bIsRolling = m_bIsRolling;
			picsRoll.nAvailableTimes = m_nAvailableTimes;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &picsRoll, sizeof(picsRoll));
		}
		break;
	case GAME_SCENE_CHANGE_BET:
		{
			CMD_S_CHANGE_BET changeBet = {0};
			changeBet.lGameNeed = m_lCurrentChip;
			changeBet.lUserScore = m_lUserScore;
			changeBet.lDefultChip = m_lDefultChip;
			return m_pITableFrame->SendGameScene(pIServerUserItem,&changeBet, sizeof(changeBet));
		}
		break;
	case GAME_SCENE_STOP:
		{
			CMD_S_STOP_SCROLL stopScroll;
			ZeroMemory(&stopScroll, sizeof(stopScroll));
			stopScroll.bIsRolling = m_bIsRolling;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &stopScroll, sizeof(stopScroll));
		}
		break;
	case GAME_SCENE_CHANGE_PRIZE:
		{
			CMD_S_CHANGE_PRIZE changePrize = {0};
			changePrize.lChipPrize = m_lChipPrize;
			changePrize.lUserScore = m_lUserScore;
			changePrize.nTimes = m_nTimes;

			return m_pITableFrame->SendGameScene(pIServerUserItem, &changePrize, sizeof(changePrize));
		}
		break;
	case GAME_SCENE_END:
		{
			CMD_S_GET_PRIZE gameEnd;
			ZeroMemory(&gameEnd, sizeof(gameEnd));
			gameEnd.lChipPrize = m_lChipPrize;
			gameEnd.lUserScore = m_lUserScore + m_lChipPrize;
			EnsureChip();
			gameEnd.lDefultChip = m_lDefultChip;
			gameEnd.lCurrentChip = m_lCurrentChip;
			gameEnd.nPrizeTimes = m_nPrizeTimes;
			gameEnd.lFinalPrize = m_lFinalPrize;
			gameEnd.nStarCount = m_nStarCount;
			m_nTimes = 1;
			gameEnd.nTimes = m_nTimes;
			m_bLackChip = false;
			gameEnd.bLackChip = m_bLackChip;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &gameEnd, sizeof(gameEnd));
		}
		break;
	}

	return false;
}

//ʱ�����
bool CTableFrameSink:: OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case CHECK_CHIP:
		{
		}
		break;
	case GO_TIME_OVER:
		{

		}
		break;
	case AUTO_TIME_OVER:
		{

		}
		break;
	case GETPRIZE_OVER:
		{

		}
		break;
	}

	return true;
}

//�����¼�
bool CTableFrameSink:: OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}

//�����¼�
bool CTableFrameSink:: OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}

//ȷ����ע��
void CTableFrameSink::EnsureChip()
{
	LONGLONG lDefultChip = m_lDefultChip;
	if(m_lUserScore >= 550000000)
		m_lDefultChip = m_lUserScore/10;
	else if(m_lUserScore >= 55000000)
		m_lDefultChip = 5000000;
	else if (m_lUserScore >= 5500000)
		m_lDefultChip = 500000;
	else if(m_lUserScore >= 550000)
		m_lDefultChip = 50000;
	else if (m_lUserScore >= 55000)
		m_lDefultChip = 5000;
	else if (m_lUserScore >= 5500)
		m_lDefultChip = 500;
	else if (m_lUserScore >= 550)
		m_lDefultChip = 50;
	else if (m_lUserScore >= 55)
		m_lDefultChip = 5;
	else if(m_lUserScore > 0)
		m_lDefultChip = 1;
	else if (m_lUserScore <= 0)
	{
		m_lUserScore = 0;
		m_lCurrentChip = 0;
		m_lDefultChip = 0;
	}
	if (m_lDefultChip != lDefultChip || m_lUserScore < m_lCurrentChip)
	{
		m_lCurrentChip = m_lDefultChip;
	}
}
//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ǿ�����ó���Ϸ״̬
	pIServerUserItem->SetUserStatus(US_PLAYING, pIServerUserItem->GetTableID(), wChairID);
	//��ȡ��ҽ������
	m_wChipUser = 0;
	m_lUserScore = m_pITableFrame->GetTableUserItem(m_wChipUser)->GetUserScore()/10;//��ҽ������
	EnsureChip();//ȷ����ע
	m_lCurrentChip = m_lDefultChip;
	int nYear = m_nYear;
	int nMonth = m_nMonth;
	int nDay = m_nDay;
	unsigned long long timestamp = time(NULL);
	struct tm *ptm = localtime((time_t*)&timestamp);
	m_nYear = ptm->tm_year + 1900;//��1900��
	m_nMonth = ptm->tm_mon;
	m_nDay = ptm->tm_mday;
	if (m_nYear-nYear>0 || (m_nYear==nYear && m_nMonth>nMonth) || (m_nYear==nYear && m_nMonth==nMonth && m_nDay>nDay))
	{
		m_nAvailableTimes = 999;
	}

	//������Ϣ
	CMD_S_GAME_READY gameReady = {0};
	gameReady.lFinalPrize = m_lFinalPrize;
	gameReady.lGameNeed = m_lCurrentChip;
	gameReady.lUserScore = m_lUserScore;
	gameReady.lDefultChip = m_lDefultChip;
	gameReady.wUserID = m_wChipUser;
	gameReady.nAvailableTimes = m_nAvailableTimes;
	gameReady.nTimes = m_nTimes;
	gameReady.nStarCount = m_nStarCount;
	gameReady.bIsRolling = m_bIsRolling;
	gameReady.bIsSelected = m_bIsSelected;
	gameReady.bLackChip = m_bLackChip;

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_READY, &gameReady, sizeof(gameReady));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_READY, &gameReady, sizeof(gameReady));

	return true;
}
//�ͻ�����Ϣ�¼�
bool CTableFrameSink:: OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GO_GAME:
		{
			const WORD wChairID = pIServerUserItem->GetUserInfo()->wChairID;
			//������֤
			ASSERT(INVALID_CHAIR!=wChairID);
			if (INVALID_CHAIR == wChairID)
				printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
			if (m_nAvailableTimes<=0 || m_lUserScore <= 0)
				return true;
			//���տͻ�����Ϣ
			CMD_C_START_GAME* startGame = (CMD_C_START_GAME*)pData;
			m_nAvailableTimes = startGame->nAvailableTimes;
			m_lUserScore = startGame->lUserScore;
			m_lCurrentChip = startGame->lGameNeed;
			m_lDefultChip = startGame->lDefultChip;
			EnsureChip();
			if (m_lUserScore < m_lCurrentChip)
				return true;
			//����ת����Ϣ
			CMD_S_PICTURES_ROLL picsRoll = {0};
			m_nAvailableTimes = m_nAvailableTimes--;
			picsRoll.nAvailableTimes = m_nAvailableTimes;
			m_lUserScore -= m_lCurrentChip;
			picsRoll.lUserScore = m_lUserScore;
			picsRoll.lGameNeed = m_lCurrentChip;
			picsRoll.lDefultChip = m_lDefultChip;
			m_bIsRolling = true;
			picsRoll.bIsRolling = m_bIsRolling;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PICTURES_ROLL,&picsRoll,sizeof(picsRoll));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PICTURES_ROLL,&picsRoll,sizeof(picsRoll));
			pIServerUserItem->WriteUserScore(-m_lCurrentChip, 0, 0, 0, SCORE_TYPE_LOSE, 0);

			const tagStockInfo* pStockInfo = NULL;
			pStockInfo = m_pITableFrame->GetStockInfo(0, m_pITableFrame->GetTableID(), eStockType_Player);//---------------��ȡ���ṹ��
			int nStorage = pStockInfo->mStockScore;//������
printf("--------------storage=%d--------------\n",nStorage);
			int nRevenue = m_pGameServiceOption->wRevenueRatio;//˰�ձ���
			printf("nRevenue=%d\n",nRevenue);
			int nRevenueChip = 0;//û��ʹ��---(  ��ȡ��nRevenue/1000����˰��(ʵ��˰�ձ���50/1000=20) )
			//if (nStorage >= nRevenueChip)
			//{
			//	nStorage -= nRevenueChip;//Ҫд����
			//}

			if (nRevenue>0 && m_lCurrentChip>0)
			{
				nRevenueChip= m_lCurrentChip*nRevenue;
				nRevenueChip += m_miniRevenue;
				m_miniRevenue = nRevenueChip%1000;
				nRevenueChip = nRevenueChip/1000;

				if (nRevenueChip>m_lCurrentChip)
				{
					nRevenueChip = m_lCurrentChip;
				}
			}

			printf("m_miniRevenue=%d nRevenueChip=%d \n",m_lCurrentChip,m_miniRevenue,nRevenueChip);

			float fUserLucky = 1;//m_pITableFrame->GetUserFactor(pIServerUserItem->GetUserID());//����ֵ0.6~2.9
			int n = (int)(fUserLucky*(rand()%116)) + 1;
			//���ͽ����Ϣ
			CMD_S_SEND_RESULT sendResult = {0};
//printf("%d %d\n",n,fUserLucky);
			if(n > 69 && nStorage > m_lCurrentChip*10 && nStorage>0)//�������ж�
			{
	
				sendResult.bPictureIsAllSame = true;
				int nSprite = rand()%100;
				int nSpriteID = 0;
				if (nSprite < 40)
					nSpriteID = 9;
				else if (nSprite < 70)
					nSpriteID = 8;
				else if (nSprite < 85)
					nSpriteID = 7;
				else if (nSprite < 92)
					nSpriteID = 6;
				else if (nSprite < 95)
					nSpriteID = 5;
				else if (nSprite < 96)
					nSpriteID = 4;
				else if (nSprite < 97)
					nSpriteID = 3;
				else if (nSprite < 98)
					nSpriteID = 2;
				else if (nSprite < 99)
					nSpriteID = 1;
				else if (nSprite < 100)
					nSpriteID = 0;
				m_nPrizeTimes = 10-nSpriteID;
				sendResult.nPrizeTimes = m_nPrizeTimes;
				sendResult.lChipPrize = m_nPrizeTimes*m_lCurrentChip;
				for (int i = 0; i < 3; i++)
				{
					sendResult.bResultData[i] = nSpriteID;
				}
			}
			else
			{
				sendResult.bPictureIsAllSame = false;
				m_pITableFrame->ChangeStock(eStockType_Player, wChairID, (int)m_lCurrentChip-nRevenueChip, -nRevenueChip, 0);//û�г�����ͬͼ����Ѷ�ע������
printf("�䣺  ���=%d, ��ע=%lld  ˰��=%d\n",m_pITableFrame->GetStockInfo(0, m_pITableFrame->GetTableID(),eStockType_Player)->mStockScore,m_lCurrentChip,nRevenueChip);
				EnsureChip();
				m_nPrizeTimes = 0;
				sendResult.lChipPrize = 0;
				int nSprite0 = rand()%10;
				int nSprite1 = rand()%10;
				int nSprite2 = rand()%10;
				if (nSprite0 == nSprite1 && nSprite1 == nSprite2)
				{
					sendResult.bResultData[0] = 0;
					sendResult.bResultData[1] = 1;
					sendResult.bResultData[2] = 0;
				}
				else
				{
					sendResult.bResultData[0] = nSprite0;
					sendResult.bResultData[1] = nSprite1;
					sendResult.bResultData[2] = nSprite2;
				}
			}
			m_nStarCount = 0;
			m_bLackChip = false;
			sendResult.nStarCount = m_nStarCount;
			sendResult.bLackChip = m_bLackChip;
			sendResult.lCurrentChip = m_lCurrentChip;
			sendResult.lDefultChip = m_lDefultChip;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_RESLUT,&sendResult,sizeof(sendResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_RESLUT,&sendResult,sizeof(sendResult));

			return true;
			/*tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubStartGame(pUserData->wChairID , pData, wDataSize);*/
		}
	case SUB_C_AUTORUN_GAME:
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubAutoRunGame(pUserData->wChairID , pData, wDataSize);
		}
	case SUB_C_CHANGE_BET:
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubChangeBet(pUserData->wChairID , pData, wDataSize);
		}
	case SUB_C_STOP_GAME:
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubStopScroll(pUserData->wChairID , pData, wDataSize);
		}
	case SUB_C_CLOWN_BET:
		{
			const WORD wChairID = pIServerUserItem->GetUserInfo()->wChairID;
			//������֤
			ASSERT(INVALID_CHAIR!=wChairID);
			if (INVALID_CHAIR == wChairID)
				printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);

			CMD_C_CLOWN_BET* clownBet = (CMD_C_CLOWN_BET*)pData;
			m_lChipPrize = clownBet->lChipPrize;
			m_lUserScore = clownBet->lUserScore;
			m_nClownIndex = clownBet->nClownIndex;
			m_nStarCount = clownBet->nStarCount;
			m_nTimes = clownBet->nTimes;
			m_bLackChip = clownBet->bLackChip;
			//������Ϣ
			CMD_S_CLOWN_BET clown = {0};
			m_bLackChip = false;
			const tagStockInfo* pStockInfo = NULL;
			pStockInfo = m_pITableFrame->GetStockInfo(0, m_pITableFrame->GetTableID(), eStockType_Player);
			int nStorage = pStockInfo->mStockScore;
			printf("���=%d\n",nStorage);
			if (nStorage - m_lChipPrize*2 < 0)//���С�ڵ�ǰ��ע����עʧ��  ���߿��Ϊ0��С��ʧ��
				clown.bChooseRight = false;
			else
			{
				if (nStorage<0)
				{
					clown.bChooseRight = false; //���Ϊ0��С��ʧ��(�������ж�)
				}
				else
				{
					float fUserLucky = 1;//m_pITableFrame->GetUserFactor(pIServerUserItem->GetUserID());//����ֵ0.6~2.9
					int n = (int)(fUserLucky*(rand()%116)) + 1;
					if (m_nStarCount < 5)//ǰ����ǣ��¶Ը���1/2
					{
						if (n > 59)
							clown.bChooseRight = true;
						else
							clown.bChooseRight = false;
					}
					else if(m_nStarCount < 7)//�����߿��ǣ��¶Ը���1/3
					{
						if (n > 78)
							clown.bChooseRight = true;
						else
							clown.bChooseRight = false;
					}
					else if(m_nStarCount == 7)//�ڰ˿��ǣ��¶Ը���1/4
					{
						if (n > 88)
							clown.bChooseRight = true;
						else
							clown.bChooseRight = false;
					}
					
				}
				
			}
			//����������=8ʱ��ֱ��������Ϸ���棬��������>4�ж��⽱��
			if (clown.bChooseRight == true)
			{
				pIServerUserItem->WriteUserScore(-m_lChipPrize/m_nTimes*(m_nTimes-1), 0, 0, 0, SCORE_TYPE_LOSE, 0);//��ȥ��Ͷ�ı���
				clown.lChipPrize = m_lChipPrize*2;
				clown.lUserScore = m_lUserScore;
				clown.nStarCount = ++m_nStarCount;
				clown.nClownIndex = m_nClownIndex;
				m_nTimes = 1;
			}
			else
			{
				m_pITableFrame->ChangeStock(eStockType_Player, wChairID, (int)m_lChipPrize, 0, 0);//��עʧ�������ļ�����
printf("��עʧ�ܣ�Add  mStockScore=%d, m_lChipPrize %lld\n", m_pITableFrame->GetStockInfo(0, m_pITableFrame->GetTableID(),eStockType_Player)->mStockScore, m_lChipPrize);
				if (m_lUserScore >= m_lChipPrize)
				{
					pIServerUserItem->WriteUserScore(-m_lChipPrize, 0, 0, 0, SCORE_TYPE_LOSE, 0);
					clown.lUserScore = m_lUserScore - m_lChipPrize;
				}
				else
				{
					pIServerUserItem->WriteUserScore(-m_lChipPrize/m_nTimes*(m_nTimes-1), 0, 0, 0, SCORE_TYPE_LOSE, 0);
					clown.lUserScore = m_lUserScore;
					m_lChipPrize = 0;
					m_bLackChip = true;
				}
				clown.lChipPrize = m_lChipPrize;
				clown.nStarCount = m_nStarCount;
				clown.nClownIndex = -m_nClownIndex;
			}
			clown.nTimes = m_nTimes;
			clown.bLackChip = m_bLackChip;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CLOWN_BET,&clown,sizeof(clown));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CLOWN_BET,&clown,sizeof(clown));
			return true;
			/*tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubClownBet(pUserData->wChairID , pData, wDataSize);*/
		}
	case SUB_C_CHANGE_PRIZE:
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubChangePrize(pUserData->wChairID , pData, wDataSize);
		}
	case SUB_C_GET_PRIZE:
		{
			const WORD wChairID = pIServerUserItem->GetUserInfo()->wChairID;
			//������֤
			ASSERT(INVALID_CHAIR!=wChairID);
			if (INVALID_CHAIR == wChairID)
				printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
			//���տͻ�����Ϣ
			CMD_C_GET_PRIZE* cGameEnd = (CMD_C_GET_PRIZE*)pData;
			m_wChipUser = cGameEnd->wUserID;
			m_lChipPrize = cGameEnd->lChipPrize;
			m_lUserScore = cGameEnd->lUserScore + m_lChipPrize;
			m_lDefultChip = cGameEnd->lDefultChip;
			m_lCurrentChip = cGameEnd->lCurrentChip;
			m_nPrizeTimes = cGameEnd->nPrizeTimes;
			m_nStarCount = cGameEnd->nStarCount;
			m_nTimes = cGameEnd->nTimes;
			m_bLackChip = cGameEnd->bLackChip;
			m_lFinalPrize = cGameEnd->lFinalPrize;
			pIServerUserItem->WriteUserScore(m_lFinalPrize, 0, 0, 0, SCORE_TYPE_WIN, 0);
printf("mStockScore=%d\n",m_pITableFrame->GetStockInfo(0, m_pITableFrame->GetTableID(), eStockType_Player)->mStockScore);
			m_pITableFrame->ChangeStock(eStockType_Player, wChairID, (int)-m_lFinalPrize, 0, 0);//������ý�Ҵӿ���ȥ
printf("���Ӯ��minus  mStockScore=%d  m_lFinalPrize=%lld\n", m_pITableFrame->GetStockInfo(0, m_pITableFrame->GetTableID(), eStockType_Player)->mStockScore, m_lFinalPrize);
			EnsureChip();//ˢ����ע��
			//�����ս���Ϣ
			CMD_S_GET_PRIZE sGameEnd = {0};
			sGameEnd.wUserID = m_wChipUser;
			sGameEnd.lFinalPrize = m_lFinalPrize;
			sGameEnd.lUserScore = m_lUserScore;
			sGameEnd.lDefultChip = m_lDefultChip;
			sGameEnd.lCurrentChip = m_lCurrentChip;
			m_lChipPrize = 0;
			sGameEnd.lChipPrize = m_lChipPrize;
			sGameEnd.nPrizeTimes = m_nPrizeTimes;
			sGameEnd.nStarCount = m_nStarCount;
			m_nTimes = 1;
			sGameEnd.nTimes = m_nTimes;
			sGameEnd.bLackChip = m_bLackChip;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GET_PRIZE,&sGameEnd,sizeof(sGameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GET_PRIZE,&sGameEnd,sizeof(sGameEnd));
			return true;
			/*tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubGetPrize(pUserData->wChairID , pData, wDataSize);*/
		}
	}
	return false;
}
//��ʼ��Ϸ
bool CTableFrameSink::OnSubStartGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	return true;
}
//�Զ���Ϸ
bool CTableFrameSink::OnSubAutoRunGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	//������֤
	ASSERT(INVALID_CHAIR!=wChairID);
	if (INVALID_CHAIR == wChairID)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
	CMD_C_AUTORUN_GAME* cAutoRun = (CMD_C_AUTORUN_GAME*)pDataBuffer;
	m_bIsRolling = cAutoRun->bIsRolling;
	m_bIsSelected = cAutoRun->bIsSelected;
	CMD_S_AUTORUN_GAME sAutoRun = {0};
	m_bIsSelected = !m_bIsSelected;
	sAutoRun.bIsRolling = m_bIsRolling;
	sAutoRun.bIsSelected = m_bIsSelected;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_AUTORUN_GAME,&sAutoRun,sizeof(sAutoRun));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_AUTORUN_GAME,&sAutoRun,sizeof(sAutoRun));
	return true;
}
//�ı������ע
bool CTableFrameSink::OnSubChangeBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	//������֤
	ASSERT(INVALID_CHAIR!=wChairID);
	if (INVALID_CHAIR == wChairID)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);

	CMD_C_CHANGE_BET* cChangeBet = (CMD_C_CHANGE_BET*)pDataBuffer;
	m_lCurrentChip = cChangeBet->lGameNeed;
	m_lUserScore = cChangeBet->lUserScore;
	m_lDefultChip = cChangeBet->lDefultChip;
	CMD_S_CHANGE_BET sChangeBet = {0};
	//���Ӷ�ע
	if (cChangeBet->bPlusChip == true)
	{
		if (m_lUserScore>m_lCurrentChip)
		{
			if (m_lCurrentChip<m_lDefultChip*2 && m_lDefultChip != 1)
				m_lCurrentChip += m_lDefultChip/5;
			else if(m_lCurrentChip < 5 && m_lDefultChip == 1)
				m_lCurrentChip += m_lDefultChip;
			
		}
		sChangeBet.lGameNeed = m_lCurrentChip;
		sChangeBet.lDefultChip = m_lDefultChip;
	}
	//���ٶ�ע
	else if(cChangeBet->bMinusChip == true)
	{
		if (m_lCurrentChip>m_lDefultChip/5 && m_lDefultChip != 1)
			m_lCurrentChip -= m_lDefultChip/5;
		else if (m_lCurrentChip > 1 && m_lDefultChip == 1)
			m_lCurrentChip -= m_lDefultChip;
		sChangeBet.lGameNeed = m_lCurrentChip;
		sChangeBet.lDefultChip = m_lDefultChip;
	}
	sChangeBet.lUserScore = m_lUserScore;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BET,&sChangeBet,sizeof(sChangeBet));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BET,&sChangeBet,sizeof(sChangeBet));
	return true;
}
//ֹͣת��
bool CTableFrameSink::OnSubStopScroll(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	//������֤
	ASSERT(INVALID_CHAIR!=wChairID);
	if (INVALID_CHAIR == wChairID)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
	//���տͻ�����Ϣ
	CMD_C_STOP_SCROLL* cStopRoll = (CMD_C_STOP_SCROLL*)pDataBuffer;
	m_bIsRolling = cStopRoll->bIsRolling;
	//������Ϣ
	CMD_S_STOP_SCROLL sStopRoll = {0};
	m_bIsRolling = false;
	sStopRoll.bIsRolling = m_bIsRolling;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_SCROLL,&sStopRoll,sizeof(sStopRoll));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STOP_SCROLL,&sStopRoll,sizeof(sStopRoll));

	return true;
}
//�жϲʻ�С��
bool CTableFrameSink::OnSubClownBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	return true;
}
//�ı��С���ע����
bool CTableFrameSink::OnSubChangePrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	//������֤
	ASSERT(INVALID_CHAIR!=wChairID);
	if (INVALID_CHAIR == wChairID)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);

	CMD_C_CHANGE_PRIZE* cChangePrize = (CMD_C_CHANGE_PRIZE*)pDataBuffer;
	m_lChipPrize = cChangePrize->lChipPrize;
	m_lUserScore = cChangePrize->lUserScore;
	m_nTimes = cChangePrize->nTimes;
	CMD_S_CHANGE_PRIZE sChangePrize = {0};
	//������ע
	if (cChangePrize->bTwiceChip == true)
	{
		if (m_nTimes == 1)
		{
			if (m_lUserScore >= m_lChipPrize)
			{
				m_lChipPrize = m_lChipPrize*2;
				sChangePrize.lChipPrize = m_lChipPrize;
				m_lUserScore = m_lUserScore - m_lChipPrize/2;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = 2;
			}
			else
			{
				sChangePrize.lChipPrize = m_lChipPrize;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = m_nTimes;
			}
		}
		else if (m_nTimes == 2)
		{
			m_lChipPrize = m_lChipPrize/2;
			sChangePrize.lChipPrize = m_lChipPrize;
			m_lUserScore = m_lUserScore + m_lChipPrize;
			sChangePrize.lUserScore = m_lUserScore;
			sChangePrize.nTimes = 1;
		}
		else if (m_nTimes == 3)
		{
			m_lChipPrize = m_lChipPrize*2/3;
			sChangePrize.lChipPrize = m_lChipPrize;
			m_lUserScore = m_lUserScore + m_lChipPrize/2;
			sChangePrize.lUserScore = m_lUserScore;
			sChangePrize.nTimes = 2;
		}
		else if (m_nTimes == 5)
		{
			m_lChipPrize = m_lChipPrize*2/5;
			sChangePrize.lChipPrize = m_lChipPrize;
			m_lUserScore = m_lUserScore + m_lChipPrize*3/2;
			sChangePrize.lUserScore =  m_lUserScore;
			sChangePrize.nTimes = 2;
		}
	}
	//����
	else if (cChangePrize->bThreeTimesChip == true)
	{
		if (m_nTimes == 1)
		{
			if (m_lUserScore >= m_lChipPrize*2)
			{
				m_lChipPrize = m_lChipPrize*3;
				sChangePrize.lChipPrize = m_lChipPrize;
				m_lUserScore = m_lUserScore - m_lChipPrize*2/3;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = 3;
			}
			else
			{
				sChangePrize.lChipPrize = m_lChipPrize;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = m_nTimes;
			}
		}
		else if (m_nTimes == 2)
		{
			if (m_lUserScore >= m_lChipPrize/2)
			{
				m_lChipPrize = m_lChipPrize*3/2;
				sChangePrize.lChipPrize = m_lChipPrize;
				m_lUserScore = m_lUserScore - m_lChipPrize/3;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = 3;
			}
			else
			{
				sChangePrize.lChipPrize = m_lChipPrize;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = m_nTimes;
			}
		}
		else if (m_nTimes == 3)
		{
			m_lChipPrize = m_lChipPrize/3;
			sChangePrize.lChipPrize = m_lChipPrize;
			m_lUserScore = m_lUserScore + 2*m_lChipPrize;
			sChangePrize.lUserScore = m_lUserScore;
			sChangePrize.nTimes = 1;
		}
		else if (m_nTimes == 5)
		{
			m_lChipPrize = m_lChipPrize*3/5;
			sChangePrize.lChipPrize = m_lChipPrize;
			m_lUserScore = m_lUserScore + m_lChipPrize*2/3;
			sChangePrize.lUserScore = m_lUserScore;
			sChangePrize.nTimes = 3;
		}
	}
	//�屶
	else if (cChangePrize->bFiveTimesChip == true)
	{
		if (m_nTimes == 1)
		{
			if (m_lUserScore >= m_lChipPrize*4)
			{
				m_lChipPrize = m_lChipPrize*5;
				sChangePrize.lChipPrize = m_lChipPrize;
				m_lUserScore = m_lUserScore - m_lChipPrize*4/5;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = 5;
			}
			else
			{
				sChangePrize.lChipPrize = m_lChipPrize;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = m_nTimes;
			}
		}
		else if (m_nTimes == 2)
		{
			if (m_lUserScore >= m_lChipPrize*3/2)
			{
				m_lChipPrize = m_lChipPrize*5/2;
				sChangePrize.lChipPrize = m_lChipPrize;
				m_lUserScore = m_lUserScore - m_lChipPrize*3/5;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = 5;
			}
			else
			{
				sChangePrize.lChipPrize = m_lChipPrize;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = m_nTimes;
			}
		}
		else if (m_nTimes == 3)
		{
			if (m_lUserScore >= m_lChipPrize*2/3)
			{
				m_lChipPrize = m_lChipPrize*5/3;
				sChangePrize.lChipPrize = m_lChipPrize;
				m_lUserScore = m_lUserScore - m_lChipPrize*2/5;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = 5;
			}
			else
			{
				sChangePrize.lChipPrize = m_lChipPrize;
				sChangePrize.lUserScore = m_lUserScore;
				sChangePrize.nTimes = m_nTimes;
			}
		}
		else if (m_nTimes == 5)
		{
			m_lChipPrize = m_lChipPrize/5;
			sChangePrize.lChipPrize = m_lChipPrize;
			m_lUserScore = m_lUserScore + 4*m_lChipPrize;
			sChangePrize.lUserScore = m_lUserScore;
			sChangePrize.nTimes = 1;
		}
	}
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_PRIZE,&sChangePrize,sizeof(sChangePrize));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_PRIZE,&sChangePrize,sizeof(sChangePrize));

	return true;
}
//�ս�
bool CTableFrameSink::OnSubGetPrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	return true;
}
//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if ( bLookonUser )
		return true;

	//���ñ���
	m_nTimes = 1;
	m_nStarCount = 0;
	m_nClownIndex = 0;
	m_nPrizeTimes = 0;
	m_lUserScore = 0;
	m_lCurrentChip = 0;
	m_lChipPrize = 0;
	m_bIsRolling = false;
	m_bIsSelected = false;
	m_bLackChip = false;
	m_wChipUser = INVALID_CHAIR;

	return true;
}
//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}