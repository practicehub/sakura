/*!	@file
	@brief �ҏW�E�B���h�E�i�O�g�j�Ǘ��N���X

	@author Norio Nakatani
	@date 1998/05/13 �V�K�쐬
	@date 2002/01/14 YAZAKI PrintPreview�̕���
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000, genta
	Copyright (C) 2001-2002, YAZAKI
	Copyright (C) 2002, aroka, genta, MIK
	Copyright (C) 2003, MIK, genta, wmlhq
	Copyright (C) 2004, Moca
	Copyright (C) 2005, genta, Moca
	Copyright (C) 2006, ryoji, aroka, fon, yukihane
	Copyright (C) 2007, ryoji
	Copyright (C) 2008, nasukoji
	Copyright (C) 2009, nasukoji

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/

#ifndef _CEDITWND_H_
#define _CEDITWND_H_

class CEditWnd;

#include "CEditDoc.h"
#include "CShareData.h"
#include "CFuncKeyWnd.h"
#include "CTabWnd.h"	//@@@ 2003.05.31 MIK
#include "CMenuDrawer.h"
#include "CImageListMgr.h"

//by �S
#include"CDropTarget.h"

//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��ύX
class CPrintPreview;// 2002/2/10 aroka
class CDropTarget;

const static int MENUBAR_MESSAGE_MAX_LEN = 30;



//! �ҏW�E�B���h�E�i�O�g�j�Ǘ��N���X
//	@date 2002.2.17 YAZAKI CShareData�̃C���X�^���X�́ACProcess�ɂЂƂ���̂݁B
class SAKURA_CORE_API CEditWnd
{
public:
	/*
	||  Constructors
	*/
	CEditWnd();
	~CEditWnd();

	/*
	|| �����o�֐�
	*/
	//	Mar. 7, 2002 genta �����^�C�v�p�����ǉ�
	// 2007.06.26 ryoji �O���[�v�w������ǉ�
	HWND Create( HINSTANCE, HWND, int nGroup, const char*, int, BOOL, int = -1 );	/* �쐬 */


	LRESULT DispatchEvent( HWND, UINT, WPARAM, LPARAM );	/* ���b�Z�[�W���� */
//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��ύX
//	BOOL DispatchEvent_PPB( HWND, UINT, WPARAM, LPARAM );	/* ����v���r���[ ����o�[ �_�C�A���O�̃��b�Z�[�W���� */

	void PrintPreviewModeONOFF( void );	/* ����v���r���[���[�h�̃I��/�I�t */
	BOOL DoMouseWheel( WPARAM wParam, LPARAM lParam );	// �}�E�X�z�C�[������	// 2007.10.16 ryoji

	LRESULT OnPaint( HWND, UINT, WPARAM, LPARAM );	/* �`�揈�� */
	LRESULT OnSize( WPARAM, LPARAM );	/* WM_SIZE ���� */
	LRESULT OnLButtonUp( WPARAM, LPARAM );
	LRESULT OnLButtonDown( WPARAM, LPARAM );
	LRESULT OnMouseMove( WPARAM, LPARAM );
	LRESULT OnMouseWheel( WPARAM, LPARAM );
	LRESULT OnHScroll( WPARAM, LPARAM );
	LRESULT OnVScroll( WPARAM, LPARAM );

	void CreateToolBar( void );			/* �c�[���o�[�쐬 */
	void DestroyToolBar( void );		/* �c�[���o�[�j�� */
	void CreateStatusBar( void );		/* �X�e�[�^�X�o�[�쐬 */
	void DestroyStatusBar( void );		/* �X�e�[�^�X�o�[�j�� */

	void LayoutToolBar( void );			/* �c�[���o�[�̔z�u���� */			// 2006.12.19 ryoji
	void LayoutFuncKey( void );			/* �t�@���N�V�����L�[�̔z�u���� */	// 2006.12.19 ryoji
	void LayoutTabBar( void );			/* �^�u�o�[�̔z�u���� */			// 2006.12.19 ryoji
	void LayoutStatusBar( void );		/* �X�e�[�^�X�o�[�̔z�u���� */		// 2006.12.19 ryoji
	void EndLayoutBars( BOOL bAdjust = TRUE );	/* �o�[�̔z�u�I������ */	// 2006.12.19 ryoji

	//@@@ 2002.01.14 YAZAKI ����v���r���[�̃o�[��CPrintPreview�Ɉړ�

	void InitMenu( HMENU, UINT, BOOL );
