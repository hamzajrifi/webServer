<?php

// get the value of the "name" parameter
$name = $_GET["name"];

// create a new cookie
    setcookie("name", $name);

// set the Content-Type header to indicate that the response will be in HTML format
header("Content-Type: text/html");

// output the HTML code to create the web page
echo "<html><head><title>Greetings</title></head><body>";
if ($name) {
    echo "<h1>Hello, $name!</h1>";
} else {
    echo "<h1>Please enter your name.</h1>";
}
echo '<form method="GET" action="phpCookie.php">';
echo 'Name: <input type="text" name="name">';
echo '<input type="submit" value="Submit">';
echo '</form>';
echo "</body></html>";

?>
