# Programmer_Maze

Developed with Unreal Engine 4

## Plan on code evaluation
The most challenging part here is the fact the C/C++ code is executed in linear form
and we have no way to put `pause` instruction without suspending the execution thread.
To solve this problem there are two methods to overcome it:

### Virtual Machine
The most obvious way to overcome it is the evaluate the code outside the main execution context,
which putting the code into the execution unit in which the clock is customizable.

**Planned Approach**:
The `eval()` will be changed to `asVMInstruction()` which will return a filled `VM_INSTRUCTION` structure which represent the instruction that will run inside the VM. Then, the interop will be provided by the `MazeMainGameMode` toward those instructions which are specific to the world.

**Advantages**:
- Highly efficient: It is very efficient to use this approach as there are no overhead on having another thread waiting for main thread performing something. The execution can be anywhere which the code is executed periodically such as `AActor::Tick(float delta)`
- Thread safety: Since the virtual machine is ticking on the same thread the normal code is running. There are no risk of the race condition which might corrupt the application state which might crash the entire application.

**Disadvantages**:
- Highly complicated: It is complicated to implement a working virtual machine.

### Spawn another thread and execute it
This approach spawns another thread to execute the code directly and wait on the `FEvent` on the block that require the animation. The `FEvent` will be signaled then on finish.

**Planned Approach**:
The `eval()` will be expanded with a new parameter `execContext` which will hold the information of the current state of the execution. `FThread` class is used to run new thread.

**Advantages**:
- Relatively simple compared to VM: simpler since no need to deal with the instruction definitions and so on

**Disadvantages**:
- Inefficient: It creates the overhead on executing code in another thread
- Thread safety: The `UObject` are in fact not thread safe. This means the code which deal with the `UObject`s such as `AActor` must be done of main thread using functions such as ` FFunctionGraphTask::CreateAndDispatchWhenReady`
- Stability: The multi-threaded code are prone to error and easier to crash
