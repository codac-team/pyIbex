from vibes import vibes
from pyIbex import PixelMap2D, CtcPixelMap, IntervalVector, Interval
from scipy import misc
import numpy as np
"""
Example : image contractor
"""

# Generate the image using vibes
# Only use for drawing
vibes.beginDrawing()
vibes.newFigure('Map')
vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 500, 'height': 500})
vibes.drawCircle(4, 4, 3, '[k]')
vibes.drawLine([[-3, -1], [7, -1]], '[k]')
vibes.drawLine([[7, -1], [7, 1]], '[k]')
vibes.axisLimits(-4,8,-4,8)



# Read the image and extract the last layer 
img = misc.imread('./img.png')[:,:,3]
# normalize in order to have a binary image.
img = img/np.max(img)
if np.max(img) != 1:
	print("Error, img must be a binary image")

# convert img into a unsigned int64 image type
img = img.astype(np.uint64) 
# vertical flip to inverse the y axis
img = np.flipud(img)

# PixelMap2D constructor takes :
#			-	a binary image in uint64 format.
#			- x position of the lower left pixel in world frame
#			- y position of the lower left pixel in world frame
#			- size of a pixel in the x direction (in meter)
#			- size of a pixel in the y direction (in meter)
x_res = 12.0 / img.shape[0]
y_res = 12.0 / img.shape[1]
p = PixelMap2D(img, -4, -4, x_res, y_res)
# Compute the intgral image (in place)
p.compute_integral_image()
# build the contractor
ctc = CtcPixelMap(p)

X0 = IntervalVector([[-1, 4], [0, 3]])
vibes.drawBox(X0[0][0], X0[0][1], X0[1][0], X0[1][1], "b")
ctc.contract(X0)
vibes.drawBox(X0[0][0], X0[0][1], X0[1][0], X0[1][1], "r")


