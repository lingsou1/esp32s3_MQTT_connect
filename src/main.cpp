/*
接线说明:无

程序说明:一个简单的MQTT连接程序

注意事项:尽量不要在loop()函数中使用延时函数,可能会对MQTT服务有影响

函数示例:无

作者:灵首

时间:2023_4_10

*/
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiMulti.h>

WiFiMulti wifi_multi; // 建立WiFiMulti 的对象,对象名称是 wifi_multi
WiFiClient wifiClient;  //建立WiFiClient
PubSubClient mqttClient(wifiClient);  //根据WiFiClient来建立PubSubClient对象

const char* mqttServer = "test.ranye-iot.net";  //这是需要连接的MQTT服务器的网址,可更改

#define LED_A 10
#define LED_B 11


/*
# brief 连接WiFi的函数
# param 无
# retval 无
*/
void wifi_multi_con(void)
{
  int i = 0;
  while (wifi_multi.run() != WL_CONNECTED)
  {
    delay(1000);
    i++;
    Serial.print(i);
  }
}



/*
# brief 写入自己要连接的WiFi名称及密码,之后会自动连接信号最强的WiFi
# param 无
# retval  无
*/
void wifi_multi_init(void)
{
  wifi_multi.addAP("haoze2938", "12345678");
  wifi_multi.addAP("LINGSOU1029", "12345678");
  wifi_multi.addAP("haoze1029", "12345678"); // 通过 wifi_multi.addAP() 添加了多个WiFi的信息,当连接时会在这些WiFi中自动搜索最强信号的WiFi连接
}



/**
* @brief 生成客户端名称并连接服务器同时串口输出信息
*
* @param 无
* @return 无
*/
void connectMQTTServer(){
  //生成客户端的名称(同一个服务器下不能存在两个相同的客户端名称)
  String clientId = "esp32s3---" + WiFi.macAddress();

  //尝试连接服务器,并通过串口输出有关信息
  if(mqttClient.connect(clientId.c_str())){
    Serial.println("MQTT Server Connect successfully!!!.\n");
    Serial.println("Server Address: ");
    Serial.println(mqttServer);
    Serial.print("\n");
    Serial.println("ClientId:");
    Serial.println(clientId);
    Serial.print("\n");
  }
  else{
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    Serial.print("\n");
    delay(3000);
  }

}



void setup() {
  // 连接串口
  Serial.begin(9600);
  Serial.print("serial is OK\n");

  //led灯设置
  pinMode(LED_A,OUTPUT);
  pinMode(LED_B,OUTPUT);
  digitalWrite(LED_A,0);
  digitalWrite(LED_B,0);

  // wifi 连接设置
  wifi_multi_init();
  wifi_multi_con();
  Serial.print("wifi connected!!!\n");

  // 输出连接信息(连接的WIFI名称及开发板的IP地址)
  Serial.print("\nconnect wifi:");
  Serial.print(WiFi.SSID());
  Serial.print("\n");
  Serial.print("\nIP address:");
  Serial.print(WiFi.localIP());
  Serial.print("\n");

  //设置连接的MQTT服务器
  mqttClient.setServer(mqttServer,1883);

  //连接MQTT服务器
  connectMQTTServer();

}



void loop() {
  //检查MQTT连接,若连接则保持心跳,否尝试重连
  if(mqttClient.connected()){
    mqttClient.loop();  //这是在保持客户端心跳
  }
  else{
    connectMQTTServer();  //重连服务器
  }

  
}