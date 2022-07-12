>light
>position
	0.619, 0.46, -0.344, 0
>ambient
	0.3, 0.3, 0.3
>diffuse
	1, 1, 1
>specular
	0.5, 0.5, 0.5
>toonShading
	1
>stepAmount
	8
>stepSize
	0.088
>specSize
	0.049
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
	Camera
>component_count
	1
>transform
>position
	24, -10.5, 0
>rotation
	-0.216421, -1.55683, 0
>scale
	1, 1, 1
>Camera
>enabled
	1
>near
	1
>far
	300
>fovY
	90

>entity
>childs
	0
>name
	Ground
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
	game/assets/Maps/Menu_map.fbx
>texturePath
	game/assets/Textures/Forest_Texture.png
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
	Hero
>component_count
	1
>transform
>position
	11.725, -19.648, 10.875
>rotation
	0, -3.49066, 0
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
	1.5708, 0, 0
>scale
	75, 75, 75
>boneID
	31
>socketMesh
	game/assets/Models/Hero/HeroSword.fbx
>socketTexture
	game/assets/Textures/Dungeons_Texture_01.png
>transform
>position
	0, 0, 0
>rotation
	-1.5708, 3.14159, 0
>scale
	75, 75, 75
>boneID
	16
>socketMesh
	game/assets/Models/Hero/HeroShield.fbx
>socketTexture
	game/assets/Textures/Dungeons_Texture_01.png
>animGraph
>count
	1
>file
	game/assets/Models/Hero/Hero_Idle_2.fbx

>entity
>childs
	0
>name
	MenuCanvas
>component_count
	11
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
	38.7, 15.6
>scale
	430, 308.1
>tint
	0, 0, 0, 0.5
>Button
>enabled
	1
>position
	50, 34.8
>scale
	270, 53
>tint
	0, 0, 0, 1
>text
	Play
>textScale
	1.5
>idle
	0.8, 0, 0, 1
>hovered
	0.8, 0.4, 0.4, 1
>activated
	1, 0, 0, 1
>Button
>enabled
	1
>position
	50, 51.5
>scale
	270, 53
>tint
	0, 0, 0, 1
>text
	Physic demo
>textScale
	1.5
>idle
	0.8, 0, 0, 1
>hovered
	0.8, 0.4, 0.4, 1
>activated
	1, 0, 0, 1
>Button
>enabled
	1
>position
	50, 67.8
>scale
	270, 53
>tint
	0, 0, 0, 1
>text
	Quit
>textScale
	1.5
>idle
	0, 0, 0, 1
>hovered
	0.314159, 0.314159, 0.314159, 1
>activated
	0.7, 0.7, 0.7, 1
>MenuScript
>enabled
	1
>LevelSelector
>enabled
	0
>ImagePanel
>enabled
	1
>position
	45.2, 17.2
>scale
	350, 70
>tint
	1, 1, 1, 1
>imageFlip
	0
>Texture
	game/assets/Textures/FrakkarenaWhite.png
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\Fire_and_Thunder.mp3
>loop
	1
>volume
	3
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\Warrior_Rising.mp3
>loop
	1
>volume
	3
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\We_re_Back_to_Fight.mp3
>loop
	1
>volume
	3
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\Menu_Button.wav
>loop
	0
>volume
	1
