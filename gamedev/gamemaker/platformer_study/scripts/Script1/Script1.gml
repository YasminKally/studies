function Script1(){

}

randomize();

global.transition = false;
global.maxHp = 3;
global.hp = global.maxHp;

function changeSprite(_sprite){
	image_speed = 1;
	// checks if the sprite is correct
	if(sprite_index != _sprite){
		sprite_index = _sprite;
		image_index = 0;
	}
}
