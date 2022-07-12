>light
>position
	-0.333, 1, 1, 0
>ambient
	0.521569, 0.521569, 0.521569
>diffuse
	1, 1, 1
>specular
	0.5, 0.5, 0.5
>toonShading
	1
>stepAmount
	7
>stepSize
	0.138
>specSize
	0.1
>shadow
	1
>shadowPCF
	0
>shadowDistance
	99

>entity
>childs
	0
>name
	PauseCanvas
>component_count
	5
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Panel
>enabled
	0
>position
	0, 0
>scale
	760, 428
>tint
	1, 1, 1, 0.2
>Panel
>enabled
	0
>position
	4, 14.1
>scale
	250, 114.7
>tint
	0.141593, 0.141593, 0.141593, 0.5
>Button
>enabled
	0
>position
	6, 17.4
>scale
	220, 40
>tint
	0, 0, 0, 1
>text
	Resume
>textScale
	1
>idle
	0.8, 0, 0, 1
>hovered
	0.8, 0.4, 0.4, 1
>activated
	1, 0, 0, 1
>Button
>enabled
	0
>position
	7, 29.7
>scale
	200, 30
>tint
	0, 0, 0, 1
>text
	Menu
>textScale
	1
>idle
	0, 0, 0, 1
>hovered
	0.380531, 0.380531, 0.380531, 1
>activated
	0.247788, 0.228052, 0.228052, 1
>PauseTrigger
>enabled
	0

>entity
>childs
	0
>name
	Camera
>component_count
	2
>transform
>position
	72.2686, -36.6231, 33.9683
>rotation
	-0.959931, -0.785398, 0
>scale
	1, 1, 1
>Camera
>enabled
	1
>near
	10
>far
	1000
>fovY
	90
>FollowCamera
>enabled
	1
>offset
	10, 23, 10
>lerpFactor
	0.08

>entity
>childs
	0
>name
	PortalManager
>component_count
	3
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>PortalManager
>enabled
	1
>activationTime
	60
>closeTime
	20
>LevelSelector
>enabled
	0
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\Portal.wav
>loop
	1
>volume
	60

>entity
>childs
	4
>name
	SkelClone
>component_count
	5
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Skeleton
>enabled
	0
>attackRange
	10
>attackTime
	1
>chargingTime
	0.6
>hurtEffectTime
	0.1
>hurtEffectTint
	0.6, 0.6, 0, 1
>deathTime
	2
>Life
>enabled
	0
>life
	3
>invincibilityTime
	0.75
>EnemyStats
>enabled
	0
>baseLife
	3
>baseDamage
	3
>baseSpeed
	900
>baseAttackSpeed
	0.5
>baseScore
	5
>increaseRatio
	0.5
>SphereCollider
>enabled
	0
>cTrigger
	0
>cStatic
	0
>cKinematic
	0
>cDynamic
	1
>posX
	0
>posY
	0
>posZ
	0
>rotX
	1
>rotY
	1
>rotZ
	1
>EnemyMeshRotation
>enabled
	0

>entity
>childs
	0
>name
	GFX
>component_count
	1
>transform
>position
	0, -0.3, 0
>rotation
	0, 0, 0
>scale
	0.048, 0.048, 0.048
>AnimatedDraw
>enabled
	0
>meshPath
	game/assets/Models/Skeleton/Skeleton.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_01.png
>skmodel.material.ambient
	0.8, 0.8, 0.8
>skmodel.material.diffuse
	1, 1, 1
>skmodel.material.specular
	1, 1, 1
>skmodel.material.emissive
	0, 0, 0
>skmodel.material.tint
	0, 0, 0, 1
>skmodel.material.shininess
	32
>skmodel
>count
	1
>transform
>position
	0, 0, 0
>rotation
	2.0944, 0, -0.349066
>scale
	10000, 10000, 10000
>boneID
	27
>socketMesh
	game/assets/Models/Skeleton/SkelSword.fbx
>socketTexture
	game/assets/Textures/Dungeons_Texture_01.png
>animGraph
>count
	5
