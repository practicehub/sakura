/*!	@file
	@brief �e�L�X�g�̃��C�A�E�g���Ǘ�

	@author Norio Nakatani
	@date 1998/03/06 �V�K�쐬
	@date 1998/04/14 �f�[�^�̍폜������
	@date 1999/12/20 �f�[�^�̒u��������
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2001, jepro
	Copyright (C) 2002, MIK, aroka, genta, YAZAKI
	Copyright (C) 2003, genta
	Copyright (C) 2005, Moca, genta, D.S.Koba

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/

class CLayoutMgr;

#ifndef _CLAYOUTMGR_H_
#define _CLAYOUTMGR_H_

#include <windows.h>// 2002/2/10 aroka
#include "global.h"// 2002/2/10 aroka
#include "CLineComment.h"	//@@@ 2002.09.22 YAZAKI
#include "CBlockComment.h"	//@@@ 2002.09.22 YAZAKI

class CBregexp;// 2002/2/10 aroka
class CLayout;// 2002/2/10 aroka
class CDocLineMgr;// 2002/2/10 aroka
class CDocLine;// 2002/2/10 aroka
class CMemory;// 2002/2/10 aroka
class CEditDoc;// 2003/07/20 genta
struct Types;// 2005.11.20 Moca



struct LayoutReplaceArg {
	int			nDelLineFrom;			/*!< �폜�͈͍s  From ���C�A�E�g�s�ԍ� */
	int			nDelColmFrom;			/*!< �폜�͈͈ʒuFrom ���C�A�E�g�s���ʒu */
	int			nDelLineTo;				/*!< �폜�͈͍s  To   ���C�A�E�g�s�ԍ� */
	int			nDelColmTo;				/*!< �폜�͈͈ʒuTo   ���C�A�E�g�s���ʒu */
	CMemory*	pcmemDeleted;			/*!< �폜���ꂽ�f�[�^ */
	const char*	pInsData;				/*!< �}������f�[�^ */
	int			nInsDataLen;			/*!< �}������f�[�^�̒��� */

	int			nAddLineNum;			/*!< �ĕ`��q���g ���C�A�E�g�s�̑��� */
	int			nModLineFrom;			/*!< �ĕ`��q���g �ύX���ꂽ���C�A�E�g�sFrom(���C�A�E�g�s�̑�����0�̂Ƃ��g��) */
	int			nModLineTo;			/*!< �ĕ`��q���g �ύX���ꂽ���C�A�E�g�sFrom(���C�A�E�g�s�̑�����0�̂Ƃ��g��) */

	int			nNewLine;				/*!< �}�����ꂽ�����̎��̈ʒu�̍s(���C�A�E�g�s) */
	int			nNewPos;				/*!< �}�����ꂽ�����̎��̈ʒu�̃f�[�^�ʒu(���C�A�E�g���ʒu) */

//	BOOL		bUndo;					/*!< Undo���삩�ǂ��� */	@date 2002/03/24 YAZAKI bUndo�폜
};


