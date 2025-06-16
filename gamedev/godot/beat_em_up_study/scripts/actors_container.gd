extends Node2D

const PREFAB_MAP := {
	Collectible.Type.KNIFE: preload("res://scenes/props/knife.tscn"),
	Collectible.Type.FOOD: preload("res://scenes/props/food.tscn"),
} 

const ENEMY_MAP := {
	Character.Type.PUNK: preload("res://scenes/characters/basic_enemy.tscn"),
	Character.Type.GOON: preload("res://scenes/characters/goon_enemy.tscn"),
	Character.Type.BOUNCER: preload("res://scenes/characters/igor_boss.tscn"),
}

@export var player: Player

func _ready() -> void:
	EntityManager.spawn_collectible.connect(on_spawn_collectible.bind())
	EntityManager.spawn_enemy.connect(on_spawn_enemy.bind())

func on_spawn_collectible(type: Collectible.Type, initial_state: Collectible.State, 
collectible_global_position: Vector2, collectible_direction: Vector2, initial_height: float) -> void:
	var collectible: Collectible = PREFAB_MAP[type].instantiate()
	collectible.state = initial_state
	collectible.height = initial_height
	collectible.global_position = collectible_global_position
	collectible.direction = collectible_direction
	call_deferred("add_child", collectible)

func on_spawn_enemy(enemy_data: EnemyData) -> void:
	var enemy: Character = ENEMY_MAP[enemy_data.type].instantiate()
	enemy.global_position = enemy_data.global_position
	enemy.player = player
	add_child(enemy)
	
