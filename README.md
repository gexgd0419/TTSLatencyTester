This is a latency tester for SAPI5 voices, which helps you know the part that increases the latency.

WARNING: This is for testing purpose only, not intended for general use. This might be buggy and incompatible with some TTS clients.

Compile the solution with Visual Studio 2022 in Release configuration, ATL support required. Register the TestTTSEngine.dll with `regsvr32` as an administrator.

After registering TestTTSEngine, all your SAPI5 voices will have a "forwarded" version, with "[Forwarded]" prepended to the name. For example, if you have `Microsoft Zira Desktop`, then a voice named `[Forwarded] Microsoft Zira Desktop` will be added. When you use those "forwarded" voices, the TTS requests and generated audio will be intercepted and processed by TestTTSEngine, so that it can log the timestamp of each step, and remove the leading silence.

As the leading silence is removed when using the forwarded voices, you can compare it with the original voice to check if removing leading silence reduces the latency.

You can use TestTTSClient.exe, which is a console program, to perform tests and get the timestamp log. It's written in C++ and calls COM interfaces directly, so it might be more efficient. But if you want to test latency in other scenarios, you can use the logger directly as a COM object to log and get the timestamps, as in the following VBScript code:

``` vbscript
Dim spv, logger, logitem, logs
Set spv = CreateObject("SAPI.SpVoice")
Set logger = CreateObject("TestTTSEngine.Logger")
Set spv.Voice = spv.GetVoices("EngineCLSID;TokenID", "Gender=Female")(0)
logger.LogStart
logger.LogStep "Client Speak start"
spv.Speak "This is a test."
logger.LogStep "Client Speak end"
logger.LogEnd
logs = ""
For Each logitem in logger.GetLogItems()
    logs = logs & logitem.ElapsedSeconds & vbTab & logitem.Message & vbNewLine
Next
WScript.Echo logs
```

To remove the engine, use `regsvr32 /u` to unregister TestTTSEngine.dll first, before removing the files.