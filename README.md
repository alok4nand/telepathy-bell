# Telepathy Bell
*Telepahty Connction Manager for [Ring](https://ring.cx)*



### Build Instructions
#### Requiments
* CMake-2.8.12+ (required by TelepathyQt)
* Qt4 or Qt5
* TelepathyQt-0.9.6+
* [Ring-daemon](https://github.com/savoirfairelinux/ring-daemon)

#### Installation
Clone the repo from github
```
git clone https://github.com/alok4nand/telepathy-bell
```
OR download the .zip file from github
```
mkdir build && cd build
cmake ../telepathy-bell -DCMAKE_INSTALL_PREFIX=/usr
```
 By default CMake looks for or uses  Qt5. You can pass USE_QT4 option (-DUSE_QT4=true) to process Qt4 build.

 If the installation prefix is left default(/usr/local) then DBus service activation will fail.

```
make -j4
make install
```

### How to use telepathy-bell
