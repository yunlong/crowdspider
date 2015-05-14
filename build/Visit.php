<?php
require_once 'class.mysqldb.php';

$dbinst = new MySQLDatabase();
//$dbinst->dbconnect( trim(CROWD_DB_HOST), trim(CROWD_DB_USER), trim(CROWD_DB_PASS), trim(CROWD_DB_NAME) );

$handle = fopen ("desc.txt", "r");
$i=0;
while (!feof ($handle)) 
{
	$i++;
    $buffer = fgets($handle, 10240);
	$strSQL = sprintf("UPDATE app_soft SET soft_desc = %s WHERE soft_id = %d;",$dbinst->quoteString($buffer), $i );
	echo $strSQL."\n";
}
fclose ($handle);

?>
