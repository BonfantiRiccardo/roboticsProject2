define kinect ranger
(
  sensor
  (
    range [0 10]
    fov 360.0
    samples 1200
  )
  # generic model properties
  color "black"
  size [ 0.06 0.15 0.03 ]
)

define scout_mini position
(
  pose [ 0.0 0.0 0.0 0.0 ]
  
  localization "odom"

  odom_error [0.02 0.02 0.0 0.02 ]

  size [ 0.612 0.58 0.245 ]

  block 
  (
  points 4
  point[0] [0.612 0]
  point[1] [0.612 0.58]
  point[2] [0 0.58]
  point[3] [0 0]
  z [0 0.245]
  )
  origin [ 0.0 0.0 0.0 0.0 ]
  gui_nose 1
  drive "omni"
  color "red"
  
  velocity_bounds [-3 3 -3 3 -1 1 -90 90 ]					
  acceleration_bounds [-1 1 -1 1 -1 1 -90 90]
  
  kinect(pose [0.0 0.0 0.245 0.0])
)
