# ESP8266DashIR

Observe broadcasts sent by Amazon Dash Button and control an IR LED using ESP8266(ESP-WROOM-02).  
[https://miyakelp.hatenablog.jp/entry/2018/07112/000805]


## Requirements
* Arduino IDE
* [Arduino core for ESP8266](https://github.com/esp8266/Arduino)
    * Boad : Generic ESP8266
    * lwIP Variant : "v1.4 Higher Bandwidth"


## Config
* LED_OUT : I/O pin connected to IR LED
* IR_RECEIVER : I/O pin connected to IR RECEIVER
* WIFI_SSID : Wi-Fi SSID
* WIFI_PASSWORD : Wi-Fi Password
* irPatternLength : Length of "irPattern"
* irPattern : IR signal array
* dashButtonAddr : MAC address of Dash Button


## IR Pattern
LED ON (38kHz modulation) -> sleep (irPattern[0])[us] -> LED OFF -> sleep (irPattern[1])[us] -> ...


## Circuit
![Circuit](https://github.com/miyakelp/ESP8266DashIR/blob/master/circuit.png)

Note: GP1UXC41QS requires a RC filter, but this circuit has not.


## License
MIT License  
2019 (c) miyakelp
