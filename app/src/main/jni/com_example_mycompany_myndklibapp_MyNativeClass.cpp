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
