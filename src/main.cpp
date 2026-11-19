/**
 * VÍ DỤ 4-5: ĐỌC TỔNG HỢP TẤT CẢ CẢM BIẾN & ĐỊNH VỊ GPS
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;

PTIT_Sensor mySensor;
PTIT_GPS myGPS;

void setup() {
    cdh.begin(115200);

    mySensor.init();
    myGPS.init(12, 14); // Chân MUX
}

void loop() {
    mySensor.update();
    myGPS.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
        lastPrint = millis();

        // 1. Dữ liệu môi trường & quán tính
        cdh.printf("[ENV] T: %.1f C | P: %.1f hPa | Alt: %.1f m\n",
                   mySensor.getTemperature(), mySensor.getPressure(), mySensor.getAltitude());
        cdh.printf("[IMU] Roll: %.1f° | Pitch: %.1f° | Heading: %.1f°\n",
                   mySensor.getRoll(), mySensor.getPitch(), mySensor.getHeading());

        // 2. Dữ liệu định vị GPS
        int sats = myGPS.getSatellites();
        if (sats > 0) {
            cdh.printf("[GPS] %d sats | Lat: %.6f, Lng: %.6f | Alt: %.1fm | Spd: %.1fkm/h\n",
                       sats, myGPS.getLat(), myGPS.getLng(),
                       myGPS.getAltitude(), myGPS.getSpeed());
        } else {
            cdh.println("[GPS] Searching satellites...");
        }
    }
}
