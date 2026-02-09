import cv2
import numpy as np
import serial
import time
from picamera2 import Picamera2

def init_ser():
    for p in ['/dev/ttyACM0', '/dev/ttyUSB0', '/dev/ttyS0']:
        try:
            s = serial.Serial(p, 9600, timeout=0.1)
            time.sleep(2)
            return s
        except: continue
    return None

ser = init_ser()
colors = {
    "Red": [(np.array([0, 120, 70]), np.array([10, 255, 255]))],
    "Blue": [(np.array([100, 150, 0]), np.array([140, 255, 255]))],
    "Green": [(np.array([40, 70, 70]), np.array([80, 255, 255]))],
    "White": [(np.array([0, 0, 180]), np.array([180, 50, 255]))]
}

# Состояние робота
holding = False
target_hatch = None
last_cmd = ""

def send(cmd):
    global last_cmd
    if cmd != last_cmd:
        if ser: ser.write(f"{cmd}\n".encode())
        last_cmd = cmd

picam2 = Picamera2()
config = picam2.create_preview_configuration(main={"format": "RGB888", "size": (320, 240)})
picam2.configure(config)
picam2.start()
picam2.set_controls({"AfMode": 2})

try:
    while True:
        frame = picam2.capture_array()
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        found = False

        if not holding:
            for name, ranges in colors.items():
                mask = cv2.inRange(hsv, ranges[0][0], ranges[0][1])
                cnts, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                for c in cnts:
                    if cv2.contourArea(c) > 300:
                        x, y, w, h = cv2.boundingRect(c)
                        ratio = h / float(w)
                        if 2.2 < ratio < 4.5:
                            found = True
                            cx = x + w // 2
                            if w > 65:
                                send("PICK")
                                holding = True
                                target_hatch = "Red" if name == "White" else name
                                time.sleep(3)
                            elif abs(cx - 160) < 25: send("FORWARD")
                            elif cx > 160: send("RIGHT")
                            else: send("LEFT")
                            break
                if found: break
        else:
            mask = cv2.inRange(hsv, colors[target_hatch][0][0], colors[target_hatch][0][1])
            cnts, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            for c in cnts:
                if cv2.contourArea(c) > 1000:
                    x, y, w, h = cv2.boundingRect(c)
                    if h/float(w) < 1.3:
                        found = True
                        cx = x + w // 2
                        if (y + h) > 210:
                            send("DROP")
                            holding = False
                            time.sleep(3)
                        elif abs(cx - 160) < 30: send("FORWARD")
                        elif cx > 160: send("RIGHT")
                        else: send("LEFT")
                        break

        if not found: send("SCAN")
        cv2.line(frame, (160, 0), (160, 240), (255, 255, 255), 1)
        cv2.line(frame, (0, 120), (320, 120), (255, 255, 255), 1)
        status = f"Target: {target_hatch}" if holding else "Searching..."
        cv2.putText(frame, status, (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)
        
        cv2.imshow("Robot Vision", frame)
        if cv2.waitKey(1) == ord('q'): break
finally:
    send("STOP")
    picam2.stop()
    cv2.destroyAllWindows()