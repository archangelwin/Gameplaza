#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
//�������
protected:
																		                    //��Ϸ�߼�
	ITableFrame							* m_pITableFrame;									//��ܽӿ�
	const tagGameServiceOption			* m_pGameServiceOption;								//���ò���
	tagGameServiceAttrib *				m_pGameServiceAttrib;								//��Ϸ����
	tagCustomRule *						m_pGameCustomRule;									//�Զ�����
//���Ա���
protected:
	static const WORD					m_wPlayerCount;										//��Ϸ����
	static const BYTE					m_GameStartMode;									//��ʼģʽ
//��Ϸ����
	LONGLONG                            m_lUserScore;                                       //�û�ӵ��Ǯ��
	LONGLONG							m_lDefultChip;									    //Ĭ����ע��С
	LONGLONG							m_lCurrentChip;									    //ʵʱ��ע��С
	LONGLONG                            m_lChipPrize;                                       //��ע����
	LONGLONG                            m_lFinalPrize;                                      //���
	int                                 m_nPrizeTimes;                                      //��������
	int                                 m_nStarCount;                                       //��ע��¶Դ���
	int                                 m_nClownIndex;                                      //С������
	int                                 m_nTimes;                                           //��ע����
	int                                 m_nAvailableTimes;                                  //���ʣ��������
	int                                 m_nYear;                                            //�Աȵ�¼����ˢ��ÿ�տ������
	int                                 m_nMonth;
	int                                 m_nDay;
	WORD                                m_wChipUser;                                        //��ǰ���
	bool                                m_bIsSelected;                                      //�Ƿ�ѡ��
	bool                                m_bIsRolling;                                       //�Ƿ���ת��
	bool                                m_bLackChip;                                        //�Ƿ�����ע
	int									m_miniRevenue;
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//��ȡ��Ϸ�ڲ�����
	virtual	LONG GetInsideScore(WORD wChairID) { return NULL; }

	//����ӿ�
public:
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0; }
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) { return 0; }
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID) { return false; }

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//ȷ����ע��
	void EnsureChip();

	//��Ϣ����
public:
	//��ʼ��Ϸ
	bool OnSubStartGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//�й���Ϸ
	bool OnSubAutoRunGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//ֹͣת��
	bool OnSubStopScroll(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//�ı��ע
	bool OnSubChangeBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//�жϲʻ�С��
	bool OnSubClownBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//�ı��С���ע
	bool OnSubChangePrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//������Ϸ
	bool OnSubGetPrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore) {}

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif