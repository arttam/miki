### Pointer to class method

Having
```cpp
void TaskManager::Task_New(cms::Message* msg) {}
```

Map of string to member method pointer might look as follows:
```cpp
    const std::unordered_map<std::string, void (TaskManager::*)(cms::Message*)> taskExecutors {
        {"Task_New", &TaskManager::Task_New},
        {"Task_Reset", &TaskManager::Task_Reset},
        {"Task_Cancel", &TaskManager::Task_Cancel},
        {"Task_Set_Priority", &TaskManager::Task_Set_Priority},
        {"Task_Finished", &TaskManager::Task_Finished},
        {"Task_InProgress", &TaskManager::Task_InProgress},
        {"Bcast_TaskList", &TaskManager::Bcast_TaskList},
        {"TaskLog_Chunk", &TaskManager::TaskLog_Chunk},
        {"TaskLog_Request", &TaskManager::TaskLog_Request}
    };
```

Invocation:
```cpp 
if (auto executorIt = taskExecutors.find(msg_type); executorIt != taskExecutors.end()) 
{
    (*this.*executorIt)(msg);
}
else
{
    std::cerr << "Method not found" << std::endl;
}
```