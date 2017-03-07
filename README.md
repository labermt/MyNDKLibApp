# MyNDKLibApp

Create an example CMake based Android Studio NDK shared library and application.

---
Created on: 2017-03-08<br />
IDE: Android Studio 2.3<br />
OS: LinuxMint 17.2<br />

This project assumes a basic understanding of C++, CMake, Java, Android and Android Studio.

Where not otherwise noted, Android Studio defaults were chosen.

Where noted, use the <***Sync Checkpoint***> <***Build Checkpoint***> and <***Run Checkpoint***>'s to confirm your progress.

---
## Step 1: Configure Android Studio for NDK development

[Download the NDK and Tools](https://developer.android.com/ndk/guides/index.html#download-ndk)

---
## Step 2: Create New Project

Select menu: File | New | New Project...

---
**Configure your new project**

Application name: My NDK Lib App<br />
Company Domain: mycompany.example.com<br />
Include C++ Support: Check On<br />

---
**Add an Activity to Mobile**

Basic Activity

---
**Customize the Activity**

Use a Fragment: Check On

---
**Customize C++ Support**

C++ Standard: C++11

Exceptions Support (-fexceptions): Check On

---
<***Build Checkpoint***>

Select menu: Build | Make Project

Note for Android Studio versions older than 2.3:

You may see the following error: "cannot find symbol variable sample_text"

If you do, fix the error by modifying the file "MyNDKLibApp/app/src/main/res/layout/fragment_main.xml" as follows:

```
android:id="@+id/sample_text"
```
to the TextView

```
<TextView
    android:id="@+id/sample_text"
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="Hello World!" />
```

---
<***Run Checkpoint***>

---
## Step 3: Add New Library Module

Select menu: File | New | New Module...

---
**New Module**

Select: Android Library

---
**Android Library**

Application/Library name: MyNDKLib<br />
Module name: MyNDKModule<br />

---
<***Run Checkpoint***>

---
## Step 4: Create A Native C++ Class

Create the following folders:

MyNDKLibApp/MyNDKModule/src/main/cpp<br />
MyNDKLibApp/MyNDKModule/src/main/cpp/MyCppLib<br />
MyNDKLibApp/MyNDKModule/src/main/cpp/MyCppLib/src<br />

---
Create:
```
// file: MyNDKLibApp/MyNDKModule/src/main/cpp/MyCppLib/src/MyCppClass.h

#ifndef MYCPPLIB_MYCPPCLASS_H
#define MYCPPLIB_MYCPPCLASS_H

class MyCppClass
{
public:
    static int fourtyTwo();
};

#endif // MYCPPLIB_MYCPPCLASS_H
```
---
Create:
```
// file: MyNDKLibApp/MyNDKModule/src/main/cpp/MyCppLib/src/MyCppClass.cpp

#include "MyCppClass.h"

#if defined(__GNUC__) && __GNUC__ >= 4
#   define MYCPPLIB_EXPORT __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590)
#   define MYCPPLIB_EXPORT __attribute__((visibility("default")))
#else
#   define MYCPPLIB_EXPORT
#endif

MYCPPLIB_EXPORT int MyCppClass::fourtyTwo()
{
    const auto result = 42;
    return result;
}
```
---
Note the file MyCppClass.cpp is not yet part of the project.

---
<***Run Checkpoint***>

---
## Step 5: Create Java Native Wrapper Class

Create:
```
// File: MyNDKLibApp/app/src/main/java/com/example/mycompany/myndklibapp/MyNativeClass.java

package com.example.mycompany.myndklibapp;

public class MyNativeClass {
    public native static int fourtyTwo();
}
```
---
Note the corresponding JNI  method for native method "fourtyTwo()" is not yet declared or implemented.

<***Run Checkpoint***>

---
## Step 6: Add javah External Tool to Android Studio

If not already configured, Add javah to Android Studio external tools. This is a one off step.

The javah tool will be used to generate a JNI header for native Java method MyNativeClass.fourtyTwo().

---
Select menu: File | Settings... | Tools | External Tools | [+]

---
**Create Tool**

Name: javah<br />
Description: javah<br />

Show console when a message is printed to standard output stream: Check On<br />
Show console when a message is printed to standard error stream: Check On<br />

Program: $JDKPath$/bin/javah<br />
Parameters: -classpath "$Classpath$" -v -jni $FileClass$<br />
Working Directory: $SourcepathEntry$/../jni<br />

---
## Step 7: Create the JNI Header Using javah Tool

Create folder: MyNDKLibApp/app/src/main/jni by Right-clicking the "app" module in the Project view.

Select: New | Folder | JNI Folder

---
**Configure Component**

Choose default values and press Finish.

---
Build the project. This will generate the class file required by the javah tool.

<***Build Checkpoint***>

Failure to successfully build before using the javah tool will generate the following message:

*Error: Could not find class file for 'com.example.mycompany.myndklibapp.MyNativeClass'.*

---
Right-click: MyNDKLibApp/app/src/main/java/com/example/mycompany/myndklibapp/MyNativeClass.java

Select menu: NDK | javah

---
Verify the machine generated file was created:

MyNDKLibApp/app/src/main/jni/com_example_mycompany_myndklibapp_MyNativeClass.h

---
## Step 8: Manually Create the Implementation for the Machine Generated JNI Header

Create:
```
// file: MyNDKLibApp/app/src/main/jni/com_example_mycompany_myndklibapp_MyNativeClass.cpp

#include "com_example_mycompany_myndklibapp_MyNativeClass.h"

#include "../../../../MyNDKModule/src/main/cpp/MyCppLib/src/MyCppClass.h"

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT jint JNICALL Java_com_example_mycompany_myndklibapp_MyNativeClass_fourtyTwo
        (JNIEnv *, jclass)
{
    const auto result = MyCppClass::fourtyTwo(); // Call native method.
    return result;
}

#ifdef __cplusplus
}
#endif
```
---
Note the implementation is not yet part of the project.

<***Run Checkpoint***>

---
## Step 9: Review Documentation on Integrating CMake into an Android Studio Build Script

Read the general overview on how to include native code in a project using CMake:

[Create a CMake build script](https://developer.android.com/studio/projects/add-native-code.html#create-cmake-script)<br />
[Link Gradle to your native library](https://developer.android.com/studio/projects/add-native-code.html#link-gradle)<br />

---
## Step 10: Add JNI Class to Build

Using the embeded tags<br />
```### <Add>```<br />
and <br />
```### </Add>``` <br />
modify the file:
```
# File: MyNDKLibApp/app/CMakeLists.txt

# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

### <Add>
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../distribution)
### </Add>

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.

### <Add>
            src/main/jni/com_example_mycompany_myndklibapp_MyNativeClass.cpp
### </Add>

            src/main/cpp/native-lib.cpp )

### <Add>
add_library(lib_mycpplib SHARED IMPORTED)
set_target_properties(lib_mycpplib PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/MyCppLib/lib/${ANDROID_ABI}/libMyCppLib.so)
### </Add>

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

### <Add>
                       # libMyCppLib.so
                       lib_mycpplib
### </Add>

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
```
<***Sync Checkpoint***><br />

---
<***Build Checkpoint***>

Note the error: '../../../../../../../../distribution/MyCppLib/lib/mips64/libMyCppLib.so', needed by '../../../../build/intermediates/cmake/debug/obj/mips64/libnative-lib.so', missing and no known rule to make it

This error is expected and indicates the project has not yet been configured to build the library.

---
## Step 11: Add the Native Library to the Module

Create:
```
# file: MyNDKLibApp/MyNDKModule/src/main/cpp/CMakeLists.txt

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(lib_build_DIR $ENV{HOME}/tmp)
file(MAKE_DIRECTORY ${lib_build_DIR})

add_subdirectory(${lib_src_DIR}/MyCppLib ${lib_build_DIR}/MyCppLib)
```

---
## Step 12: Add the Native C++ Class to the Library

Create:
```
# file: MyNDKLibApp/MyNDKModule/src/main/cpp/MyCppLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

add_library(MyCppLib SHARED src/MyCppClass.cpp)

# copy out the lib binary and remove generated files
set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../distribution)
set_target_properties(MyCppLib
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY
                      "${distribution_DIR}/MyCppLib/lib/${ANDROID_ABI}")
add_custom_command(TARGET MyCppLib POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy "${CMAKE_CURRENT_SOURCE_DIR}/src/MyCppClass.h"
                   "${distribution_DIR}/MyCppLib/include/MyCppClass.h"
                   COMMENT "Copying MyCppLib to output directory")
```

---
## Step 13: Link Module to Gradle

Right-click the "MyNDKModule" module in the Android view and select menu option "Link C++ Project with Gradle".

---
**Link C++ Project with Gradle**<br />

Build System: CMake<br />
Project Path: *FULL_PROJECT_PATH*/MyNDKLibApp/MyNDKModule/src/main/cpp/CMakeLists.txt<br />

You can select the full path using the [...] button.

Verify the dialog box shows:<br />
Path to be saved into the build.gradle file: "src/main/cpp/CMakeLists.txt"<br />

---
Confirm there are 3 CMakeLists.txt files listed in the "External Build Files" of the Android view.

---
## Step 14: Build Module and Package Library

Using the embeded tags<br />
```// <Add>```<br />
and <br />
```// </Add>```<br />
modify the file:
```
// MyNDKLibApp/app/build.gradle

apply plugin: 'com.android.application'

android {
    compileSdkVersion 25
    buildToolsVersion "25.0.0"
    defaultConfig {
        applicationId "com.example.mycompany.myndklibapp"
        minSdkVersion 15
        targetSdkVersion 25
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags "-std=c++11 -fexceptions"
            }
        }
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }

// <Add>
    sourceSets {
        main {
            // Package the shared JNI library into apk
            jniLibs.srcDirs = ['../distribution/MyCppLib/lib']
        }
    }
// </Add>

    externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
        }
    }
}

dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
        exclude group: 'com.android.support', module: 'support-annotations'
    })
    compile 'com.android.support:appcompat-v7:25.1.1'
    compile 'com.android.support:design:25.1.1'
    testCompile 'junit:junit:4.12'

// <Add>
    compile project(':MyNDKModule')
// </Add>
}
```
<***Sync Checkpoint***>

---
## Step 15: Add Library Building to Gradle

Using the embeded tags<br />

```// <Add>```<br />
and <br />
```// </Add>```<br />
modify the file:

```
// MyNDKLibApp/MyNDKModule/build.gradle

apply plugin: 'com.android.library'

android {
    compileSdkVersion 25
    buildToolsVersion "25.0.0"

    defaultConfig {
        minSdkVersion 15
        targetSdkVersion 25
        versionCode 1
        versionName "1.0"

        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"

// <Add>
        externalNativeBuild {
            cmake {
                arguments '-DANDROID_PLATFORM=android-9',
                        '-DANDROID_TOOLCHAIN=clang'
                // explicitly build library
                targets 'MyCppLib'
            }
        }
// </Add>
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
    externalNativeBuild {
        cmake {
            path 'src/main/cpp/CMakeLists.txt'
        }
    }
}

dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
        exclude group: 'com.android.support', module: 'support-annotations'
    })
    compile 'com.android.support:appcompat-v7:25.1.1'
    testCompile 'junit:junit:4.12'
}
```
<***Sync Checkpoint***><br />
<***Run Checkpoint***>

---
## Step 16: Use the Library

Modify:
```
// MyNDKLibApp/app/src/main/java/com/example/mycompany/myndklibapp/MainActivity.java
//
// ...
//
// protected void onCreate(Bundle savedInstanceState) {
//
// ...
//
// *** Change the following:
// Example of a call to a native method
//      TextView tv = (TextView) findViewById(R.id.sample_text);
//      tv.setText(stringFromJNI());
//
// *** To the following:
//
// Example of calls to native methods
        TextView tv = (TextView) findViewById(R.id.sample_text);
        final int fourtyTwo = MyNativeClass.fourtyTwo();
        if (fourtyTwo == 42) {
            tv.setText(stringFromJNI());
        }
```

<***Run Checkpoint***>

The resulting application should display the same output if and only if native MyCppClass.fourtyTwo() is successfully built, packaged and executed.

---
## Step 17: Learn More

Read: [Java Native Interface Specification](http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/jniTOC.html)<br />
Static Library Example: [android-ndk/hello-libs (see: gmath)](https://github.com/googlesamples/android-ndk/tree/master/hello-libs)<br />
