<?php
// open the file in a binary mode
$name = '../video2.mp4';
$fp = fopen($name, 'rb');

// send the right headers
header("Content-Type: video/mp4");
header("Content-Length: " . filesize($name));

// dump the picture and stop the script
fpassthru($fp);
exit;
?>