//�����v���Z�X��
	void MessageLoop( void );	/* ���b�Z�[�W���[�v */

	int	OnClose( void );	/* �I�����̏��� */

//@@@ 2002.01.14 YAZAKI �s�g�p�̂���
//void CEditWnd::ExecCmd(LPCSTR lpszCmd/*, HANDLE hFile*/);

	//	Sep. 10, 2002 genta
	void SetWindowIcon( HICON, int);
	//	Sep. 10, 2002 genta
	void GetDefaultIcon( HICON& hIconBig, HICON& hIconSmall ) const;
	bool GetRelatedIcon(const char* szFile, HICON& hIconBig, HICON& hIconSmall) const;

	void ChangeFileNameNotify( const char *pszTabCaption, const char *pszFilePath, BOOL m_bIsGrep );	//�t�@�C�����ύX�ʒm	//@@@ 2003.05.31 MIK, 2006.01.28 ryoji �t�@�C�����AGrep���[�h�p�����[�^��ǉ�

	//	Dec. 4, 2002 genta
	//	���j���[�o�[�ւ̃��b�Z�[�W�\���@�\��CEditWnd���ڊ�
	void InitMenubarMessageFont(void);
	void PrintMenubarMessage( const char* msg );
	//	Dec. 4, 2002 genta ���̂�CEditView����ړ�
	void SendStatusMessage( const char* msg );
	//	Jul. 9, 2005 genta ���j���[�o�[�E�[�ɂ͏o�������Ȃ����߂̃��b�Z�[�W���o��
	void SendStatusMessage2( const char* msg );
	/*!	SendStatusMessage2()�������ڂ����邩��\�߃`�F�b�N
		@date 2005.07.09 genta
		@note ����SendStatusMessage2()�ŃX�e�[�^�X�o�[�\���ȊO�̏�����ǉ�
		����ꍇ�ɂ͂�����ύX���Ȃ��ƐV�����ꏊ�ւ̏o�͂��s���Ȃ��D
		
		@sa SendStatusMessage2
	*/
	bool SendStatusMessage2IsEffective(void) const {
		return NULL != m_hwndStatusBar;
	}
	void WindowTopMost( int ); // 2004.09.21 Moca

	void SetFocusSearchBox( void ) const;			/* �c�[���o�[�����{�b�N�X�փt�H�[�J�X���ړ� */	// 2006.06.04 yukihane

//	void MyAppendMenu( HMENU, int, int, char* );	/* ���j���[���ڂ�ǉ� */
//#ifdef _DEBUG
	void SetDebugModeON( void );	/* �f�o�b�O���j�^���[�h�ɐݒ� */
	// 2005.06.24 Moca
	void SetDebugModeOFF( void );
//#endif
	LRESULT PopupWinList( bool bMousePos );	/*!< �E�B���h�E�ꗗ�|�b�v�A�b�v�\������ */	// 2006.03.23 fon	// 2007.02.28 ryoji �t���p�X�w��̃p�����[�^���폜
	LRESULT WinListMenu( HMENU hMenu, EditNode* pEditNodeArr, int nRowNum, BOOL bFull );	/*!< �E�B���h�E�ꗗ���j���[�쐬���� */	// 2006.03.23 fon

	/*
	|| �X�^�e�B�b�N�ȃ����o�֐�
	*/
	static int IsFuncEnable( CEditDoc*, DLLSHAREDATA*, int );	/* �@�\�����p�\�����ׂ� */
	static int IsFuncChecked( CEditDoc*, DLLSHAREDATA*, int );	/* �@�\���`�F�b�N��Ԃ����ׂ� */

	static void OnHelp_MenuItem( HWND, int );	/* ���j���[�A�C�e���ɑΉ�����w���v��\�� */
