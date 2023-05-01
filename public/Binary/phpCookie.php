<?php
// $name = "none";
$name = NULL;
$session = NULL;
// get the value of the "name" parameter
if (isset($_GET["name"]))
    $name = $_GET["name"];
else if (isset($_COOKIE["name"]))
    $name = $_COOKIE["name"];

if (isset($_GET["session"]))
    $session = $_GET["session"];
else 
if (isset($_COOKIE["session"]))
    $session = $_COOKIE["session"];
// create a new cookie
if ($name)
    setcookie("name", $name);
if ($session)
    setcookie("session", $session);

// set the Content-Type header to indicate that the response will be in HTML format
// header("Content-Type: text/html");

// output the HTML code to create the web page
echo "<html><head><title>Greetings</title></head><body>";
if ($name || $session) {
    if ($name) 
        echo "<h1>Name, $name!</h1>";
    if ($session) 
        echo "<h1>Session, $session!</h1>";
        echo "</body></html>";
}
else {
    echo "<h1>Please enter your name.</h1>";
    echo '<form method="GET" action="phpCookie.php">';
    echo 'Name: <input type="text" name="name">';
    echo 'Session: <input type="text" name="session">';
    echo '<input type="submit" value="Submit">';
    echo '</form>';
    echo "</body></html>";
}

?>
