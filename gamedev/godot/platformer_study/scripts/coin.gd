extends Area2D

@onready var game_manager: Node = %game_manager

func _on_body_entered(_body: Node2D) -> void:
	game_manager.add_score()
	queue_free()
