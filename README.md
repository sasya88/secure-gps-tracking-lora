# 🚀 Secure GPS Tracking System using LoRa

A low-power, long-range, and secure GPS tracking system built using **ESP32**, **LoRa**, and **Node.js**, with real-time location updates visualized using the **Google Maps API**. Ideal for areas with limited or no cellular coverage.

---

## Features

- **AES-128 Encryption** of GPS data
- **LoRa** communication for long-range, low-power data transfer
- **Wi-Fi + HTTP POST** to forward data to a backend server
- **Replay attack prevention** using timestamp verification
- **Google Maps integration** for live location display
- Modular and extensible for future features (LoRaWAN, MQTT, etc.)

---

## ⚙️ Hardware Used

- ESP32 (x2)
- NEO-7M GPS Module
- LoRa SX1278 Module (433 MHz)
- Wi-Fi connectivity for backend updates

---

## Software Stack

### ESP32 (Arduino C++)
- Libraries: `TinyGPSPlus`, `AESLib`, `LoRa`, `base64`

### Node.js Server
- Libraries: `express`, `body-parser`, `cors`

### Frontend
- **Google Maps JavaScript API** for real-time tracking and location visualization

---

## 🚀 Future Enhancements

- [ ] Real-time updates using WebSockets  
- [ ] LoRaWAN support  
- [ ] Battery-powered outdoor deployment  
- [ ] MQTT protocol integration  
- [ ] Multi-device GPS tracking system  

---

## 📚 References

- [GPS Tracking System Using LoRaWAN](https://www.ijsr.net/archive/v9i12/SR201126115727.pdf)  
- [Location Tracking using LoRa](https://www.researchgate.net/publication/353623552_Location_tracking_using_LoRa)  
- [LoRa-Based GPS Tracker using Arduino and LoRa Shield](https://circuitdigest.com/microcontroller-projects/lora-based-gps-tracker-using-arduino-and-lora-shield)

---