/*-----------------------------------------------------------------------
�N���X�̐錾
-----------------------------------------------------------------------*/
/*!	@brief �e�L�X�g�̃��C�A�E�g���Ǘ�

	@date 2005.11.21 Moca �F�������������o�[�ֈړ��D�s�v�ƂȂ��������������o�֐�����폜�D
*/
class SAKURA_CORE_API CLayoutMgr
{
public:
	/*
	||  Constructors
	*/
	CLayoutMgr();
	~CLayoutMgr();
	void Create( CEditDoc*, CDocLineMgr* );
	/*
	||  �Q�ƌn
	*/
	int GetLineCount( void ) { return m_nLines; }	/* �S�����s����Ԃ� */
	const char* GetLineStr( int , int* );	/* �w�肳�ꂽ�����s�̃f�[�^�ւ̃|�C���^�Ƃ��̒�����Ԃ� */
	const char* GetLineStr( int , int*, const CLayout** );	/* �w�肳�ꂽ�����s�̃f�[�^�ւ̃|�C���^�Ƃ��̒�����Ԃ� */
	bool IsEndOfLine( int nLine, int nPos );	/* �w��ʒu���s��(���s�����̒��O)�����ׂ� */	//@@@ 2002.04.18 MIK
//	const CLayout* GetLineData( int );	/* �w�肳�ꂽ�����s�̃��C�A�E�g�f�[�^(CLayout)�ւ̃|�C���^��Ԃ� */
	CLayout* Search( int );	/* �w�肳�ꂽ�����s�̃��C�A�E�g�f�[�^(CLayout)�ւ̃|�C���^��Ԃ� */
	int WhereCurrentWord( int , int , int* , int* , int* , int*, CMemory*, CMemory* );	/* ���݈ʒu�̒P��͈̔͂𒲂ׂ� */
	//	Sep. 23, 2002 genta
	/*! �^�u���̎擾
		@return �^�u��
	 */
	int GetTabSpace(void) const { return m_nTabSpace; }
	/*! ����TAB�ʒu�܂ł̕�
		@param pos [in] ���݂̈ʒu
		@return ����TAB�ʒu�܂ł̕������D1�`TAB��
	 */
	int GetActualTabSpace(int pos) const { return m_nTabSpace - pos % m_nTabSpace; }
	//	Aug. 14, 2005 genta
	int GetMaxLineSize(void) const { return m_nMaxLineSize; }
	// 2005.11.21 Moca ���p���̐F���������������珜��
	bool ChangeLayoutParam( HWND hwndProgress, int nTabSize, int nMaxLineSize );
	// Jul. 29, 2006 genta
	void GetEndLayoutPos(int& lX, int& lY);
	
protected:
	int PrevOrNextWord( int, int, int*, int*, BOOL, BOOL bStopsBothEnds );	/* ���݈ʒu�̍��E�̒P��̐擪�ʒu�𒲂ׂ� */
public:
	int PrevWord( int nLineNum, int nIdx, int* pnLineNew, int* pnColmNew, BOOL bStopsBothEnds ){ return PrevOrNextWord(nLineNum, nIdx, pnLineNew, pnColmNew, TRUE, bStopsBothEnds); }	/* ���݈ʒu�̍��E�̒P��̐擪�ʒu�𒲂ׂ� */
	int NextWord( int nLineNum, int nIdx, int* pnLineNew, int* pnColmNew, BOOL bStopsBothEnds ){ return PrevOrNextWord(nLineNum, nIdx, pnLineNew, pnColmNew, FALSE, bStopsBothEnds); }	/* ���݈ʒu�̍��E�̒P��̐擪�ʒu�𒲂ׂ� */

	int SearchWord( int	, int , const char* , int , int , int , int , int* , int* , int* , int*, CBregexp* );	/* �P�ꌟ�� */
	void CaretPos_Phys2Log( int, int, int*, int* );
	void CaretPos_Log2Phys( int, int, int*, int* );
	void DUMP( void );	/* �e�X�g�p�Ƀ��C�A�E�g�����_���v */

	/*
	|| �X�V�n
	*/
	/* ���C�A�E�g���̕ύX
		@date Jun. 01, 2001 JEPRO char* (�s�R�����g�f���~�^3�p)��1�ǉ�
		@date 2002.04.13 MIK �֑�,���s�������Ԃ牺����,��Ǔ_�Ԃ炳����ǉ�
		@date 2002/04/27 YAZAKI Types��n���悤�ɕύX�B
	*/
	void SetLayoutInfo( int, HWND, Types& refType );
	
	/* �s�������폜 */
	// 2005.11.21 Moca ���p���̐F���������������珜��
	void DeleteData_CLayoutMgr(
		int			nLineNum,
		int			nDelPos,
		int			nDelLen,
		int			*pnModifyLayoutLinesOld,
		int			*pnModifyLayoutLinesNew,
		int			*pnDeleteLayoutLines,
		CMemory&	cmemDeleted			/* �폜���ꂽ�f�[�^ */
	);
	/* ������}�� */
	// 2005.11.21 Moca ���p���̐F���������������珜��
	void InsertData_CLayoutMgr(
		int			nLineNum,
		int			nInsPos,
		const char*	pInsData,
		int			nInsDataLen,
		int*		pnModifyLayoutLinesOld,
		int*		pnInsLineNum,		/* �}���ɂ���đ��������C�A�E�g�s�̐� */
		int*		pnNewLine,			/* �}�����ꂽ�����̎��̈ʒu�̍s */
		int*		pnNewPos			/* �}�����ꂽ�����̎��̈ʒu�̃f�[�^�ʒu */
	);

