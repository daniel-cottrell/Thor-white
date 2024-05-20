import tkinter as tk
from tkinter import ttk
import json
import threading
import paho.mqtt.client as mqtt

# MQTT topic settings
subscribeTopic = "un46986700/computer"
publishTopic = "un46986700/toggle"
hostname = "csse4011-iot.zones.eait.uq.edu.au"

# Function to update the sensor data in the Tkinter GUI
def update_sensor_data(data):
    print("Received data:", data)
    device_id = data.get("ID")
    sensor_type = data.get("type")
    category = data.get("category")
    
    if device_id and sensor_type is not None and category is not None:
        if sensor_type == 0 and category == 0:  # Time delay sensor
            offset = data.get("offset")
            if offset is not None:
                delay_labels[device_id - 1].config(text=f"{offset} ms")
        elif sensor_type == 0 and category == 4:  # Gyroscope sensor
            gyro_data = data.get("gyro")
            if gyro_data is not None:
                if len(gyro_data) == 3:
                    gyro_x, gyro_y, gyro_z = gyro_data
                    # Format each value to 3 decimal places
                    gyro_x_f = "{:.3f}".format(gyro_x)
                    gyro_y_f = "{:.3f}".format(gyro_y)
                    gyro_z_f = "{:.3f}".format(gyro_z)
                    gyro_labels[device_id - 1].config(text=f"(X,Y,Z): {gyro_x_f}, {gyro_y_f}, {gyro_z_f}")
                else:
                    print(f"Invalid gyroscope data format for device {device_id}")
        elif sensor_type == 0 and category == 3:  # Accelerometer sensor
            accel_data = data.get("accel")
            if accel_data is not None:
                if len(accel_data) == 3:
                    accel_x, accel_y, accel_z = accel_data
                    # Format each value to 3 decimal places
                    accel_x_f = "{:.3f}".format(accel_x)
                    accel_y_f = "{:.3f}".format(accel_y)
                    accel_z_f = "{:.3f}".format(accel_z)
                    accel_labels[device_id - 1].config(text=f"(X,Y,Z): {accel_x_f}, {accel_y_f}, {accel_z_f}")
                else:
                    print(f"Invalid accelerometer data format for device {device_id}")
        else:
            print(f"Unknown sensor type for device {device_id}")

# Function to handle incoming MQTT messages
def on_message(client, userdata, message):
    try:
        payload = message.payload.decode("utf-8")
        print("Received message:", payload)  # Print the received JSON packet
        # Clean up the keys
        data = {key.strip(): value for key, value in json.loads(payload).items()}
        update_sensor_data(data)
    except json.JSONDecodeError as e:
        print("Error parsing JSON:", e)

# Function to print a confirmation message when connected to the MQTT broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker successfully")
    else:
        print(f"Failed to connect to MQTT broker, return code {rc}")

# Function to start the MQTT subscription
def start_mqtt_subscription():
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(hostname, 1883, 60)
    client.subscribe(subscribeTopic)
    client.loop_forever()

# Function to publish a message to the MQTT broker
def publish_message(client, message):
    client.publish(publishTopic, message)

# Callback function for the button
def on_button_click():
    user_input = text_box.get()
    if user_input.isdigit():
        device_id = int(user_input)
        if 1 <= device_id <= 9:
            message = json.dumps({"toggle": device_id})
            publish_message(client, message)
            print(f"Published message: {message}")
        else:
            print("Please enter a number between 1 and 9.")
    else:
        print("Please enter a valid number.")

# GUI setup function
def setup_gui():
    # Create the main window
    root = tk.Tk()
    root.title("Sensor Data Dashboard")
    root.geometry("710x710")

    # Create a frame to hold the content
    container = ttk.Frame(root, padding="20")
    container.pack(fill=tk.BOTH, expand=True)

    # Create cell frames
    cells = []
    global delay_labels
    global gyro_labels
    global accel_labels
    global text_box
    delay_labels = []
    gyro_labels = []
    accel_labels = []
    for i in range(9):
        cell = ttk.Frame(container, borderwidth=1, relief=tk.RAISED, width=200, height=200)
        cell.grid(row=i // 3, column=i % 3, padx=10, pady=10, sticky="nsew")
        cells.append(cell)

        device_id = i + 1
        cell_label = ttk.Label(cell, text=f"Device {device_id}".center(40), font=("Helvetica", 12, "bold"))
        cell_label.pack(pady=(5, 2))

        delay_label = ttk.Label(cell, text="Time Delay: ")
        delay_label.pack(pady=(0, 2))
        delay_value_label = ttk.Label(cell, text="")
        delay_value_label.pack(pady=(0, 2))
        delay_labels.append(delay_value_label)

        gyro_label = ttk.Label(cell, text="Gyroscope: ")
        gyro_label.pack(pady=(0, 2))
        gyro_value_label = ttk.Label(cell, text="")
        gyro_value_label.pack(pady=(0, 2))
        gyro_labels.append(gyro_value_label)

        accel_label = ttk.Label(cell, text="Accelerometer: ")
        accel_label.pack(pady=(0, 2))
        accel_value_label = ttk.Label(cell, text="")
        accel_value_label.pack(pady=(0, 2))
        accel_labels.append(accel_value_label)

    # Add a description label above the text box
    description_label = ttk.Label(root, text="Enter the Device ID to toggle gyroscope and accelerometer data:", font=("Helvetica", 10))
    description_label.pack(pady=(20, 5))

    # Add a text box and a button to the GUI
    text_box = ttk.Entry(root, width=40)
    text_box.pack(pady=(5, 5))

    button = ttk.Button(root, text="Submit", command=on_button_click)
    button.pack(pady=(5, 10))

    # Run the GUI
    root.mainloop()

# Main function
def main():
    global client
    client = mqtt.Client()
    # Start the MQTT subscription thread
    subscription_thread = threading.Thread(target=start_mqtt_subscription)
    subscription_thread.daemon = True
    subscription_thread.start()

    # Setup the GUI
    setup_gui()

if __name__ == "__main__":
    main()
