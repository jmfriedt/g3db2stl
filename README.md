# g3db2stl
Read G3DB (Binary JSON 3D Model) and save STL output: software aimed at interpreting
.g3db files and output STL of each part of the 3D model as a result.

# Compiling

``make`` should generate the executable on any GNU/Linux-Unix system. Command line
only interface allowing for the .g3db filename as input argument.

<img src="figures/Screenshot_20210422-192719.PNG">

Android ESA Sentinel 3D model page: https://play.google.com/store/apps/details?id=esa.sentinel proprietary software, yet the apk archive can be uncompressed to extract individual files including the 3D models of the satellites used in this investigation. Requesting information about the 3D embedded model leads to a response from solenix.ch which seems to be a Swiss (*non-EU*) private company to have been contracted by ESA (*European* Space Agency) to deploy their Android application. ESA will not release their 3D satellite model without NDA ... too bad.

<img src="figures/model_assembled.png">

Model extracted, assembled using FreeCAD, and ready for printing.
