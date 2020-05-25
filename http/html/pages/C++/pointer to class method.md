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

Having, for example
```cpp
bool Organiser::Scene::productPlacement();
```
I.e. class method without parameters

Mapping as follows
```cpp
   // Placement type checkers map
   std::unordered_map<std::string, std::function<bool(const Organiser::Scene*)>> typeChecker{
      {"Signage",   &Organiser::Scene::signagePlacement},
      {"ProductPl", &Organiser::Scene::productPlacement},
      {"Video",     &Organiser::Scene::videoPlacement},
      {"Other",     &Organiser::Scene::otherPlacement}};
      
```

Invocation
```cpp
     // Individual type stats update
     for (const auto placement : {"Signage", "ProductPl", "Video", "Other"})
     {
	if (typeChecker[placement](scene))
	{
	...
```
