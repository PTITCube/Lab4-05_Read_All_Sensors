/**
 * VÍ DỤ 4-06: ĐỌC TỔNG HỢP TẤT CẢ CẢM BIẾN & ĐỊNH VỊ GPS
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;
PTIT_EPS eps;

PTIT_Sensor mySensor;
PTIT_GPS myGPS;

void setup() {
    cdh.begin(115200);

    // Kích hoạt hạ áp cấp nguồn toàn bộ cảm biến, GPS qua thư viện EPS
    eps.setBuckState(true);

    mySensor.init();
    myGPS.init(12, 14); // Chân MUX
}

void loop() {
    mySensor.update();
    myGPS.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
        lastPrint = millis();

        cdh.printf("TELEM: T: %.1fC | P: %.1fhPa | Roll: %.1f | Pitch: %.1f | GPS: (%.4f, %.4f, %dsats)\n",
                   mySensor.getTemperature(), mySensor.getPressure(),
                   mySensor.getRoll(), mySensor.getPitch(),
                   myGPS.getLat(), myGPS.getLng(), myGPS.getSatellites());
    }
}
