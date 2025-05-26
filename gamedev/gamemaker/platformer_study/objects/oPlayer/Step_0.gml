// player movement
var _left, _right, _jump;
var _floor = place_meeting(x, y + 1, oFloor);

_left = keyboard_check(inputs.left);
_right = keyboard_check(inputs.right);
_jump = keyboard_check_pressed(inputs.jump);

// move if not taking dmg
if(dmgTimer <= 0){
	hspd = (_right - _left) * spd;
}
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
	if(vspd < 0){
		sprite_index = sPlayer_jump;
	} else {
		sprite_index = sPlayer_fall;
		// check if player hit an enemy while falling
		var _enemyHit = instance_place(x, y + 1, oParent_enemy);
		if(_enemyHit){
			dmg = false;
			if(!_enemyHit.dead){
				// jumps again if hits an enemy
				vspd = jstr;
				// do damage to the enemy
				_enemyHit.dmg = true;
			}
		}
	}
	// gravity exists
	vspd += grav;
	if(hspd != 0) image_xscale = sign(hspd);
}
	// change sprite if taking dmg
if(dmg){
	sprite_index = sPlayer_hit;
}

if(dmgTimer > 0){
	dmgTimer--;
	hspd = 0;
} else {
	dmg = false;
}

if(invTimer > 0){ 
	invTimer--;
	image_alpha = .5;	
} else {
	image_alpha = 1;
}
// taking dmg
var _enemy = instance_place(x + sign(hspd), y, oParent_enemy);
if(_enemy && invTimer <= 0){
	if(!_enemy.dead && !_enemy.dmg){
		dmg = true;
		dmgTimer = dmgTime;
		invTimer = invTime;
	}
}
