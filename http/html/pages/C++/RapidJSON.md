## Reading RapidJSON example

```cpp
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

string data =
        "{\"Int\":1,"
        "\"Double\":12.0000001,"
        "\"String\":\"This is a string\","
        "\"Object\":{\"name\":\"qq849635649\",\"age\":25},"
        "\"IntArray\":[10,20,30],"
        "\"DoubleArray\":[1.0,2.0,3.0],"
        "\"StringArray\":[\"one\",\"two\",\"three\"],"
        "\"MixedArray\":[\"one\",50,false,12.005],"
        "\"People\":[{\"name\":\"qq849635649\",\"age\":0,\"sex\":true},"
        "{\"name\":\"qq849635649\",\"age\":10,\"sex\":false},"
        "{\"name\":\"qq849635649\",\"age\":20,\"sex\":true}]}";

void parse() {
    //Create analytical object
    rapidjson::Document doc;
    //Analyze first, and the specific field can be parsed without parsing errors
    if(!doc.Parse(data.data()).HasParseError())
    {
        //1. Parse integers
        if(doc.HasMember("Int") && doc["Int"].IsInt())
        {
            cout << "Int = " << doc["Int"].GetInt() << endl;
        }
        //2. Parse floating point
        if(doc.HasMember("Double") && doc["Double"].IsDouble())
        {
            cout << "Double = " << doc["Double"].GetDouble() << endl;
        }
        //3. Parse the string
        if(doc.HasMember("String") && doc["String"].IsString())
        {
            cout << "String = " << doc["String"].GetString() << endl;
        }
        //4. Analyze the structure
        if(doc.HasMember("Object") && doc["Object"].IsObject())
        {
            const rapidjson::Value& object = doc["Object"];
            if(object.HasMember("name") && object["name"].IsString())
            {
                cout << "Object.name = " << object["name"].GetString() << endl;
            }
            if(object.HasMember("age") && object["age"].IsInt())
            {
                cout << "Object.age = " << object["age"].GetInt() << endl;
            }
        }
        //5. Resolve array types
        //5.1 Integer array type
        if(doc.HasMember("IntArray") && doc["IntArray"].IsArray())
        {
            //5.1.1 Convert the field to rapidjson::Value type
            const rapidjson::Value& array = doc["IntArray"];
            //5.1.2 Get array length
            size_t len = array.Size();
            //5.1.3 Traverse according to the subscript, pay attention to converting the element to the corresponding type, that is, you need to call GetInt()
            for(size_t i = 0; i < len; i++)
            {
                cout << "IntArray[" << i << "] = " << array[i].GetInt() << endl;
            }
        }
        //5.2 Floating point array type
        if(doc.HasMember("DoubleArray") && doc["DoubleArray"].IsArray())
        {
            const rapidjson::Value& array = doc["DoubleArray"];
            size_t len = array.Size();
            for(size_t i = 0; i < len; i++)
            {
                //In order to prevent type mismatch, type verification is generally added
                if(array[i].IsDouble())
                {
                    cout << "DoubleArray[" << i << "] = " << array[i].GetDouble() << endl;
                }
            }
        }
        //5.3 String array type
        if(doc.HasMember("StringArray") && doc["StringArray"].IsArray())
        {
            const rapidjson::Value& array = doc["StringArray"];
            size_t len = array.Size();
            for(size_t i = 0; i < len; i++)
            {
                //In order to prevent type mismatch, type verification is generally added
                if(array[i].IsString())
                {
                    cout << "StringArray[" << i << "] = " << array[i].GetString() << endl;
                }
            }
        }
        //5.4 Hybrid
        if(doc.HasMember("MixedArray") && doc["MixedArray"].IsArray())
        {
            const rapidjson::Value& array = doc["MixedArray"];
            size_t len = array.Size();
            for(size_t i = 0; i < len; i++)
            {
                //In order to prevent type mismatch, type verification is generally added
                if(array[i].IsString())
                {
                    cout << "MixedArray[" << i << "] = " << array[i].GetString() << endl;
                }
                else if(array[i].IsBool())
                {
                    cout << "MixedArray[" << i << "] = " << array[i].GetBool() << endl;
                }
                else if(array[i].IsInt())
                {
                    cout << "MixedArray[" << i << "] = " << array[i].GetInt() << endl;
                }
                else if(array[i].IsDouble())
                {
                    cout << "MixedArray[" << i << "] = " << array[i].GetDouble() << endl;
                }
            }
        }
        //5.5 Structure array type
        if(doc.HasMember("People") && doc["People"].IsArray())
        {
            const rapidjson::Value& array = doc["People"];
            size_t len = array.Size();
            for(size_t i = 0; i < len; i++)
            {
                const rapidjson::Value& object = array[i];
                //In order to prevent type mismatch, type verification is generally added
                if(object.IsObject())
                {
                    cout << "ObjectArray[" << i << "]: ";
                    if(object.HasMember("name") && object["name"].IsString())
                    {
                        cout << "name=" << object["name"].GetString();
                    }
                    if(object.HasMember("age") && object["age"].IsInt())
                    {
                        cout << ", age=" << object["age"].GetInt();
                    }
                    if(object.HasMember("sex") && object["sex"].IsBool())
                    {
                        cout << ", sex="  << (object["sex"].GetBool() ? "male" : "Female") << endl;
                    }
                }
            }
        }
    }
    /**
          * Finally note: because rapidjson does not do security verification, you must do security verification yourself, take int integer as an example
     * “if(object.HasMember("age") && object["age"].IsInt()) {}”
          * This sentence verification is very important. It is necessary to verify that the subsection is present and the type is correct, otherwise it will cause the program to crash
     */
}
```

