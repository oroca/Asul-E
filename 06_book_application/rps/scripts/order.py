#!/usr/bin/python

import rospy
from std_msgs.msg import String

def order():
	pub = rospy.Publisher("status", String, queue_size=10)

	rospy.init_node('order', anonymous=True)
	rate = rospy.Rate(2)

	print("Start!")
	while not rospy.is_shutdown():
		order_str = raw_input()
		print(order_str)
		rospy.loginfo(order_str)
		pub.publish(order_str)
		rate.sleep()

if __name__ == '__main__':
	try:
		order()
	except rospy.ROSInterruptException:
		pass
