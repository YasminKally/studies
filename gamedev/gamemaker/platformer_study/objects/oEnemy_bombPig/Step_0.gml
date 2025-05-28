if(dmg && !dead) state = "hit";

if(!canFall){
	// changes direction if would fall
	if(!place_meeting(x + hspd + (width / 2) * sign(hspd), y + 1, oFloor)) {
		hspd *= -1;
	}
} else {
	// gravity exists
	vspd += grav;
}

switch(state){
	case "idle":
		changeSprite(sEnemy_bombPig_idle);
		
		thrown = false;
		hspd = 0;
		
		// check if player is within vision range
		playerOnRange();
		
		// changes state after some time
		stateTimer--;
		if(stateTimer <= 0){
			state = choose("run", "idle");
			stateTimer = stateTime;
			
			if(state == "run"){
				hspd = choose(1, -1);
			}
		}
		break;
	case "run":
		changeSprite(sEnemy_bombPig_run);
		if(hspd != 0) image_xscale = sign(hspd);
		
		// changes direction if hitting a wall
		if(place_meeting(x + hspd, y, oFloor)) hspd *= -1;
		
		// checks if player is within vision range
		playerOnRange();
		
		// changes state after some time
		stateTimer--;
		if(stateTimer <= 0){
			state = choose("run", "idle");
			stateTimer = stateTime;
		}
		break;
	case "throw":
		changeSprite(sEnemy_bombPig_throw);
		hspd = 0;
		if(image_index >= 3 && !thrown){
			// throws the bomb
			var _bomb = instance_create_layer(x, y - 14, "etc_enemy", oBomb);
			_bomb.hspd = 5 * image_xscale;
			_bomb.vspd = -2;
			_bomb.state = "on";
			thrown = true;
		}
		// change to normal pig after animation end
		if(image_index >= image_number - 1){
			var _newPig = instance_create_layer(x, y, layer, oEnemy_pig);
			_newPig.state = "normal";
			_newPig.pickBomb = false;
			_newPig.hspd = 0;
			instance_destroy();
		}
		break;
	case "pick":
		changeSprite(sEnemy_bombPig_pick);
		hspd = 0;
		
		if(image_index >= image_number - 1){
			state = "idle";
		}
		break;
	case "hit":
		changeSprite(sEnemy_pig_hit);
		hspd = 0;
		if(image_index >= image_number - 1){
			state = "dead";
			dead = true;
		}
		break;
	case "dead":
		changeSprite(sEnemy_pig_dead)
		if(image_speed >= 0) image_alpha -= .01;
		if(image_index >= image_number - 1) instance_destroy();
		break;
}
