<?php
$cookie_name = "user";
$cookie_value = "John Doe";
$cookie_name2 = "age";
$cookie_value2 = 21;
setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
setcookie($cookie_name2, $cookie_value2, time() + (86400 * 30), "/"); // 86400 = 1 day
?>
<html>
<body>

<?php
if(!isset($_COOKIE[$cookie_name])) {
  echo "Cookie named '" . $cookie_name . "' is not set!";
} else {
  echo "Cookie '" . $cookie_name . "' is set!<br>";
  echo "Value is: " . $_COOKIE[$cookie_name] . "<br><br>";
}
if(!isset($_COOKIE[$cookie_name2])) {
	echo "Cookie named '" . $cookie_name2 . "' is not set!";
  } else {
	echo "Cookie '" . $cookie_name2 . "' is set!<br>";
	echo "Value is: " . $_COOKIE[$cookie_name2];
  }
?>

</body>
</html>