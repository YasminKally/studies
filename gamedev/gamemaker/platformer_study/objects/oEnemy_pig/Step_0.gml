if(dead){
	sprite_index = sEnemy_pig_dead;
	// making the enemy disapear after the animation
	if(image_speed >= 0) image_alpha -= .01; 
	if(image_alpha <= 0) instance_destroy();
	// do not execute code if the enemy is dead
	exit;	
}

// check if on the floor
var _floor = place_meeting(x, y + 1, oFloor);

if(state == "normal"){
	// checks if colliding with an off bomb
	var _bomb = instance_place(x, y, oBomb);
	if(_bomb && _floor){
		// light the bomb if not picking it
		if(!pickBomb){
			if(_bomb.state == "off"){
				state = "lightingMatch";
			}
		// changes to pick the bomb
		} else {
			instance_destroy(_bomb);
			var _newPig = instance_create_layer(x, y, layer, oEnemy_bombPig);
			_newPig.state = "pick";
			instance_destroy();
		}
	}
	
	var _cannon = instance_place(x, y, oCannon);
	if(_cannon){
		if(_cannon.state == "idle"){
			state = "lightingMatch";
		}
	}
	
	if(_floor){
		// decide if will run
		timerToDecideWalk -= 1;
		if(timerToDecideWalk <= 0){
			walking = choose(true, false);
			// decide direction if choose to run
			if(walking){
				hspd = choose(spd, -spd);
			} else {
				hspd = 0;
			}
			timerToDecideWalk = gameSpeed * 3;
		}
	}
	// change sprite direction
	if(hspd != 0 && dmg == false){ 
		sprite_index = sEnemy_pig_run;
		image_xscale = sign(hspd);
	} else {
		sprite_index = sEnemy_pig_idle;
	}
	
	// changes direction when colliding with the wall 
	if(place_meeting(x + hspd, y, oFloor)) hspd *= -1;
	// changes direction if can't fall
	if(!canFall){
		if(!place_meeting(x + hspd + (width / 2) * sign(hspd), y + 1, oFloor)) {
			hspd *= -1;
		}
	} else {
		// gravity exists
		vspd += grav;
		if(hspd != 0) image_xscale = sign(hspd);
	}
} else if(state == "lightingMatch"){
	// change sprite if on the lightingMatch state
	if(sprite_index != sEnemy_matchPig_lighting){
		sprite_index = sEnemy_matchPig_lighting;
		image_index = 0;
	}
	// stops to start lighting the match
	hspd = 0;
	
	// change state after lighting the match
	if(image_index >= image_number - 1){
		state = "lightingWick";
	}
} else if(state == "lightingWick"){
	//change sprite if on the lightingWick state
	if(sprite_index != sEnemy_matchPig_lightingWick){
		sprite_index = sEnemy_matchPig_lightingWick;
		image_index = 0;
	}
	// lights the bomb wick after the animation
	if(image_index >= image_number -1){
		var _bomb = instance_place(x, y, oBomb);
		// change bomb state to on
		if(_bomb){
			_bomb.state = "on";
			// choose if will pick or light the next bomb
			pickBomb = choose(true, false);
		}
		
		var _cannon = instance_place(x, y, oCannon);
		if(_cannon){
			_cannon.state = "on";
		}
		// goes back to normal
		state = "normal";
	}
}
// checks if the enemy is taking damage
if(dmg && !dead){
 sprite_index = sEnemy_pig_hit;
 hspd = 0;
}
	