//	static int FuncID_To_HelpContextID( int );	/* �@�\ID�ɑΉ����郁�j���[�R���e�L�X�g�ԍ���Ԃ� */

	/*
	|| �����o�ϐ�
	*/
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
	char*			m_pszAppName;
	CEditDoc		m_cEditDoc;
	HWND			m_hwndParent;
    HWND			m_hwndReBar;	// Rebar �E�B���h�E	//@@@ 2006.06.17 ryoji
    HWND			m_hwndToolBar;
	HWND			m_hwndStatusBar;
	HWND			m_hwndProgressBar;
	//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��
	//	�ϐ����ړ�
	DLLSHAREDATA*	m_pShareData;
//	int				m_nSettingType;
//@@@ 2002.01.14 YAZAKI �s�g�p�̂���
//	HBITMAP			m_hbmpOPENED;
//	HBITMAP			m_hbmpOPENED_THIS;
	CFuncKeyWnd		m_CFuncKeyWnd;
	CTabWnd			m_cTabWnd;		//�^�u�E�C���h�E	//@@@ 2003.05.31 MIK
	CMenuDrawer		m_CMenuDrawer;
	int				m_nWinSizeType;	/* �T�C�Y�ύX�̃^�C�v */
	//	���܂����΁A�ȉ���PrintPreview�֍s�������B
	BOOL			m_bDragMode;
	int				m_nDragPosOrgX;
	int				m_nDragPosOrgY;
	CDropTarget*	m_pcDropTarget;
	//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��
	//	�ϐ����ړ�
//	HANDLE			m_hThread;

//	int				m_nChildArrNum;
//	HWND			m_hwndChildArr[32];


	/* ����v���r���[�\����� */
	//	�K�v�ɂȂ����Ƃ��i�v���r���[�R�}���h��I�񂾂Ƃ��j�ɐ������A�K�v�Ȃ��Ȃ�����i�v���r���[�R�}���h���I������Ƃ��Ɂj�j�����邱�ƁB
	CPrintPreview*	m_pPrintPreview;
	//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��
	//	�ϐ����ړ�
	//	���܂����΁A�ȉ���PrintPreview�֍s������
	//  2006.08.14 Moca DC,BMP��PrintPreview�ֈړ�

	//	Oct. 12, 2000 genta
	CImageListMgr	m_cIcons;	//	Image List
	
	/*
	|| �����w���p�n
	*/
	/* IDropTarget���� */	// 2008.06.20 ryoji
	STDMETHODIMP DragEnter( LPDATAOBJECT, DWORD, POINTL, LPDWORD );
	STDMETHODIMP DragOver( DWORD, POINTL, LPDWORD );
	STDMETHODIMP DragLeave( void );
	STDMETHODIMP Drop( LPDATAOBJECT, DWORD, POINTL, LPDWORD );
	void OnDropFiles( HDROP );	/* �t�@�C�����h���b�v���ꂽ */
	//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��
	//	���\�b�h���ړ�
//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��ύX
public:
	BOOL OnPrintPageSetting( void );/* ����y�[�W�ݒ� */

private:
	UINT	m_uMSIMEReconvertMsg;
	UINT	m_uATOKReconvertMsg;

