/*
Name:		VoiceController.ino
Created:	22/11/2017 8:36:22 PM
Author:	Daniel's Gaming PC
*/

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


//Update these numbers per each device ***************************
ESP8266WebServer connected_server(223);
const char* setup_ssid = "VoiceController223";
const char* setup_password = "Uq2iI0uw";
//Update these numbers per each device ***************************


ESP8266WebServer setup_server(80);

/* Don't set these wifi credentials. They are configurated at
   runtime in web interface and stored on EEPROM */
char GLOBAL_SSID[32] = "";
char GLOBAL_PASSWORD[32] = "";

bool startup = false;
int GPIO_PIN = 0;
bool TransistorState = false;

void setup(void) {
	pinMode(GPIO_PIN, OUTPUT);
	digitalWrite(GPIO_PIN, LOW);

	Serial.begin(19200);
	Serial.println("Entering setup");
	Serial.println(setup_ssid);
	Serial.print(" server ip: ");
	Serial.println(WiFi.softAPIP());
}

void loop(void) {
	if (startup) {
		setup_server.handleClient();
	}
	else {
		connected_server.handleClient();
	}
}


//Stage: Setup ---------------------------------------------------

void onSetup() {
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(setup_ssid, setup_password);

	setup_server.on("/", handleRoot);
	setup_server.on("/connect", handleConnect);
	setup_server.onNotFound(handleRoot);

	setup_server.begin();
}

void handleRoot() {
	Serial.println("Handling root");

	Serial.println("Getting WiFi info");
	loadCredentials();

	String SSID = GLOBAL_SSID;
	String PASSWORD = GLOBAL_PASSWORD;

	String content;
	content += "<html><body>";

	content += "<form action='/connect' method='POST'>Log in to Voice Controler:<br>";
	content += "SSID:       <input type='text' name='SSID' placeholder='ssid found on router' value='" + SSID + "'><br>";
	content += "Password:   <input type='password' name='PASSWORD' placeholder='password found on router' value='" + PASSWORD + "'><br>";
	content += "<input type='submit' value='Connect'></form><br><br>";

	content += "</body></html>";
	setup_server.send(200, "text/html", content);
}

void handleConnect() {
	Serial.println("Handling setup connection");
	String content;

	if (setup_server.hasArg("SSID") && setup_server.hasArg("PASSWORD")) {
		String str_ssid = setup_server.arg("SSID");
		String str_password = setup_server.arg("PASSWORD");

		content += "<html><body>";
		content += "Connecting to given SSID and PASSWORD<br>";
		content += "SSID: " + str_ssid + "<br>";
		content += "PASSWORD: 'Password Hidden'<br>";
		content += "</body></html>";
		setup_server.send(200, "text/html", content);

		delay(500);

		Serial.println("Stopping old server and saving WiFi info");
		handleWifiSave();

		delay(1000);

		if (!connectToWiFi(str_ssid, str_password)) {
			Serial.println("Issue connecting to router, restarting");
			delay(500);
			setup();
		}
		else {
			delay(500);
			onConnected();
		}
	}
	else {
		content += "<html><body>";
		content += "Unknown SSID and or Password<br>";
		content += "Please try again";
		content += "</body></html>";
		setup_server.send(200, "text/html", content);
	}
}

//Helper Functions

void loadCredentials() {
	EEPROM.begin(512);
	EEPROM.get(0, GLOBAL_SSID);
	EEPROM.get(0 + sizeof(GLOBAL_SSID), GLOBAL_PASSWORD);
	char ok[2 + 1];
	EEPROM.get(0 + sizeof(GLOBAL_SSID) + sizeof(GLOBAL_PASSWORD), ok);
	EEPROM.end();
	if (String(ok) != String("OK")) {
		GLOBAL_SSID[0] = 0;
		GLOBAL_PASSWORD[0] = 0;
	}
	Serial.println("Recovered credentials:");
	Serial.println(GLOBAL_SSID);
	Serial.println(strlen(GLOBAL_PASSWORD)>0 ? "********" : "<no password>");
}

