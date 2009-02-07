/*!	@file
	@brief �����R�[�h�𒲍����鎞�Ɏg���C���^�[�t�F�[�X�N���X

	@author Sakura-Editor collaborators
	@date 2006/12/10 �V�K�쐬
	@date 2007/10/26 �N���X�̐����ύX (���F�����R�[�h�������ێ��N���X)
*/
/*
	Copyright (C) 2006
	Copyright (C) 2007

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

		1. The origin of this software must not be misrepresented;
		   you must not claim that you wrote the original software.
		   If you use this software in a product, an acknowledgment
		   in the product documentation would be appreciated but is
		   not required.

		2. Altered source versions must be plainly marked as such,
		   and must not be misrepresented as being the original software.

		3. This notice may not be removed or altered from any source
		   distribution.
*/

#include "stdafx.h"

#include <limits.h>
#include <stdio.h>
#include <winbase.h>
#include "charset/codechecker.h"
#include "charset/CESI.h"
#include "mem/CMemory.h"
#include "util/tchar_printf.h"
#include "charset/charset.h"
#include "charset/CCodeMediator.h"
#include "convert/convert_util2.h"
#include "charset/charcode.h"


/*!
	�}���`�o�C�g�����R�[�h�̗D�揇�ʕ\�i����l�j

	@note
	@li �������Ȃ��قǗD��x�͍����B
	@li �e���ڂ̒l�́A0 �ȏ� CODE_MAX �����ŗL���Ƃ���B

	CODE_UNICODE �� CODE_UNICODEBE �������e���ڂ̒l�́A
	�Ή����镶���R�[�h���̊i�[�� (m_pMbInfo) �C���f�b�N�X�Ƃ��Ďg���Ă��邽�߁A
	�A�������Ă���B
*/
const int gm_aMbcDefaultPriority[] =
{
	0,			//CODE_SJIS
	1,			//CODE_JIS
	2,			//CODE_EUC
	INT_MAX,	//CODE_UNICODE
	3,			//CODE_UTF8
	4,			//CODE_UTF7
	INT_MAX,	//CODE_UNICODEBE
};




/*!
	�D�揇�ʕ\���쐬

	@param[in] eFavoriteId �ŗD�悵���������R�[�hID

	@note
	(sjis, jis, eucjp, uni, utf8, utf7, unibe)

	��̕����R�[�hID��ɑΉ�����D��x���l��̒����̎d���F

	  ex. utf8 ��D�悳�������ꍇ�̎菇

               sjis, jis, eucjp, uni, utf8, utf7, unibe <-- ECodeType �̐錾��
	  Step0.  (0,    1,   2,     X,   3,    4,    X    ) <-- �������
	  Step1.  (0,    1,   3,     X,   2,    4,    X    )
	  Step2.  (0,    2,   3,     X,   1,    4,    X    )
	  Step3.  (1,    2,   3,     X,   0,    4,    X    )
*/
void CESI::MakePriorityTable( const ECodeType eFavoriteId )
{
	int i, temp;

	if( CODE_CODEMAX <= eFavoriteId
	 || eFavoriteId == CODE_UNICODE || eFavoriteId == CODE_UNICODEBE ){
		return;
	}

	for( i = eFavoriteId-1; 0 <= i; i-- ){
		if( i != CODE_UNICODE && i != CODE_UNICODEBE ){
			temp = m_aMbcPriority[i];
			m_aMbcPriority[i] = m_aMbcPriority[eFavoriteId];
			m_aMbcPriority[eFavoriteId] = temp;
		}
	}
}

/*!
	�R���X�g���N�^�����֐�
*/
void CESI::InitPriorityTable( const ECodeType eFavoriteCodeId )
{
	// �f�t�H���g�̗D�揇�ʕ\�� m_pMbPriority[] �Ɋۂ��ƃR�s�[
	for( int i = 0; i < CODE_CODEMAX; i++ ){
		m_aMbcPriority[i] = gm_aMbcDefaultPriority[i];
	}
	// eFavoriteId ���D��x���ł������Ȃ�悤�ɐݒ�
	MakePriorityTable( eFavoriteCodeId );
}