>file
	game/assets/Models/Skeleton/Skeleton_Idle.fbx
>file
	game/assets/Models/Skeleton/Skeleton_Death.fbx
>file
	game/assets/Models/Skeleton/Skeleton_Slash.fbx
>file
	game/assets/Models/Skeleton/Skeleton_Walk.fbx
>file
	game/assets/Models/Skeleton/Skeleton_Impact.fbx

>entity
>childs
	0
>name
	Collider
>component_count
	1
>transform
>position
	0, 4.5, 0
>rotation
	0, 0, 0
>scale
	1.5, 3, 1
>BoxCollider
>enabled
	0
>cTrigger
	0
>cStatic
	0
>cKinematic
	1
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	WeaponCollider
>component_count
	2
>transform
>position
	0, 4.4, 4.7
>rotation
	0, 0, 0
>scale
	1.5, 3.34433, 3.2
>BoxCollider
>enabled
	0
>cTrigger
	1
>cStatic
	0
>cKinematic
	0
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0
>EnemyWeapon
>enabled
	0

>entity
>childs
	0
>name
	Sounds
>component_count
	4
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\skeleton\SkeletonTeeth.wav
>loop
	0
>volume
	10
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\skeleton\FootstepSkel.mp3
>loop
	1
>volume
	70
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\swords\Hit_Skeleton.mp3
>loop
	0
>volume
	60
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\skeleton\Skel_Death.mp3
>loop
	0
>volume
	75

>entity
>childs
	6
>name
	Player
>component_count
	5
>transform
>position
	63, -59, 16
>rotation
	0, 0.00259682, 0
>scale
	1, 1, 1
>Life
>enabled
	1
>life
	30
>invincibilityTime
	1
>ImagePanel
>enabled
	0
>position
	-15, -10
>scale
	956, 524
>tint
	1, 1, 1, 1
>imageFlip
	0
>Texture
	game/assets/Textures/bloodyscreen.png
>SphereCollider
>enabled
	1
>cTrigger
	0
>cStatic
	0
>cKinematic
	0
>cDynamic
	1
>posX
	0
>posY
	0
>posZ
	0
>rotX
	1
>rotY
	1
>rotZ
	1
>Player
>enabled
	1
>speed
	1200
>speedWhenAttack
	600
>dashTime
	30
>dashSpeed
	100
>recoveryTime
	1
>baseDamage
	3
>attackStreakTime
	0.75
>weaponTriggerTime
	0.4
>comboAttackTime
	0.75
>attackAnimSpeed
	1.75
>attack1PlayTime
	30
>attack2PlayTime
	30
>combo kill count
	5
>comboStreakTime
	5
>hurtEffectTime
	0.5
>hurtEffectSpeed
	5
>timeAfterDeath
	5
>PlayerMeshRotation
>enabled
	1

>entity
>childs
	0
>name
	GFX
>component_count
	1
>transform
>position
	0, -0.3, 0
>rotation
	0, 0, 0
>scale
	0.05, 0.05, 0.05
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets/Models/Hero/Hero.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_01.png
>skmodel.material.ambient
	0.8, 0.8, 0.8
>skmodel.material.diffuse
	1, 1, 1
>skmodel.material.specular
	1, 1, 1
>skmodel.material.emissive
	0, 0, 0
>skmodel.material.tint
	0, 0, 0, 1
>skmodel.material.shininess
	32
>skmodel
>count
	2
>transform
>position
	0, 0, 0
>rotation
	-1.5708, -2.18166, 0
>scale
	75, 75, 75
>boneID
	12
>socketMesh
	game/assets/Models/Hero/HeroShield.fbx
>socketTexture
	game/assets/Textures/Dungeons_Texture_03.png
>transform
>position
	0, 0, 0
>rotation
	2.18166, 0, 0
>scale
	125, 125, 125
>boneID
	31
>socketMesh
	game/assets/Models/Hero/HeroSword.fbx
>socketTexture
	game/assets/Textures/Dungeons_Texture_03.png
>animGraph
>count
	7
