#!/bin/env python3

import cv2
import flask
from picamera2 import Picamera2

web_app = flask.Flask(__name__,
                      template_folder='.')


pi_cam = Picamera2()

pi_cam.preview_configuration.main.size = (640, 360)
pi_cam.preview_configuration.main.format = "RGB888" #opencv lavora in BGR a 8 bit per canale
pi_cam.preview_configuration.align() # prende il formato standard più simile ai valori che ho impostato

pi_cam.configure("preview") # applica le impostazioni

pi_cam.start()

def gen_frames():
  while True:
      frame = pi_cam.capture_array()
      ret, buffer = cv2.imencode('.jpg', frame)
      frame = buffer.tobytes()
      yield (b'--frame\r\n'
             b'Content-Type: image-jpeg\r\n\r\n' + frame + b'\r\n')
      
@web_app.route('/')
def index():
   return flask.render_template('index.html')

@web_app.route('/video_feed')
def video_feed():
   return flask.Response(gen_frames(),
                         mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    web_app.run(host='0.0.0.0', port=8080)

