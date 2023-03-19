#!/bin/env python3
# to check camera: 
# libcamera-vid -t 0 --inline --vflip --listen -o tcp://0.0.0.0:8888
# v4l2-ctl --device /dev/video0 --all | less
import cv2
import flask

web_app = flask.Flask(__name__,
                      template_folder='.')

def gen_frames():
  while True:
      frame = cv2.imread('sample.jpg')
      ret, buffer = cv2.imencode('.jpg', frame)
      frame = buffer.tobytes()
      print(f'sending frame ({len(frame)} B)')
      yield (b'--frame\r\n'
             b'Content-Type: image-jpeg\r\n\r\n' + frame + b'\r\n')
      
@web_app.route('/')
def index():
   return flask.render_template('index.html')

@web_app.route('/video_feed')
def video_feed():
   rsp = flask.Response(gen_frames(),
                         mimetype='multipart/x-mixed-replace; boundary=frame')
   print(f'encoding: {rsp.stream.encoding}')
   return rsp

@web_app.route('/video_feed2')
def video_feed2():
   rsp = flask.Response(gen_frames(),
                         mimetype='multipart/x-mixed-replace; boundary=frame')
   print(f'encoding: {rsp.stream.encoding}')
   return rsp

@web_app.route('/video_feed3')
def video_feed3():
   rsp = flask.Response(gen_frames(),
                         mimetype='multipart/x-mixed-replace; boundary=frame')
   print(f'encoding: {rsp.stream.encoding}')
   return rsp

@web_app.route('/video_feed4')
def video_feed4():
   rsp = flask.Response(gen_frames(),
                         mimetype='multipart/x-mixed-replace; boundary=frame')
   print(f'encoding: {rsp.stream.encoding}')
   return rsp

if __name__ == '__main__':
   web_app.run(debug=False, host='0.0.0.0')

