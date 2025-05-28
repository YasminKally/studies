// Inherit the parent event
event_inherited();
width = sprite_get_width(sEnemy_pig_idle);
visionRange = 100;
thrown = false;

state = "idle"; // idle, run, pick, throw, hit

stateTime = gameSpeed;
stateTimer = stateTime;

playerOnRange = function(){
	var _playerSeen = collision_line(x, y - 14, x + visionRange * image_xscale, y - 14, oPlayer, false, true);
	if(_playerSeen){
		state = "throw";
	}
}
