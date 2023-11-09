# IcosphereGrid
This is a small prototype project to try generating and utilizing a triangulated grid on a sphere. It uses an icosahedron (D20) as a base, and then repeatedly subdivides each triangular surface into 4 subtriangles, Sierpinski style.

Each vertex is normalized to the sphere's surface, and since each vertex's normal is set to be its position in relation to the centre of the sphere, the grid will be displayed as an actual sphere. 

Using the grid I generate a simple random map, with 4 tile types (grass, mountain, water, hole), and procedurally generate the mesh describing these tiles. 

![Generated Icosphere](/Images/SpherePicture.png)


## 3D Mathematics Course - FutureGames
I have chosen to use this project as my base for a set of small assignments for a 3D Mathematics course. The descriptions of each assignment follow:

The AssignmentTestActor ([.h](/Source/IcosphereGrid/Public/MathCourseSpecifics/AssignmentTestActor.h) and [.cpp](/Source/IcosphereGrid/Private/MathCourseSpecifics/AssignmentTestActor.cpp)) is used to test the functionality of these assignments in the editor.

### Assignment 1: State and Context
Here we pick 2 random nodes on the globe, and display the relationships ([.h](/Source/IcosphereGrid/Public/TriangleNodeRelations.h) and [.cpp](/Source/IcosphereGrid/Private/TriangleNodeRelations.cpp)) between them as a bitmask. In the image below, for example, the node marked by the blue sphere is to the North and West of the node marked by the red sphere. 

![AssignmentOne](/Images/AssignmentOne.png)

All available flags are below. "Has Free Path" means that, given an array of travelable tile types, there is a travelable path between the two nodes. "Same Region" means that there is a path between the two nodes that never touch a different region. Only 2 nodes with the same tile type can be part of the same region.
```
UENUM(BlueprintType, meta = (Bitflags))
enum class ETriangleNodeRelations
{
	TNR_NULL			UMETA(Hidden),
	TNR_NORTH			UMETA(DisplayName = "North"),
	TNR_SOUTH			UMETA(DisplayName = "South"),
	TNR_WEST			UMETA(DisplayName = "West"),
	TNR_EAST			UMETA(DisplayName = "East"),
	TNR_SAMEREGION		UMETA(DisplayName = "Same Region"),
	TNR_NEIGHBOUR		UMETA(DisplayName = "Neighbour"),
	TNR_HASFREEPATH		UMETA(DisplayName = "Has Free Path")
};
```

The region checks uses a simple floodfill algorithm. 

The most interesting part of this work was to check whether a node was to the west or east of another node. It's complicated, since west/east is different depending on where you are. My solution uses the cross product of the tiles normal vector and the global up vector, and the dot product of the cross product and the nodes' position delta. [See it here](/Source/IcosphereGrid/Private/TriangleNodeRelations.cpp#L20C1-L38C2)


### Assignment 2: Interpolation
Once again we pick 2 random nodes on the globe. This time, I've created both positional and rotational spherical interpolation ([.h](/Source/IcosphereGrid/Public/SphericalMathHelpers.h) and [.cpp](/Source/IcosphereGrid/Private/SphericalMathHelpers.cpp)) between these points, allowing an actor to travel around the globe as if in orbit.

![AssignmentTwo](/Images/AssignmentTwo.png)

Interpolating the positions are relatively simple. I just interpolate between the start and end vector, and manually set the vector's length. Note that this is not a true spherical linear interpolation, as the angular velocity is not constant. The movement is slower near the start and end point, giving the effect of a controlled landing.

For the rotational interpolation, things are trickier. Determining the Up vector is easy, it's just our position, relative to the centre of the globe. By taking the cross product between the vector between our start and end location and the Up vector, we can determine our Right vector. Finally, we cross our Up and Right vectors for the Forward vector, and now we can create a rotation using the three axes.
