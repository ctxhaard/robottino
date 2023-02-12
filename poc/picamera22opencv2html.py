#!/bin/env python3

import cv2
import flask
import queue
import threading
import numpy as np
from picamera2 import Picamera2, Preview
from libcamera import Transform

web_app = flask.Flask(__name__,
                      template_folder='.')


pi_cam = Picamera2()

q_preview = queue.Queue(1)
q_raw = queue.Queue(1)
q_gray = queue.Queue(1)
q_mask = queue.Queue(1)


def camera_fetch():
   while True:
      # print('camera fetch')
      frame = pi_cam.capture_array()
      # print('frame fetched')
      q_raw.put(frame)
      (img, gray, mask) = process(frame)
      q_preview.put(img)
      q_gray.put(gray)
      q_mask.put(mask)
      # print('fetch loaded on queues')
   
fetch_thread = threading.Thread(target=camera_fetch, daemon=True)

config = pi_cam.create_preview_configuration(main={
   'size': (640, 360),
   'format': "RGB888" #opencv lavora in BGR a 8 bit per canale
}, transform=Transform(vflip=1, hflip=1))
pi_cam.configure(config)
pi_cam.preview_configuration.align()

#config.align() # prende il formato standard più simile ai valori che ho impostato
#pi_cam.preview_configuration.transform = Transform(vflip=1, hflip=1)
#pi_cam.configure("preview") # applica le impostazioni

pi_cam.start()

def process(img):
   # Conversione dell'immagine in scala di grigio
   gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

   # Selezione dei colori gialli
   hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
   lower_yellow = np.array([20, 100, 100])
   upper_yellow = np.array([30, 255, 255])
   mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

   # Rimozione del rumore
   kernel = np.ones((5, 5), np.uint8)
   mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
   mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

   # Trovare i contorni
   contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

   # Disegnare un cerchio intorno alla palla gialla
   for contour in contours:
      (x, y, w, h) = cv2.boundingRect(contour)
      if w > 20 and h > 20:
         cv2.circle(img, (int(x + w / 2), int(y + h / 2)), int(w / 2), (0, 0, 255), 2)
   return (img, gray, mask)


def gen_frames(queue):
  while True:
      # print(f'getting frame from queue {queue}')
      frame = queue.get()
      ret, buffer = cv2.imencode('.jpg', frame)
      frame = buffer.tobytes()
      queue.task_done()
      # print(f'done with frame on queue {queue}')
      yield (b'--frame\r\n'
             b'Content-Type: image-jpeg\r\n\r\n' + frame + b'\r\n')


@web_app.route('/')
def index():
   return flask.render_template('index.html')

@web_app.route('/video_feed')
def video_feed():
   return flask.Response(gen_frames(q_preview),
                         mimetype='multipart/x-mixed-replace; boundary=frame')

@web_app.route('/video_feed2')
def video_feed2():
   return flask.Response(gen_frames(q_raw),
                         mimetype='multipart/x-mixed-replace; boundary=frame')

@web_app.route('/video_feed3')
def video_feed3():
   return flask.Response(gen_frames(q_gray),
                         mimetype='multipart/x-mixed-replace; boundary=frame')

@web_app.route('/video_feed4')
def video_feed4():
   return flask.Response(gen_frames(q_mask),
                         mimetype='multipart/x-mixed-replace; boundary=frame')

fetch_thread.start()
web_app.run(host='0.0.0.0', port=8080)
q_preview.join()
print('bye!')
