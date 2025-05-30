extends Node2D

const SPEED := 30

var dir := 1
@onready var raycast_right: RayCast2D = $raycast_right
@onready var raycast_left: RayCast2D = $raycast_left
@onready var animated_sprite: AnimatedSprite2D = $AnimatedSprite2D

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if raycast_right.is_colliding():
		dir = -1
		animated_sprite.flip_h = true
	if raycast_left.is_colliding():
		dir = 1
		animated_sprite.flip_h = false
	
	position.x += dir * SPEED * delta
	
