#!/usr/bin/python

import rospy
import serial
from std_msgs.msg import String

ser = serial.Serial(port = '/dev/ttyUSB1', baudrate=115200)


def callback(data):
	global ser
	
	ser.write(data.data)
	
def SerialSend():

	rospy.init_node('SerialSend', anonymous=True)

	rate = rospy.Rate(10)

	rospy.loginfo("Serial Sending Start")

	rospy.Subscriber('ss', String, callback)

	rospy.spin()

if __name__ == '__main__':
	try:
		SerialSend()
	except rospy.ROSInterruptException:
		pass
