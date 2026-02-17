from flask import Flask, render_template_string, Response, request
import serial
import time
import io
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
app = Flask(__name__)

picam2 = Picamera2()
config = picam2.create_preview_configuration(main={"format": "MJPEG", "size": (640, 480)})
picam2.configure(config)
picam2.start()

def send_to_arduino(cmd):
    if ser:
        ser.write(f"{cmd}\n".encode())
        print(f"Команда: {cmd}")

def gen_frames():
    while True:
        request = picam2.capture_array() 
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + request.tobytes() + b'\r\n')

HTML = """
<html>
    <head>
        <title>Robot Control</title>
        <style>
            body { text-align: center; font-family: sans-serif; background: #222; color: white; }
            .btn { width: 100px; height: 50px; margin: 10px; font-weight: bold; cursor: pointer; }
            .stop { background: red; color: white; }
            .move { background: #444; color: white; }
            .action { background: orange; color: black; }
        </style>
    </head>
    <body>
        <h1>Robot FPV Control</h1>
        <img src="/video_feed" style="width: 80%; max-width: 640px; border: 2px solid white;">
        <br><br>
        <button class="btn move" onclick="send('FORWARD')">W</button><br>
        <button class="btn move" onclick="send('LEFT')">A</button>
        <button class="btn stop" onclick="send('STOP')">STOP</button>
        <button class="btn move" onclick="send('RIGHT')">D</button><br>
        <button class="btn move" onclick="send('BACK')">S</button>
        <br><hr>
        <button class="btn action" onclick="send('PICK')">ЗАХВАТ</button>
        <button class="btn action" onclick="send('DROP')">СБРОС</button>
        
        <script>
            function send(cmd) { fetch('/command/' + cmd); }
            // Остановка при отпускании клавиш (опционально)
            document.onkeydown = (e) => {
                let keys = {87:'FORWARD', 65:'LEFT', 83:'BACK', 68:'RIGHT', 32:'STOP'};
                if(keys[e.keyCode]) send(keys[e.keyCode]);
            };
        </script>
    </body>
</html>
"""

@app.route('/')
def index():
    return render_template_string(HTML)

@app.route('/video_feed')
def video_feed():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/command/<cmd>')
def command(cmd):
    send_to_arduino(cmd)
    return "OK"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, threaded=True)