>file
	game/assets/Models/Hero/Hero_Idle_1.fbx
>file
	game/assets/Models/Hero/Hero_Walk.fbx
>file
	game/assets/Models/Hero/Hero_Run.fbx
>file
	game/assets/Models/Hero/Hero_Slash_1.fbx
>file
	game/assets/Models/Hero/Hero_Slash_3.fbx
>file
	game/assets/Models/Hero/Hero_Slash_4.fbx
>file
	game/assets/Models/Hero/Hero_Death.fbx

>entity
>childs
	0
>name
	Collider
>component_count
	1
>transform
>position
	0, 5, 0
>rotation
	0, 0, 0
>scale
	1.3, 3, 1
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	0
>cKinematic
	1
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	WeaponCollider
>component_count
	2
>transform
>position
	0, 5.2, 6.8
>rotation
	0, 0, 0
>scale
	5.1, 2.9, 4.6
>BoxCollider
>enabled
	1
>cTrigger
	1
>cStatic
	0
>cKinematic
	0
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0
>PlayerWeapon
>enabled
	1

>entity
>childs
	0
>name
	ComboWeaponCollider
>component_count
	2
>transform
>position
	0, 4.6, 0
>rotation
	0, 0, 0
>scale
	10, 3.9, 10
>BoxCollider
>enabled
	0
>cTrigger
	1
>cStatic
	0
>cKinematic
	0
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	1
>rotY
	1
>rotZ
	1
>PlayerWeapon
>enabled
	1

>entity
>childs
	0
>name
	Score
>component_count
	4
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Panel
>enabled
	1
>position
	0.2, 67.7
>scale
	105, 45.7
>tint
	0, 0, 0, 0.5
>Text
>enabled
	1
>position
	4, 67.5
>scale
	3, 1
>tint
	1, 0.929204, 0, 1
>text
	x99
>Panel
>enabled
	1
>position
	0, 81
>scale
	165, 40
>tint
	0, 0, 0, 0.5
>Text
>enabled
	1
>position
	4, 80
>scale
	3, 1
>tint
	1, 1, 1, 1
>text
	99999

>entity
>childs
	0
>name
	Sounds
>component_count
	9
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\hero\FootstepHero.mp3
>loop
	1
>volume
	4
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\hero\Hero_Hit_1.mp3
>loop
	0
>volume
	2.5
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\hero\Hero_Hit_2.wav
>loop
	0
>volume
	2.5
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\swords\Hit_Miss.mp3
>loop
	0
>volume
	2.5
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\hero\Hero_Hurt.mp3
>loop
	0
>volume
	1.5
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\hero\Hero_Death.mp3
>loop
	0
>volume
	3
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\hero\Hero_Dash.mp3
>loop
	0
>volume
	1
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\Pick_Heart.wav
>loop
	0
>volume
	1.5
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\We_re_Back_to_Fight.mp3
>loop
	1
>volume
	3

>entity
>childs
	4
>name
	GolemClone
>component_count
	5
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Golem
>enabled
	0
>attackRange
	20
>chargeTime
	2.5
>attackTime
	0.5
>riseLegTime
	1
>hurtEffectTime
	0.1
>hurtEffectTint
	0.7, 0.5, 0, 1
>deathTime
	4
>EnemyStats
>enabled
	0
>baseLife
	12
>baseDamage
	6
>baseSpeed
	450
>baseAttackSpeed
	0.333
>baseScore
	20
>increaseRatio
	0.5
>SphereCollider
>enabled
	0
>cTrigger
	0
>cStatic
	0
>cKinematic
	0
>cDynamic
	1
>posX
	0
>posY
	0
>posZ
	0
>rotX
	1
>rotY
	1
>rotZ
	1
>Life
>enabled
	0
>life
	12
>invincibilityTime
	0.75
>EnemyMeshRotation
>enabled
	0

>entity
>childs
	0
>name
	GFX
>component_count
	1
>transform
>position
	0, -0.3, 0
>rotation
	0, 0, 0
>scale
	0.08, 0.08, 0.08
