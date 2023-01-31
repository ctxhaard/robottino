#!/bin/env python3

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

class Motor:
  PWM_FREQUENCY_HZ = 100.0

  def __init__(self, num, en, dir, pwm, sf=None):
    self.num = num
    self.en = en
    self.dir = dir
    self.pwm = pwm
    self.sf = sf
    self.pwm_handle = self.gpio_init()
    self.check()

  def __del__(self):
    self.gpio_deinit()

  def gpio_init(self):
    GPIO.setup( (self.en, self.dir), GPIO.OUT)
    if self.sf: GPIO.setup(self.sf, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    pwm = GPIO.PWM(self.pwm, Motor.PWM_FREQUENCY_HZ)
    pwm.start(0)
    return pwm

  def gpio_deinit(self):
    self.pwm_handle.stop()
    self.pwm_handle = None
    GPIO.cleanup((self.en, self.dir, self.pwm))
    if self.sf: GPIO.cleanup(self.sf)

  def check(self):
    if self.sf:
      GPIO.output(self.en, GPIO.HIGH)
      time.sleep(0.1)
      print("Motor controller {} is on").format(self.num) if not GPIO.input(self.sf) else print("Motor controller {} ERROR!").format(self.num)
      GPIO.output(self.en, GPIO.LOW)

m1 = Motor(1, 22, 24, 12, sf = 5)
m2 = Motor(2, 23, 25, 13, sf = 6)

import sys

try:
  while True:
    time.sleep(0.1)
except KeyboardInterrupt:
    del m1
    del m2
    print("Bye")
    sys.exit(0)
