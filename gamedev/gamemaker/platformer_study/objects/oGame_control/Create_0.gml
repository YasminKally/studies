// function to draw hearts
drawHeart = function(x, y){
	draw_sprite_ext(sHeart_idle, get_timer() / 100000, x, y, 2, 2, 0, c_white, 1);
}