>AnimatedDraw
>enabled
	0
>meshPath
	game/assets/Models/Golem/Golem.fbx
>texturePath
	game/assets/Textures/Golem_Texture.png
>skmodel.material.ambient
	0.8, 0.8, 0.8
>skmodel.material.diffuse
	1, 1, 1
>skmodel.material.specular
	1, 1, 1
>skmodel.material.emissive
	0, 0, 0
>skmodel.material.tint
	0, 0, 0, 1
>skmodel.material.shininess
	32
>skmodel
>count
	0
>animGraph
>count
	5
>file
	game/assets/Models/Golem/Golem_Idle.fbx
>file
	game/assets/Models/Golem/Golem_Death.fbx
>file
	game/assets/Models/Golem/Golem_Stomp.fbx
>file
	game/assets/Models/Golem/Golem_Walk.fbx
>file
	game/assets/Models/Golem/Golem_Hit.fbx

>entity
>childs
	0
>name
	Collider
>component_count
	1
>transform
>position
	0, 7.4, 0
>rotation
	0, 0, 0
>scale
	4.3, 5.73333, 2.29333
>BoxCollider
>enabled
	0
>cTrigger
	0
>cStatic
	0
>cKinematic
	1
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	AttackGFX
>component_count
	1
>transform
>position
	0, 0.2, 0
>rotation
	1.5708, 0, 0
>scale
	7.5, 7.5, 7.5
>StaticDraw
>enabled
	0
>meshPath
	ProceduralQuad
>texturePath
	game/assets/Textures/Golem_Stomp.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	0, 0, 0
>model.material.emissive
	1, 1, 1
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Sounds
>component_count
	5
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Hit.mp3
>loop
	0
>volume
	30
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Roar.mp3
>loop
	0
>volume
	50
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Walk.mp3
>loop
	1
>volume
	30
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\swords\Hit_Golem.mp3
>loop
	0
>volume
	10
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Death.mp3
>loop
	0
>volume
	50

>entity
>childs
	0
>name
	GameOverCanvas
>component_count
	14
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Panel
>enabled
	0
>position
	10.8, 15.6
>scale
	594.3, 299
>tint
	0.190265, 0.18774, 0.18774, 0.712
>Text
>enabled
	0
>position
	27.6, 19.9
>scale
	4.2, 2
>tint
	1, 0, 0, 1
>text
	GAME OVER
>Text
>enabled
	0
>position
	32, 36.7
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	level : 0 x 1000
>Text
>enabled
	0
>position
	28.2, 43
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	skeleton : 0 x 5
>Text
>enabled
	0
>position
	30.4, 48
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	golem : 0 x 20
>Text
>enabled
	0
>position
	22.9, 53
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	combo bonus : 
>Text
>enabled
	0
>position
	55, 37
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	0
>Text
>enabled
	0
>position
	55, 43
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	0
>Text
>enabled
	0
>position
	55, 48
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	0
>Text
>enabled
	0
>position
	55, 53
>scale
	1.3, 1
>tint
	1, 1, 1, 1
>text
	0
>Text
>enabled
	0
>position
	38, 61
>scale
	1.7, 1
>tint
	1, 0.97071, 0.699115, 1
>text
	Result : 0
>Button
>enabled
	0
>position
	30, 70
>scale
	300, 45
>tint
	0, 0, 0, 1
>text
	Menu
>textScale
	1.4
>idle
	0, 0, 0, 1
>hovered
	0.477876, 0.477876, 0.477876, 1
>activated
	0.336283, 0.327355, 0.327355, 1
>GameOverTrigger
>enabled
	0
>ImagePanel
>enabled
	0
>position
	66, 36.8
>scale
	120, 150
>tint
	1, 1, 1, 1
>imageFlip
	0
>Texture
	game/assets/Textures/RankingLetters_D.png

>entity
>childs
	2
>name
	Portal
>component_count
	2
>transform
>position
	-69.2385, -57.4549, -73.4458
>rotation
	0, -0.841248, 0
>scale
	7, 8, 1.8
>Portal
>enabled
	0