/*!
	�f�t�H���g�R���X�g���N�^
*/
bool CESI::SetInformation( const char *pS, const int nLen, ECodeType eFavoriteCodeId )
{
	if( pS == NULL || nLen < 1 ){
		return false;
	}

	InitPriorityTable( eFavoriteCodeId );   // MBCode �́i���z�� m_pMbInfo �̓Y�����Ɏg�p�����j�D�揇�ʕ\���쐬�B
	ScanMBCode( pS, nLen );    // �}���`�o�C�g�R�[�h�ɂ������钲���B
	ScanUnicode( pS, nLen );   // Unicode �ɂ������钲���B
	SetDataLen( nLen );      // �ΏۂƂȂ����f�[�^�����L�^�B

	return true;
}



/*!
	�����R�[�h ID ������i�[�z�� m_pMbInfo �܂��� m_pWcInfo �̓Y�������擾

	@return
	�@nCodeType �� CODE_UNICODE, CODE_UNICODEBE �̏ꍇ�� m_pWcInfo �p�̓Y�������C
	����ȊO�̏ꍇ�� m_pMbInfo �p�̓Y�������ԋp����邱�Ƃɒ��ӁB
*/
int CESI::GetIndexById( const ECodeType eCodeType ) const
{
	int nret;
	if( CODE_UNICODE == eCodeType ){
		nret = 0;
	}else if( CODE_UNICODEBE == eCodeType ){
		nret = 1;
	}else{
		nret = m_aMbcPriority[eCodeType];
		// �D�揇�ʕ\�̗D��x�������̂܂� m_aMbcInfo �̓Y�����Ƃ��Ďg���B
	}
	return nret;
}

/*!
	���W�����]���l���i�[

	@return
	�@nCodeType �� CODE_UNICODE, CODE_UNICODEBE �̏ꍇ�� m_pWcInfo �ցC
	����ȊO�̏ꍇ�� m_pMbInfo �֒l���i�[����邱�Ƃɒ��ӁB
*/
void CESI::SetEvaluation( const ECodeType eCodeId, const int v1, const int v2 )
{
	int nidx;
	struct tagEncodingInfo *pcEI;

	nidx = GetIndexById( eCodeId );
	if( eCodeId == CODE_UNICODE || eCodeId == CODE_UNICODEBE ){
		pcEI = &m_aWcInfo[nidx];
	}else{
		pcEI = &m_aMbcInfo[nidx];
	}
	pcEI->eCodeID = eCodeId;
	pcEI->nSpecific = v1;
	pcEI->nPoints = v2;

	return;
}


/*!
	���W�����]���l���擾

	@return
	�@nCodeType �� CODE_UNICODE, CODE_UNICODEBE �̏ꍇ�� m_pWcInfo ����C
	����ȊO�̏ꍇ�� m_pMbInfo ����l���擾����邱�Ƃɒ��ӁB
*/
void CESI::GetEvaluation( const ECodeType eCodeId, int *pv1, int *pv2 ) const
{
	int nidx;
	const struct tagEncodingInfo *pcEI;

	nidx = GetIndexById( eCodeId );
	if( eCodeId == CODE_UNICODE || eCodeId == CODE_UNICODEBE ){
		pcEI = &m_aWcInfo[nidx];
	}else{
		pcEI = &m_aMbcInfo[nidx];
	}
	*pv1 = pcEI->nSpecific;
	*pv2 = pcEI->nPoints;

	return;
}



