/******************************************************************************
**  Filename    CSerialPort.h
**  Purpose     This class can read, write and watch one serial port.
**              It sends messages to its owner when something happends on the port
**              The class creates a thread for reading and writing so the main
**              program is not blocked.
**  Author      Remon Spekreijse, mrlong, liquanhai, viruscamp, itas109, sanwer
******************************************************************************/


#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#ifndef WM_COMM_MSG_BASE
#define WM_COMM_MSG_BASE		WM_USER + 617		//!< ��Ϣ��ŵĻ���
#endif

#define WM_COMM_BREAK_DETECTED		WM_COMM_MSG_BASE + 1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_COMM_MSG_BASE + 2	// The CTS (clear-to-send) signal changed state.
#define WM_COMM_DSR_DETECTED		WM_COMM_MSG_BASE + 3	// The DSR (data-set-ready) signal changed state.
#define WM_COMM_ERR_DETECTED		WM_COMM_MSG_BASE + 4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY.
#define WM_COMM_RING_DETECTED		WM_COMM_MSG_BASE + 5	// A ring indicator was detected.
#define WM_COMM_RLSD_DETECTED		WM_COMM_MSG_BASE + 6	// The RLSD (receive-line-signal-detect) signal changed state.
#define WM_COMM_RXCHAR				WM_COMM_MSG_BASE + 7	// A character was received and placed in the input buffer.
#define WM_COMM_RXFLAG_DETECTED		WM_COMM_MSG_BASE + 8	// The event character was received and placed in the input buffer.
#define WM_COMM_TXEMPTY_DETECTED	WM_COMM_MSG_BASE + 9	// The last character in the output buffer was sent.
#define WM_COMM_RXSTR               WM_COMM_MSG_BASE + 10   // Receive string

#define COMM_MAX_PORT_NUMBER 200   //��󴮿��ܸ���

//���ú��ַ�ʽ���գ�
//	ReceiveString 0 һ���ַ�һ���ַ����գ���Ӧ��Ӧ����ΪWM_COMM_RXCHAR��
//	ReceiveString 1 ���ַ������գ���Ӧ��Ӧ����ΪWM_COMM_RXSTR��
#define IsReceiveString  1

class CSerialPort
{
public:
	CSerialPort();
	virtual ~CSerialPort();

	// port initialisation
	// stopsbits: 0,1,2 = 1, 1.5, 2
	BOOL		InitPort(HWND pPortOwner, UINT portnr = 1, UINT baud = 9600,
						 char parity = 'N', UINT databits = 8, UINT stopsbits = ONESTOPBIT,
						 DWORD dwCommEvents = EV_RXCHAR | EV_CTS, UINT nBufferSize = 512,
						 DWORD ReadIntervalTimeout = 1000,
						 DWORD ReadTotalTimeoutMultiplier = 1000,
						 DWORD ReadTotalTimeoutConstant = 1000,
						 DWORD WriteTotalTimeoutMultiplier = 1000,
						 DWORD WriteTotalTimeoutConstant = 1000);

	BOOL		StartMonitoring();
	BOOL		ResumeMonitoring();
	BOOL		SuspendMonitoring();
	BOOL		IsThreadSuspend(HANDLE hThread);

	DWORD		 GetWriteBufferSize();///��ȡд�����С
	DWORD		 GetCommEvents();///��ȡ�¼�
	DCB			 GetDCB();///��ȡDCB

	///д���ݵ�����
	void		WriteToPort(LPCSTR string);
	void		WriteToPort(LPCSTR string, int len);
	void		WriteToPort(LPBYTE buffer, int len);
	void		ClosePort();
	BOOL		IsOpen();

protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);///������
	static DWORD WINAPI CommThread(LPVOID pParam);///�̺߳���
	static void	ReceiveChar(CSerialPort* port);
	static void ReceiveStr(CSerialPort* port); //add by itas109 2016-06-22
	static void	WriteChar(CSerialPort* port);

	// thread
	HANDLE				m_Thread;
	BOOL				m_bIsSuspened;///thread�����߳��Ƿ����

	// synchronisation objects
	CCriticalSection	cs;		//ͬ�����󣬻�����
	CRITICAL_SECTION	m_csCommunicationSync;///�ٽ���Դ
	BOOL				m_bThreadAlive;///�����߳����б�־

	// handles
	HANDLE				m_hShutdownEvent;  //stop�������¼�
	HANDLE				m_hComm;		   // ���ھ��
	HANDLE				m_hWriteEvent;	 // write

	// Event array.
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed.
	///�¼����飬����һ��д�¼��������¼����ر��¼�
	///һ��Ԫ������һ���¼����������¼��̴߳���˿ڡ�
	///д�¼��ͽ����ַ��¼�λ��overlapped�ṹ�壨m_ov.hEvent����
	///���˿ڹر�ʱ����һ��ͨ�õĹرա�
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;///�첽I/O
	COMMTIMEOUTS		m_CommTimeouts;///��ʱ����
	DCB					m_dcb;///�豸���ƿ�

	// owner window
	//CWnd*				m_pOwner;
	HWND				m_pOwner;


	// misc
	UINT				m_nPortNr;		///���ں�
	char*				m_szWriteBuffer;///д������
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;///д�����С

	int					m_nWriteSize;//д���ֽ��� //add by mrlong 2007-12-25
};

#endif __SERIALPORT_H__
