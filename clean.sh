#!/bin/sh

rm -rf $(find ./ -name "debug" -or -name "release" -and -type d)
rm -rf $(find ./ -name "Debug" -or -name "Release" -and -type d)
rm -rf $(find ./ -name ".svn" -and -type d)
rm -rf $(find ./ -name "ipch" -and -type d)
rm -rf $(find ./ -name "*~")
rm -rf $(find ./ -name "*.suo")
rm -rf $(find ./ -name "*.ncb")
rm -rf $(find ./ -name "*.user")
rm -rf $(find ./ -name "*.idb")
rm -rf $(find ./ -name "*.sdf")
rm -rf $(find ./ -name "*.vcproj")
rm -rf $(find ./ -name "*.filters")
# 以下src以下でのみ
rm -rf $(find src -name "*.lib")
rm -rf $(find src -name "*.pdb")
