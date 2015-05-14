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

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdexcept>
#include <utility>
#include <exception>
#include <new>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

#include <cstdarg>
#include <clocale>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cerrno>
#include <climits>
#include <cassert>
#include <cmath>
#include <pthread.h>
#include <time.h>
#include <dirent.h>
#include <locale.h>

#include <libintl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/err.h>


#include "Types.h"
#include "spider.h"

#ifdef USE_INIFILE_H
#include "IniFile.h"
#endif 

#ifdef USE_BITSET_H
#include "BitSet.h"
#endif 

#ifdef USE_BUFFER_H
#include "Buffer.h"
#endif

#ifdef USE_LIST_H
#include "List.h"
#endif 

#ifdef USE_MD5_H
#include "MD5.h"
#endif 

#ifdef USE_SHA_H
#include "Sha.h"
#endif 

#ifdef USE_SLAB_H
#include "Slab.h"
#endif

#ifdef USE_LOGGER_H
#include "Logger.h"
#endif

#ifdef USE_URI_H
#include "Uri.h"
#include "ParserDom.h"
using namespace htmlcxx;
#endif

#ifdef USE_HTTPMSG_H
#include "HttpMsg.h"
#endif

#ifdef USE_SOCKET_H
#include "Socket.h"
#endif 

#ifdef USE_POLLER_H
#include "Poller.h"
#endif

#ifdef USE_THREAD_H
#include "Thread.h"
#endif

#ifdef USE_UTILS_H
#include "Utils.h"
#endif

#ifdef USE_HASHTABLE_H
#include "HashTable.h"
#endif

#ifdef USE_HTTPCLI_H
#include "HttpCli.h"
#endif

#ifdef USE_CRAWLER_H
#include "Crawler.h"
#endif

#ifdef USE_RAWSERVER_H
#include "RawServer.h"
#endif

#ifdef USE_SOCK_PROXY_H
#include "SockProxy.h"
#endif

#endif

