/**
 * @copyright   		Copyright (C) 2007, 2008  
 * @author              YunLong.Lee	<yunlong.lee@163.com> XuGang.Wang wangxg@csip.org.cn
 * @version             1.0beta
 */

#include <string>
#include <iostream>
#include <mysql/mysql.h>
#include "MySQLDB.h"

#define MAX_MYSQL_RETRIES 4

using namespace std;

/*
static dbConnParam dbparam = {
    dbhost      :   "219.235.232.23",
    dbname      :   "fanfourank",
    dbuser      :   "root",
    dbpasswd    :   "123456",
    dbport      :   3306,
    dbsocket    :   "/tmp/sock.pid",
    dbcflag     :   0,
};
*/

MySQLDB::MySQLDB(dbConnParam& param)
{
	m_connected = false;
	m_dbparam.dbhost        = param.dbhost;
	m_dbparam.dbname        = param.dbname;
	m_dbparam.dbuser        = param.dbuser;
	m_dbparam.dbpasswd      = param.dbpasswd;
	m_dbparam.dbport        = 3306;
	m_dbparam.dbsocket      = "/tmp/sock.pid";
	m_dbparam.dbcflag       = 0;
}
	
MySQLDB::~MySQLDB(void)
{
	closeMySQL();
	//Logger::closeLog();
}
	
bool MySQLDB::InitMySQL(void)
{
	if(mysql_init(&m_conn) == NULL)
	{
		showErrMsg("init mysql data stauct fail");
 		return false;
	}
	return true;
}
	
bool MySQLDB::connectMySQL(dbConnParam * p)
{
	if(mysql_real_connect(&m_conn, p->dbhost.c_str(), p->dbuser.c_str(), p->dbpasswd.c_str(), 
								p->dbname.c_str(), p->dbport, p->dbsocket.c_str(), p->dbcflag) == NULL)
	{
		showErrMsg("connect database fail");
		return false;
	}
	return true;
}

bool MySQLDB::execQuery(string sql)
{
	int	retries = 0;
	struct timespec	query_retry_delay = {1, 0};
	
	if(!m_connected) 
	{
		if(InitMySQL() == true)
		{
			if(connectMySQL(&m_dbparam) == true)
			{
				m_connected = true;
			}
		}
	}
//	else
//	{
//		return false;	
//	}

	while(mysql_query(&m_conn, sql.c_str()) != 0) 
	{
		showErrMsg("mysql_query failed");

		if(retries == MAX_MYSQL_RETRIES) 
		{
			fprintf(stderr, "Retried query %i times, giving up: \"%s\"\n", retries, sql.c_str());
			return false;
		}

		if(retries > 1) 
		{
			fprintf(stderr, "sleeping before trying again, make sure your query string isnt malformed.\n");
			nanosleep(&query_retry_delay, NULL);
		}

		if(mysql_ping(&m_conn)) 
		{
			closeMySQL();
			m_connected = false;
			if(connectMySQL(&m_dbparam) == true)
			{
				m_connected = true;
			}		
		} 
		retries++;
	}

	return true;
}
	
MYSQL_RES* MySQLDB::storeResultSet()
{
	MYSQL_RES*	result;
	if((result = mysql_store_result(&m_conn)) == NULL) 
	{
		showErrMsg("Failed to store result\n");
		return NULL;
	}

	return result;
}

void MySQLDB::closeMySQL()
{
	if( m_connected == true )
	{
		mysql_close(&m_conn);
	}
}

void MySQLDB::freeRecordSet(MYSQL_RES *result)
{
	mysql_free_result(result);
}

bool MySQLDB::selectMySQLDB(string dbname)
{
	if (mysql_select_db(&m_conn, dbname.c_str()) != 0) 
	{
		return false;
	}
	return true;
}

MYSQL_FIELD * MySQLDB::fetchField( MYSQL_RES* result )
{
	return mysql_fetch_field(result);
}

MYSQL_ROW MySQLDB::fetchRow(MYSQL_RES* result)
{
	return mysql_fetch_row(result);
}

int MySQLDB::getInsertId()
{
	return mysql_insert_id(&m_conn);
}

int MySQLDB::getRowsNum(MYSQL_RES* result)
{
	return mysql_num_rows(result);
}

int MySQLDB::db_escape_string(char *dst, char *src, int srclen)
{
	return mysql_real_escape_string(&m_conn, dst, src, srclen);
}

int MySQLDB::getAffectedRows()
{
	return mysql_affected_rows(&m_conn);
}

void MySQLDB::showErrMsg(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	fprintf(stderr, "Error %u (%s)\n", mysql_errno(&m_conn), mysql_error(&m_conn));
}
	
int MySQLDB::getFieldsNum(MYSQL_RES* result)
{
	return mysql_num_fields(result);
}

