// T10TestDlg.cpp : implementation of CT10TestDlg
#include "stdafx.h"
#include "T10TestTool.h"
#include "T10TestDlg.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CT10TestDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_CONNECT,     &CT10TestDlg::OnBnClickedBtnConnect)
    ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CT10TestDlg::OnBnClickedBtnDisconnect)
    ON_BN_CLICKED(IDC_BTN_CPU_TEST,   &CT10TestDlg::OnBnClickedBtnCpuTest)
    ON_BN_CLICKED(IDC_BTN_CLEAR_LOG,  &CT10TestDlg::OnBnClickedBtnClearLog)
    ON_BN_CLICKED(IDC_RADIO_USB,      &CT10TestDlg::OnRadioUsb)
    ON_BN_CLICKED(IDC_RADIO_SERIAL,   &CT10TestDlg::OnRadioSerial)
    ON_MESSAGE(WM_UPDATE_LOG,          &CT10TestDlg::OnUpdateLog)
    ON_MESSAGE(WM_UPDATE_CARD_STATUS,  &CT10TestDlg::OnUpdateCardStatus)
    ON_MESSAGE(WM_UPDATE_CMD_COUNT,    &CT10TestDlg::OnUpdateCmdCount)
    ON_MESSAGE(WM_UPDATE_ERR_COUNT,    &CT10TestDlg::OnUpdateErrCount)
    ON_MESSAGE(WM_CARD_TEST_DONE,      &CT10TestDlg::OnCardTestDone)
END_MESSAGE_MAP()

CT10TestDlg::CT10TestDlg(CWnd* pParent)
    : CDialogEx(IDD_T10TESTTOOL_DIALOG, pParent)
    , m_hDevice(INVALID_HANDLE_VALUE)
    , m_bConnected(FALSE)
    , m_pWorkerThread(nullptr)
    , m_nCmdCount(0)
    , m_nErrorCount(0)
{
    m_hIcon = AfxGetApp()->LoadStandardIcon(IDI_APPLICATION);
    InterlockedExchange(&m_nThreadCmd, CMD_STOP);
}

void CT10TestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_LOG,           m_editLog);
    DDX_Control(pDX, IDC_STATIC_CARD_STATUS, m_staticCardStatus);
    DDX_Control(pDX, IDC_STATIC_FW_VER,      m_staticFwVer);
    DDX_Control(pDX, IDC_STATIC_DEV_UID,     m_staticDevUid);
    DDX_Control(pDX, IDC_STATIC_DEV_SNR,     m_staticDevSnr);
    DDX_Control(pDX, IDC_STATIC_CMD_COUNT,    m_staticCmdCount);
    DDX_Control(pDX, IDC_STATIC_ERR_COUNT,    m_staticErrCount);
    DDX_Control(pDX, IDC_BTN_CONNECT,         m_btnConnect);
    DDX_Control(pDX, IDC_BTN_DISCONNECT,      m_btnDisconnect);
    DDX_Control(pDX, IDC_BTN_CPU_TEST,       m_btnCpuTest);
    DDX_Control(pDX, IDC_BTN_CLEAR_LOG,      m_btnClearLog);
    DDX_Control(pDX, IDC_RADIO_USB,          m_radioUsb);
    DDX_Control(pDX, IDC_RADIO_SERIAL,       m_radioSerial);
    DDX_Control(pDX, IDC_CHK_RESEARCH_ON_ERR, m_chkReSearchOnErr);
    DDX_Control(pDX, IDC_EDIT_COM_PORT,      m_editComPort);
}

