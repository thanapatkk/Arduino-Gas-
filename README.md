# Gas Leak Detection System

โปรเจกต์ตรวจจับแก๊สรั่วด้วย ESP32  
แจ้งเตือนผ่าน LINE Massage Api แบบเรียลไทม์

## อุปกรณ์
- ESP32
- Gas Sensor
- LCD I2C
- Buzzer / LED

## การต่อวงจร (Wiring)

- Gas Sensor (AO) → GPIO 34  
- LED → GPIO 26 (ผ่าน R 220Ω)  
- Buzzer → GPIO 25  
- LCD I2C SDA → GPIO 21  
- LCD I2C SCL → GPIO 22  
- VCC → 5V  
- GND → GND

## ฟีเจอร์
- แสดงค่าแก๊สแบบเรียลไทม์
- แจ้งเตือนเมื่อเกิน Threshold
- ส่งข้อความพร้อมวันเวลาเข้า LINE
https://cdn.discordapp.com/attachments/644996991683657751/1456968985106055253/IMG_6742.jpg?ex=695a4b9b&is=6958fa1b&hm=22ebb44a2b0b3282b22aee2e0620d07305e6ff4e076151e8ddc689ffa0206689&
