import FreeCAD,Mesh

# Translation vector
tr=FreeCAD.Vector(-0.191849,-109.674469,-3.620361)
# Rotation quaternion
q1=(-0.707107,-0.000000,0.000000,0.707107)
# https://forum.freecadweb.org/viewtopic.php?t=16949

# get the object from FreeCAD (see the list on the upper-left menu)
cm1=FreeCAD.ActiveDocument.getObject("shape1_part1_ai")
# r=FreeCAD.Rotation(*q1)
# r.Axis
# r.Angle
cm1.Placement=FreeCAD.Placement(cm1.Placement.Base,FreeCAD.Rotation(*q1))
cm1.Placement.Base=tr
mat1=FreeCAD.Matrix()
mat1.scale(6.226656,6.226656,6.226656 )
mesh1=cm1.Mesh.copy() #Cube is name of the Obj in the Doc
mesh1.transform(mat1) #add the result to the docuemnt
Mesh.show(mesh1)

# repeat for all objects that were included in the .g3db file
