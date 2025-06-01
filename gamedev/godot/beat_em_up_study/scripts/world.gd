extends Node2D

@onready var player := $ActorsContainer/player
@onready var camera := $camera

func _process(_delta: float) -> void:
	if player.position.x > camera.position.x:
		camera.position.x = player.position.x 
