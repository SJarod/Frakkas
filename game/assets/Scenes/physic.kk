>light
>position
	-0.333, 1, 1, 0
>ambient
	0.3, 0.3, 0.3
>diffuse
	1, 1, 1
>specular
	0.5, 0.5, 0.5
>toonShading
	1
>stepAmount
	5
>stepSize
	0.1
>specSize
	0.1
>shadow
	1
>shadowPCF
	0
>shadowDistance
	30

>entity
>childs
	0
>name
	Camera
>component_count
	2
>transform
>position
	4.60383, -1.02, 2.02149
>rotation
	-0.523599, 1.5708, 0
>scale
	1, 1, 1
>Camera
>enabled
	1
>near
	1
>far
	1000
>fovY
	90
>FollowCamera
>enabled
	1
>offset
	-8, 6, 0
>lerpFactor
	0.1

>entity
>childs
	0
>name
	Ground
>component_count
	2
>transform
>position
	0, -7.3, 0
>rotation
	0, 0, 0
>scale
	40, 3.8, 40
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	white
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
	Player
>component_count
	3
>transform
>position
	4.60384, -0.72, -2.97851
>rotation
	1.33646e-08, 0.00709875, 1.55786e-06
>scale
	1, 1, 1
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.437091, 0.0170334, 0.641593, 1
>model.material.shininess
	32
>PhysicPlayer
>enabled
	1
>speed
	900
>jumpForce
	1200
>gravityFactor
	3

>entity
>childs
	0
>name
	Wall_0
>component_count
	1
>transform
>position
	-40, 10, 0
>rotation
	0, 0, 0
>scale
	1, 20, 40
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
	Wall_1
>component_count
	2
>transform
>position
	40, 10, 0
>rotation
	0, 0, 0
>scale
	1, 20, 40
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.800885, 0.731578, 0.499667, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Wall_2
>component_count
	1
>transform
>position
	0, 10, 40
>rotation
	0, 1.55334, 0
>scale
	1, 20, 40
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
	Wall_3
>component_count
	1
>transform
>position
	0, 10, -40
>rotation
	0, 1.55334, 0
>scale
	1, 20, 40
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
	Roof
>component_count
	1
>transform
>position
	0, 30, 0
>rotation
	0, 0, 0
>scale
	40, 1, 40
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
	RigidCube_0
>component_count
	2
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	Platform_0
>component_count
	2
>transform
>position
	0, -2.97785, -29.9177
>rotation
	0, 0, 0
>scale
	5, 1, 5
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	Platform_1
>component_count
	2
>transform
>position
	35.0581, 2.95551, -18.9367
>rotation
	0, 0, 0
>scale
	4, 1, 5
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	Platform_2
>component_count
	2
>transform
>position
	35.0581, 9.55096, -3.55404
>rotation
	0, 0, 0
>scale
	4, 1, 5
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	Platform_3
>component_count
	2
>transform
>position
	35.0581, 15.8977, 22.1137
>rotation
	0, 0, 0
>scale
	3.95444, 1, 10.062
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	Platform_4
>component_count
	2
>transform
>position
	35.0581, 17.2682, -24.0214
>rotation
	0, 0, 0
>scale
	4, 1, 5
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	Platform_5
>component_count
	2
>transform
>position
	0, 15, 0
>rotation
	0, 0, 0
>scale
	5, 1, 5
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	Platform_6
>component_count
	2
>transform
>position
	-33.3774, 2.9443, 26.783
>rotation
	0, 0, 0
>scale
	2, 7, 2
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.274336, 0.274336, 0.274336, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Platform_7
>component_count
	2
>transform
>position
	-15.5208, 2.9443, 26.783
>rotation
	0, 0, 0
>scale
	2, 7, 2
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.274336, 0.274336, 0.274336, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Platform_8
>component_count
	2
>transform
>position
	-33.3774, 2.9443, -24.2682
>rotation
	0, 0, 0
>scale
	2, 7, 2
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.274336, 0.274336, 0.274336, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Platform_9
>component_count
	2
>transform
>position
	-15.5208, 2.9443, -25.1564
>rotation
	0, 0, 0
>scale
	2, 7, 2
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.274336, 0.274336, 0.274336, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Slope_0
>component_count
	2
>transform
>position
	19.3, -0.8, -19.3
>rotation
	0, 0, -0.307178
>scale
	12.9, 1, 5.1
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	1, 0.690265, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Slope_1
>component_count
	2
>transform
>position
	36.2, 12.4, 5.4
>rotation
	0.467748, 0, 0
>scale
	5, 1, 7.7
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	1, 0.690265, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Slope_2
>component_count
	2
>transform
>position
	-24.7423, 3.24867, -31.8431
>rotation
	0.949458, 0, 0
>scale
	11.2, 1, 9
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	1, 0.690265, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Slope_3
>component_count
	2
