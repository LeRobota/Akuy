# Akuy – Air Quality Visualizer

![img](/readmePics/akuy.jpg "akuy")
This project serves a dual purpose: introducing users to IoT (Internet of Things) and raising awareness of an invisible but crucial data point in our environment – **air quality**. The kit can be built at home for around 10 euros.
Akuy connects to the nearest air quality monitoring station and retrieves the **AQI (Air Quality Index)** – an international scale used to evaluate air quality and identify the presence of various pollutants. It then displays the AQI value using an RGB LED that changes color from **green to purple**, corresponding to the AQI reference scale (green = clean air, purple = heavily polluted).
Unlike a mobile app, Akuy is **constantly visible**, highlighting an otherwise imperceptible environmental factor. This allows users to monitor air quality throughout the day — provided the nearest station supplies reliable data.
If you're already familiar with **Arduino**, you can follow the tutorial below to build your own.

### ⚠️ A video showing the full assembly (in french) is available here : 


---

## 🧰 Components
![img](/readmePics/01.jpg "01")
- [Lolin D1 Mini board](https://www.gotronic.fr/art-carte-wemos-d1-mini-v4-47521.htm)
- [Addressable RGB LED](https://www.gotronic.fr/art-mini-led-rgb-neopixel-ada4356-30782.htm)
- 3 jumper wires (preferably of different colors)
- [2.54mm female headers](https://www.gotronic.fr/art-connecteur-fh1x3-22729.htm)
- [2.54mm angled male headers](https://www.gotronic.fr/art-epuise-34687.htm)
- Micro USB or USB-C cable (depending on your Lolin model)
- 3D printer filament: *any color* for the base, *white/translucent* for the top

---

## 🛠️ Assembly Instructions
![img](/readmePics/akuy25.png "schema")
### Step 0
Print the 3D parts. Use **white, transparent or translucent filament** for the top part of the object to diffuse the LED light.

### Step 1
![img](/readmePics/02.jpg "02")
Strip and solder your 3 jumper wires to the **female header connector**. Make sure **GND is in the center**.

### Step 2
![img](/readmePics/03.jpg "03")
Solder the **angled male headers** to the **top side** of the Lolin board (USB port side).

- For versions V1, V2, V3: use pins `5V`, `GND`, `D4`.
- For version V4: use pins `VBUS`, `GND`, `2`.

### Step 3
![img](/readmePics/04.jpg "04")
![img](/readmePics/05.jpg "05")
Solder the pins on your LED. Be sure to use the **“Din”** side (not “Dout”).

### Step 4
![img](/readmePics/06.jpg "06")
![img](/readmePics/07.jpg "07")
Using a hot glue gun (or the LED’s adhesive, if present), glue the LED to the 3D-printed part **after** passing the **female header connector through the rectangular hole**.

### Step 5
![img](/readmePics/08.jpg "08")
Secure the Lolin board into its 3D-printed housing using a small amount of hot glue.  
⚠️ Don’t use too much glue if you plan to reuse the board later.

### Step 6
![img](/readmePics/09.jpg "09")
Connect the **male and female headers**, following the correct wire order as per the diagram.

### Step 7
![img](/readmePics/10.jpg "10")
Connect your board via USB. If the LED lights up, the connections are good.  
You're ready to move on to **programming**.

---

## 💻 Programming Akuy

### Step 8
![img](/readmePics/11.jpg "11")
![img](/readmePics/12.jpg "12")
This tutorial uses data from [https://waqi.info](https://waqi.info).  
Other providers such as OpenWeatherMap also offer similar services.

To access the air quality API, you’ll need a free token. Request it here:  
👉 [https://aqicn.org/data-platform/token/](https://aqicn.org/data-platform/token/)

### Step 9
![img](/readmePics/13.jpg "13")
You’ll also need the **GPS coordinates** of the location where you want to get AQI data.  
You can find them here: 👉 [https://www.gps-coordinates.net/](https://www.gps-coordinates.net/)

### Step 10
![img](/readmePics/14.jpg "14")
Open the `akuy25_blank` file in the **Arduino IDE**.  
(Download the IDE for free here: [https://www.arduino.cc/en/software/](https://www.arduino.cc/en/software/))

In the `infos.h` tab, replace the placeholders with:

- Your **WiFi SSID**
- Your **WiFi password**
- Your **AQI token** (from Step 8)
- **Latitude and Longitude** of your desired location

Optional: adjust the LED brightness (max: `255`).

⚠️ If you're sharing your code publicly, remember to **remove your WiFi credentials** beforehand!

### Step 11
![img](/readmePics/15.jpg "15")
![img](/readmePics/16.jpg "16")
If you’ve never programmed an **ESP8266** with Arduino:

1. Go to: `File > Preferences > Additional Boards Manager URLs`
2. Add this URL:  
   `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. Click the **Boards Manager icon** (top-left) and search for “esp”.
4. Install the **ESP8266 board package**.

### Step 12
![img](/readmePics/17.jpg "17")
In `Tools > Board`, select:

- `LOLIN(WEMOS) D1 R2 & Mini`

In `Tools > Port`, select the port your board is connected to.

Upload the program.

---

## ✅ Done!

- The LED will remain **blue** while attempting to connect to WiFi.
- If it **stays blue**, the connection failed.

⚠️ Note: **ESP8266 only supports 2.4GHz WiFi**, not 5GHz networks.
