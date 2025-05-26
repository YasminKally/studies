// Inherit the parent event
event_inherited();
width = sprite_get_width(sEnemy_pig_idle);

// enemy state
state = "normal";

// choose direction to run
spd = choose(1, -1);
walking = choose(true, false);
// decide direction if choose to run
if(walking){
	hspd = choose(spd, -spd);
} else {
	hspd = 0;
}
// timer to decide if will run
timerToDecideWalk = gameSpeed * 3;