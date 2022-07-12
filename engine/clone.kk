
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
	game/assets/Sounds/skeleton/SkeletonTeeth.wav
>loop
	0
>volume
	10
>Sound
>enabled
	1
>soundPath
	game/assets/Sounds/skeleton/FootstepSkel.mp3
>loop
	1
>volume
	70
>Sound
>enabled
	1
>soundPath
	game/assets/Sounds/swords/Hit_Skeleton.mp3
>loop
	0
>volume
	60
>Sound
>enabled
	1
>soundPath
	game/assets/Sounds/skeleton/Skel_Death.mp3
>loop
	0
>volume
	75
