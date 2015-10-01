from pyIbex import SetVisitor, BoolInterval, YES, NO, MAYBE
from pyIbex import Interval, IntervalVector
from vibes import vibes

class ToVibes(SetVisitor):

    """
    Plot a  box within the frame [-max,max]x[-max,max]
     
    The frame avoids, in particular, to plot unbounded OUT boxes.
    """
    def __init__(self, max, figureName=None):
        SetVisitor.__init__(self)
        self.frame = IntervalVector(2, max*Interval(-1, 1))
        self.figureName = figureName
        if figureName != None:
            vibes.newFigure(figureName)
            vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 500, 'height': 500})
    """
    Function that will be called automatically on every boxes (leaves) of the set.
    """
    def visit_leaf(self, box, status):

        framebox= box & self.frame;

        #  Associate a color to the box.
        #  - YES (means "inside") is in green
        #  - NO (means "outside") is in red
        #  - MAYBE (means "boundary") is in blue.
      

        if status == YES:
            color="[r]"
        elif status == NO:
            color="[b]"
        else :
            color="[y]"
      
        # Plot the box with Vibes
        vibes.drawBox(framebox[0].lb(), framebox[0].ub(), framebox[1].lb(), framebox[1].ub(), color);

    def pre_visit(self):
        vibes.selectFigure(self.figureName)

    def post_visit(self):
        vibes.axisEqual()
