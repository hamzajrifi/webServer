<?php
echo getcwd();
$url = parse_url('www.domain.com?test=1&test2=2&test3=3');
parse_str($url['query'], $keyvalue);

//   $NAME = $_GET['name']; // Gets ?name=value
// var_dump($keyvalue);
// echo "\n";
// echo ${NAME};
// echo $keyvalue[test2];
//  phpinfo();
?>