/*!
	�D�揇�ʕ\����ł��D��x�̍��������R�[�h���擾
*/
ECodeType CESI::GetFavoriteCode( void ) const
{
	int i, v;
	for( v = 0, i = 1; i < CODE_CODEMAX; i++ ){
		if( m_aMbcPriority[v] > m_aMbcPriority[i] ){
			v = i;
		}
	}
	return static_cast<ECodeType>( v );
}



/*!
	�z�� m_pMbInfo �̗v�f�ւ̃|�C���^��]�����Ƀ\�[�g���� m_ppMbInfo �ɃZ�b�g

	m_pMbInfo �Ɋi�[����镶���R�[�h���̌��X�̏��Ԃ́Am_pMbPriority[] �e�[�u���̓Y�����ɏ]���B
	�o�u���\�[�g�́A���������������r�I�ύX���Ȃ��B
*/
void CESI::SortMBCInfo( void )
{
	MBCODE_INFO *pei_tmp;
	int i, j;

	/*
		�u���L�o�C�g�� �| �s���o�C�g�����|�C���g�� (.nPoints)�v�̐��̑傫�����Ƀ\�[�g�i�o�u���\�[�g�j
	*/
	for( i = 0; i < NUM_OF_MBCODE; i++ ){
		m_apMbcInfo[i] = &m_aMbcInfo[i];
	}
	for( i = 1; i < NUM_OF_MBCODE; i++ ){
		for( j = 0; j < NUM_OF_MBCODE - i; j++ ){
			if( m_apMbcInfo[j]->nPoints < m_apMbcInfo[j+1]->nPoints ){
				pei_tmp = m_apMbcInfo[j+1];
				m_apMbcInfo[j+1] = m_apMbcInfo[j];
				m_apMbcInfo[j] = pei_tmp;
			}
		}
	}
}









/*!
	SJIS �̕����R�[�h����������W����
*/
void CESI::GetEncodingInfo_sjis( const char* pS, const int nLen )
{
	const char *pr, *pr_end;
	int nillbytes, num_of_sjis_encoded_bytes, num_of_sjis_hankata;
	int nret;
	ECharSet echarset;

	if( nLen < 1 ){
		SetEvaluation( CODE_SJIS, 0, 0 );
		m_nMbcSjisHankata = 0;
		return;
	}

	nillbytes = 0;
	num_of_sjis_encoded_bytes = 0;
	num_of_sjis_hankata = 0;
	pr = pS;
	pr_end = pr + nLen;

	for( ; 0 != (nret = CheckSjisChar(pr, pr_end-pr, &echarset)); pr += nret ){
		if( echarset != CHARSET_BINARY ){
			if( echarset == CHARSET_JIS_ZENKAKU ){
				num_of_sjis_encoded_bytes += nret;
			}
			if( echarset == CHARSET_JIS_HANKATA ){
				num_of_sjis_hankata++;
			}
		}else{
			if( pr_end - pr < GuessSjisCharsz(pr[0]) ){ break; }
			nillbytes += nret;
		}
	}

	SetEvaluation( CODE_SJIS,
		num_of_sjis_encoded_bytes,
		num_of_sjis_encoded_bytes - nillbytes );

	m_nMbcSjisHankata = num_of_sjis_hankata;

	return;
}