BOOL CT10TestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // Default to USB mode
    m_radioUsb.SetCheck(BST_CHECKED);
    m_radioSerial.SetCheck(BST_UNCHECKED);
    m_editComPort.SetWindowText("1");
    m_editComPort.EnableWindow(FALSE);

    // Check "Re-search on error" by default
    m_chkReSearchOnErr.SetCheck(BST_CHECKED);

    m_staticCardStatus.SetWindowText("---");
    m_staticFwVer.SetWindowText("---");
    m_staticDevUid.SetWindowText("---");
    m_staticDevSnr.SetWindowText("---");
    m_staticCmdCount.SetWindowText("0");
    m_staticErrCount.SetWindowText("0");

    UpdateButtonStates();
    return TRUE;
}

// ============================================================
// Helper: Append a line to the log edit box (thread safe via PostMessage)
// ============================================================
void CT10TestDlg::AddLog(const CString& msg)
{
    CString* pStr = new CString(msg);
    PostMessage(WM_UPDATE_LOG, 0, (LPARAM)pStr);
}

void CT10TestDlg::SetCardStatus(const CString& status, COLORREF /*color*/)
{
    CString* pStr = new CString(status);
    PostMessage(WM_UPDATE_CARD_STATUS, 0, (LPARAM)pStr);
}

void CT10TestDlg::UpdateCmdCount(LONG count)
{
    PostMessage(WM_UPDATE_CMD_COUNT, (WPARAM)count, 0);
}

void CT10TestDlg::UpdateErrCount(LONG count)
{
    PostMessage(WM_UPDATE_ERR_COUNT, (WPARAM)count, 0);
}

void CT10TestDlg::UpdateButtonStates()
{
    m_btnConnect.EnableWindow(!m_bConnected);
    m_btnDisconnect.EnableWindow(m_bConnected);
    m_btnCpuTest.EnableWindow(m_bConnected && (m_pWorkerThread == nullptr));
}

// ============================================================
// Message handlers (run in UI thread)
// ============================================================
LRESULT CT10TestDlg::OnUpdateLog(WPARAM /*wParam*/, LPARAM lParam)
{
    CString* pMsg = reinterpret_cast<CString*>(lParam);
    if (pMsg)
    {
        CString cur;
        m_editLog.GetWindowText(cur);
        if (!cur.IsEmpty()) cur += "\r\n";
        cur += *pMsg;
        m_editLog.SetWindowText(cur);
        int len = m_editLog.GetWindowTextLength();
        m_editLog.SetSel(len, len);
        m_editLog.ScrollWindow(0, -9999);
        delete pMsg;
    }
    return 0;
}

LRESULT CT10TestDlg::OnUpdateCardStatus(WPARAM /*wParam*/, LPARAM lParam)
{
    CString* pMsg = reinterpret_cast<CString*>(lParam);
    if (pMsg)
    {
        m_staticCardStatus.SetWindowText(*pMsg);
        delete pMsg;
    }
    return 0;
}

LRESULT CT10TestDlg::OnUpdateCmdCount(WPARAM wParam, LPARAM /*lParam*/)
{
    CString s;
    s.Format("%ld", (LONG)wParam);
    m_staticCmdCount.SetWindowText(s);
    return 0;
}

LRESULT CT10TestDlg::OnUpdateErrCount(WPARAM wParam, LPARAM /*lParam*/)
{
    CString s;
    s.Format("%ld", (LONG)wParam);
    m_staticErrCount.SetWindowText(s);
    return 0;
}

LRESULT CT10TestDlg::OnCardTestDone(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    m_pWorkerThread = nullptr;
    InterlockedExchange(&m_nErrorCount, 0);
    UpdateErrCount(0);
    UpdateButtonStates();
    AddLog("=== 卡片测试线程已停止 ===");
    return 0;
}

