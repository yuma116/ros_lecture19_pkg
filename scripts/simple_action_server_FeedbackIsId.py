#! /usr/bin/env python
import rospy
import actionlib
from lecture_pkg.msg import *
import time


class DoDishesServer:
  def __init__(self):
    self.server = actionlib.SimpleActionServer("do_dishes", DoDishesAction, self.execute, False)
    self.server.start()

  def execute(self, goal):
    # Do lots ...
    print "Requesting dishwasher %d"%(goal.dishwasher_id)
    result = self.server.get_default_result()
    result.total_dishes_cleaned = 100
    print "Returning dishes_cleaned %d"%(result.total_dishes_cleaned)
    #time.sleep(10)
    
    dishes = goal.dishwasher_id
    for i in xrange(dishes):
      feedback = DoDishesFeedback()
      feedback.percent_complete = i
      self.server.publish_feedback( feedback)
      print("feedback:"+str(feedback))
    
    self.server.set_succeeded(result)

if __name__ == "__main__":
  rospy.init_node("do_dishes_server")
  server = DoDishesServer()
  rospy.spin()
