// player movement
var _left, _right, _jump;
var _floor = place_meeting(x, y + 1, oFloor);

_left = keyboard_check(inputs.left);
_right = keyboard_check(inputs.right);
_jump = keyboard_check_pressed(inputs.jump);

hspd = (_right - _left) * spd;

// on the floor
if(_floor){
	// jump
	if(_jump) vspd = jstr;
	
	// running animation
	if(hspd != 0){
		sprite_index = sPlayer_run;
		image_xscale = sign(hspd);
	} else {
		sprite_index = sPlayer_idle;
	}
// not on the floor 
} else { 
	// jump animation
	if(vspd < 0) sprite_index = sPlayer_jump;
	else sprite_index = sPlayer_fall;
	// gravity exists
	vspd += grav
	if(hspd != 0) image_xscale = sign(hspd);
}
