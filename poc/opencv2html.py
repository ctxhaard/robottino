#!/bin/env python3
# to check camera: 
# libcamera-vid -t 0 --inline --vflip --listen -o tcp://0.0.0.0:8888
# v4l2-ctl --device /dev/video0 --all | less
import cv2
import flask

web_app = flask.Flask(__name__,
                      template_folder='.')
camera = cv2.VideoCapture('libcamerasrc')

print(camera.isOpened())
print(camera.grab())
print(camera.read())

def gen_frames():
  while True:
      success, frame = camera.read()
      if not success:
          print('cannot read frame from camera')
          break
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
   web_app.run(debug=True, host='0.0.0.0')

