// Inherit the parent event
event_inherited();

// choose direction to run
vel = choose(1, -1);
walking = false;
deltaTime = game_get_speed(gamespeed_fps);
timerToDecideWalk = deltaTime * 2;