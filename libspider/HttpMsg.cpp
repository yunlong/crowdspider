/*
 *  crowdspider is (just) a web crawler
 *  @author           YunLong.Lee    <yunlong.lee@163.com>
 *  @version          0.5v
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 */

#define USE_HTTPMSG_H
#define USE_UTILS_H

#include "matrix.h"

namespace matrix { 
/**************************************
 * THttpHdr
 **************************************/
THttpHdr::THttpHdr( const string& strKey ) : m_strKey(strKey)
{
    // Empty
}

THttpHdr::THttpHdr( const string& strKey, const string& strVal ) : m_strKey(strKey), m_strVal(strVal)
{
    // Empty
}

const string& THttpHdr::GetKey( void ) const
{
    return m_strKey;
}

const string& THttpHdr::GetVal( void ) const
{
    return m_strVal;
}

void THttpHdr::SetVal( const string& strVal )
{
    m_strVal = strVal;
}

/**************************************
 *
 * THttpMsg
 *
 **************************************/
THttpMsg::THttpMsg( void ) : m_nHttpVer(0)
{
    // Empty
}

THttpMsg::THttpMsg( THttpMsg& other )
{
	THttpHdrList::iterator itr =  other.m_listHdrs.begin() ;
	while( itr != other.m_listHdrs.end() )
	{
		THttpHdr* pHdr = *itr;
		m_listHdrs.push_back( new THttpHdr( *pHdr ) );
		itr++;
	}
}

void THttpMsg :: ShowAllHttpHdr(void)
{
	THttpHdr* pHdr;
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        pHdr = *itr;
        ShowMsg("%s: %s\n", pHdr->GetKey().c_str(), pHdr->GetVal().c_str());
    }
}

THttpMsg::~THttpMsg( void )
{
	while(!m_listHdrs.empty())
    {
    	THttpHdr* pHdr = m_listHdrs.front();
        delete pHdr;
        m_listHdrs.pop_front();
    }
}

void THttpMsg::GetHttpVer( unsigned int* puMajor, unsigned int* puMinor ) const
{
    assert( puMajor && puMinor );

    *puMajor = HIWORD( m_nHttpVer );
    *puMinor = LOWORD( m_nHttpVer );
}

void THttpMsg::SetHttpVer( unsigned int uMajor, unsigned int uMinor )
{
    assert( uMajor < 10 && uMinor < 10 );

    m_nHttpVer = MAKEDWORD( uMajor,uMinor );
}

size_t THttpMsg::GetHdrCount( void ) const
{
    return m_listHdrs.size();
}

size_t THttpMsg :: GetAllHdrLen( void )
{
    size_t nLen = 0;
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        nLen += ( pHdr->GetKey().length() + 2 + pHdr->GetVal().length() + 2 );
    }
    return nLen;
}

void THttpMsg :: ClearAllHdr(void)
{
	while(!m_listHdrs.empty())
    {
    	THttpHdr* pHdr = m_listHdrs.front();
        delete pHdr;
        m_listHdrs.pop_front();
    }
}

size_t THttpMsg :: GetHdrLen( unsigned int nIndex )
{	
	int Len;
	THttpHdr * pHdr = GetHdr(nIndex);
	if(pHdr != NULL)
	{
		Len = pHdr->GetKey().length() + 2 + pHdr->GetVal().length() + 2;
		return Len;
	}
	return -1;
} 

string THttpMsg::GetHdr( const string& strKey )
{
    string strVal;

    THttpHdrList::iterator itr =  m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( 0 == strcasecmp( strKey.c_str(), pHdr->GetKey().c_str() ) )
        {
            strVal = pHdr->GetVal();
            break;
        }
    }
    return strVal;
}

THttpHdr* THttpMsg::GetHdr( unsigned int nIndex )
{
 	THttpHdrList::iterator itr =  m_listHdrs.begin();
    for( unsigned int n = 0; n < nIndex; n++ )
    {
        itr++;
    }
    return *itr;
}

void THttpMsg::SetHdr( const string& strKey, const string& strVal )
{
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( 0 == strcasecmp( strKey.c_str(), pHdr->GetKey().c_str() ) )
        {
            pHdr->SetVal( strVal );
            return;
        }
    }
    m_listHdrs.push_back( new THttpHdr( strKey, strVal ) );
}

void THttpMsg::SetHdr( const THttpHdr& hdrNew )
{
    THttpHdrList::iterator itr = m_listHdrs.begin();
	for( ; itr != m_listHdrs.end(); ++itr )
    {
        THttpHdr* pHdr = *itr;
        if( hdrNew.GetKey() == pHdr->GetKey() )
        {
            pHdr->SetVal( hdrNew.GetVal() );
            return;
        }
    }
    m_listHdrs.push_back( new THttpHdr( hdrNew ) );
}

}


