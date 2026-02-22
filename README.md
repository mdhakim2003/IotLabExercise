## 🖥️ Node-RED Flow Integration

This project includes a Node-RED flow for real-time IoT monitoring, database logging, and dashboard visualization.

### 🔹 Flow Overview
- **MQTT Input**: Subscribes to sensor data from `channels/3201453/publish`.  
- **SQLite Database**: Stores sensor readings (`Occupancy`, `Temperature`, `Humidity`, `Pressure`, `Dust`, `Light`, `Distance`, `Fan_State`) in `ALL_SENS_DATA`.  
- **Thingspeak Integration**: Sends sensor data to Thingspeak via HTTP GET requests for cloud visualization.  
- **Dashboard Widgets**:  
  - **Table**: Displays historical sensor logs with timestamps.  
  - **Charts**: Line charts for each sensor vs timestamp.  
  - **Gauges**: Real-time gauges for temperature, humidity, pressure, dust, light, and distance.  
  - **Custom UI Template**: Occupancy status (👤 OCCUPIED / 🚫 EMPTY) with animations.  

### 🔹 Importing the Flow
1. Copy the `.json` file from the `NodeRED/` folder in this repo.  
2. Open Node-RED editor → Menu → **Import**.  
3. Paste the JSON code or upload the file.  
4. Deploy the flow to start monitoring.  

### 🔹 Dashboard Preview
The Node-RED dashboard provides:  
- Real-time gauges for environmental conditions  
- Line charts for historical trends  
- Occupancy status indicator  
- Fan state monitoring  

---

## 📊 Example Data Flow
1. **Sensor → MQTT → Node-RED**  
2. **Node-RED → SQLite Database** (local storage)  
3. **Node-RED → Thingspeak** (cloud visualization)  
4. **Node-RED → Dashboard** (real-time charts, gauges, tables)  

---