/*!
	JIS �̕����R�[�h����������W����
*/
void CESI::GetEncodingInfo_jis( const char* pS, const int nLen )
{
	const char *pr, *pr_end;
	char *pr_next;
	int nescbytes, nillbytes;
	int nlen, nerror;
	EMyJisEscseq emyjisesc;

	if( nLen < 1 ){
		SetEvaluation( CODE_JIS, 0, 0 );
		return;
	}

	nescbytes = 0;
	nillbytes = 0;
	pr = pS;
	pr_end = pS + nLen;
	emyjisesc = MYJISESC_ASCII7;

	do{
		switch( emyjisesc ){
		case MYJISESC_ASCII7:
			nlen = CheckJisAscii7Part( pr, pr_end-pr, &pr_next, &emyjisesc, &nerror );
			break;
		case MYJISESC_HANKATA:
			nlen = CheckJisHankataPart( pr, pr_end-pr, &pr_next, &emyjisesc, &nerror );
			break;
		case MYJISESC_ZENKAKU:
			nlen = CheckJisZenkakuPart( pr, pr_end-pr, &pr_next, &emyjisesc, &nerror );
			break;
		//case MYJISESC_UNKNOWN:
		default:
			nlen = CheckJisUnknownPart( pr, pr_end-pr, &pr_next, &emyjisesc, &nerror );
		}
		nescbytes += pr_next-(pr+nlen);
		nillbytes += nerror;
		pr = pr_next;
	}while( pr_next < pr_end );

	if( nillbytes ){
		SetEvaluation( CODE_JIS, 0, INT_MIN );
	}else{
		SetEvaluation( CODE_JIS, nescbytes, nescbytes );
	}

	return;
}



/*!
	EUC-JP �̕����R�[�h����������W����
*/
void CESI::GetEncodingInfo_eucjp( const char* pS, const int nLen )
{
	const char *pr, *pr_end;
	int nillbytes, num_of_eucjp_encoded_bytes;
	int nret;
	ECharSet echarset;

	if( nLen < 1 ){
		SetEvaluation( CODE_EUC, 0, 0 );
		return;
	}

	nillbytes = 0;
	num_of_eucjp_encoded_bytes = 0;
	pr = pS;
	pr_end = pr + nLen;

	for( ; 0 != (nret = CheckEucjpChar(pr, pr_end-pr, &echarset)); pr += nret ){
		if( echarset != CHARSET_BINARY ){
			if( 1 < nret ){
				num_of_eucjp_encoded_bytes += nret;
			}
		}else{
			if( pr_end - pr < GuessEucjpCharsz(pr[0]) ){ break; }
			nillbytes += nret;
		}
	}

	SetEvaluation( CODE_EUC,
		num_of_eucjp_encoded_bytes,
		num_of_eucjp_encoded_bytes - nillbytes );

	return;
}




/*!
	UTF-7 �̕����R�[�h����������W����

	@note
	�@1 �o�C�g�ȏ�̃G���[�i������ ill-formed�j��������� UTF-7 �Ɣ��肳��Ȃ��B
*/
void CESI::GetEncodingInfo_utf7( const char* pS, const int nLen )
{
	const char *pr, *pr_end;
	char *pr_next;
	int npoints, nlen_setd, nlen_setb;
	bool berror;

	if( nLen < 1 ){
		SetEvaluation( CODE_UTF7, 0, 0 );
		return;
	}

	CMemory cmbuffer;

	npoints = 0;
	berror = false;
	pr = pS;
	pr_end = pS + nLen;


	do{ // �������[�v --------------------------------------------------

		/* �Z�b�gD/O ������̌��� */

		nlen_setd = CheckUtf7DPart( pr, pr_end-pr, &pr_next, &berror );

		if( berror || pr+nlen_setd == pr_end ){
			// �G���[���o���A�܂��́A�f�[�^�̌������I�������ꍇ�A�����I���B
			//goto finish_codecheck;
			break;
		}
		pr = pr_next;

		/* �Z�b�gB ������̌��� */

		nlen_setb = CheckUtf7BPart( pr, pr_end-pr, &pr_next, &berror );

		if( pr+nlen_setb == pr_next && pr_next == pr_end ){
			// �Z�b�g�a������̏I�[���� '-' �������A���A
			// ���̓ǂݍ��݈ʒu�������f�[�^�̏I�[������ɂ���ꍇ�A�G���[���������Č����I��
			berror = false;
			//goto finish_codecheck;
			break;
		}
		if( berror ){
			// ��ȏ�̃G���[��������΁A�����I���B
			//goto finish_codecheck;
			break;
		}

		// ���ۂɃf�R�[�h���ē��e���m�F����B
		wchar_t* pdata;
		int ndatalen, nret;
		ECharSet echarset;

		cmbuffer.AllocBuffer( nlen_setb );
		pdata = reinterpret_cast<wchar_t*>( cmbuffer.GetRawPtr() );
		if( pdata != NULL ){
			ndatalen = _DecodeBase64( pr, nlen_setb, reinterpret_cast<char*>(pdata) ) / sizeof(wchar_t);
			CMemory::SwapHLByte( reinterpret_cast<char*>(pdata), ndatalen * sizeof(wchar_t) );
			for( int i = 0; i < ndatalen; i++ ){
				nret = CheckUtf16leChar( &pdata[i], ndatalen - i, &echarset );
				if( echarset == CHARSET_BINARY ){
					berror = true;
					goto finish_codecheck;
				}

				if( nret == 1 ){
					if( IsUtf7SetD(pdata[i]) ){
						berror = true;
						goto finish_codecheck;
					}
				}
			}
		}
		pr = pr_next;
		npoints += nlen_setb; // �Z�b�g�a�̕�����̒������|�C���g�Ƃ��ĉ��Z����B

	}while( pr_next < pr_end );  // �������[�v�I��  --------------------

finish_codecheck:;


	if( berror ){
		// �G���[���������ꂽ�ꍇ�A�|�C���g���}�C�i�X�l�ɂ��Ă����B
		npoints = INT_MIN;
	}

	if( npoints < 0 ){
		SetEvaluation( CODE_UTF7, 0, npoints );
	}else{
		SetEvaluation( CODE_UTF7, npoints, npoints );
		// UTF-7 ���L�̕�����́A�Z�b�g�a�̕�����̒����Ƃ���B
	}

	return;
}



