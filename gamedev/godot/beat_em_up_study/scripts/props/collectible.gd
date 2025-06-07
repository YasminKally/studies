class_name Collectible
extends Area2D

const GRAVITY := 600.0

@onready var animation_player := $AnimationPlayer
@onready var collectible_sprite: Sprite2D = $CollectibleSprite
@onready var hit_box: Area2D = $HitBox

@export var damage: int
@export var knockdown_intensity: float
@export var speed: float
@export var type: Type

enum State {FALL, GROUNDED, FLY}

enum Type {KNIFE, GUN, FOOD}

var anim_map := {
	 State.FALL: "fall",
	 State.GROUNDED: "grounded",
	 State.FLY: "fly",
}

var direction := Vector2.ZERO
var height := 0.0
var height_speed := 0.0
var state = State.FALL
var velocity := Vector2.ZERO

func _ready() -> void:
	height_speed = knockdown_intensity
	if state == State.FLY:
		velocity = direction * speed
	hit_box.area_entered.connect(on_emit_damage.bind())
	hit_box.body_exited.connect(on_exit_screen.bind())
	hit_box.position = Vector2.UP * height

func _process(delta: float) -> void:
	handle_fall(delta)
	handle_animations()
	collectible_sprite.flip_h = velocity.x < 0
	collectible_sprite.position = Vector2.UP * height
	position += velocity * delta
	monitorable = state == State.GROUNDED
	hit_box.monitoring = state == State.FLY

func handle_animations() -> void:
	animation_player.play(anim_map[state])

func handle_fall(delta: float) -> void:
	if state == State.FALL:
		height += height_speed * delta
		if height < 0:
			height = 0
			state = State.GROUNDED
		else: 
			height_speed -= GRAVITY * delta

func on_emit_damage(target_hurt_box: HurtBox) -> void:
	target_hurt_box.damage_received.emit(damage, direction, HurtBox.HitType.KNOCKDOWN)
	queue_free()

func on_exit_screen(_wall: AnimatableBody2D) -> void:
	queue_free()
