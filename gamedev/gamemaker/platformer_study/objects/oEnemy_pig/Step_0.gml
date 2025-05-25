// check if on the floor
var _floor = place_meeting(x, y + 1, oFloor);

if(_floor){
	// decide if will run
	timerToDecideWalk -= 1;
	if(timerToDecideWalk <= 0){
		vel = choose(1, -1);
		walking = choose(true, false);
		timerToDecideWalk = deltaTime * 2;
	}
	
	if(walking){
		//walks if on the floor
		hspd = vel;
	}
	// change sprite direction
	if(hspd != 0){ 
		sprite_index = sEnemy_pig_run;
		image_xscale = sign(hspd);
	} else {
		sprite_index = sEnemy_pig_idle;
	}
} else {
	// gravity exists
	vspd += grav;
	hspd = 0;
	
	if(hspd != 0) image_xscale = sign(hspd);
}