/*!
	UTF-8 �̕����R�[�h����������W����
*/
void CESI::GetEncodingInfo_utf8( const char* pS, const int nLen )
{
	const char *pr, *pr_end;
	int nillbytes, num_of_utf8_encoded_bytes;
	int nret;
	ECharSet echarset;

	if( nLen < 1 ){
		SetEvaluation( CODE_UTF8, 0, 0 );
		return;
	}

	nillbytes = 0;
	num_of_utf8_encoded_bytes = 0;
	pr = pS;
	pr_end = pS + nLen;

	for( ; 0 != (nret = CheckUtf8Char(pr, pr_end-pr, &echarset)); pr += nret ){
		if( echarset != CHARSET_BINARY ){
			if( 1 < nret ){
				num_of_utf8_encoded_bytes += nret;
			}
		}else{
			if( pr_end - pr < GuessUtf8Charsz(pr[0]) ){ break; }
			nillbytes += nret;
		}
	}

	SetEvaluation( CODE_UTF8,
		num_of_utf8_encoded_bytes,
		num_of_utf8_encoded_bytes - nillbytes );

	return;
}


/*
	UTF-16 �`�F�b�J���Ŏg�����s�R�[�h�m�F�֐�
*/
bool CESI::_CheckUtf16Eol( const char* pS, const int nLen, const bool bbig_endian )
{
	wchar_t wc0;
	wchar_t wc1;

	if( nLen >= 4 ){
		if( bbig_endian ){
			wc0 = (pS[0] << 8) | pS[1];
			wc1 = (pS[2] << 8) | pS[3];
		}else{
			wc0 = pS[0] | (pS[1] << 8);
			wc1 = pS[2] | (pS[3] << 8);
		}
		if( (wc0 == 0x000d && wc1 == 0x000a) || wc0 == 0x000d || wc0 == 0x000a ){
			return true;
		}
	}else if( nLen >= 2 ){
		if( bbig_endian ){
			wc0 = (pS[0] << 8) | pS[1];
		}else{
			wc0 = pS[0] | (pS[1] << 8);
		}
		if( wc0 == 0x000d || wc0 == 0x000a ){
			return true;
		}
	}
	return false;

}



