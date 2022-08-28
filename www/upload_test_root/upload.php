<?php
file_put_contents('upload_store/cgi_video.mp4', file_get_contents('php://input'));
header("Status: 201 Created");
?>