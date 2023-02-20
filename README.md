# SkiFree-3D

This is the implementation of a 3D game where you control a skier on a slope who must collect gifts and avoid various obstacles. The game is an endless type, with the player's objective being to obtain as many points as possible.

### Scene construction

#### ➢ Game field

The character moves in the world (skiing on the slope) and thus modifies its global coordinates in the world. The obstacles and gifts remain fixed in the scene.

The field is created using a large square (quad) on which a repeating texture is applied. The square is drawn at the player's position.

The illusion of the field moving under the character is given by modifying the texture coordinates of the field. This modification consists of a displacement applied to all UVs of the field mesh.


#### ➢ Player

The player is made up of at least 3 parallelepipeds (body and two skis). The meshes that make up the player are created so that the player object can be translated and rotated as a single unit.


#### ➢ Obstacles

During the game, the player can encounter 3 types of obstacles on the slope:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;* Trees, formed from at least 2 meshes. <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;* Stones, formed from at least 3 meshes. <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;* Light poles, made up of 2 meshes. <br>

A texture is applied to each primitive used in creating the objects.


#### ➢ Collectibles (gifts)

The collectible object is imagined as a gift that the player can collect. The object consists of a single cube mesh. A texture has been applied to the object. <br><br>


### Player control (input)

The skier continuously moves down the slope at a constant speed. <br><br>

### Camera positioning

The camera is a perspective type and follows the player.

### Generation of obstacles and collectible elements <br><br>

The 3D objects representing obstacles and collectibles are dynamically generated during the game as follows:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;* They are not instantiated and deleted outside the game's viewport. <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;* The algorithm for generating obstacles places the 3D objects on the field so that the player has to avoid them (instantiation position randomized within certain limits). <br><br>

### Collisions

Collision between the player and obstacles and between the player and gifts is achieved with the sphere-sphere method.

Sphere-Sphere collision: https://developer.mozilla.org/enUS/docs/Games/Techniques/3D_collision_detection