/*!
	UTF-16 LE/BE �̕����R�[�h����������W����

	@note ���s�����𐔂���B���ł� ASCII �ł̉��s�R�[�h��������B
	�@nLen �͂Q�Ŋ���؂����̂������Ă��邱�Ƃ�����B
*/
void CESI::GetEncodingInfo_uni( const char* pS, const int nLen )
{
	const char *pr1, *pr2, *pr_end;
	int nillbytes1, nillbytes2;
	int nnewlinew1, nnewlinew2;
	register int nret1, nret2;
	ECharSet echarset1, echarset2;

	if( nLen < 1 || nLen % 2 == 1 ){
		if( nLen < 1 ){
			SetEvaluation( CODE_UNICODE, 0, 0 );
			SetEvaluation( CODE_UNICODEBE, 0, 0 );
		}else{
			// 2 �Ŋ���؂�Ȃ������̃f�[�^�������ꍇ�́A��₩��O���B
			SetEvaluation( CODE_UNICODE, 0, INT_MIN );
			SetEvaluation( CODE_UNICODEBE, 0, INT_MIN );
		}
		m_eWcBomType = ESI_BOMTYPE_UNKNOWN;
		return;
	}

	nillbytes1 = nillbytes2 = 0;
	nnewlinew1 = nnewlinew2 = 0;
	pr1 = pr2 = pS;
	pr_end = pS + nLen;

	for( ; ; ){

		nret1 = CheckUtf16leChar( reinterpret_cast<const wchar_t*>(pr1), (pr_end - pr1)/sizeof(wchar_t), &echarset1 );
		nret2 = CheckUtf16beChar( reinterpret_cast<const wchar_t*>(pr2), (pr_end - pr2)/sizeof(wchar_t), &echarset2 );
		if( nret1 == 0 && nret2 == 0 ){
			// LE BE �������̃`�F�b�N���I�������B
			break;
		}

		//
		// (pr_end - pr1) �� (pr_end - pr2) �͏�� sizeof(wchar_t) �Ŋ���؂�邱�ƁB
		//

		// UTF-16 LE �̏���
		if( nret1 != 0 ){
			if( echarset1 != CHARSET_BINARY ){
				if( nret1 == 1 ){
					// UTF-16 LE �ł̉��s�R�[�h���J�E���g
					if( _CheckUtf16EolLE(pr1, pr_end-pr1) ){ nnewlinew1++; }
				}
			}else{
				unsigned int n = GuessUtf16Charsz( pr1[0] | static_cast<wchar_t>(pr1[1] << 8) );
				if( (pr_end-pr1)/sizeof(wchar_t) < n ){
					break;
				}
				nillbytes1 += nret1*sizeof(wchar_t);
			}
			pr1 += nret1*sizeof(wchar_t);
		}

		// UTF-16 BE �̏���
		if( nret2 != 0 ){
			if( echarset2 != CHARSET_BINARY ){
				if( nret2 == 1 ){
					// UTF-16 BE �ł̉��s�R�[�h���J�E���g
					if( _CheckUtf16EolBE(pr2, pr_end-pr2) ){ nnewlinew2++; }
				}
			}else{
				unsigned int n = GuessUtf16Charsz((static_cast<wchar_t>(pr2[0] << 8)) | pr2[1]);
				if( (pr_end-pr2)/sizeof(wchar_t) < n ){
					break;
				}
				nillbytes2 += nret2*sizeof(wchar_t);
			}
			pr2 += nret2*sizeof(wchar_t);
		}
	}

	if( nillbytes1 < 1 ){
		SetEvaluation( CODE_UNICODE, nnewlinew1, nnewlinew1 );
	}else{
		SetEvaluation( CODE_UNICODE, 0, INT_MIN );
	}
	if( nillbytes2 < 1 ){
		SetEvaluation( CODE_UNICODEBE, nnewlinew2, nnewlinew2 );
	}else{
		SetEvaluation( CODE_UNICODEBE, 0, INT_MIN );
	}

	// BOM��ʂ𐄑����Ă����B
	m_eWcBomType = GuessUtf16Bom();

	return;
}