## Writing (via allocator)
```cpp
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


void Serialize_2()
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    //1. Integer type
    doc.AddMember("Int", 1, allocator);

    //2. Floating point type
    doc.AddMember("Double", 12.00001, allocator);

    //3. String type
    //The right way
    string str= "This is a string";
    rapidjson::Value str_value(rapidjson::kStringType);
    str_value.SetString(str.c_str(), str.size());
    if(!str_value.IsNull())
    {
        doc.AddMember("String", str_value, allocator);
    }
    /**
          * Note: The following method is incorrect, it may succeed or fail, because the string is written into json to open up memory again,
          * If you use this method, when the data is a string constant, there is no problem, if it is a variable, it will display garbled characters, so
          * To be safe, we explicitly allocate memory (no need to release)
     */
    //doc.AddMember("String", str.data(), allocator);

    //4. Structure type
    rapidjson::Value object(rapidjson::kObjectType);
    object.AddMember("name", "qq849635649", allocator); //Note: Constants are no problem
    object.AddMember("age", 25, allocator);
    doc.AddMember("Object", object, allocator);

    //5. Array Type
    //5.1 Integer array
    rapidjson::Value IntArray(rapidjson::kArrayType);
    IntArray.PushBack(10, allocator);
    IntArray.PushBack(20, allocator);
    IntArray.PushBack(30, allocator);
    doc.AddMember("IntArray", IntArray, allocator);

    //5.2 Floating point array
    rapidjson::Value DoubleArray(rapidjson::kArrayType);
    DoubleArray.PushBack(1.0, allocator);
    DoubleArray.PushBack(2.0, allocator);
    DoubleArray.PushBack(3.0, allocator);
    doc.AddMember("DoubleArray", DoubleArray, allocator);

    //5.3 Character array
    rapidjson::Value StringArray(rapidjson::kArrayType);
    string strValue1 = "one";
    string strValue2 = "two";
    string strValue3 = "three";
    str_value.SetString(strValue1.c_str(), strValue1.size());
    StringArray.PushBack(str_value, allocator);
    str_value.SetString(strValue2.c_str(), strValue2.size());
    StringArray.PushBack(str_value, allocator);
    str_value.SetString(strValue3.c_str(), strValue3.size());
    StringArray.PushBack(str_value, allocator);
    doc.AddMember("StringArray", StringArray, allocator);

    //5.4 Structure array
    rapidjson::Value ObjectArray(rapidjson::kArrayType);
    for(int i = 1; i < 4; i++)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("name", "qq849635649", allocator);//Note: Constants are no problem
        obj.AddMember("age", i * 10, allocator);
        ObjectArray.PushBack(obj, allocator);
    }
    doc.AddMember("ObjectArray", ObjectArray, allocator);

    rapidjson::StringBuffer strBuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
    doc.Accept(writer);

    string data = strBuf.GetString();
    cout << data << endl;
}
```
