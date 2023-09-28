#include "wifiScanner.h"

int n = 0;
String ssid = "";
String pass = "";
static int no_ssid = 0;
void wifiScan(BluetoothSerial &SerialBT)
{
  
  n = WiFi.scanNetworks();
  Serial.println("Scan started");
  SerialBT.println("Scan started");
  
  delay(500);

  
  if(n == 0)
  {
      SerialBT.println("Not networks found");
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
        SerialBT.println(")");
        //SerialBT.println("\r\n")
      }
      SerialBT.println("Enter the no. of the network you want to connect");
      while (SerialBT.available() == 0){}
      no_ssid = SerialBT.read();
      wifiConnection(SerialBT);
  }


}

void wifiInit(BluetoothSerial &SerialBT, unsigned long &timeout, int &trial)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFi.SSID(no_ssid - 49), pass);

    SerialBT.println("Connecting to Wifi...");
    timeout = millis();
    while((WiFi.status() != WL_CONNECTED) && (millis() - timeout < 8000))
    {
        SerialBT.print(".");
        delay(1000);
    }
    
    SerialBT.println("");

    if(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_NO_SSID_AVAIL)
    {
        SerialBT.println("Password is not correct");
    }
    else if(WiFi.status() != WL_CONNECTED && WiFi.status() == WL_NO_SSID_AVAIL)
    {
        SerialBT.println("Wifi network is not avaliable");
    }
    else
    {
        SerialBT.print("");
        SerialBT.println("Connected successfully");
        SerialBT.print("IP Address : ");
        SerialBT.println(WiFi.localIP());
        trial = 0;
    }
}

void wifiConnection(BluetoothSerial &SerialBT)
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
