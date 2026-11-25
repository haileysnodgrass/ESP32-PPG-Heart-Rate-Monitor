# ESP32 PPG Heart Rate Monitor

Real-time photoplethysmography (PPG) heart rate monitor written in C++ for an ESP32 microcontroller. Continuously samples an analog pulse sensor into a circular buffer, detects heartbeat peaks, and computes beats per minute (BPM) using median inter-beat interval filtering.

## How It Works

1. **Signal Acquisition (Circular Buffer):** Continuously reads raw analog PPG values from GPIO pin 36 at ~100 Hz (`10 ms` intervals) and stores each reading alongside its `millis()` timestamp in a 300-sample circular buffer.
2. **Local Maxima Peak Detection:** Every 300 samples, scans the buffer to identify signal peaks—defined as local maxima within a `300 ms` minimum peak width window (filtering out high-frequency noise and dicrotic notches).
3. **Outlier Rejection via Median IBI:** Calculates the time difference (inter-beat interval) between consecutive peaks and takes the **median** interval rather than the mean to prevent motion artifacts or missed beats from skewing the BPM.
4. **Physiological Range Validation:** Converts the median interval to BPM (`60000.0 / median_interval_ms`). Outputs readings within a valid physiological range (`30–200 BPM`) over Serial; otherwise outputs `"Acquiring Signal"`.

## Hardware & Setup

* **Microcontroller:** ESP32 (uses ADC input pin `GPIO 36`)
* **Sensor:** Analog PPG / optical pulse sensor
* **Serial Output:** `115200` baud

### Running the Code
1. Connect the analog pulse sensor signal wire to `GPIO 36`, plus `3.3V` and `GND`.
2. Open `heart_rate_monitor.ino` in the Arduino IDE (or PlatformIO).
3. Select your ESP32 board and COM port, then flash the firmware.
4. Open the Serial Monitor at `115200` baud to view live BPM readings.

## Key Parameters

| Constant | Value | Purpose |
| :--- | :--- | :--- |
| `BUFFER_LENGTH` | `300` | Number of samples held in the rolling buffer |
| `BUFFER_MIN_PEAK_WIDTH` | `300 ms` | Minimum time window for local maximum detection |
| `HR_CALC_RATE` | `300` | Sample interval between BPM recalculations |

## Future Improvements
* Replace `delay(10)` with non-blocking `millis()` timing for deterministic sampling intervals.
* Add a digital bandpass filter to further reduce motion artifacts before peak detection.
* Stream live BPM and raw PPG waveforms over Bluetooth Low Energy (BLE).
