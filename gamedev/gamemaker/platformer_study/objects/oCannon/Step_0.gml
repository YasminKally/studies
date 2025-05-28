switch(state){
	case "idle":
		changeSprite(sCannon_idle);
		shot = false;
		break;
	case "on":
		changeSprite(sCannon_on)
		
		if(image_index >= 1.5 && !shot){
			// instance the cannon ball
			var _cannonBall = instance_create_layer(x, y - 6, "projectiles", oCannon_ball);
			_cannonBall.hspd = 6 * image_xscale;
			shot = true
		}
		
		if(image_index >= image_number - 1){
			state = "wait";
		}
		break;
	case "wait":
		changeSprite(sCannon_idle);
		waitTimer--;
		if(waitTimer <= 0){
			waitTimer = waitTime;
			state = "idle";
		}
		break;
}