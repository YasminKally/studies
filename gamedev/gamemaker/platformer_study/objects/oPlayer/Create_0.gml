event_inherited();
// create fade transition out
if(room != main_screen){
	global.out_transition = layer_sequence_create("transition", oPlayer.x, oPlayer.y - 14, sqTransition_out);
	state = "out"; // normal, out, in
	sprite_index = sPlayer_door_out;
} else {
	state = "normal";
	sprite_index = sPlayer_idle;
};
image_speed = 0;

jstr = -7;

dmg = false;
dmgTime = gameSpeed * .5;
dmgTimer = 0;

invTime = gameSpeed;
invTimer = 0;

restartTime = gameSpeed * 2;
restartTimer = restartTime;

inputs = {
	left : ord("A"),
	right : ord("D"),
	jump : vk_space
}