	/* ������u�� */
	void ReplaceData_CLayoutMgr(
		LayoutReplaceArg*	pArg
	);

protected:
	/*
	||  �Q�ƌn
	*/
	const char* GetFirstLinrStr( int* );	/* ���A�N�Z�X���[�h�F�擪�s�𓾂� */
	const char* GetNextLinrStr( int* );	/* ���A�N�Z�X���[�h�F���̍s�𓾂� */
	void XYLogicalToLayout( CLayout*, int, int, int, int*, int* );	/* �_���ʒu�����C�A�E�g�ʒu�ϊ� */


	/*
	|| �X�V�n
	*/
//	void SetMaxLineSize( int, int, int );	/* �܂�Ԃ��������̕ύX�^���C�A�E�g���č\�z */
	// 2005.11.21 Moca ���p���̐F���������������珜��
	void DoLayout( HWND );	/* ���݂̐܂�Ԃ��������ɍ��킹�đS�f�[�^�̃��C�A�E�g�����Đ������܂� */
//	void DoLayout( int, BOOL, HWND, BOOL, BOOL );	/* �V�����܂�Ԃ��������ɍ��킹�đS�f�[�^�̃��C�A�E�g�����Đ������܂� */
//	int DoLayout3( CLayout* , int, int, int );	/* �w�背�C�A�E�g�s�ɑΉ�����_���s�̎��̘_���s����w��_���s�������ă��C�A�E�g���� */
	// 2005.11.21 Moca ���p���̐F���������������珜��
	int DoLayout_Range( CLayout* , int, int, int, int, int* );	/* �w�背�C�A�E�g�s�ɑΉ�����_���s�̎��̘_���s����w��_���s�������ă��C�A�E�g���� */
	CLayout* DeleteLayoutAsLogical( CLayout*, int, int , int, int, int, int* );	/* �_���s�̎w��͈͂ɊY�����郌�C�A�E�g�����폜 */
	void ShiftLogicalLineNum( CLayout* , int );	/* �w��s����̍s�̃��C�A�E�g���ɂ��āA�_���s�ԍ����w��s�������V�t�g���� */

	/*
	|| �����o�ϐ�
	*/
public:
	CDocLineMgr*	m_pcDocLineMgr;	/* �s�o�b�t�@�Ǘ��}�l�[�W�� */
protected:
	CLayout*		m_pLayoutTop;
	CLayout*		m_pLayoutBot;
	//	2002.10.07 YAZAKI add
	int				m_nLineTypeBot;	/*!< �^�C�v 0=�ʏ� 1=�s�R�����g 2=�u���b�N�R�����g 3=�V���O���N�H�[�e�[�V���������� 4=�_�u���N�H�[�e�[�V���������� */
	int				m_nLines;					/* �S�����s�� */
	int				m_nMaxLineSize;				/* �܂�Ԃ������� */
	BOOL			m_bWordWrap;				/* �p�����[�h���b�v������ */
	BOOL			m_bKinsokuHead;				/* �s���֑������� */	//@@@ 2002.04.08 MIK
	BOOL			m_bKinsokuTail;				/* �s���֑������� */	//@@@ 2002.04.08 MIK
	BOOL			m_bKinsokuRet;				/* ���s�������Ԃ牺���� */	//@@@ 2002.04.13 MIK
	BOOL			m_bKinsokuKuto;				/* ��Ǔ_���Ԃ牺���� */	//@@@ 2002.04.17 MIK
	char*			m_pszKinsokuHead_1;			/* �s���֑����� */	//@@@ 2002.04.08 MIK
	char*			m_pszKinsokuHead_2;			/* �s���֑����� */	//@@@ 2002.04.08 MIK
	char*			m_pszKinsokuTail_1;			/* �s���֑����� */	//@@@ 2002.04.08 MIK
	char*			m_pszKinsokuTail_2;			/* �s���֑����� */	//@@@ 2002.04.08 MIK
	char*			m_pszKinsokuKuto_1;			/* ��Ǔ_�Ԃ炳������ */	//@@@ 2002.04.17 MIK
	char*			m_pszKinsokuKuto_2;			/* ��Ǔ_�Ԃ炳������ */	//@@@ 2002.04.17 MIK
	int				m_nTabSpace;				/* TAB�����X�y�[�X */
	BOOL			m_bDispComment;				/* �R�����g�̐F���� */		// 2005.11.21 Moca
	BOOL			m_bDispSString;				/* �V���O���N�H�[�e�[�V�����̐F���� */		// 2005.11.21 Moca
	BOOL			m_bDispWString;				/* �_�u���N�H�[�e�[�V�����̐F���� */		// 2005.11.21 Moca
	CLineComment	m_cLineComment;				/* �s�R�����g�f���~�^ */		//@@@ 2002.09.22 YAZAKI
	CBlockComment	m_cBlockComment;			/* �u���b�N�R�����g�f���~�^ */	//@@@ 2002.09.22 YAZAKI
	int				m_nStringType;				/* �������؂�L���G�X�P�[�v���@ 0=[\"][\'] 1=[""][''] */
//	int				m_nIndentLayout;			/* �܂�Ԃ���2�s�ڈȍ~���������\�� */	//@@@ 2002.09.29 YAZAKI
	//	Oct. 1, 2002 genta �C���f���g���v�Z�֐���ێ�
	int (CLayoutMgr::*getIndentOffset)( CLayout* );


