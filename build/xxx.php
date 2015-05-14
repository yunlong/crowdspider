<?php


/***
$fp = fsockopen("www.mydomain.com",80);
fputs($fp,"GET /downloads HTTP/1.0");
fputs($fp,"Host: www.mydomain.com");
fputs($fp,"Authorization: Basic " . base64_encode("user:pass") . "");
fpassthru($fp);
 */
echo "Authorization: Basic ".base64_encode("yunlong.lee@163.com:timberland1973")."\n";
echo strlen("yunlong.lee@163.com:timberland1973")."\n";
echo strlen(base64_encode("yunlong.lee@163.com:timberland1973"))."\n";
echo strlen("yunlong.lee:timberland1973")."\n";
echo strlen(base64_encode("yunlong.lee@:timberland1973"))."\n";
echo "Authorization: Basic ".base64_encode("yunlong.lee@163.com:xxxx9923")."\n";
?> 
