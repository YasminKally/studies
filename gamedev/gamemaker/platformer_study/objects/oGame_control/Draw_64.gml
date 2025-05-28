// drawing hp quantity
draw_text(20, 20, global.hp);

// drawing hearts for each hp
var _x = 20;
var _y = 40;

repeat(global.hp){
	drawHeart(_x, _y);
	_x += 30;
}
