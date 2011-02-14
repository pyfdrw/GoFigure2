import os
address = os.getcwd()
#address = "/home/es1076/GITROOT/GoFigure2/Testing/Sikuli/gofigure2_images"

#address = address+"/gofigure2_images"
#print address
setBundlePath(address)

click("NavigationIcon.png")
click("TableWidgetIcon.png")
click("MeshSegmentationIcon.png")


wait(2)
click("Add_new_track.png")
type(Key.ENTER)
#x = region.getLastMatch()

#click(find("Mode.png").right(200))
#click("Manual.png")

import random
region = find(Pattern("XY_ViewRegion.png").similar(0.30))
print region

for i in range(1,5):
	#could make it a straight line by moving differentials in one direction.
	xclick = random.randint(region.x,region.w+region.x)
	yclick = random.randint(region.y,region.h+region.y)
	print xclick, yclick
	
	#change time point
	
	click(Location(xclick,yclick))
	click("Apply.png")
	keyDown(Key.CTRL)
	type("c")
	keyUp(Key.CTRL)
	wait(4)