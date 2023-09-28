#include "wifiScanner.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define FORMAT_LITTLEFS_IF_FAILED true
DynamicJsonDocument Config(2048);
JsonObject obj = Config.as<JsonObject>();

uint8_t isConnected = 0;

unsigned long timestart = 0;
unsigned long timeout = 0;
int trial = 0;
<<<<<<< Updated upstream
static int no_ssid = 0;
int u = 0;
String credentials_array[2];
||||||| Stash base
static int no_ssid = 0;
=======

>>>>>>> Stashed changes

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;

<<<<<<< Updated upstream
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
    u = 1;
||||||| Stash base
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
=======
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    isConnected = 1;
    Serial.println(isConnected);
    SerialBT.println("Hi");
>>>>>>> Stashed changes
  }
<<<<<<< Updated upstream

  if (event == ESP_SPP_CLOSE_EVT) {
||||||| Stash base
 
  if(event == ESP_SPP_CLOSE_EVT ){
=======
 
  if(event == ESP_SPP_CLOSE_EVT ){
    isConnected = 0;
>>>>>>> Stashed changes
    Serial.println("Client disconnected");
  }
}

<<<<<<< Updated upstream
void createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
||||||| Stash base
void setup() 
{
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin("Yalla"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  wifiScan();
  wifiInit(timeout, trial);
=======
void setup() 
{
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin();
  Serial.println("The device started, now you can pair it with bluetooth!");

  while(!isConnected);
  SerialBT.println(isConnected);
  wifiScan(SerialBT);
  wifiInit(SerialBT, timeout, trial);
>>>>>>> Stashed changes
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);
  String credentials = "";

<<<<<<< Updated upstream
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    credentials += file.readString();
  }
  deserializeJson(Config, credentials);
  file.close();
  credentials_array[0] = Config["ssid"];
  credentials_array[1] = Config["pass"];
  return;
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Yalla");  //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  readFile(LittleFS, "/mydir/wifi.json");
  if (credentials_array[0] == "") {
    while (u == 0) { SerialBT.register_callback(callback); }
    wifiScan();
    wifiInit(timeout, trial);
||||||| Stash base
  if(trial > 3)
  {
    wifiScan();
    wifiInit(timeout, trial);
=======
  if(trial > 3)
  {
    wifiScan(SerialBT);
    wifiInit(SerialBT, timeout, trial);
>>>>>>> Stashed changes
  }
  else {
    readFile(LittleFS, "/mydir/wifi.json");
    ssid = credentials_array[0];
    pass = credentials_array[1];
    wifiInit(timeout, trial);
  }
}

void loop() {

  if (trial > 3) {
    wifiScan();
    wifiInit(timeout, trial);
  } else {
    if ((millis() - timestart > 2000) && (WiFi.status() != WL_CONNECTED)) {
      SerialBT.println("Reconnecting to Wifi...");
      WiFi.disconnect();
      wifiInit(SerialBT, timeout, trial);
      trial++;
      WiFi.reconnect();
      timestart = millis();
    }
  }
}

<<<<<<< Updated upstream
void wifiScan() {

  n = WiFi.scanNetworks();
  SerialBT.println("Scan started");

  delay(500);


  if (n == 0) {
    SerialBT.println("Not networks found");
  } else {
    for (int i = 0; i < n; i++) {
      SerialBT.print(i + 1);
      SerialBT.print(": ");
      SerialBT.print(WiFi.SSID(i));
      SerialBT.print(" (");
      SerialBT.print(WiFi.RSSI(i));
      SerialBT.print(")");
      SerialBT.println("");
    }
    SerialBT.print("Enter the no. of the network you want to connect");
    while (SerialBT.available() == 0) {}
    no_ssid = SerialBT.parseInt();
    wifiConnection();
  }
}

void wifiInit(unsigned long &timeout, int &trial) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi.SSID(no_ssid - 1), pass);

  SerialBT.print("Connecting to Wifi...");
  timeout = millis();
  while ((WiFi.status() != WL_CONNECTED) && (millis() - timeout < 8000)) {
    SerialBT.print(".");
    delay(1000);
  }

  SerialBT.println("");

  if (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_NO_SSID_AVAIL) {
    SerialBT.println("Password is not correct");
    wifiConnection();
  } else if (WiFi.status() != WL_CONNECTED && WiFi.status() == WL_NO_SSID_AVAIL) {
    SerialBT.println("Wifi network is not avaliable");
  } else {
    SerialBT.print("");
    SerialBT.print("Connected successfully");
    SerialBT.print("IP Address : ");
    SerialBT.print(WiFi.localIP());
    createDir(LittleFS, "/mydir");  // Create a mydir folder
    Config["ssid"] = WiFi.SSID(no_ssid - 1);
    Config["pass"] = pass;
    String output = "";
    serializeJson(Config, output);
    writeFile(LittleFS, "/mydir/wifi.txt", output);
    trial = 0;
  }
}

void wifiConnection() {
  ssid = WiFi.SSID(no_ssid - 1);
  pass.trim();

  if ((WiFi.encryptionType(no_ssid - 1)) != WIFI_AUTH_OPEN) {
    SerialBT.println("Please enter the password of the network you chose");
    while (!SerialBT.available()) {}
    SerialBT.setTimeout(5000);
    pass = SerialBT.readString();
    pass.trim();
  } 
  else 
  {
    pass = "";
  }
}
||||||| Stash base
void wifiScan()
{
  
  n = WiFi.scanNetworks();
  SerialBT.print("Scan started");
  
  delay(500);

  
  if(n == 0)
  {
      SerialBT.print("Not networks found");
  }
  else
  {
      for(int i = 0; i < n; i++)
      {
        SerialBT.print(i + 1);
        SerialBT.print(": ");
        SerialBT.print(WiFi.SSID(i));
        SerialBT.print(" (");
        SerialBT.print(WiFi.RSSI(i));
        SerialBT.print(")");
      }
      SerialBT.print("Enter the no. of the network you want to connect");
      while (SerialBT.available() == 0){}
      no_ssid = SerialBT.read();
      wifiConnection();
  }


}

void wifiInit(unsigned long &timeout, int &trial)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFi.SSID(no_ssid - 49), pass);

    SerialBT.print("Connecting to Wifi...");
    timeout = millis();
    while((WiFi.status() != WL_CONNECTED) && (millis() - timeout < 8000))
    {
        SerialBT.print(".");
        delay(1000);
    }
    
    SerialBT.println("");

    if(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_NO_SSID_AVAIL)
    {
        SerialBT.print("Password is not correct");
    }
    else if(WiFi.status() != WL_CONNECTED && WiFi.status() == WL_NO_SSID_AVAIL)
    {
        SerialBT.print("Wifi network is not avaliable");
    }
    else
    {
        SerialBT.print("");
        SerialBT.print("Connected successfully");
        SerialBT.print("IP Address : ");
        SerialBT.print(WiFi.localIP());
        trial = 0;
    }
}

void wifiConnection()
{
    ssid = WiFi.SSID(no_ssid-49);
    pass.trim();
  
    if((WiFi.encryptionType(no_ssid-49)) != WIFI_AUTH_OPEN)
    {
        SerialBT.println("Please enter the password of the network you chose");
        while(!SerialBT.available()){}
        SerialBT.setTimeout(5000);
        pass = SerialBT.readString();
        pass.trim();
        for(int i = 0; i < pass.length(); i++) SerialBT.print("*");
        SerialBT.println("");
    }
    else
    {
        pass = "";
    }
}
=======
>>>>>>> Stashed changes
