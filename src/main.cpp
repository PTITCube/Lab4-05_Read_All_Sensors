/**
 * VÍ DỤ 4-5: ĐỌC TỔNG HỢP TẤT CẢ CẢM BIẾN & ĐỊNH VỊ GPS
 * 
 * Mô tả:
 * Ví dụ này minh họa cách khởi tạo và lấy dữ liệu chi tiết từ toàn bộ hệ thống
 * cảm biến trên vệ tinh CubeSat:
 * 1. BMP280: Đo nhiệt độ và áp suất khí quyển (I2C).
 * 2. MPU6050/MPU6500: Gia tốc kế và con quay hồi chuyển 6 trục (I2C).
 * 3. QMC5883L/P: Cảm biến la bàn từ trường 3 trục (I2C).
 * 4. GPS (NEO-xM): Định vị tọa độ vệ tinh, độ cao, tốc độ và thời gian UTC (UART).
 */

#include <Arduino.h>
#include <PTITCube.h>

// Khai báo đối tượng cảm biến và GPS
PTIT_Sensor mySensor;
PTIT_GPS myGPS;

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    Serial.println("\n[EXAMPLE] Bắt đầu bài test Toàn bộ Cảm biến & Định vị GPS...");
    
    // Khởi tạo các cảm biến I2C (BMP280, MPU6050, QMC5883)
    mySensor.init();

    // Khởi tạo module GPS qua UART2
    myGPS.init();

    Serial.println("[EXAMPLE] Đã nạp xong cấu hình. Đang bắt đầu vòng lặp đọc dữ liệu...\n");
}

void loop() {
    // Luôn gọi update liên tục trong vòng lặp để cập nhật dữ liệu cảm biến và nhận gói tin NMEA từ GPS
    mySensor.update();
    myGPS.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
        lastPrint = millis();

        Serial.println("=================================================");
        Serial.println("       DỮ LIỆU CẢM BIẾN TOÀN DIỆN & TỌA ĐỘ GPS   ");
        Serial.println("=================================================");

        // 1. BMP280
        Serial.println(">>> 1. BMP280 (Nhiệt độ & Áp suất)");
        Serial.print("    - Nhiệt độ          : "); Serial.print(mySensor.getTemperature(), 2); Serial.println(" °C");
        Serial.print("    - Áp suất           : "); Serial.print(mySensor.getPressure(), 2); Serial.println(" hPa");

        // 2. MPU6050/MPU6500
        Serial.println(">>> 2. MPU6050/MPU6500 (Gia tốc & Con quay hồi chuyển)");
        Serial.print("    - Gia tốc (m/s^2)   : X="); Serial.print(mySensor.getAccX(), 3); 
        Serial.print("  Y="); Serial.print(mySensor.getAccY(), 3); 
        Serial.print("  Z="); Serial.println(mySensor.getAccZ(), 3);
        
        Serial.print("    - Tốc độ góc (rad/s): X="); Serial.print(mySensor.getGyroX(), 3); 
        Serial.print("  Y="); Serial.print(mySensor.getGyroY(), 3); 
        Serial.print("  Z="); Serial.println(mySensor.getGyroZ(), 3);

        // 3. QMC5883L/P
        Serial.println(">>> 3. QMC5883 (La bàn từ trường 3 trục)");
        Serial.print("    - Từ trường (Gauss) : X="); Serial.print(mySensor.getMagX(), 3); 
        Serial.print("  Y="); Serial.print(mySensor.getMagY(), 3); 
        Serial.print("  Z="); Serial.println(mySensor.getMagZ(), 3);

        // 4. GPS
        Serial.println(">>> 4. GPS (Định vị vệ tinh toàn cầu)");
        int sats = myGPS.getSatellites();
        if (sats > 0) {
            Serial.printf("    - Tọa độ (Lat, Lng) : %f, %f\n", myGPS.getLat(), myGPS.getLng());
            Serial.printf("    - Cao độ (Altitude) : %.2f m\n", myGPS.getAltitude());
            Serial.printf("    - Tốc độ (Speed)    : %.2f km/h\n", myGPS.getSpeed());
            Serial.printf("    - Hướng (Course)    : %.2f độ\n", myGPS.getCourse());
            Serial.printf("    - Số vệ tinh khóa   : %d\n", sats);
            Serial.printf("    - Sai số HDOP       : %.2f\n", myGPS.getHDOP());
            Serial.print ("    - Thời gian chuẩn   : "); Serial.println(myGPS.getTimeString());
        } else {
            Serial.println("    - Trạng thái        : Đang quét tìm vệ tinh... (Hãy đặt anten hướng ra ngoài trời)");
        }
        
        Serial.println("=================================================\n");
    }
}
