<?php

define('CROWD_DB_HOST', 'localhost');
define('CROWD_DB_IPADDR', '127.0.0.1');
define('CROWD_DB_USER', 'root');
define('CROWD_DB_PASS', '123456');
define('CROWD_DB_NAME', 'appstore');

class MySQLDatabase
{
	/**
	 * Database connection
	 * @var resource
	 */
	var $conn;

	function MySQLDatabase()
	{
	}

	function dbconnect($dbhost, $dbuser, $dbpass, $dbname, $selectdb = true)
	{
		if ( !extension_loaded( 'mysql' ) ) 
		{
			trigger_error( 'notrace:mysql extension not loaded', E_USER_ERROR );
			return false;
		}

		$this->conn = @mysql_connect($dbhost, $dbuser, $dbpass);
	
		if (!$this->conn) 
		{
			return false;
		}
		
		if($selectdb != false)
		{
			if ( !mysql_select_db($dbname) ) 
			{
				return false;
			}
		}

		$this->query("SET CHARACTER SET utf8");
		$this->query("SET NAMES utf8");
	
		return true;	
	}

	/**
	 * generate an ID for a new row
     * This is for compatibility only. Will always return 0, because MySQL supports
     * autoincrement for primary keys.
     * @param string $sequence name of the sequence from which to get the next ID
     * @return int always 0, because mysql has support for autoincrement
	 */
	function genId($sequence)
	{
		return 0; // will use auto_increment
	}

	/**
	 * Get a result row as an enumerated array
     * @param resource $result
     * @return array
	 */
	function fetchRow($result)
	{
		return @mysql_fetch_row($result);
	}

	/**
	 * Fetch a result row as an associative array
     * @return array
	 */
	function fetchArray($result)
    {
       	return @mysql_fetch_assoc( $result );
    }

	//Fetch a result row as an object
    function fetchObject($result)
    {
        return @mysql_fetch_object( $result );
    }

    /**
     * Fetch a result row as an associative array
     * @return array
     */
    function fetchBoth($result)
    {
        return @mysql_fetch_array( $result, MYSQL_BOTH );
    }

	/**
	 * Get the ID generated from the previous INSERT operation
     * @return int
	 */
	function getInsertId()
	{
		return mysql_insert_id($this->conn);
	}

	/**
	 * Get number of rows in result
     * @param resource query result
     * @return int
	 */
	function getRowsNum($result)
	{
		return @mysql_num_rows($result);
	}

	/**
	 * Get number of affected rows
     * @return int
	 */
	function getAffectedRows()
	{
		return mysql_affected_rows($this->conn);
	}

	/**
	 * Close MySQL connection
	 */
	function close()
	{
		mysql_close($this->conn);
	}

	/**
	 * will free all memory associated with the result identifier result.
     * @param resource query result
     * @return bool TRUE on success or FALSE on failure. 
	 */
	function freeRecordSet($result)
	{
		return mysql_free_result($result);
	}

	/**
	 * Returns the text of the error message from previous MySQL operation
     * @return bool Returns the error text from the last MySQL function, 
	 * or '' (the empty string) if no error occurred. 
	 */
	function error()
	{
		return @mysql_error();
	}

	/**
	 * Returns the numerical value of the error message from previous MySQL operation 
     * @return int Returns the error number from the last MySQL function, or 0 (zero) if no error occurred. 
	 */
	function errno()
	{
		return @mysql_errno();
	}

    /**
     * Returns escaped string text with single quotes around it to be safely stored in database
     * @param string $str unescaped string text
     * @return string escaped string text with single quotes around
     */
    function quoteString($str)
    {
         $str = "'".str_replace('\\"', '"', addslashes($str))."'";
         return $str;
    }

    /**
     * perform a query on the database
     * @param string $sql a valid MySQL query
     * @param int $limit number of records to return
     * @param int $start offset of first record to return
     * @return resource query result or FALSE if successful
     * or TRUE if successful and no result
     */
   	function query($sql, $limit=0, $start=0)
	{
		if ( !empty($limit) ) 
		{
			if (empty($start)) 
			{
				$start = 0;
			}
			$sql = $sql. ' LIMIT '.(int)$start.', '.(int)$limit;
		}

		//echo $sql."<br>";
		$result = @mysql_query($sql, $this->conn);
		//echo $result."<br>";
		if ( $result ) 
		{
			return $result;
       	} 
		else 
		{
			return false;
       	}
    }

    /**
	 * Get field name
     * @param resource $result query result
     * @param int numerical field index
     * @return string
	 */
	function getFieldName($result, $offset)
	{
		return mysql_field_name($result, $offset);
	}

	/**
	 * Get field type
     * @param resource $result query result
     * @param int $offset numerical field index
     * @return string
	 */
   	function getFieldType($result, $offset)
	{
		return mysql_field_type($result, $offset);
	}

	/**
	 * Get number of fields in result
     * @param resource $result query result
     * @return int
	 */
	function getFieldsNum($result)
	{
		return mysql_num_fields($result);
	}

	/*
 	 *	db_begin() - Begin a transaction
 	 *
 	 *	Begin a transaction for databases that support them
 	 *	may cause unexpected behavior in databases that don't
 	 */
	function startTransaction() 
	{
		return $this->query("BEGIN WORK");
	}

	/*
 	 * db_commit() - Commit a transaction
	 *
	 * Commit a transaction for databases that support them
	 * may cause unexpected behavior in databases that don't
 	 */
	function commitTransaction() 
	{	
		return $this->query("COMMIT");
	}

	/*
	 * db_rollback() - Roll back a transaction
	 *
	 * Rollback a transaction for databases that support them
	 * may cause unexpected behavior in databases that don't
	 */
	function rollbackTransaction() 
	{
		return $this->query("ROLLBACK");
	}

	/*
	 *  db_reset_result() - Reset a result set.
	 *  Reset is useful for db_fetch_array sometimes you need to start over
	 *  @param		string	Query result set handle
	 *  @param		int		Row number
	 */
	function resetResult($result, $row = 0) 
	{
		return @mysql_data_seek($result, $row);
	}
}

?>

