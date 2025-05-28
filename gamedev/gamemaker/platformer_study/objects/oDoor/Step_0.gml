// doesn't do anything while a transtion is occuring
if(global.transition) exit;

switch(state){
	case "idle":
		sprite_index = sDoor;
		image_speed = 1;
		// check if player is colliding with the door
		var _player = instance_place(x, y, oPlayer)
		if(_player){
			// check if player is pressing W
			if(keyboard_check_pressed(ord("W")) && isExit){
				state = "opening";
				// changes the player state to animate
				_player.state = "in";
			}
		}
		break;
	case "opening":
		changeSprite(sDoor_opening)
		// remain open while the player is enetring
		if(image_index >= image_number - 1){
			image_speed = 0;
			// creating the fade transition in
			layer_sequence_create("transition", oPlayer.x, oPlayer.y - 14, sqTransition_in);
		}
		break;
	case "closing":
		changeSprite(sDoor_closing)
		if(image_index >= image_number - 1) state = "idle";
		break;
}