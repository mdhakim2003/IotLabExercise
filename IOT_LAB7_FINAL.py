import serial
import paho.mqtt.publish as publish
import time
import json

# Serial setup
ser = serial.Serial('COM2', 9600, timeout=1)
ser.flushInput()

# MQTT settings
mqtt_host = "broker.emqx.io"
mqtt_port = 1883
mqtt_topic = "channels/3201453/publish"

while True:
    data = ser.readline()
    if len(data) > 0:
        print("Raw data:", data)

        strdata = data.decode("utf-8").rstrip("\r\n")
        parts = strdata.split(",")

        if len(parts) != 9:
            print("Invalid data length:", parts)
            continue

        # Replace Arduino timestamp with real system time
        timestamp = int(time.time())

        # ✅ FIX: use parts, not fields — and strip CRLF
        fan_state = int(parts[8].strip())

        # Build JSON payload with correct field names
        payload = {
            "Log_Timestamp": timestamp,
            "Occupancy": int(parts[1]),
            "Temperature_Data": float(parts[2]),
            "Humidity_Data": float(parts[3]),
            "Pressure_Data": float(parts[4]),
            "Dust_Data": float(parts[5]),
            "Light_Data": float(parts[6]),
            "Distance_Data": float(parts[7]),
            "Fan_State": fan_state   # ✅ use cleaned value
        }

        try:
            print("Publishing:", payload)
            publish.single(
                mqtt_topic,
                json.dumps(payload),
                hostname=mqtt_host,
                port=mqtt_port,
                transport="tcp"
            )

        except KeyboardInterrupt:
            print("Stopped by user.")
            break

        except Exception as e:
            print("Error while publishing:", e)