	int				m_nPrevReferLine;
	CLayout*		m_pLayoutPrevRefer;
	
	// 2006.10.01 Moca EOF�J�[�\���ʒu���L������(DoLayout/DoLayout_Range�Ŗ����ɂ���)
	int				m_nEOFLine; //!< EOF�s��
	int				m_nEOFColumn; //!< EOF���ʒu

	//	Jul. 20, 2003 genta
	//	�^�C�v�ʂ̐ݒ���擾���邽�߂�CEditDoc�ւ̎Q�Ƃ��K�v
	CEditDoc*		m_pcEditDoc;
	
	/*
	|| �����w���p�n
	*/
	//@@@ 2002.09.23 YAZAKI
	CLayout* CreateLayout( CDocLine* pCDocLine, int nLine, int nOffset, int nLength, int nTypePrev, int nIndent );
	CLayout* InsertLineNext( CLayout*, CLayout* );
	void AddLineBottom( CLayout* );
public:
	void Init();
	void Empty();

private:
	bool IsKinsokuHead( const char *pLine, int length );	/*!< �s���֑��������`�F�b�N���� */	//@@@ 2002.04.08 MIK
	bool IsKinsokuTail( const char *pLine, int length );	/*!< �s���֑��������`�F�b�N���� */	//@@@ 2002.04.08 MIK
	bool IsKutoTen( unsigned char c1, unsigned char c2 );	/*!< ��Ǔ_�������`�F�b�N���� */	//@@@ 2002.04.17 MIK
	bool IsKinsokuKuto( const char *pLine, int length );	/*!< ��Ǔ_�������`�F�b�N���� */	//@@@ 2002.04.17 MIK
	//	2005-08-20 D.S.Koba �֑��֘A�����̊֐���
	/*! ��Ǔ_�Ԃ牺���̏����ʒu��
		@date 2005-08-20 D.S.Koba
		@date Sep. 3, 2005 genta �œK��
	*/
	bool IsKinsokuPosKuto(const int nRest, const int nCharChars ) const {
		return nRest < nCharChars;
	}
	bool IsKinsokuPosHead(const int, const int, const int);	//!< �s���֑��̏����ʒu��
	bool IsKinsokuPosTail(const int, const int, const int);	//!< �s���֑��̏����ʒu��

	//@@@ 2002.09.22 YAZAKI
	// 2005.11.21 Moca ���p���̐F���������������珜��
	bool CheckColorMODE( int& nCOMMENTMODE, int& nCOMMENTEND, int nPos, int nLineLen, const char* pLine );
	int Match_Quote( char szQuote, int nPos, int nLineLen, const char* pLine );
//	int getMaxLineSize( CLayout* pLayoutPrev );
	
	//	Oct. 1, 2002 genta �C���f���g���v�Z�֐��Q
	int getIndentOffset_Normal( CLayout* pLayoutPrev );
	int getIndentOffset_Tx2x( CLayout* pLayoutPrev );
	int getIndentOffset_LeftSpace( CLayout* pLayoutPrev );
};


///////////////////////////////////////////////////////////////////////
#endif /* _CLAYOUTMGR_H_ */


/*[EOF]*/