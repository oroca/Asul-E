#!/usr/bin/python

import rospy
import time
import serial
import math
from std_msgs.msg import String
from rps.msg import Num

detect = 0
count = 0
num_circle = 0

pub = rospy.Publisher('ss', String, queue_size=10)

def callback(data):
	global detect
	global count
	global num_circle
	global pub
	
	sam = 5

	if detect == 1:
		if count < sam:

			count = count+1;
			tmp = data.numC
			num_circle = num_circle + tmp
			print "Number of Circle: ", tmp, num_circle, "\n"
		elif count == sam:
			result = num_circle/float(sam);
			detect = 0
			count = 0
			num_circle = 0

			if result > 2.0:
				hand = 'r'
			elif result > 0.8:
				hand = 'p'
			else:
				hand = 's'

			pub.publish(hand)
			print "Detect: ", hand,"Print: ", result, "\n"
def callback2(data):
	global detect

	tmp = data.data
	if tmp == 's':
		time.sleep(1)
		print "3\n"

		time.sleep(1)
		print "2\n"

		time.sleep(1)
		print "1\n"

		detect = 1

		pub2 = rospy.Publisher("Status", String, queue_size = 10)
		pub2.publish('x')
def rpc_core():

	rospy.init_node('rpc_core', anonymous=True)
	rospy.loginfo("read Start!")

	rospy.Subscriber("/numC", Num, callback)

	rospy.Subscriber('status', String, callback2)
	rate = rospy.Rate(20)

	rospy.spin()


if __name__ == '__main__':
	try:
		rpc_core()
	except rospy.ROSInterruptException:
		pass
