# Formation movement

## Introduction
In many RTS games with a 'battle' mechanic, the player needs to be able to position units easily around the battle map. One game series that does this well is the Total War series. While playing Rome Total War, I was fascinated by this seemingly easy, but in practice quite tricky issue: how does one create a valid path with enough clearance to obstacles for a formation of soldiers?

## Demo
Below is a video demonstration of the formation movement prototype (UE4.22 project):

_TODO_

## Approach

The approach can be explained in three steps:
1. Planning a path;
2. Following the path;
3. Assigning the optimal formation positions.

### 1. Planning a path
Let's first start with planning a path for a group of soldiers. My approach for this was as follows:
1. Create a signed distance field (SDF). In my implementation, this is a grid that, per cell, annotates the distance to the nearest obstacle.
2. Calculate the required clearance, by drawing an imaginary circle around the desired formation. (Note: this can be improved by calculating a path for a rectangular shape, see Section Todo)
3. Plan a path using the A* algorithm and the SDF grid. The output is a path that always had sufficient space through obstacles.

### 2. Following the path
Now that the path is created, the formation must actually follow this path. I approached it the following way:
1. Create a "Formation Commander". This is an invisible agent that moves along the path.
2. Let the actual soldiers follow their position in the formation, relative to the Formation Commander. We know they have space around the formation commander, because the path 

The position in the formation can be seen as the 'attraction point', aka a the point to which the agent is pushed. For this demo, I used the built-in navmesh system to let the soldiers follow their relative position.

### 3. Assigning the optimal formation positions
Picture the situation where we want to move an entire formation just a few meters to their back. If soldiers had a static position in the formation (e.g. 2 meters left of the formation commander), then the entire formation would have to rotate 180 degrees and back. While playing around with this prototype, I noticed this makes the movement very clumsy and slow.

Ideally, whenever we issue a movement command, each soldier in the formation moves to the relative position that is closest to them. More formally, we want to minimize the total combined distance from an agent's current position to their new position in the formation.

This is a combinatorial optimization problem that can be solved with the <i>Hungarian algorithm</i> (HA). We 'feed' the HA the current soldier positions and a vector of possible relative positions in the formation. The HA will assign a relative position in the formation that minimizes the total distance. In practice, this means that formations require less time 'regrouping', agents have to avoid less collisions with other formation members and movement becomes more smooth in general.

## Future work
More advanced techniques can be applied to improve this prototype.
### Path planning. 
If we have very wide or very long formations, a circle around the formation is not a good estimate anymore for the required clearance. One improvement would thus be to allow path planning for rectangular shapes. This requires the path planner not only to take into account the position, but also the orientation of the formation.

This is not a new concept and existing algorithms exist. Personally, I am currently working on a crowd simulation engine (see: github.com/Felixbuenen/ECMGenerator) that implements the Explicit Corridor Map (ECM) navigation mesh. 
The ECM allows us to efficiently query at any point on the map how far the nearest obstacle is. With this information, we could more easily create paths for rectangular shapes.

### Re-formation events
A wide formation cannot fit on a narrow path. A nice addition to this prototype would be to temporarily change the formation such that it fits through narrow gaps. 

### Optimization of the Hungarian Algorithm. 
The downside of the HA is that its complexity is O(n^2). For larger groups of agents, this introduces a serious performance bottleneck. To use this prototype in larger scenarios, optimization would be required.
