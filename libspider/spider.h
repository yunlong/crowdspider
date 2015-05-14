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

#ifndef __SPIDER_H__
#define __SPIDER_H__

//#define ENABLE_DEBUG_MODE
#define _FILE_OFFSET_BITS 64

#include <openssl/ssl.h>
#include <openssl/err.h>

#define	TRACE(x)	do { printf("%s, %d, %s\n", __FILE__, __LINE__, x);	\
					} while(0);
					
#define SOFTWARE_NAME 		"crowdspider by yunlong.lee"
#define SOFTWARE_VERSION 	"0.5v"

typedef enum 
{
/////////////////////////////
	PROXY_DIRECT,
	PROXY_HTTP,
	PROXY_FTP,
	PROXY_SOCK4,
	PROXY_SOCK5,
	HTTP_CONNECT
/////////////////////////////	
} ProxyType;

typedef struct
{
	string		name;
	ProxyType	type;
	string		host;
	string		user;
	string		pass;
	bool		auth;
	int			port;
} Proxy_t;

typedef unsigned long long  	ULONG64;
typedef unsigned long  			ULONG32;


#define FTP_PORT 	21
#define HTTP_PORT 	80
#define TRUE	1
#define FALSE   0

/* The default no of connections and maximum redirections allowed  */
#define DEFAULT_CONN_NUM  	4
#define MAX_REDIRECTIONS  	5
/* The timeout period for connections in seconds */
#define CONN_TIMEOUT 		120
/* The max number of trys and the delay between each */
#define MAX_TRY_ATTEMPTS    30
#define RETRY_INTERVAL		5
/* The maximum number of servers to ping at once */
#define MAX_SIMU_PINGS		5
/* The max number of seconds to wait for a server response to ping */
#define MAX_PING_WAIT		4
#define MAX_BPS				-1   

/*Output the file to the directory ( "./" by default) */
#define  MAX_SIZE       	((ULONG64)(-1))
#define MAX_BLK_SIZE		((ULONG32)(-1))
#define MIN_BLK_SIZE 		32

#define MIN_FILE_SIZE		(64 * 1024)
#define MAX_FILE_SIZE	   ((ULONG64)(-1))

#define MAX_THREADS 	6
#define MAX_NAME_LEN	255
#define MAX_PATH_LEN	1024

#define LINE_BUFFER 	256
#define FTP_BUF_SIZE 	4096
//#define HTTP_BUF_SIZE 	4096
#define CACHE_SIZE		(512*1024)
#define MAX_MSG_SIZE 	1024
#define DEFAULT_USER  			"anonymous"
#define DEFAULT_PASSWD 			"yunlong.lee@163.com"

#define NSR_PROCESS_ERROR(expr)  \
	if (expr)	\
		goto errout
	
#define NSR_PROCESS_ERROUT(expr, retval)     \
	if (expr)	\
	{		\
		ret = retval;	\
		goto errout;	\
	}

#define NSR_PROCESS_RETURN(expr, retval)  \
	if (expr)	\
		return retval

#define NSR_PROCESS_SERVICE(expr)         \
	if ((expr) != 0)	\
		goto errout;

#define NSR_PROCESS_SOCKSLEN(ret, size)     \
	if (ret < 0)	\
		goto errout;	\
	else if (ret < (int)(size))	\
	{		\
		ret = -EPROTO;	\
		goto errout;	\
	}


#endif

