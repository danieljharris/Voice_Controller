/*
 Name:		WiFiExtender.ino
 Created:	1/14/2017 9:20:59 PM
 Author:	Daniel's Gaming PC
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//const char* ssid = "VM2455088";
//const char* password = "zt4FgkqzMfpn";

const char* newssid = "EspTest1";
const char* newpassword = "qaz12345";

ESP8266WebServer server(80);

String str_ssid = "default";
String str_password = "pass";

void setup(void) {
	Serial.begin(19200);
	Serial.println("");

	// set both access point and station
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(newssid, newpassword);

	Serial.print(newssid);
	Serial.print(" server ip: ");
	Serial.println(WiFi.softAPIP());

	server.on("/", handleRoot);
	server.on("/connect", handleConnect);
	server.onNotFound(handleNotFound);

	server.begin();
	Serial.println("HTTP server started");
}

void loop(void) {
	server.handleClient();
}

void handleConnect() {
	Serial.println("Handling connect:");
	String content;

	if (server.hasArg("SSID") && server.hasArg("PASSWORD")) {
		str_ssid = server.arg("SSID");
		str_password = server.arg("PASSWORD");


		content += "<html><body>";
		content += "Connecting to given SSID and PASSWORD<br>";
		content += "SSID: " + str_ssid + "<br>";
		content += "PASSWORD: " + str_password + "<br>";
		content += "</body></html>";
		server.send(200, "text/html", content);

		delay(500);

		WiFi.softAPdisconnect(true);
		WiFi.disconnect(true);
		WiFi.mode(WIFI_OFF);

		delay(500);

		const char* ssid = str_ssid.c_str();
		const char* password = str_password.c_str();
		connectToWiFi(ssid, password);
	}
	else {
		content += "<html><body>";
		content += "Unknown SSID and or Password<br>";
		content += "Please try again";
		content += "</body></html>";
		server.send(200, "text/html", content);
	}
}

void handleRoot() {
	Serial.println("Handling root:");

	String content;
	content += "<html><body><form action='/connect' method='POST'>Log in to Voice Controler:<br>";
	content += "SSID:       <input type='text' name='SSID' placeholder='ssid found on router'><br>";
	content += "Password:   <input type='password' name='PASSWORD' placeholder='password found on router'><br>";
	content += "<input type='submit' name='SUBMIT' value='Connect'></form><br>";
	content += "</body></html>";
	server.send(200, "text/html", content);
}

void handleNotFound() {
	Serial.println("Unknown page:");
	handleRoot();
}

void connectToWiFi(const char *ssid, const char *password) {
	Serial.println("Connecting to: ");
	Serial.println("SSID: ");
	Serial.print(ssid);
	Serial.println("Password: ");
	Serial.print(password);

	WiFi.begin(ssid, password);

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.print("Connected to: ");
	Serial.print(WiFi.SSID());
	Serial.print(", IP address: ");
	Serial.println(WiFi.localIP());
}