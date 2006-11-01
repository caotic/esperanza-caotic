#make clean
#rm -rf Esperanza.app
#/sw/qt4.2/bin/qmake -config release
#make -j 2
strip Esperanza.app/Contents/MacOS/Esperanza
mkdir Esperanza.app/Contents/Frameworks
mkdir Esperanza.app/Contents/lib
echo "QtCore"
cp -R /sw/qt4.2/lib/QtCore.Framework Esperanza.app/Contents/Frameworks
rm Esperanza.app/Contents/Frameworks/QtCore.Framework/Versions/4/QtCore_debug
rm Esperanza.app/Contents/Frameworks/QtCore.Framework/QtCore_debug
rm -rf Esperanza.app/Contents/Frameworks/QtCore.Framework/Versions/4/Headers
install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore Esperanza.app/Contents/Frameworks/QtCore.framework/Versions/4/QtCore
install_name_tool -change /sw/qt4.2/lib/QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore Esperanza.app/Contents/MacOs/Esperanza

echo "QtGui"
cp -R /sw/qt4.2/lib/QtGui.Framework Esperanza.app/Contents/Frameworks
rm Esperanza.app/Contents/Frameworks/QtGui.Framework/Versions/4/QtGui_debug
rm Esperanza.app/Contents/Frameworks/QtGui.Framework/QtGui_debug
rm -rf Esperanza.app/Contents/Frameworks/QtGui.Framework/Versions/4/Headers
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui Esperanza.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui
install_name_tool -change /sw/qt4.2/lib/QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui Esperanza.app/Contents/MacOs/Esperanza
install_name_tool -change /sw/qt4.2/lib/QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore Esperanza.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui

echo "xmmsclient"
cp /sw/lib/libxmmsclient.dylib Esperanza.app/Contents/lib
install_name_tool -id @executable_path/../lib/libxmmsclient.dylib Esperanza.app/Contents/MacOS/Esperanza
install_name_tool -change /sw/lib/libxmmsclient.dylib @executable_path/../lib/libxmmsclient.dylib Esperanza.app/Contents/MacOs/Esperanza
cp /sw/lib/libxmmsclient++.dylib Esperanza.app/Contents/lib
install_name_tool -id @executable_path/../lib/libxmmsclient++.dylib Esperanza.app/Contents/MacOS/Esperanza
install_name_tool -change /sw/lib/libxmmsclient++.dylib @executable_path/../lib/libxmmsclient++.dylib Esperanza.app/Contents/MacOs/Esperanza
install_name_tool -change /sw/lib/libxmmsclient.dylib @executable_path/../lib/libxmmsclient.dylib Esperanza.app/Contents/lib/libxmmsclient++.dylib

echo "mkdmg"
mkdir -p tmp/Esperanza
mv Esperanza.app tmp/Esperanza
mkdmg tmp/Esperanza
mv tmp/Esperanza.dmg .
rm -rf tmp

