# Programmer_Maze

Developed with Unreal Engine 4

## Latent protocol
Latent protocol is the asynchronous dispatching protocol used inside the application.

### Main goals
To simulate the operation of fiber since it is not provided by UE.

### Assumptions made
Provided nothing except the context (`FScriptExecutionContext`) changed it is possible for functions to save and restore their local state and having it to perform as it is executed linearly.

### Actions
The latent function are having following requirements.
- Saving their state when calling a function which known to be latent compliant
- Aware with context state `yielding` and `contextRestore`

Callers must aware the result are saved on the first element inside the stack.

### Yielding
`yielding` is a state that the code are depending on the external operations which are not able to completes immediately. When the latent function sets it, all of its caller must returns immediately without saving its state. Similar with the unwind.

### contextRestore
When the external operation done, the host will unset the `yielding` and set the `contextRestore` to true. All functions involved must restore its state from its local vars. The function which asks for yielding must unset it.