>transform
>position
	-25, 3.24867, 34.1
>rotation
	-0.959931, 0, 0
>scale
	11.2, 1, 9
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	1, 0.690265, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	RigidSphere_3
>component_count
	2
>transform
>position
	36.3923, 11.1768, 16.4318
>rotation
	0, 0, 0
>scale
	1, 1, 1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	red
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
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	RigidCube_1
>component_count
	2
>transform
>position
	6.22955, 0, 17.1146
>rotation
	0, 0, 0
>scale
	1, 1, 1
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_2
>component_count
	2
>transform
>position
	34.7928, 7.62775, -16.4049
>rotation
	0, 0, 0
>scale
	1, 1, 1
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_5
>component_count
	2
>transform
>position
	36.4619, 3.4022, 25.783
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_3
>component_count
	2
>transform
>position
	36.4619, -1.12682, 12.0728
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_4
>component_count
	2
>transform
>position
	36.4619, 3.40199, 12.0728
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_5
>component_count
	2
>transform
>position
	36.4619, 7.92827, 12.0728
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_5
>component_count
	2
>transform
>position
	36.4619, 7.92827, 16.6532
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidSphere_1
>component_count
	3
>transform
>position
	0, 18.209, 0
>rotation
	0, 0, 0
>scale
	2, 2, 2
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	black
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
	0
>rotY
	0
>rotZ
	0
>PhysicBallObject
>enabled
	1

>entity
>childs
	0
>name
	RigidCube_5
>component_count
	2
>transform
>position
	36.4619, 3.04624, 16.7203
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_5
>component_count
	2
>transform
>position
	36.4619, 7.92827, 21.3184
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_5
>component_count
	2
>transform
>position
	36.4619, 7.92827, 25.8761
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	RigidCube_5
>component_count
	2
>transform
>position
	36.4619, -1.12007, 25.8112
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	red
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
	FootGround
>component_count
	2
>transform
>position
	-24.4628, -2.74817, 0.785322
>rotation
	0, 0, 0
>scale
	11.8743, 1, 24.3694
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	green
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
	RigidSphere_0
>component_count
	2
>transform
>position
	0, 3.14516, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	red
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
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	RigidSphere_2
>component_count
	3
>transform
>position
	34.4213, 21.7061, -24.1025
>rotation
	0, 0, 0
>scale
	3, 3, 3
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	black
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
	0
>rotY
	0
>rotZ
	0
>PhysicBallObject
>enabled
	1

>entity
>childs
	0
>name
	BallTrigger
>component_count
	3
>transform
>position
	-0.122023, 0, -29.7606
>rotation
	0, 0, 0
>scale
	1, 1, 1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	transparent
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
>PhysicBallTrigger
>enabled
	1

>entity
>childs
	0
>name
	GoalTrigger_2
>component_count
	3
>transform
>position
	-24.5878, 3.70365, -24.3296
>rotation
	0, 0, 0
>scale
	7.9, 6.2, 1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.982301, 0.204284, 0.720667, 0.455
>model.material.shininess
	32
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
>PhysicGoalTrigger
>enabled
	1

>entity
>childs
	0
>name
	GoalTrigger_1
>component_count
	3
>transform
>position
	-24.5878, 3.70365, 25.912
>rotation
	0, 0, 0
>scale
	7.9, 6.2, 1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	0.982301, 0.204284, 0.720667, 0.455
>model.material.shininess
	32
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
>PhysicGoalTrigger
>enabled
	1

>entity
>childs
	0
>name
	RigidSphere_4
>component_count
	3
>transform
>position
	-23.9206, 0.583286, 1.13372
>rotation
	0, 0, 0
>scale
	1, 1, 1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	black
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
	0
>rotY
	0
>rotZ
	0
>PhysicBallObject
>enabled
	1

>entity
>childs
	0
>name
	Slope_4
>component_count
	2
>transform
>position
	-0.518108, -4.70016, -19.3
>rotation
	0, 0, 0.148353
>scale
	12.9, 1, 5.1
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.emissive
	0, 0, 0
>model.material.tint
	1, 0.690265, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Border_1
>component_count
	2
>transform
>position
	-11.5, -2.57787, 6.08242
>rotation
	0, 0, 0
>scale
	1, 2.3, 19.1
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	Border_2
>component_count
	2
>transform
>position
	-36.9445, -2.57787, 0.751944
>rotation
	0, 0, 0
>scale
	1, 2.3, 21.9239
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	blue
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
	RigidCube_Ball
>component_count
	3
>transform
>position
	36.4619, -1.18867, 21.3184
>rotation
	0, 0, 0
>scale
	2, 2, 2
>BoxCollider
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
	0
>rotY
	0
>rotZ
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	black
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
>PhysicBallObject
>enabled
	1

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