/*!
	�e�L�X�g�̕����R�[�h�����W���Đ�������D

	@return ���̓f�[�^���Ȃ����� false
*/
void CESI::ScanMBCode( const char* pS, const int nLen )
{
	/* �}���`�o�C�g�����̒��� */
	GetEncodingInfo_sjis( pS, nLen );
	GetEncodingInfo_eucjp( pS, nLen );
	GetEncodingInfo_jis( pS, nLen );
	GetEncodingInfo_utf8( pS, nLen );
	GetEncodingInfo_utf7( pS, nLen );
	SortMBCInfo();
}

void CESI::ScanUnicode( const char* pS, const int nLen )
{
	GetEncodingInfo_uni( pS, nLen );
	GuessUtf16Bom();
}




/*!
	UTF-16 �� BOM �̎�ނ𐄑�

	@retval CESI_BOMTYPE_UTF16LE   Little-Endian(LE)
	@retval CESI_BOMTYPE_UTF16BE   Big-Endian(BE)
	@retval CESI_BOMTYPE_UNKNOWN   �s��
*/
EBOMType CESI::GuessUtf16Bom( void ) const
{
	int i, j;
	EBOMType ebom_type;

	i = m_aWcInfo[ESI_WCIDX_UTF16LE].nSpecific;  // UTF-16 LE �̉��s�̌�
	j = m_aWcInfo[ESI_WCIDX_UTF16BE].nSpecific;  // UTF-16 BE �̉��s�̌�
	ebom_type = ESI_BOMTYPE_UNKNOWN;
	if( i > j && j < 1 ){
		ebom_type = ESI_BOMTYPE_LE;   // LE
	}else if( i < j && i < 1 ){
		ebom_type = ESI_BOMTYPE_BE;   // BE
	}
	if( ebom_type != ESI_BOMTYPE_UNKNOWN ){
		if( m_aWcInfo[ebom_type].nSpecific - m_aWcInfo[ebom_type].nPoints > 0 ){
			// �s���o�C�g�����o����Ă���ꍇ�́ABOM �^�C�v�s���Ƃ���B
			ebom_type = ESI_BOMTYPE_UNKNOWN;
		}
	}

	return ebom_type;
}














#ifdef _DEBUG