>BoxCollider
>enabled
	0
>cTrigger
	1
>cStatic
	0
>cKinematic
	0
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	DoorGFX
>component_count
	1
>transform
>position
	0, -1, 0
>rotation
	0, 0, 0
>scale
	0.35, 0.35, 1
>StaticDraw
>enabled
	1
>meshPath
	game/assets/Models/Door.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_04.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	PortalGFX
>component_count
	1
>transform
>position
	0, -0.154, 0
>rotation
	0, 0, 0
>scale
	0.852, 0.836, 0.6
>StaticDraw
>enabled
	0
>meshPath
	ProceduralCube
>texturePath
	game/assets/Textures/Portal.jpg
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Forest
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	0.05, 0.05, 0.05
>StaticDraw
>enabled
	1
>meshPath
	game/assets/Maps/Forest_map.fbx
>texturePath
	game/assets/Textures/Forest_Texture.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	0, 0, 0
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	GroundCollider
>component_count
	1
>transform
>position
	29, -65.522, 16.2981
>rotation
	-0, -0, -0
>scale
	165.062, 1, 159.648
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Wall1
>component_count
	1
>transform
>position
	25.0362, -60, 140.28
>rotation
	-0, -0, -0
>scale
	146.022, 12.6654, 1
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Wall2
>component_count
	1
>transform
>position
	138.326, -60, 4.47085
>rotation
	-0, -1.55334, -0
>scale
	146.022, 12.6654, 1
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Wall3
>component_count
	1
>transform
>position
	25.0362, -60, -116.383
>rotation
	-0, -0, -0
>scale
	146.022, 12.6654, 1
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Wall4
>component_count
	1
>transform
>position
	-108.804, -60, 4.46984
>rotation
	-0, 1.55334, -0
>scale
	146.022, 12.6654, 1
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Obstacle1
>component_count
	1
>transform
>position
	94.2741, -59.1465, 110.739
>rotation
	0, 0, 0
>scale
	6.5, 6.8, 6.3
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Obstacle2
>component_count
	1
>transform
>position
	47.5349, -63.3176, 110.559
>rotation
	0, 0, 0
>scale
	2.92064, 1.36403, 3.11
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Spawner_0
>component_count
	1
>transform
>position
	-99.1335, -58.0039, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Spawner
>enabled
	1
>spawnTime
	4
>skeletonRatio
	60
>golemRatio
	10
>nothingRatio
	30

>entity
>childs
	0
>name
	Obstacle3
>component_count
	1
>transform
>position
	154.684, -58.5841, 117.646
>rotation
	0, 0, 0
>scale
	2.021, 8.864, 2.81
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Obstacle4
>component_count
	1
>transform
>position
	-29.8454, -62.7966, 106.928
>rotation
	0, 0, 0
>scale
	3.021, 3.164, 6.61
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Obstacle5
>component_count
	1
>transform
>position
	-72.0501, -58.886, 64.9801
>rotation
	0, 0, 0
>scale
	3.221, 8.364, 3.61
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	2
>name
	Portal_4
>component_count
	2
>transform
>position
	93.6813, -57.4549, -96.9792
>rotation
	0, 0.015708, 0
>scale
	7, 8, 1.8
>Portal
>enabled
	0
>BoxCollider
>enabled
	0
>cTrigger
	1
>cStatic
	0
>cKinematic
	0
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	DoorGFX
>component_count
	1
>transform
>position
	0, -1, 0
>rotation
	0, 0, 0
>scale
	0.35, 0.35, 1
>StaticDraw
>enabled
	1
>meshPath
	game/assets/Models/Door.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_04.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	PortalGFX
>component_count
	1
>transform
>position
	0, -0.154, 0
>rotation
	0, 0, 0
>scale
	0.852, 0.836, 0.6
>StaticDraw
>enabled
	0
>meshPath
	ProceduralCube
>texturePath
	game/assets/Textures/Portal.jpg
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Obstacle6
>component_count
	1
>transform
>position
	-17.8099, -60.9105, -5.83033
