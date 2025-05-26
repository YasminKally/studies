// gravity exists
vspd += grav;

if(state == "off"){
	sprite_index = sBomb;
} else if(state == "on"){
	if(sprite_index != sBomb_on){
		sprite_index = sBomb_on;
		image_index = 0;
	}
	
	if(boomTimer > 0){
		boomTimer--;
	} else {
		state = "boom";
	}
} else if(state == "boom"){
	if(sprite_index != sBomb_boom){
		sprite_index = sBomb_boom;
		image_index = 0;
	}
	//destroys the instance after the animation ends
	if(image_index >= image_number - 1){
		instance_destroy();
	}
	// checks if the explosion hit the player
	var _playerHit = instance_place(x, y, oPlayer)
	if(_playerHit){
		with(_playerHit){
			dmg = true;
			dmgTimer = dmgTime;
			invTimer = invTime;
		}
	}
}