/*!
	���W���������_���v����

	@param[out] pcmtxtOut �o�͂́A���̃|�C���^���w���I�u�W�F�N�g�ɒǉ������B

	@note
		@li m_nTargetDataLen �� 1 �����̂Ƃ��̓T���v���f�[�^���݂̂��_���v���ďI���B
		@li CDlgProperty::SetData() �֐��Ƌ�������悤�ɏo�͂���B
*/
void CESI::GetDebugInfo( const char* pS, const int nLen, CNativeT* pcmtxtOut )
{
	TCHAR szWork[10240];
	int v1, v2, v3, v4;
	int i;

	ECodeType ecode_result;

	// �e�X�g���s
	CESI cesi;
	cesi.SetInformation( pS, nLen, CODE_SJIS );
	ecode_result = CCodeMediator::CheckKanjiCode( &cesi );

	//
	//	���ʌ��ʂ𕪐�
	//

	pcmtxtOut->AppendString( _T("--�����R�[�h��������-----------\r\n") );
	pcmtxtOut->AppendString( _T("���ʌ��ʂ̏��\r\n") );


	if( cesi.m_nTargetDataLen < 1 || cesi.m_dwStatus == ESI_NOINFORMATION ){
		pcmtxtOut->AppendString( _T("\t���ʌ��ʂ��擾�ł��܂���B\r\n") );
		return;
	}
	if( cesi.m_dwStatus != ESI_NODETECTED ){
		// nstat == CESI_MBC_DETECTED or CESI_WC_DETECTED
		pcmtxtOut->AppendString( _T("\t�����炭����ɔ��肳��܂����B\r\n") );
	}else{
		pcmtxtOut->AppendString( _T("\t�R�[�h�����o�ł��܂���ł����B\r\n") );
	}


	pcmtxtOut->AppendString( _T("�D�悳�ꂽ�����R�[�h\r\n") );


	CCodeTypeName ccodetype( cesi.GetFavoriteCode() );
	auto_sprintf( szWork, _T("\t%s\r\n"), ccodetype.Normal() );
	pcmtxtOut->AppendString( szWork );


	pcmtxtOut->AppendString( _T("�T���v���f�[�^��\r\n") );


	auto_sprintf( szWork, _T("\t%d �o�C�g\r\n"), cesi.GetDataLen() );
	pcmtxtOut->AppendString( szWork );


	pcmtxtOut->AppendString( _T("�ŗL�o�C�g���ƃ|�C���g��\r\n") );
	
	
	pcmtxtOut->AppendString( _T("\tUNICODE\r\n") );
	cesi.GetEvaluation( CODE_UNICODE, &v1, &v2 );
	cesi.GetEvaluation( CODE_UNICODEBE, &v3, &v4 );
	auto_sprintf( szWork, _T("\t\tUTF16LE �ŗL�o�C�g�� %d,\t�|�C���g�� %d\r\n"), v1, v2 );
	pcmtxtOut->AppendString( szWork );
	auto_sprintf( szWork, _T("\t\tUTF16BE �ŗL�o�C�g�� %d,\t�|�C���g�� %d\r\n"), v3, v4 );
	pcmtxtOut->AppendString( szWork );
	pcmtxtOut->AppendString( _T("\t\tBOM �̐������ʁ@") );
	switch( cesi.m_eWcBomType ){
	case ESI_BOMTYPE_LE:
		auto_sprintf( szWork, _T("LE\r\n") );
		break;
	case ESI_BOMTYPE_BE:
		auto_sprintf( szWork, _T("BE\r\n") );
		break;
	default:
		auto_sprintf( szWork, _T("�s��\r\n") );
	}
	pcmtxtOut->AppendString( szWork );
	pcmtxtOut->AppendString( _T("\tMBC �� ��L�ȊO�� UNICODE �t�@�~��\r\n") );
	for( i = 0; i < NUM_OF_MBCODE; ++i ){
		cesi.GetEvaluation( cesi.m_apMbcInfo[i]->eCodeID, &v1, &v2 );
		CCodeTypeName ccodetypename( cesi.m_apMbcInfo[i]->eCodeID );
		auto_sprintf( szWork, _T("\t\t%d.%s\t�ŗL�o�C�g�� %d\t�|�C���g�� %d\r\n"), i+1, ccodetypename.Normal(), v1, v2 );
		pcmtxtOut->AppendString( szWork );
	}
	if( cesi.m_dwStatus & ESI_MBC_HANKATACOUNTED ){
		auto_sprintf( szWork, _T("\t\t�iSJIS ���p�J�^�J�i %d �o�C�g�����l������܂����B�j\r\n"), cesi.m_nMbcSjisHankata );
		pcmtxtOut->AppendString( szWork );
	}
	if( cesi.m_dwStatus & ESI_MBC_CESU8DETECTED ){
		pcmtxtOut->AppendString( _T("\t\t�iCESU-8 �����o����܂����B�j\r\n") );
	}

	pcmtxtOut->AppendString( _T("���茋��\r\n") );



	CCodeTypeName ccodetypename( ecode_result );
	auto_sprintf( szWork, _T("\t%s\r\n"), ccodetypename.Normal() );
	pcmtxtOut->AppendString( szWork );


	return;
}

#endif