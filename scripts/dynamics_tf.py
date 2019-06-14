#!/usr/bin/env python  

import rospy
import tf

if __name__ == '__main__':
    rospy.init_node('dynamics_tf')

    i = 0
    r = rospy.Rate(10)
    while not rospy.is_shutdown():
        br = tf.TransformBroadcaster()
        br.sendTransform((i, 0, 0),
                     tf.transformations.quaternion_from_euler(0, 0, 0),
                     rospy.Time.now(),
                     "link4",
                     "link3")
        i += 0.001
        r.sleep()


