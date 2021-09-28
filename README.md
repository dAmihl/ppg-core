# PPG - Procedural Puzzle Generator

My (abandoned) bachelor's project experimenting with procedurally generating puzzles for video games.

## Disclaimer:

In the rare case of you thinking to use this anywhere near production: please don't.
This is pure experimental.

__This is the core repository. For extending projects please refer to these links:__

[ppg-yaml; YAML extension for the use with ppg-core](https://github.com/dAmihl/ppg-yaml)

[ppg-visualizer](https://github.com/dAmihl/ppg-visualizer)

[gdppg; A Godot module for ppg-yaml](https://github.com/dAmihl/gdppg)

## Examples:

I have also created a very small game in Godot Engine using this system which can be found here: [simplePPG](https://github.com/dAmihl/simplePPG)

## Build:

* Install Premake5
* Run e.g. `premake5 vs2019`
* Open project file

## Generation

The newly used experimental generation method is loosely based on / inspired by [Wave function collapse](https://github.com/mxgmn/WaveFunctionCollapse).
We can assume that in the beginning, every node is connected to every other node. Through constraints the entropy of every possible relation is reduced. When a relation is picked randomly (and the "wave" for this relation "collapsed"), the constraints are inherited / passed to the related nodes, which further reduces the entropy of the remaining relations.

1) Generate a number of nodes based on Context. A node consists of an Object and a Goal State

2) Generate a adjacency matrix for the nodes. The matrix can have these values. FREE, USED, TUSED, NOT, NTNOT

   * FREE describes a free cell (marked as _)
   * USED means that a relation X<Y was created in cell (x,y) (marked as u)
   * TUSED means that this cell describes a relation which is already satisfied by another transitive relationship (marked as tu)
   * NOT means that this cell must not be used as relation (marked as *)
   * NTNOT means that this cell must not be used as relation, and will not be copied to transitive relationships (marked as #)

3) Initialize the matrix with base constraints, i.e. identity:

   ```markdown
    | A | B | C | D |
    |---|---|---|---|
   A| * |   |   |   |
   B|   | * |   |   |
   C|   |   | * |   |
   D|   |   |   | * |
   ```

4. Next, set the constraints defined by rules of the context. Currently there are 4 rule types:

   * BEFORE(X,Y) means, that X can only occur BEFORE Y. This generally means, that a relation of Y -> X is not allowed.
   * AFTER(X,Y) means, that X can only occur AFTER Y; X -> Y is not allowed
   * STRICT AFTER(X,Y) means, that X ONLY occurs DIRECTLY after Y. This means that any relation for a node W != Y with W -> X is not allowed. Transitivity, e.g. Y -> W -> X is also not allowed.
   * STRICT BEFORE(X,Y) means that X ONLY occurs DIRECTLY before Y. So any relation with a node W != Y with X -> W is not allowed. Transitivity, e.g. X -> W -> Y is therefore also not allowed.

   If we take above example and add the Rules BEFORE(C, B) and STRICT_BEFORE(A,B) we get this matrix:

   ```markdown
    | A | B | C | D |
    |---|---|---|---|
   A| * | * |   |   |
   B|   | * |   |   |
   C| # | * | * |   |
   D| # |   |   | * |
   ```

   The NOT in (B,A) comes from STRICT BEFORE(A,B), since a relation B->A is not allowed.
   The Non-Transitive-NOT (NTNOT - #) comes also from STRICTBEFORE(A,B), since A is only allowed to connect to B.
   Finally, the NOT in (B,C) comes from BEFORE(C,B), because a relation B->C must not exist.

5. When the initialization is done. choose a Relation at random and collapse it. For example, assume Relation (C, A) is used:

   ```markdown
    | A | B | C | D |
    |---|---|---|---|
   A| * | * | U |   |
   B|   | * |   |   |
   C| # | * | * |   |
   D| # |   |   | * |
   ```

   * copy every NOT in row A into row C. if a cell is NT-NOT, overwrite it with the NOT
     This means, that every node which was not allowed to connect to A before, is now also not allowed to connect with C 
   * copy every NOT from column C into column A.
     This means, that every node which C was not allowed to connect to, A is now also not allowed to connect.
   * for every USED in column A, set a TUSED in column C
     This means, that C inherits the transitive connections. When C is connected to a node W, A is now also transitively and implicitly connected to W. We don't want another relation (A,W) which would be redundant.

6. Choose next relation to collapse based on entropy of C and A.
   The entropy of a node is defined by the number of available relations. In the above example, A is able to create a relationship with B and D (A,B) and (D,A).
   C is able to create a relationship with B, D, and D (note that this means (C,B), (C,D) and (D,C)). So the entropy of A is smaller than the entropy of C, so we continue with A.
   If any entropy is 0, choose the other node.

7. Choose a relation from column A or row A, e.g. (A,B).

8. repeat until no FREE cells are available.

