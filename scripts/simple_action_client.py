#! /usr/bin/env python

import rospy
import actionlib

from lecture_pkg.msg import *

if __name__ == "__main__":
  rospy.init_node("do_dishes_client")
  client = actionlib.SimpleActionClient("do_dishes",DoDishesAction)
  client.wait_for_server()

  goal = DoDishesGoal()
  goal.dishwasher_id = 10
  print "Requesting dishwasher %d"%(goal.dishwasher_id)
  # Fill in ...
  client.send_goal(goal)
  print client.wait_for_result(rospy.Duration.from_sec(5.0))

  result = client.get_result()
  print "Resulting dishwasher %d"%(result.total_dishes_cleaned)
