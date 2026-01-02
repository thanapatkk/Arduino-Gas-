# Gas Leak Detection System

โปรเจกต์ตรวจจับแก๊สรั่วด้วย ESP32  
แจ้งเตือนผ่าน LINE Notify แบบเรียลไทม์

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
