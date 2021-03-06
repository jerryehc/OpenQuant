#pragma once
#include "Include/FTPluginCore.h"
#include "Include/FTPluginQuoteInterface.h"
#include "Include/FTPluginTradeInterface.h"
#include "Protocol/ProtoDataStruct_Quote.h"
#include "TimerWnd.h"
#include "MsgHandler.h"
#include "JsonCpp/json.h"

class CPluginQuoteServer;

class CPluginBrokerQueue : public CTimerWndInterface, public CMsgHandlerEventInterface
{
public:
	CPluginBrokerQueue();
	virtual ~CPluginBrokerQueue();
	
	void Init(CPluginQuoteServer* pQuoteServer, IFTQuoteData*  pQuoteData);
	void Uninit();	
	void SetQuoteReqData(int nCmdID, const Json::Value &jsnVal, SOCKET sock);
	void NotifyQuoteDataUpdate(int nCmdID, INT64 nStockID);
	void PushStockData(INT64 nStockID, SOCKET sock);

	void NotifySocketClosed(SOCKET sock);

protected:
	//CTimerWndInterface
	virtual void OnTimeEvent(UINT nEventID);

	//CMsgHandlerEventInterface
	virtual void OnMsgEvent(int nEvent,WPARAM wParam,LPARAM lParam);

protected:
	struct	StockDataReq
	{
		SOCKET	sock;
		DWORD	dwReqTick; 
		INT64	nStockID;		
		BrokerQueue_Req req;
	};
	struct StockMktCode
	{
		int nMarketType;
		std::string strCode;
	};

	//tomodify 1
	typedef BrokerQueueAckBody	QuoteAckDataBody;
	typedef std::vector<StockDataReq*>			VT_STOCK_DATA_REQ;

protected:
	void ReleaseAllReqData();
	void HandleTimeoutReq();
	void ReplyAllReadyReq();
	void ReplyStockDataReq(StockDataReq *pReq, const QuoteAckDataBody &data);
	void ReplyDataReqError(StockDataReq *pReq, int nErrCode, LPCWSTR pErrDesc);	
	void SetTimerHandleTimeout(bool bStartOrStop);
	
private:
	bool DoFillAckDataBody(INT64 nStockID, QuoteAckDataBody& ackbody);
	void DoClearReqInfo(SOCKET socket);

protected:
	CPluginQuoteServer* m_pQuoteServer;
	IFTQuoteData*		m_pQuoteData;
	CTimerMsgWndEx		m_TimerWnd;
	CMsgHandler			m_MsgHandler;
	BOOL m_bStartTimerHandleTimeout;

	VT_STOCK_DATA_REQ m_vtReqData;
};