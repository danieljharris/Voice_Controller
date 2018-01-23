/*
 Name:		WIFI_PC_Toggle.ino
 Created:	7/11/2017 4:47:39 PM
 Author:	Daniel's Gaming PC
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

const char* ssid = "VM2455088";
const char* password = "zt4FgkqzMfpn";

ESP8266WebServer server(90);

String webPage = "";
int gpio2_pin = 2;

void setup(void)
{
	webPage += "<h1>Daniel's ESP8266 PC Power Controller</h1>";
	webPage += "<p>Toggle PC <a href='toggle'> <button>Toggle</button> </a></p>";

	//Serial.begin(115200);
	//delay(10);

	// preparing GPIOs
	pinMode(gpio2_pin, OUTPUT);
	digitalWrite(gpio2_pin, LOW);

	// Connect to WiFi network
	WiFi.begin(ssid, password);

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		//Serial.print(".");
	}

	//Serial.print("\nConnected to ");
	//Serial.println(ssid);
	//Serial.print("IP address: ");
	//Serial.println(WiFi.localIP());

	//if (mdns.begin("esp8266", WiFi.localIP(), 90))
	//{
	//	//Serial.println("\nMDNS responder started");
	//}

	if (MDNS.begin("esp8266")) {
		Serial.println("MDNS responder started");
	}

	//Old & authenticated
	server.on("/", handleRoot);
	server.on("/login", handleLogin);
	server.on("/toggle", handleToggle);

	//New & google assistant
	server.on("/home/pc/on",        HTTP_PUT, handleGoogle);
	server.on("/home/pc/off",       HTTP_PUT, handleGoogle);
	server.on("/home/pc/on%20the",  HTTP_PUT, handleGoogle);
	server.on("/home/pc/off%20the", HTTP_PUT, handleGoogle);

	//here the list of headers to be recorded
	const char * headerkeys[] = { "User-Agent","Cookie" };
	size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
	//ask server to track these headers
	server.collectHeaders(headerkeys, headerkeyssize);

	server.begin();
	//Serial.println("\nHTTP server started");
}

void loop(void)
{
	server.handleClient();
}


//Check if header is present and correct
bool is_authentified()
{
	if (server.hasHeader("Cookie"))
	{
		String cookie = server.header("Cookie");

		if (cookie.indexOf("ESPSESSIONID=1") != -1)
		{
			return true;
		}
	}
	return false;
}




//root page can be accessed only if authentification is ok
void handleRoot()
{
	if (!is_authentified())
	{
		String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}

	String content;
	content += "<html><body><H2>Hello, welcome to Daniel's ESP PC Power Controler!</H2><br>";
	content += "Toggle PC <a href='toggle'> <button>Toggle</button></a><br><br>";
	content += "You can <a href=\"/login?DISCONNECT=YES\">disconnect here</a></body></html>";
	server.send(200, "text/html", content);
}



//login page, also called for disconnect
void handleLogin()
{
	String msg;
	if (server.hasHeader("Cookie"))
	{
		String cookie = server.header("Cookie");
	}

	if (server.hasArg("DISCONNECT"))
	{
		String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}

	if (server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
	{
		if (server.arg("USERNAME") == "Daniel" &&  server.arg("PASSWORD") == "lololokk3142")
		{
			String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
			server.sendContent(header);
			return;
		}

		msg = "Wrong username/password! try again.";
	}

	String content = "<html><body><form action='/login' method='POST'>Log in to Daniel's ESP Project:<br>";
	content += "User:       <input type='text' name='USERNAME' placeholder='user name'><br>";
	content += "Password:   <input type='password' name='PASSWORD' placeholder='password'><br>";
	content += "                 <input type='submit' name='SUBMIT' value='Submit'></form><br>";
	content += msg + "<br><br>";
	content += "You can <a href=\"/login?DISCONNECT=YES\">disconnect here</a></body></html>";
	server.send(200, "text/html", content);
}


//root page can be accessed only if authentification is ok
void handleToggle()
{
	if (is_authentified())
	{
		doToggle();
	}

	String header = "HTTP/1.1 301 OK\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
	server.sendContent(header);
}

void handleGoogle()
{
	doToggle();
	server.send(200, "text/plain", "PC toggled");
}

void doToggle()
{
	//Toggles GPIO 2!
	digitalWrite(gpio2_pin, HIGH);
	delay(1000);
	digitalWrite(gpio2_pin, LOW);
	delay(1000);
}