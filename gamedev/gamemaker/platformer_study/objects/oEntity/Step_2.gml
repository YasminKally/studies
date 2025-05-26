// horizontal collision and movement
// check if the player is colliding with the walls
var _xCol = instance_place(x + hspd, y, oFloor);
if(_xCol){
	// right
	if(hspd > 0){
		x = _xCol.bbox_left + (x - bbox_right);
	}
	// left
	if(hspd < 0){
		x = _xCol.bbox_right + (x - bbox_left);
	}
	
	hspd = 0;
}

x += hspd;

// vertical collision and jump
// check if the player is colliding with the floor
var _yCol = instance_place(x, y + vspd, oFloor);
if(_yCol){
	// down
	if(vspd > 0){
		y = _yCol.bbox_top + (y - bbox_bottom)
	}
	// up
	if(vspd < 0){
		y = _yCol.bbox_bottom + (y - bbox_top)
	}
	
	vspd = 0;
}

y += vspd;
