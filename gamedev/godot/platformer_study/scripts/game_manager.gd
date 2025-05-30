extends Node

var score = 0
@onready var score_label: Label = $score_label

func add_score():
	score += 1
	score_label.text = "you collected " + str(score) + " coins"
