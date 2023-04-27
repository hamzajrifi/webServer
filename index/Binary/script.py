import cgi
import cgitb
import http.cookies

# enable error reporting
cgitb.enable()

# get the form data submitted by the user
form = cgi.FieldStorage()

# get the value of the "name" parameter
name = form.getvalue("name")

# create a new cookie
cookie = http.cookies.SimpleCookie()
cookie["name"] = name

# set the Content-Type header to indicate that the response will be in HTML format
print("Content-Type: text/html")
print()

# output the HTML code to create the web page
print("<html><head><title>Greetings</title></head><body>")
if name:
    print("<h1>Hello, {}!</h1>".format(name))
else:
    print("<h1>Please enter your name.</h1>")
print('<form method="GET" action="script.py">')
print('Name: <input type="text" name="name">')
print('<input type="submit" value="Submit">')
print('</form>')
print("</body></html>")

# set the cookie in the response
print(cookie.output())
