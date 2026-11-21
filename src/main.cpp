/**
 * VÍ DỤ 4-06: ĐỌC TỔNG HỢP TẤT CẢ CẢM BIẾN & ĐỊNH VỊ GPS
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;
PTIT_EPS eps;

PTIT_Sensor sensor;
PTIT_GPS gps;

void setup() {
    cdh.begin();

    // Kích hoạt hạ áp cấp nguồn toàn bộ cảm biến, GPS qua thư viện EPS
    eps.setBuckState(true);

    sensor.init();
    gps.init();
}

void loop() {
    sensor.update();
    gps.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
        lastPrint = millis();

        cdh.printf("TELEM: T: %.1fC | P: %.1fhPa | Roll: %.1f | Pitch: %.1f | GPS: (%.4f, %.4f, %dsats)\n",
                   sensor.getTemperature(), sensor.getPressure(),
                   sensor.getRoll(), sensor.getPitch(),
                   gps.getLat(), gps.getLng(), gps.getSatellites());
    }
}