//by �S
private:
	enum {icNone, icDown, icClicked, icDoubleClicked} m_IconClicked;
	LRESULT OnNcLButtonDown(WPARAM, LPARAM);
	LRESULT OnNcLButtonUp(WPARAM, LPARAM);
	LRESULT OnLButtonDblClk(WPARAM, LPARAM);

	int	CreateFileDropDownMenu( HWND );	//�J��(�h���b�v�_�E��)	//@@@ 2002.06.15 MIK
	HWND	m_hwndSearchBox;
	HFONT	m_fontSearchBox;
	void	ProcSearchBox( MSG* );	//����(�{�b�N�X)
	int		m_nCurrentFocus;
	BOOL	m_bIsActiveApp;	// ���A�v�����A�N�e�B�u���ǂ���	// 2007.03.08 ryoji
	BOOL	m_bUIPI;	// �G�f�B�^�|�g���C�Ԃł�UI���������m�F�p�t���O	// 2007.06.07 ryoji
	BOOL	m_bPageScrollByWheel;		// �z�C�[������ɂ��y�[�W�X�N���[������	// 2009.01.12 nasukoji
	BOOL	m_bHorizontalScrollByWheel;	// �z�C�[������ɂ�鉡�X�N���[������		// 2009.01.12 nasukoji

	//	Jul. 21, 2003 genta ToolBar��Owner Draw
	LPARAM ToolBarOwnerDraw( LPNMCUSTOMDRAW pnmh );

	//	Dec. 4, 2002 genta
	//	���j���[�o�[�ւ̃��b�Z�[�W�\���@�\��CEditWnd���ڊ�
	HFONT		m_hFontCaretPosInfo;	/*!< �L�����b�g�̍s���ʒu�\���p�t�H���g */
	int			m_nCaretPosInfoCharWidth;	/*!< �L�����b�g�̍s���ʒu�\���p�t�H���g�̕� */
	int			m_nCaretPosInfoCharHeight;	/*!< �L�����b�g�̍s���ʒu�\���p�t�H���g�̍��� */
	int			m_pnCaretPosInfoDx[64];	/* ������`��p�������z�� */

	LPTSTR		m_pszMenubarMessage; //!< ���j���[�o�[�E�[�ɕ\�����郁�b�Z�[�W
	LPTSTR		m_pszLastCaption;
	int m_nTimerCount; //!< OnTimer�p 2003.08.29 wmlhq

	// �c�[���o�[�X�V�p�Ǝ����ۑ��p�̃^�C�}�[�𕪗����� 20060128 aroka
	LRESULT OnTimer( WPARAM, LPARAM );	// WM_TIMER ����	// 2007.04.03 ryoji
	void OnEditTimer( void );	/* �^�C�}�[�̏��� */
	void OnToolbarTimer( void );	/* �^�C�}�[�̏��� 20060128 aroka */
	void UpdateToolbar( void );		// �c�[���o�[�̕\�����X�V����		// 2008.09.23 nasukoji
	void OnCaptionTimer( void );
	void OnSysMenuTimer( void );
	void OnCommand( WORD, WORD , HWND );
	void Timer_ONOFF( BOOL ); /* �X�V�̊J�n�^��~ 20060128 aroka */
public:
	BOOL IsActiveApp() const { return m_bIsActiveApp; };	// ���A�v�����A�N�e�B�u���ǂ���	// 2007.03.08 ryoji
	BOOL IsPageScrollByWheel() const { return m_bPageScrollByWheel; };		// �z�C�[������ɂ��y�[�W�X�N���[���L��	// 2009.01.12 nasukoji
	BOOL IsHScrollByWheel() const { return m_bHorizontalScrollByWheel; };	// �z�C�[������ɂ�鉡�X�N���[���L��		// 2009.01.12 nasukoji
	void SetPageScrollByWheel( BOOL bState ) { m_bPageScrollByWheel = bState; };	// �z�C�[������ɂ��y�[�W�X�N���[���L����ݒ肷��iTRUE=����, FALSE=�Ȃ��j	// 2009.01.12 nasukoji
	void SetHScrollByWheel( BOOL bState ) { m_bHorizontalScrollByWheel = bState; };	// �z�C�[������ɂ�鉡�X�N���[���L����ݒ肷��iTRUE=����, FALSE=�Ȃ��j	// 2009.01.12 nasukoji
	void ClearMouseState( void );		// 2009.01.12 nasukoji	�}�E�X�̏�Ԃ��N���A����i�z�C�[���X�N���[���L����Ԃ��N���A�j

};


///////////////////////////////////////////////////////////////////////
#endif /* _CEDITWND_H_ */


/*[EOF]*/