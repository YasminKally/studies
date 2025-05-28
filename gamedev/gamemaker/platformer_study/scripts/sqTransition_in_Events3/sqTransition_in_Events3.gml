// Auto-generated stubs for each available event.

function sqTransition_in_Moment()
{
	global.transition = true;
	if(room_next(room) != -1){
		room_goto_next()
	} else {
		room_goto(main_screen);
	}
}