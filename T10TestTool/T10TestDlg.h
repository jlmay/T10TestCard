// T10TestDlg.h : header file for main dialog
#pragma once

#include "dcrf32.h"
#include <afxmt.h>

// Worker thread message
#define WM_UPDATE_LOG        (WM_USER + 100)
#define WM_UPDATE_CARD_STATUS (WM_USER + 101)
#define WM_UPDATE_CMD_COUNT  (WM_USER + 102)
#define WM_UPDATE_ERR_COUNT  (WM_USER + 103)
#define WM_CARD_TEST_DONE    (WM_USER + 104)

// Thread control flags
enum eThreadCmd {
    CMD_STOP = 0,
    CMD_POLLING
};

class CT10TestDlg : public CDialogEx
{
public:
    CT10TestDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_T10TESTTOOL_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

// ---- Device handle ----
    HANDLE      m_hDevice;
    BOOL        m_bConnected;

// ---- Worker thread ----
    CWinThread* m_pWorkerThread;
    volatile LONG m_nThreadCmd;   // CMD_STOP or CMD_POLLING
    LONG        m_nCmdCount;      // 0084000008 execution count
    LONG        m_nErrorCount;    // error count

// ---- Controls ----
    CEdit       m_editLog;
    CStatic     m_staticCardStatus;
    CStatic     m_staticFwVer;
    CStatic     m_staticDevUid;
    CStatic     m_staticDevSnr;
    CStatic     m_staticCmdCount;
    CStatic     m_staticErrCount;
    CButton     m_btnConnect;
    CButton     m_btnDisconnect;
    CButton     m_btnCpuTest;
    CButton     m_btnClearLog;
    CButton     m_radioUsb;
    CButton     m_radioSerial;
    CButton     m_chkReSearchOnErr;
    CEdit       m_editComPort;

// ---- Helpers ----
    void AddLog(const CString& msg);
    void SetCardStatus(const CString& status, COLORREF color = RGB(0,0,0));
    void UpdateCmdCount(LONG count);
    void UpdateErrCount(LONG count);
    void UpdateButtonStates();

// ---- Static thread proc ----
    static UINT CardTestThreadProc(LPVOID pParam);
    void DoCardTestLoop();

protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnConnect();
    afx_msg void OnBnClickedBtnDisconnect();
    afx_msg void OnBnClickedBtnCpuTest();
    afx_msg void OnBnClickedBtnClearLog();
    afx_msg void OnRadioUsb();
    afx_msg void OnRadioSerial();
    afx_msg LRESULT OnUpdateLog(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateCardStatus(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateCmdCount(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateErrCount(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCardTestDone(WPARAM wParam, LPARAM lParam);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
};
