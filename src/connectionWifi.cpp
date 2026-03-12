//
// Created by HP Envy on 22/02/2026.
//

#include "connectionWifi.h"
#include "projectSettings.h"

#include <Arduino.h>
#include <WiFi.h>

#ifdef _MQTT_MODE
#include <MqttClient.h>
#endif

#include "actuationTasks.h"
#include "pinOut.h"

#ifdef _UDP_MODE
#include <WiFiUdp.h>
WiFiUDP Udp;
#endif

#ifdef _CLIENT_MODE
WiFiClient client;
IPAddress clientIP = IPAddress(192, 168, 143, 168);
#endif

#ifdef _MQTT_MODE
WiFiClient client;
MqttClient mqttClient(client);

const char broker[] = MQTT_SERVER;
int port = MQTT_PORT;
const char robotReadTopic[] = MQTT_ROBOTREAD;
const char robotWriteTopic[] = MQTT_ROBOTWRITE;
#endif

String numericPart = "";
char codeReceived;

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void wifiInitialization(const unsigned int localPort, int status, char ssid[], char pass[]) {
    //Initialize serial and wait for port to open:
    Serial.begin(SERIAL_BAUD);
    // check for the Wi-Fi module:
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }

    // attempt to connect to WiFi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("Connected to WiFi");
    printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:

#ifdef _MQTT_MODE
    if (!mqttClient.connect(broker, port)) {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
        digitalWrite(PIN_LED_RED, HIGH);
        while (1);
    }

    mqttClient.onMessage(mqttClientPoll);

    mqttClient.subscribe(robotReadTopic);
#endif

#ifdef _UDP_MODE
    Udp.begin(localPort);
#endif

#ifdef _CLIENT_MODE
    connectClient();
#endif
    Serial.println("\nWeee!");
    digitalWrite(PIN_LED_GREEN, HIGH);
}

#ifdef _CLIENT_MODE
void connectClient() {
    if (!client.connect(clientIP, CLIENT_PORT)) {
        Serial.println("Connection to host failed");
        delay(1000);
        return;
    }
    Serial.print("Connected");
}

void checkPackets() {
    if (!client.connected() > 0) {
        connectClient();
    }

    while (client.available() > 0) {
        String line = client.readStringUntil('\n');
        Serial.print("code received: ");
        Serial.println(line);
        numericPart = "";
        for (int i = 0; i < line.length(); i++) {
            int character = line[i];
            if (isDigit(character)) {
                numericPart += (char) character;
            } else if (character != '\n') {
                codeReceived = character;
            } else {
                break;
            }
        }
    }
    interpreter(codeReceived, numericPart);
}
#endif

#ifdef _UDP_MODE
int checkPackets(uint8_t packetBuffer[], unsigned long systemTime) {
    int packetSize = Udp.parsePacket();
    // Serial.print("Checked for packet at: ");
    // Serial.println(systemTime);
    if (!packetSize) return 0;

    Serial.print("Received packet of size ");
    Serial.print(packetSize);
    Serial.print(" at system time: ");
    Serial.println(systemTime);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());


    // read the packet into packetBufffer
    Udp.read(packetBuffer, packetSize);

    uint8_t msg_type = packetBuffer[0];

    if (msg_type == MSG_DISCOVER) {

        uint8_t reply[2];
        reply[0] = MSG_DISCOVER_REPLY;
        reply[1] = ROBOTID;

        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(reply, 2);
        Udp.endPacket();

        Serial.println("Discovery 'n tie reply sent");
        return 0;
    }

    if (msg_type == MSG_COMMAND) {

        if (packetSize < sizeof(CommandPacket)) return 394;

        CommandPacket packet{};
        memcpy(&packet, packetBuffer, sizeof(packet));

        if (packet.robot_id != ROBOTID) return 679;

        Serial.println("----New Command----");
        Serial.println(packet.angle1);
        Serial.println(packet.distance);
        Serial.println(packet.angle2);
        Serial.println(packet.collectorOnQ);
        Serial.println(packet.shootKickerAtEnd);
        Serial.println(packet.gameState);
        Serial.println("-------------------");

        uint8_t reply[2];
        reply[0] = 1;
        reply[1] = ROBOTID;

        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(reply, 2);
        Udp.endPacket();
        return 1;
    }
    return 0;
}
#endif

#ifdef _MQTT_MODE
void checkPackets() {
    mqttClient.poll();
}

void mqttClientPoll(int messageSize) {
    String line = mqttClient.messageTopic();
    Serial.print(messageSize);
    Serial.println(" bytes recieved!");

    numericPart = "";
    for (int i = 0; i < line.length(); i++) {
        int character = line[i];
        if (isDigit(character)) {
            numericPart += (char) character;
        } else if (character != '\n') {
            codeReceived = character;
        } else {
            break;
        }
    }
    interpreter(static_cast<char>(mqttClient.read()));
    // use the Stream interface to print the contents
    while (mqttClient.available()) {
        Serial.println(static_cast<char>(mqttClient.read()));
    }
}

void mqttClientPublish(char packet) {
    mqttClient.beginMessage(robotWriteTopic);
    mqttClient.print(packet);
    mqttClient.endMessage();
}
#endif

void interpreter(char instructionFactor) {
    switch (instructionFactor) {
        case 'F': driveForward(); break;
        case 'L': turnLeft(); break;
        case 'B': driveBackward(); break;
        case 'R': turnRight(); break;
        case 'S': allStop(); break;
        case 'I': pumpActuateInCB(); break;
        case 'O': pumpActuateOutCB(); break;
        case 'J': globalSpeedModSet(125); break;
        case 'T': mqttClientPublish('+'); break;
        default: allStop(); break;
    }
}