void saveCredentials() {
	EEPROM.begin(512);
	EEPROM.put(0, GLOBAL_SSID);
	EEPROM.put(0 + sizeof(GLOBAL_SSID), GLOBAL_PASSWORD);
	char ok[2 + 1] = "OK";
	EEPROM.put(0 + sizeof(GLOBAL_SSID) + sizeof(GLOBAL_PASSWORD), ok);
	EEPROM.commit();
	EEPROM.end();
}

void handleWifiSave() {
	Serial.println("wifi save");
	setup_server.arg("SSID").toCharArray(GLOBAL_SSID, sizeof(GLOBAL_SSID) - 1);
	setup_server.arg("PASSWORD").toCharArray(GLOBAL_PASSWORD, sizeof(GLOBAL_PASSWORD) - 1);
	setup_server.sendHeader("Location", "wifi", true);
	setup_server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	setup_server.sendHeader("Pragma", "no-cache");
	setup_server.sendHeader("Expires", "-1");
	setup_server.send(302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
	setup_server.client().stop(); // Stop is needed because we sent no content length

	//Turns off access point
	WiFi.softAPdisconnect(true);
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);

	saveCredentials();
}

//Stage: Setup ---------------------------------------------------


bool connectToWiFi(String str_ssid, String str_password) {
	Serial.println("Connecting to");
	Serial.print("SSID: ");
	Serial.println(str_ssid);
	Serial.print("Password: ");
	Serial.println(str_password);

	const char* ssid = str_ssid.c_str();
	const char* password = str_password.c_str();
	WiFi.begin(ssid, password);

	// Wait for connection
	int i = 0;
	while (WiFi.status() != WL_CONNECTED && i < 15) {
		delay(500);
		Serial.print(".");
		i++;
	}

	if (i == 15) {
		Serial.println("Could not connect!");

		return false;
	}
	else {
		Serial.print("Connected to: ");
		Serial.print(WiFi.SSID());
		Serial.print(", IP address: ");
		Serial.println(WiFi.localIP());

		return true;
	}
}


//Stage: Connected -----------------------------------------------

void onConnected() {
	//On
	connected_server.on("/on",           HTTP_PUT, handleCommandOn);
	connected_server.on("/on%20the",     HTTP_PUT, handleCommandOn);

	//Off
	connected_server.on("/off",          HTTP_PUT, handleCommandOff);
	connected_server.on("/off%20the",    HTTP_PUT, handleCommandOff);

	//Toggle
	connected_server.on("/toggle",       HTTP_PUT, handleCommandToggle);
	connected_server.on("/toggle%20the", HTTP_PUT, handleCommandToggle);

	//connected_server.on("/on",           handleCommandOn);
	//connected_server.on("/on%20the",     handleCommandOn);
	//connected_server.on("/off",          handleCommandOff);
	//connected_server.on("/off%20the",    handleCommandOff);
	//connected_server.on("/toggle",       handleCommandOn);
	//connected_server.on("/toggle%20the", handleCommandOff);

	connected_server.onNotFound(handleUnknownCommand);

	connected_server.begin();
	startup = false;
}

void handleCommandOn() {
	TransistorState = true;
	digitalWrite(GPIO_PIN, HIGH);
	connected_server.send(200, "text/plain", "Turning on");
}

void handleCommandOff() {
	TransistorState = false;
	digitalWrite(GPIO_PIN, LOW);
	connected_server.send(200, "text/plain", "Turning off");
}

void handleCommandToggle() {
	TransistorState = !TransistorState;

	String content;
	content += "Toggleing ";
	if (TransistorState) {
		content += "on";
		digitalWrite(GPIO_PIN, HIGH);
	}
	else {
		content += "off";
		digitalWrite(GPIO_PIN, LOW);
	}

	connected_server.send(200, "text/plain", content);
}

void handleUnknownCommand() {
	connected_server.send(200, "text/plain", "Unknown command");
}

//Stage: Connected -----------------------------------------------