// ============================================================
// Connect button
// ============================================================
void CT10TestDlg::OnBnClickedBtnConnect()
{
    short port = 100;  // USB default

    if (m_radioSerial.GetCheck() == BST_CHECKED)
    {
        CString strPort;
        m_editComPort.GetWindowText(strPort);
        int comNum = _ttoi(strPort);
        if (comNum < 1) comNum = 1;
        port = (short)(comNum - 1);  // COM1 -> 0, COM2 -> 1, ...
    }

    AddLog("Connecting device...");

    HANDLE hDev = dc_init(port, 115200);
    if ((INT_PTR)hDev < 0)
    {
        CString msg;
        msg.Format("Connection failed! dc_init returned: %d  (port=%d)", (int)(INT_PTR)hDev, port);
        AddLog(msg);
        return;
    }

    m_hDevice = hDev;
    m_bConnected = TRUE;

    CString msg;
    msg.Format("Connection successful! Handle: 0x%p  (port=%d)", m_hDevice, port);
    AddLog(msg);

    // --- Get firmware version ---
    unsigned char buf[256] = {0};
    if (dc_getver(m_hDevice, buf) == 0)
    {
        CString ver((char*)buf);
        m_staticFwVer.SetWindowText(ver);
        AddLog("Firmware version: " + ver);
    }
    else
    {
        m_staticFwVer.SetWindowText("Read failed");
        AddLog("Warning: dc_getver failed");
    }

    // --- Get device UID ---
    memset(buf, 0, sizeof(buf));
    if (dc_GetDeviceUid(m_hDevice, (char*)buf) == 0)
    {
        CString uid((char*)buf);
        m_staticDevUid.SetWindowText(uid);
        AddLog("Device UID: " + uid);
    }
    else
    {
        m_staticDevUid.SetWindowText("Read failed");
        AddLog("Warning: dc_GetDeviceUid failed");
    }

    // --- Get device long version ---
    memset(buf, 0, sizeof(buf));
    if (dc_readdevsnr(m_hDevice, buf) == 0)
    {
        CString snr((char*)buf);
        m_staticDevSnr.SetWindowText(snr);
        AddLog("Device SNR: " + snr);
    }
    else
    {
        m_staticDevSnr.SetWindowText("Read failed");
        AddLog("Warning: dc_readdevsnr failed");
    }

    UpdateButtonStates();
}

// ============================================================
// Disconnect button
// ============================================================
void CT10TestDlg::OnBnClickedBtnDisconnect()
{
    // Stop worker thread first
    InterlockedExchange(&m_nThreadCmd, CMD_STOP);
    if (m_pWorkerThread != nullptr)
    {
        AddLog("Stopping test thread, please wait...");
        WaitForSingleObject(m_pWorkerThread->m_hThread, 5000);
        m_pWorkerThread = nullptr;
    }

    if (m_bConnected && (INT_PTR)m_hDevice >= 0)
    {
        dc_exit(m_hDevice);
        m_hDevice = INVALID_HANDLE_VALUE;
        m_bConnected = FALSE;
        AddLog("Device disconnected.");
        m_staticFwVer.SetWindowText("---");
        m_staticDevUid.SetWindowText("---");
        m_staticDevSnr.SetWindowText("---");
        m_staticCardStatus.SetWindowText("---");
        m_staticCmdCount.SetWindowText("0");
        m_staticErrCount.SetWindowText("0");
        InterlockedExchange(&m_nCmdCount, 0);
        InterlockedExchange(&m_nErrorCount, 0);
    }

    UpdateButtonStates();
}

// ============================================================
// CPU Card Test button
// ============================================================
void CT10TestDlg::OnBnClickedBtnCpuTest()
{
    if (!m_bConnected || m_pWorkerThread != nullptr)
        return;

    // Read checkbox state at this moment
    BOOL bReSearch = (m_chkReSearchOnErr.GetCheck() == BST_CHECKED);

    InterlockedExchange(&m_nCmdCount, 0);
    InterlockedExchange(&m_nErrorCount, 0);
    UpdateCmdCount(0);
    UpdateErrCount(0);
    InterlockedExchange(&m_nThreadCmd, CMD_POLLING);

    m_pWorkerThread = AfxBeginThread(CardTestThreadProc, this);
    if (m_pWorkerThread == nullptr)
    {
        AddLog("Error: Failed to create test thread!");
        return;
    }

    m_btnCpuTest.EnableWindow(FALSE);
    AddLog(bReSearch
        ? "=== CPU Card Test started (Re-search on error: ON) ==="
        : "=== CPU Card Test started (Re-search on error: OFF) ===");
}

