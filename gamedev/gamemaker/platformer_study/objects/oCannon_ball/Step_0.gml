if(collided){
	if(!place_meeting(x, y + 1, oFloor)){
		vspd += grav;	
	}
	hspd -= hspd * .1;
	if(hspd == 0 && vspd == 0){
		var _bomb = instance_create_layer(x, y, "etc_enemy", oBomb);
		_bomb.state = "on";
		instance_destroy();
	}
}

if(place_meeting(x + sign(hspd) * sprite_get_width(sCannon_ball) / 2, y, oFloor)){
	if(vspd == 0){
		hspd *= -.3;
	}
	collided = true;
}

var _playerHit = instance_place(x, y, oPlayer)
if(_playerHit && !collided && !_playerHit.dmg){
	hspd = 0;
	with(_playerHit){
		dmg = true;
		global.hp--;
		dmgTimer = dmgTime;
		invTimer = invTime;
	}
	if(sprite_index != sBomb_boom){
		sprite_index = sBomb_boom;
		image_index = 0;
	}
}

if(sprite_index == sBomb_boom && image_index >= image_number - 1){
	instance_destroy();
}