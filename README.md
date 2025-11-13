# myESPFeeder-OOP

This is an object-oriented rework of my previous **myESPFeeder** project.  
It’s built for **ESP32** using **PlatformIO** and **C++**, following a modular design to improve readability, scalability, and maintenance.

---

## 🧠 Summary

Functionally, this project behaves the same as my original **myESPFeeder** —  
it automates feeding via a servo motor, real-time clock (RTC), and optional **Blynk** integration for remote control.

The difference here is in **structure**:  
this version is rewritten using **OOP principles**, splitting logic into multiple classes (`FeedManager`, `BlynkManager`, `LCDManager`, etc.)  
to make future updates and debugging easier.

---

## ⚙️ Notes

- Performance **may improve slightly** due to better task separation and reduced redundancy.  
- Hardware-level speed should be roughly the same.  
- Expect some **minor errors or imperfect practices** — this is my first serious project written in **OOP style** for embedded systems.  
- My main goal with this version is **cleaner structure** and **maintainable code**.

---

## 🚀 Features

- Scheduled and manual feeding via servo motor  
- LCD display for status and timing  
- Real-Time Clock integration for accurate scheduling  
- WiFi + Blynk connectivity for remote control  
- Emergency stop and basic safety handling  

---

## 🧰 Tools & Frameworks

- **Board:** ESP32  
- **Framework:** PlatformIO (Arduino Core)  
- **Language:** C++  

---

## 🔗 Related Projects

- Original version: [myESPFeeder](https://github.com/yourshoji/myESPFeeder)  
- OOP version (this): [myESPFeeder-OOP](https://github.com/yourshoji/myESPFeeder-OOP)

---

## 💬 Final Note

This project is part of my learning journey in embedded software engineering.  
It demonstrates both my **understanding of hardware integration** and my ongoing **improvement in C++ design and code organization**.
