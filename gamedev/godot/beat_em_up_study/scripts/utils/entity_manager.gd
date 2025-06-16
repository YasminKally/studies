extends Node

signal spawn_collectible(type: Collectible.Type, initial_state: Collectible.State, 
collectible_global_position: Vector2, collectible_direction: Vector2, initial_height: float)

signal spawn_enemy(enemy_data: EnemyData)

signal death_enemy(enemy: Character)
