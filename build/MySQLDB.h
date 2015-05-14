/**
 * @copyright   		Copyright (C) 2007, 2008  
 * @author              YunLong.Lee	<yunlong.lee@163.com> XuGang.Wang wangxg@csip.org.cn
 * @version             1.0beta
 */

#ifndef __MYSQL_DB_H__
#define __MYSQL_DB_H__

#include <string>
#include <list>
#include <mysql/mysql.h>
using namespace std;

typedef struct {
	string	dbhost;
	string	dbname;
	string	dbuser;
	string	dbpasswd;
	int 	dbport;
	string	dbsocket;
	int		dbcflag;
} dbConnParam;

#define POOL_SIZE 100
#define MAX_REF_CNT	5

typedef struct {
	MYSQL conn;
	int   refcnt;
}MySQLConn;

class MySQLConnPool
{
private:
    std::list<MySQLConn* > m_pool;
    int status;
public:
	MySQLConn* getConn(void);
	void putConn(MySQLConn* conn);
	void clearPool(void);
	
};


class MySQLDB {
private:	
    MYSQL 			m_conn;
    bool 			m_connected;
    dbConnParam     m_dbparam;
public:
	MySQLDB(dbConnParam& param);
	~MySQLDB(void);
	
	bool InitMySQL(void);
	bool connectMySQL(dbConnParam * p);
	bool execQuery(string sql);
	MYSQL_RES* storeResultSet();
	void closeMySQL();
	void freeRecordSet(MYSQL_RES *result);
	bool selectMySQLDB(string dbname);
	MYSQL_FIELD * fetchField( MYSQL_RES* result );
	MYSQL_ROW fetchRow(MYSQL_RES* result);
	int getInsertId();
	int getRowsNum(MYSQL_RES* result);
	int db_escape_string(char *dst, char *src, int srclen);
	int getAffectedRows();
	void showErrMsg(char *msg);	
	int getFieldsNum(MYSQL_RES* result);
};

#endif
