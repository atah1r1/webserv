<?php
// open the file in a binary mode
$name = './image.jpeg';
$fp = fopen($name, 'rb');

// send the right headers
header("Content-Type: image/jpeg");
header("Content-Length: " . filesize($name));

// dump the picture and stop the script
fpassthru($fp);
exit;
?>