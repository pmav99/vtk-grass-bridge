cd /usr/local/lib/vtk-5.9/java/
cp vtk.jar vtkGRASS.jar
jar xvf vtkGRASS.jar
cd -
javac -classpath /usr/local/lib/vtk-5.9/java/vtk.jar *.java
cp *.java *.class /usr/local/lib/vtk-5.9/java/vtk/.
cd /usr/local/lib/vtk-5.9/java/
jar cvf vtkGRASS.jar vtk
