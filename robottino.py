#!/bin/env python3

import RPi.GPIO as GPIO
import time
import sys
import asyncio

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

  def __del__(self):
    self.gpio_deinit()

  def gpio_init(self):
    GPIO.setup( (self.en, self.dir, self.pwm), GPIO.OUT)
    if self.sf: GPIO.setup(self.sf, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    pwm = GPIO.PWM(self.pwm, Motor.PWM_FREQUENCY_HZ)
    pwm.start(0.0)
    return pwm

  def gpio_deinit(self):
    self.pwm_handle.stop()
    self.pwm_handle = None
    GPIO.cleanup((self.en, self.dir, self.pwm))
    if self.sf: GPIO.cleanup(self.sf)

  async def check(self):
    print(f"checking motor {self.num}")
    self.pwm_handle.start(50.0)
    GPIO.output(self.en, GPIO.HIGH)
    GPIO.output(self.dir, GPIO.HIGH)
    await asyncio.sleep(1.0)
    GPIO.output(self.dir, GPIO.LOW)
    await asyncio.sleep(1.0)
    GPIO.output(self.en, GPIO.LOW)
    self.pwm_handle.stop()
    if self.sf:
      print(f"Motor controller {self.num} is on") if GPIO.input(self.sf) else print("Motor controller {self.num} ERROR!")
    print(f"checking motor {self.num} ended")


async def main():
  m1 = Motor(1, 22, 24, 12, sf = 5)
  m2 = Motor(2, 23, 25, 13, sf = 6)
  await asyncio.gather(
    m1.check(),
    m2.check()
  )

try:
  asyncio.run(main())
  while True:
    time.sleep(0.1)
except KeyboardInterrupt:
    print("Bye")
    GPIO.cleanup()
    sys.exit(0)
