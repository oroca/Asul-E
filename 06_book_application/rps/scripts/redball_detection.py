#!/usr/bin/env python
#'''
#if you want to enable /dev/video0 for VideoCapture of opencv 
#$> sudo modprobe bcm2835-v4l2         : load driver
#$> sudo apt-get install v4l-utils     
#$> sudo v4l-ctl -d /dev/video0 -p 60  : set fps to 60
#'''

import numpy as np
import cv2
import os
import rospy
from rps.msg import Num

def redball_detection(capture):
#''' detecting a red ball on the camera'''


	rospy.init_node('redball_detection', anonymous=True)
	rate = rospy.Rate(2)

	pub = rospy.Publisher("numC", Num, queue_size=10)

	print("Ros Start: Redball detection")


	while not rospy.is_shutdown():
		if capture.isOpened() == False:
			print ('error: fail to open camera')
			os.system('pause')
			return
	
		capture.set(cv2.CAP_PROP_FPS, 30)
		capture.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
		capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
		capture.set(cv2.CAP_PROP_BRIGHTNESS, 0.5)

		while cv2.waitKey(25) != ord('d') and capture.isOpened():
			ret, frame = capture.read()
			if not ret:
				print ('error: fail to get image')
				break

			frameHSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
#		'''  may need to modify range '''
			threshLow = cv2.inRange(frameHSV, (0, 100, 100), (10, 255, 255))
			threshHigh = cv2.inRange(frameHSV, (160, 100, 100), (179, 255, 255))
			cv2.imshow('threshLow', threshLow)
			cv2.imshow('threshHigh', threshHigh)

			frameRed = cv2.addWeighted(threshLow, 1.0, threshHigh, 1.0, 0.0)
			frameRed = cv2.GaussianBlur(frameRed, (9, 9), 3, 3)

			frameRed = cv2.dilate(frameRed, np.ones((5, 5), np.uint8))
			frameRed = cv2.erode(frameRed, np.ones((5, 5), np.uint8))
		
			cv2.imshow('frameRed', frameRed)
			rows, cols = frameRed.shape

			circles = cv2.HoughCircles(frameRed, cv2.HOUGH_GRADIENT, 2, rows/4)
			idx = 0
			if circles is not None:
				for circle in circles[0]:
#				''' circles found @ (x, y)'''
					idx += 1
					x, y, r = circle
					cv2.circle(frame, (x, y), 3, (0, 255, 0), -1)
					cv2.circle(frame, (x, y), r, (0, 0, 255), 3)

			pub.publish(idx)
				
			cv2.imshow('image', frame)			
	
		cv2.destroyAllWindows()
		capture.release()

if __name__ == '__main__':
	try:
		cap = cv2.VideoCapture(0)

		redball_detection(cap)
	except rospy.ROSInterruptException:
		pass

