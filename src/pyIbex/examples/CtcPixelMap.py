import pyIbex
from pyIbex import Ctc, Interval, IntervalVector

try:
	import numpy as np
except ImportError:
	print("Error importing numpy")
import copy


class CtcPixelMap2D(pyIbex.Ctc):
	"""
	Image contractor (from Yan Sliswka thesis)
	params:
		img : numpy array which contains the an occupancy grid.
				  value must be 0 or 1
				  1-valued pixel are occuped while 0-valued are not.
		x0 : x origin of the bottom left corner
		y0 : y origin of the bottom left corner
		x_res : pixel size in x direction
		y_res : pixel size in y  direction
		is_II: optional defaut value True
					if false compute the Integral Image of img 

	store x0, y0 into _origin field
	store x_res, y_res into _resolution field
	"""
	def __init__(self, img, x0, y0, x_res, y_res, is_II=True):
		Ctc.__init__(self, 2)
		img = img.astype(np.int)
		# test on image
		if np.max(img) > 1 and np.min(img) < 0:
			print("Error min max image !!! ")
		self._origin = np.array([x0, y0])
		self._leaf_size = np.array([x_res, y_res])
		self._II = copy.copy(img)
		if is_II == False:
			self.II = self.II.cumsum(axis=0).cumsum(axis=1)

		self._boundingBox = IntervalVector([[x0, x0*img.shape[0]*x_res], [y0, y0*img.shape[0]*y_res]])
		
	def world_to_grid(self, box):
		"""
		convert world coordinate (double value) into integer value 
		corresponding to pixels of the grid
		params:
			box : box of coordinate in world frame
		return:
			x, y x and y coordinates in image frame (in pixel)
		"""
		box_int = np.zeros(2*box.size(), np.int)
		
		for i in range(0, box.size()):
			box[i] = (box[i] - self._origin[i]) / self._leaf_size[i]
			# Limit range to image size on pixel_coord
			box[i] &= Interval(0,self._II.shape[i]);
			box_int[2*i] = np.floor(box[i].lb())
			box_int[2*i+1] = np.ceil(box[i].ub()-1)
		return box_int

	def grid_to_world(self, box, box_int):
		"""
		convert grid coordinate (integer values) into world coordinate
		params:
			box     : box of coordinate in world frame
			box_int : box of coordinate in grid frame
		return:
			box
		"""
		for i in range(0, box.size()-1):
			box[i] &= Interval(box[2*i], box[2*i+1]+1) * self._leaf_size[i] + self._origin[i];
			if(box[i].is_empty()):
				box.set_empty()
				return;

	def contract(self, box):
		if(box.is_empty()):
		 return;

		# Convert world coordinates into pixel coordinates
		box_int = self.world_to_grid(box);

		# Contractor the box
		self._contract(box_int[0],box_int[1],box_int[2],box_int[3])
	
		# Check the result
		if(pixel_coords[0] == -1):
			box.set_empty();
			return;

			#Convert pixel coordinates into world coordinates
			grid_to_world(box, box_int)


	def _contract(self, cxmin, cxmax, cymin, cymax):

    # compute enclosed pixels on consecutive lines from all dimensions of the box

		cxmax = np.max(0,np.min(self._II.shape[0]-1,cxmax));
		cxmin = np.min(self._II.shape[0]-1,np.max(0,cxmin));
		cymax = np.max(0,np.min(self._II.shape[1]-1,cymax));
		cymin = np.min(self._II.shape[1]-1,np.max(0,cymin));
	
		if (enclosed_pixels(cxmin,cxmax,cymin,cymax) == 0):
			cxmin = -1;
			return;
		# right
		for i in range(cxmin, cxmax+1):
			if(enclosed_pixels(cxmin,i,cymin,cymax)>0):
				cxmin = i;
				break;
		# left
		for i in range(cxmax, cxmin-1, -1):
			if(enclosed_pixels(i,cxmax,cymin,cymax)>0):
				cxmax = i;
				break;
		# down
		for j in range(cymin, cymax+1):
			if(enclosed_pixels(cxmin,cxmax,cymin,j)>0):
				cymin = j;
				break;
		# up
		for j in range(cymax,cymin-1, -1):
			if(enclosed_pixels(cxmin,cxmax,j,cymax)>0):
				cymax = j;
				break;

	def enclosed_pixels(xmin, xmax, ymin, ymax):		
		b1 = self._II[xmax,ymax]
		b2 = self._II[xmax,ymin-1]
		b3 = self._II[xmin-1,ymax]
		b4 = self._II[xmin-1,ymin-1]
		return b1 - b2 - b3 + b4;

    
if __name__ == '__main__':
	a = np.zeros((10,10))
	a[5:7,4:6] = 1
	print(a)
	ctc = CtcPixelMap2D(a, 0, 0, 1, 1)
	X = IntervalVector(2, [2, 10])
	ctc.contract(X)
	print(X)
