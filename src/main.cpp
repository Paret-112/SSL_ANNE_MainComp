#include <Arduino.h>

#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;

char ssid[] = "Paret";        // your network SSID (name)
char pass[] = "Insurance";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[256]; //buffer to hold incoming packet
char  replyBuffer[] = "acknowledged";       // a string to send back
char testPacket[256] = "motorTest";
WiFiUDP Udp;


// CHANGE BEFORE USING
#define PIN_MOTDR_L1_SPD 4
#define PIN_MOTDR_L1_FOR 24
#define PIN_MOTDR_L1_BAC 26
#define PIN_MOTDR_L2_SPD 6
#define PIN_MOTDR_L2_FOR 30
#define PIN_MOTDR_L2_BAC 28

#define PIN_MOTDR_R1_SPD 3
#define PIN_MOTDR_R1_FOR 25
#define PIN_MOTDR_R1_BAC 27
#define PIN_MOTDR_R2_SPD 5
#define PIN_MOTDR_R2_FOR 31
#define PIN_MOTDR_R2_BAC 29

void wifiInitialization();
void motorTest();
void printWifiStatus();

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MOTDR_L1_SPD, OUTPUT);
  pinMode(PIN_MOTDR_L1_FOR, OUTPUT);
  pinMode(PIN_MOTDR_L1_BAC, OUTPUT);
  pinMode(PIN_MOTDR_L2_SPD, OUTPUT);
  pinMode(PIN_MOTDR_L2_FOR, OUTPUT);
  pinMode(PIN_MOTDR_L2_BAC, OUTPUT);

  pinMode(PIN_MOTDR_R1_SPD, OUTPUT);
  pinMode(PIN_MOTDR_R1_FOR, OUTPUT);
  pinMode(PIN_MOTDR_R1_BAC, OUTPUT);
  pinMode(PIN_MOTDR_R2_SPD, OUTPUT);
  pinMode(PIN_MOTDR_R2_FOR, OUTPUT);
  pinMode(PIN_MOTDR_R2_BAC, OUTPUT);

  wifiInitialization();
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);



    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyBuffer);
    Udp.endPacket();
  }

  if (packetBuffer[0] == testPacket[0]) {
    motorTest();
    Serial.println("Initialized Motor Test");
  }

  // Set motor speed factor
  analogWrite(PIN_MOTDR_L1_SPD, 128);
  analogWrite(PIN_MOTDR_L2_SPD, 128);
  analogWrite(PIN_MOTDR_R1_SPD, 128);
  analogWrite(PIN_MOTDR_R2_SPD, 128);
}

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

void motorTest() {
  digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
  digitalWrite(PIN_MOTDR_L2_FOR, HIGH);

  digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
  digitalWrite(PIN_MOTDR_R2_FOR, HIGH);

  delay(3000);

  digitalWrite(PIN_MOTDR_L1_FOR, LOW);
  digitalWrite(PIN_MOTDR_L2_FOR, LOW);

  digitalWrite(PIN_MOTDR_R1_FOR, LOW);
  digitalWrite(PIN_MOTDR_R2_FOR, LOW);

  digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
  digitalWrite(PIN_MOTDR_L2_BAC, HIGH);

  digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
  digitalWrite(PIN_MOTDR_R2_BAC, HIGH);

  delay(3000);

  digitalWrite(PIN_MOTDR_L1_BAC, LOW);
  digitalWrite(PIN_MOTDR_L2_BAC, LOW);

  digitalWrite(PIN_MOTDR_R1_BAC, LOW);
  digitalWrite(PIN_MOTDR_R2_BAC, LOW);
}

void wifiInitialization() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
  Udp.begin(localPort);
}



