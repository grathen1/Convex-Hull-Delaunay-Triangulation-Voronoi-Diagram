# Lab work
 The main purpose is to create a program where we can built three structures: 
 #### Convex Hull, Delaunay Triangulation and Voronoi Diagram.

User should have ability to choose how many points must be randomly generated, can choose mode where points is genereted by mouse click and readed from the file.

The program show us the result graphically by using [SFML library](https://www.sfml-dev.org/). All the graphics should show us the aproximate view of the algorithm and how structures are builted.
Libraries with builted in functions with algorithms is restricted.

## Convex Hull

#### The algorithm which I have used is ['Graham`s Scan'](https://en.wikipedia.org/wiki/Graham_scan).
1. The point with the smallest y-coordinate is selected (if there are several, then the one with the smallest x-coordinate is taken) as the starting point.
2. The points are sorted by the angle they form with the starting point. Sorting allows each point to be sequentially checked to see if it forms a convex angle with the last two points in the current shell.
3. All points are iterated in sorted order, and for each point it is decided whether to add it to the convex hull. If the current point forms a "right turn" (convex corner) with the last two points in the envelope, then the last point is removed from the envelope because it cannot be part of the minimal convex envelope.
4. This process is repeated until all points have been tested, resulting in a minimal convex hull of the set of points.

https://github.com/grathen1/Convex-Hull-Delaunay-Triangulation-Voronoi-Diagram/assets/130998746/cdd5b47a-93d5-4164-8202-1a24eb7e8fc9

## Delaunay Triangulation
The algorithm is ['Bowyer-Watson algorithm'](https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm) (also known as incremental algorithm).
1. At the beginning of the algorithm, a large triangle is created that completely covers all points of the set. This triangle serves as the initial basis for triangulation.
2. Set of points are inserted one by one. For each point, all triangles whose inscribed circles contain this point (that is, "bad" triangles) are determined. These triangles are removed, leaving a polygonal "hole".
(The process of removing "bad" triangles and creating new ones ensures that the Delaunay triangulation property is satisfied: no set point is inside the inscribed circle of any of the triangulation triangles.)
3. Hole boundaries are now re-triangulated with the new point, forming new triangles.
4. After inserting all the points, the triangles that contain the vertices of the "super-triangle" are removed, leaving the Delaunay triangulation for the initial set of points.
5. To determine whether a point lies within the inscribed circle of a triangle, the calculation of the determinant is used based on the coordinates of the points of the triangle and the checked point.