// ============================================================
// Clear log button
// ============================================================
void CT10TestDlg::OnBnClickedBtnClearLog()
{
    m_editLog.SetWindowText("");
}

// ============================================================
// Radio button handlers
// ============================================================
void CT10TestDlg::OnRadioUsb()
{
    m_editComPort.EnableWindow(FALSE);
}

void CT10TestDlg::OnRadioSerial()
{
    m_editComPort.EnableWindow(TRUE);
}

// ============================================================
// Worker Thread entry point
// ============================================================
UINT CT10TestDlg::CardTestThreadProc(LPVOID pParam)
{
    CT10TestDlg* pDlg = reinterpret_cast<CT10TestDlg*>(pParam);
    pDlg->DoCardTestLoop();
    pDlg->PostMessage(WM_CARD_TEST_DONE, 0, 0);
    return 0;
}

// ============================================================
// Worker Thread: CPU card stability test loop
//
// Flow:
//   1. ONCE at start  -> dc_reset + dc_config_card + dc_pro_resetInt
//   2. LOOP            -> dc_pro_commandlinkInt(0084000008) continuously
//   3. On error        -> if "Re-search on error" checked: re-init card and retry
//                        else: break (stop loop)
//   4. On no card      -> wait and keep polling until card appears
// ============================================================
void CT10TestDlg::DoCardTestLoop()
{
    BOOL bReSearchOnErr = (m_chkReSearchOnErr.GetCheck() == BST_CHECKED);

    // ---- Phase 1: One-time initialization ----
    AddLog("Step 1/3: Resetting RF field...");
    dc_reset(m_hDevice, 10);
    dc_config_card(m_hDevice, 'A');

    AddLog("Step 2/3: Searching for card...");
    unsigned int  snLen = 0;
    unsigned char snBuf[64] = {0};
    short ret = dc_card_n(m_hDevice, 0x00, &snLen, snBuf);

    if (ret != 0)
    {
        SetCardStatus("No card");
        AddLog("No card found. Waiting for card to be placed...");
        // Wait for card to appear
        while (InterlockedCompareExchange(&m_nThreadCmd, CMD_POLLING, CMD_POLLING) == CMD_POLLING)
        {
            if (dc_card_n(m_hDevice, 0x00, &snLen, snBuf) == 0)
                break;
            SetCardStatus("No card - waiting...");
            Sleep(300);
        }
    }

    // Build and show UID
    CString snHex;
    for (unsigned int i = 0; i < snLen && i < 64; i++)
    {
        CString b;
        b.Format("%02X", snBuf[i]);
        snHex += b;
    }
    SetCardStatus("Card OK  UID: " + snHex);
    AddLog("Card detected, UID: " + snHex);

    AddLog("Step 3/3: CPU card reset (dc_pro_resetInt)...");
    unsigned char atsLen = 0;
    unsigned char atsBuf[256] = {0};
    ret = dc_pro_resetInt(m_hDevice, &atsLen, atsBuf);
    if (ret != 0)
    {
        CString err;
        err.Format("CPU card reset FAILED (ret=%d). Test aborted.", ret);
        AddLog(err);
        return;  // Cannot proceed without successful reset
    }

    CString atsHex;
    for (unsigned char i = 0; i < atsLen; i++)
    {
        CString b;
        b.Format("%02X", atsBuf[i]);
        atsHex += b;
    }
    AddLog("CPU card reset SUCCESS  ATS: " + atsHex);
    AddLog("=== Starting stability test: 0084000008 loop ===");

    // ---- Phase 2: Continuous APDU loop ----
    // APDU: 00 84 00 00 08  (GET CHALLENGE, 8 bytes)
    unsigned char cmd[5] = {0x00, 0x84, 0x00, 0x00, 0x08};
    unsigned char rspBuf[512] = {0};
    unsigned int  rspLen = 0;

    while (InterlockedCompareExchange(&m_nThreadCmd, CMD_POLLING, CMD_POLLING) == CMD_POLLING)
    {
        rspLen = 0;
        memset(rspBuf, 0, sizeof(rspBuf));
        ret = dc_pro_commandlinkInt(m_hDevice, 5, cmd, &rspLen, rspBuf, 7);

        if (ret != 0)
        {
            // APDU command failed
            LONG errCnt = InterlockedIncrement(&m_nErrorCount);
            UpdateErrCount(errCnt);

            CString errMsg;
            errMsg.Format("APDU ERROR (ret=%d)  Error count: %ld", ret, errCnt);
            AddLog(errMsg);

            if (!bReSearchOnErr)
            {
                // Stop on error
                AddLog("Test STOPPED (Re-search on error: OFF).");
                break;
            }

            // Re-search card and re-initialize
            AddLog("Re-searching card (Re-search on error: ON)...");

            dc_reset(m_hDevice, 10);
            dc_config_card(m_hDevice, 'A');

            int retry = 0;
            while (InterlockedCompareExchange(&m_nThreadCmd, CMD_POLLING, CMD_POLLING) == CMD_POLLING)
            {
                if (dc_card_n(m_hDevice, 0x00, &snLen, snBuf) == 0)
                    break;
                SetCardStatus("Re-searching...");
                Sleep(300);
                retry++;
                if (retry > 50)
                {
                    // Timeout: no card for 15 seconds
                    AddLog("Re-search timeout. No card found. Stopping.");
                    return;
                }
            }

            // Re-initialize card
            snHex.Empty();
            for (unsigned int i = 0; i < snLen && i < 64; i++)
            {
                CString b;
                b.Format("%02X", snBuf[i]);
                snHex += b;
            }
            SetCardStatus("Card OK  UID: " + snHex);
            AddLog("Card found again, UID: " + snHex);

            ret = dc_pro_resetInt(m_hDevice, &atsLen, atsBuf);
            if (ret != 0)
            {
                AddLog("CPU card re-reset FAILED. Stopping.");
                return;
            }
            atsHex.Empty();
            for (unsigned char i = 0; i < atsLen; i++)
            {
                CString b;
                b.Format("%02X", atsBuf[i]);
                atsHex += b;
            }
            AddLog("CPU card re-reset SUCCESS  ATS: " + atsHex);
            continue;  // Continue APDU loop
        }

        // APDU success
        LONG cnt = InterlockedIncrement(&m_nCmdCount);
        UpdateCmdCount(cnt);

        CString rspHex;
        for (unsigned int i = 0; i < rspLen; i++)
        {
            CString b;
            b.Format("%02X", rspBuf[i]);
            rspHex += b;
        }

        CString logMsg;
        logMsg.Format("[%ld] APDU OK  0084000008 -> %s", cnt, (LPCTSTR)rspHex);
        AddLog(logMsg);

        Sleep(200);
    }

    AddLog("Card test loop exited.");
}

// ============================================================
// OnDestroy: cleanup
// ============================================================
void CT10TestDlg::OnDestroy()
{
    InterlockedExchange(&m_nThreadCmd, CMD_STOP);
    if (m_pWorkerThread != nullptr)
    {
        WaitForSingleObject(m_pWorkerThread->m_hThread, 5000);
        m_pWorkerThread = nullptr;
    }
    if (m_bConnected && (INT_PTR)m_hDevice >= 0)
    {
        dc_exit(m_hDevice);
        m_hDevice = INVALID_HANDLE_VALUE;
    }
    CDialogEx::OnDestroy();
}

void CT10TestDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CT10TestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}