>rotation
	0, 0, 0
>scale
	5.121, 4.064, 3.91
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Spawner_4
>component_count
	1
>transform
>position
	-73.5472, -58.0041, -38.7405
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Spawner
>enabled
	1
>spawnTime
	4
>skeletonRatio
	60
>golemRatio
	10
>nothingRatio
	30

>entity
>childs
	0
>name
	Obstacle7
>component_count
	1
>transform
>position
	21.1865, -64.6691, 27.8004
>rotation
	0, 0.942478, 0
>scale
	1.945, 2.864, 7.219
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Obstacle8
>component_count
	1
>transform
>position
	54.4677, -60.3009, 41.9672
>rotation
	0, 0, 0
>scale
	3.221, 3.864, 3.61
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Obstacle9
>component_count
	1
>transform
>position
	47.5434, -61.0241, -60.6546
>rotation
	0, 0, 0
>scale
	3.121, 2.664, 3.41
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	0
>name
	Obstacle10
>component_count
	1
>transform
>position
	110.101, -58.5305, -22.8437
>rotation
	0, 0, 0
>scale
	7.56925, 5.464, 5.35449
>BoxCollider
>enabled
	1
>cTrigger
	0
>cStatic
	1
>cKinematic
	0
>cDynamic
	0
>posX
	1
>posY
	1
>posZ
	1
>rotX
	1
>rotY
	1
>rotZ
	1

>entity
>childs
	2
>name
	Portal_2
>component_count
	2
>transform
>position
	-70.3118, -57.4549, 93.0738
>rotation
	0, 1.10305, 0
>scale
	7, 8, 1.8
>Portal
>enabled
	0
>BoxCollider
>enabled
	0
>cTrigger
	1
>cStatic
	0
>cKinematic
	0
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	DoorGFX
>component_count
	1
>transform
>position
	0, -1, 0
>rotation
	0, 0, 0
>scale
	0.35, 0.35, 1
>StaticDraw
>enabled
	1
>meshPath
	game/assets/Models/Door.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_04.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	PortalGFX
>component_count
	1
>transform
>position
	0, -0.154, 0
>rotation
	0, 0, 0
>scale
	0.852, 0.836, 0.6
>StaticDraw
>enabled
	0
>meshPath
	ProceduralCube
>texturePath
	game/assets/Textures/Portal.jpg
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	2
>name
	Portal_3
>component_count
	2
>transform
>position
	97.8379, -57.4549, 81.2191
>rotation
	0, -1.23569, 0
>scale
	7, 8, 1.8
>Portal
>enabled
	0
>BoxCollider
>enabled
	0
>cTrigger
	1
>cStatic
	0
>cKinematic
	0
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	DoorGFX
>component_count
	1
>transform
>position
	0, -1, 0
>rotation
	0, 0, 0
>scale
	0.35, 0.35, 1
>StaticDraw
>enabled
	1
>meshPath
	game/assets/Models/Door.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_04.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	PortalGFX
>component_count
	1
>transform
>position
	0, -0.154, 0
>rotation
	0, 0, 0
>scale
	0.852, 0.836, 0.6
>StaticDraw
>enabled
	0
>meshPath
	ProceduralCube
>texturePath
	game/assets/Textures/Portal.jpg
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Spawner_1
>component_count
	1
>transform
>position
	-74.4201, -54.7675, 125.334
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Spawner
>enabled
	1
>spawnTime
	4
>skeletonRatio
	60
>golemRatio
	10
>nothingRatio
	30

>entity
>childs
	0
>name
	Spawner_2
>component_count
	1
>transform
>position
	99.3119, -58.004, -70.8572
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Spawner
>enabled
	1
>spawnTime
	4
>skeletonRatio
	60
>golemRatio
	10
>nothingRatio
	30

>entity
>childs
	0
>name
	Spawner_3
>component_count
	1
>transform
>position
	115.11, -58.004, 100.92
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Spawner
>enabled
	1
>spawnTime
	4
>skeletonRatio
	60
>golemRatio
	10
>nothingRatio
	30
