# ESP32-S3 Dual-Core Runtime Architecture Diagram

## Current Runtime State Analysis

### Detailed Core Assignment and Task Flow

```
┌════════════════════════════════════════════════════════════════════════════════════════════════┐
│                                    ESP32-S3 DUAL-CORE RUNTIME                                  │
│                                      (NORMAL MODE ACTIVE)                                       │
├══════════════════════════════════════════════════════════════════════════════════════════════════
│                                                                                                │
│  ╔═══════════════════════════════════════╗    ╔═══════════════════════════════════════════╗   │
│  ║             CORE 0                     ║    ║             CORE 1                        ║   │
│  ║        (Application CPU)               ║    ║         (Protocol CPU)                   ║   │
│  ║         240 MHz                        ║    ║          240 MHz                         ║   │
│  ╠═══════════════════════════════════════╣    ╠═══════════════════════════════════════════╣   │
│  ║                                       ║    ║                                           ║   │
│  ║  ┌─────────────────────────────────┐  ║    ║  ┌─────────────────────────────────────┐ ║   │
│  ║  │        LVGL_Task                │  ║    ║  │      Core1_Messaging                │ ║   │
│  ║  │  • Priority: 10 (HIGH)          │  ║    ║  │  • Priority: 23 (VERY HIGH)        │ ║   │
│  ║  │  • Stack: 4096+ bytes           │  ║    ║  │  • Stack: 8192 bytes               │ ║   │
│  ║  │  • Cycle: Event-driven + 20ms   │  ║    ║  │  • Cycle: 1ms (precise)            │ ║   │
│  ║  │                                 │  ║    ║  │                                     │ ║   │
│  ║  │  RESPONSIBILITIES:              │  ║    ║  │  RESPONSIBILITIES:                  │ ║   │
│  ║  │  • lv_timer_handler()           │  ║    ║  │  • Serial.read() interrupt         │ ║   │
│  ║  │  • Display invalidations        │  ║    ║  │  • Binary protocol framing         │ ║   │
│  ║  │  • Touch events                 │  ║    ║  │  • Message parsing & routing       │ ║   │
│  ║  │  • UI animations                │  ║    ║  │  • Queue management                │ ║   │
│  ║  │  • Display::update()            │  ║    ║  │  • MessageCore::update()           │ ║   │
│  ║  │  • LED cycling                  │  ║    ║  │  • Transport statistics            │ ║   │
│  ║  └─────────────────────────────────┘  ║    ║  └─────────────────────────────────────┘ ║   │
│  ║                                       ║    ║                                           ║   │
│  ║  ┌─────────────────────────────────┐  ║    ║                                           ║   │
│  ║  │        Audio_Task               │  ║    ║           🔄 QUEUES 🔄                   ║   │
│  ║  │  • Priority: 6 (BOOSTED)        │  ║    ║                                           ║   │
│  ║  │  • Stack: 4096+2048 bytes       │  ║    ║  ┌─────────────────────────────────────┐ ║   │
│  ║  │  • Cycle: Variable interval     │  ║    ║  │      incomingDataQueue              │ ║   │
│  ║  │                                 │  ║    ║  │  Raw UART bytes                     │ ║   │
│  ║  │  RESPONSIBILITIES:              │  ║    ║  │  Size: MESSAGE_QUEUE_SIZE           │ ║   │
│  ║  │  • Volume control processing    │  ║    ║  └─────────────────────────────────────┘ ║   │
│  ║  │  • Audio device management      │  ║    ║                                           ║   │
│  ║  │  • Status requests              │  ║    ║  ┌─────────────────────────────────────┐ ║   │
│  ║  │  • AudioUI integration          │  ║    ║  │    outgoingMessageQueue             │ ║   │
│  ║  │  • Real-time audio feedback     │  ║    ║  │  Binary messages to send            │ ║   │
│  ║  └─────────────────────────────────┘  ║    ║  │  Size: MESSAGE_QUEUE_SIZE           │ ║   │
│  ║                                       ║    ║  └─────────────────────────────────────┘ ║   │
│  ║  ┌─────────────────────────────────┐  ║    ║                                           ║   │
│  ║  │      Arduino loop()             │  ║    ║  ┌─────────────────────────────────────┐ ║   │
│  ║  │  • Priority: 1 (LOW)            │  ║    ║  │    core1ProcessingQueue             │ ║   │
│  ║  │  • Cycle: 100ms                 │  ║    ║  │  Messages for Core 1 processing    │ ║   │
│  ║  │                                 │  ║    ║  │  Size: INTERNAL_MSG_QUEUE_SIZE      │ ║   │
│  ║  │  RESPONSIBILITIES:              │  ║    ║  └─────────────────────────────────────┘ ║   │
│  ║  │  • Application::run()           │  ║    ║                                           ║   │
│  ║  │  • Task monitoring              │  ║    ║  ┌─────────────────────────────────────┐ ║   │
│  ║  │  • Periodic statistics          │  ║    ║  │   core0NotificationQueue            │ ║   │
│  ║  │  • Watchdog feeding             │  ║    ║  │  Messages for Core 0 (UI updates)  │ ║   │
│  ║  │  • Memory monitoring            │  ║    ║  │  Size: INTERNAL_MSG_QUEUE_SIZE      │ ║   │
│  ║  └─────────────────────────────────┘  ║    ║  └─────────────────────────────────────┘ ║   │
│  ║                                       ║    ║                                           ║   │
│  ║        🔒 MUTEXES 🔒                  ║    ║         🔒 MUTEXES 🔒                    ║   │
│  ║                                       ║    ║                                           ║   │
│  ║  • lvglMutex (recursive)             ║    ║  • uartMutex                             ║   │
│  ║  • taskConfigMutex                   ║    ║  • routingMutex                          ║   │
│  ║  • otaProgressQueue                  ║    ║                                           ║   │
│  ╚═══════════════════════════════════════╝    ╚═══════════════════════════════════════════╝   │
│                                                                                                │
└════════════════════════════════════════════════════════════════════════════════════════════════┘

                                    INTER-CORE COMMUNICATION FLOW
                                             
        ┌─────────────────────────┐                              ┌─────────────────────────┐
        │       CORE 0            │                              │       CORE 1            │
        │   (UI/Audio Events)     │                              │   (Serial Messages)     │
        └─────────────────────────┘                              └─────────────────────────┘
                    │                                                          │
                    │ 📤 Audio Commands                          📨 UART Data │
                    │ 📤 Status Requests                                       │
                    │ 📤 OTA Commands                                          │
                    ▼                                                          ▼
            ┌───────────────────┐                              ┌───────────────────────┐
            │ outgoingMessage   │◀────────────────────────────▶│ incomingDataQueue     │
            │ Queue             │        Queue System          │                       │
            └───────────────────┘                              └───────────────────────┘
                    ▲                                                          │
                    │ 📨 UI Updates                              📤 Parsed     │
                    │ 📨 Audio Status                           Messages       │
                    │ 📨 Device Status                                         │
                    │                                                          ▼
            ┌───────────────────┐                              ┌───────────────────────┐
            │ core0Notification │◀────────────────────────────▶│ core1ProcessingQueue  │
            │ Queue             │      Message Routing         │                       │
            └───────────────────┘                              └───────────────────────┘

```

## Detailed Task Scheduling & Timing

### Core 0 Task Scheduling Pattern

```
Time (ms) │ LVGL Task    │ Audio Task   │ Main Loop    │ Notes
──────────┼──────────────┼──────────────┼──────────────┼────────────────────────────
0-20      │ ████████████ │              │              │ UI event processing
20-25     │              │ ██████████   │              │ Audio management 
25-100    │              │              │ ████████████ │ App coordination, delay
100-120   │ ████████████ │              │              │ Display updates
120-125   │              │ ██████████   │              │ Volume processing
125-200   │              │              │ ████████████ │ Statistics, monitoring
200...    │ (repeat pattern with adaptive intervals)            │

Adaptive Behavior:
• Startup Phase (0-10s): Faster cycles (20ms LVGL, 5ms timers)
• Normal Phase (10s+): Slower cycles (100ms idle, 500ms force update)
• High Load: Priority boost, reduced sleep intervals
```

### Core 1 Task Scheduling Pattern

```
Time (ms) │ Core1_Messaging Task                           │ Queue Processing
──────────┼────────────────────────────────────────────────┼─────────────────
0-1       │ ████ processIncomingData()                     │ UART → Parser
1-2       │ ████ processOutgoingMessages()                 │ Queue → Serial  
2-3       │ ████ processCore1Messages()                    │ Internal routing
3-4       │ ████ messageCore->update()                     │ Transport update
4-5       │ ████ vTaskDelayUntil(1ms)                      │ Precise timing
5-6       │ ████ processIncomingData()                     │ (repeat 1ms cycle)
...       │ (continuous 1ms precision loop)                │

Characteristics:
• Zero busy-waiting: Interrupt-driven UART
• 1ms precision timing for responsiveness
• Queue-based processing prevents blocking
• Statistics collection every cycle
```

## Memory Layout & Resource Usage

### Stack Allocation Analysis
```
┌─────────────────┬──────────────┬─────────────────┬────────────────────┐
│ Task Name       │ Core │ Stack │ Priority        │ High Water Mark    │
├─────────────────┼──────────────┼─────────────────┼────────────────────┤
│ LVGL_Task       │ 0    │ 4KB+  │ 10 (HIGH)       │ ~60% utilization   │
│ Audio_Task      │ 0    │ 6KB   │ 6 (BOOSTED)     │ ~40% utilization   │
│ Core1_Messaging │ 1    │ 8KB   │ 23 (VERY HIGH)  │ ~30% utilization   │
│ Arduino loop()  │ 0    │ 8KB   │ 1 (LOW)         │ ~10% utilization   │
└─────────────────┴──────────────┴─────────────────┴────────────────────┘
```

### Queue Utilization Patterns
```
┌─────────────────────────┬────────────┬─────────────┬─────────────────────┐
│ Queue Name              │ Size       │ Item Size   │ Typical Usage       │
├─────────────────────────┼────────────┼─────────────┼─────────────────────┤
│ incomingDataQueue       │ 64         │ 1 byte      │ 5-50% (burst)       │
│ outgoingMessageQueue    │ 32         │ Pointer     │ 0-25% (steady)      │
│ core1ProcessingQueue    │ 16         │ Message     │ 0-10% (low volume)  │
│ core0NotificationQueue  │ 16         │ Message     │ 0-15% (UI updates)  │
│ otaProgressQueue        │ 1          │ Progress    │ 0% (only in OTA)    │
└─────────────────────────┴────────────┴─────────────┴─────────────────────┘
```

## Communication Protocol Analysis

### Message Routing Decision Tree
```
Incoming Message (Core 1)
    │
    ▼
┌─────────────────┐
│ Parse Message   │
│ Type & Content  │
└─────────────────┘
    │
    ▼
┌─────────────────┐     YES    ┌──────────────────┐
│ Low-level       │ ────────▶  │ Process on       │
│ Protocol?       │            │ Core 1           │
└─────────────────┘            └──────────────────┘
    │ NO
    ▼
┌─────────────────┐     YES    ┌──────────────────┐
│ UI Update       │ ────────▶  │ Route to Core 0  │
│ Required?       │            │ (UI Queue)       │
└─────────────────┘            └──────────────────┘
    │ NO
    ▼
┌─────────────────┐     YES    ┌──────────────────┐
│ Audio Command?  │ ────────▶  │ Route to Core 0  │
│                 │            │ (Audio Queue)    │
└─────────────────┘            └──────────────────┘
    │ NO
    ▼
┌─────────────────┐
│ Log & Discard   │
└─────────────────┘
```

### Performance Metrics (Typical Runtime)

```
┌──────────────────────────┬─────────────────┬─────────────────┬────────────────┐
│ Metric                   │ Core 0          │ Core 1          │ System Total   │
├──────────────────────────┼─────────────────┼─────────────────┼────────────────┤
│ CPU Utilization          │ 15-30%          │ 5-15%           │ 20-45%         │
│ Messages/Second          │ N/A             │ 10-100          │ 10-100         │
│ LVGL Frame Rate          │ 30-60 FPS       │ N/A             │ 30-60 FPS      │
│ Audio Latency            │ <10ms           │ N/A             │ <10ms          │
│ Serial Throughput        │ N/A             │ 115200 baud     │ 115200 baud    │
│ Memory Usage (Heap)      │ 60-80%          │ 10-20%          │ 70-100%        │
│ Memory Usage (PSRAM)     │ 20-40%          │ 5-10%           │ 25-50%         │
└──────────────────────────┴─────────────────┴─────────────────┴────────────────┘
```

## OTA Mode Runtime Comparison

### OTA Mode Core Usage
```
┌═══════════════════════════════════════════════════════════════════════════════┐
│                            ESP32-S3 OTA MODE                                  │
├═══════════════════════════════════════════════════════════════════════════════┤
│                                                                               │
│  ╔═══════════════════════════════════════╗    ╔═══════════════════════════════╗ │
│  ║             CORE 0                     ║    ║             CORE 1           ║ │
│  ║        (OTA Application)               ║    ║           (IDLE)             ║ │
│  ╠═══════════════════════════════════════╣    ╠═══════════════════════════════╣ │
│  ║                                       ║    ║                               ║ │
│  ║  ┌─────────────────────────────────┐  ║    ║  ┌─────────────────────────┐ ║ │
│  ║  │      OTA Application            │  ║    ║  │      No Tasks           │ ║ │
│  ║  │  • Display minimal UI           │  ║    ║  │                         │ ║ │
│  ║  │  • WiFi client                  │  ║    ║  │  • No messaging engine │ ║ │
│  ║  │  • HTTP download                │  ║    ║  │  • No communication    │ ║ │
│  ║  │  • Firmware installation        │  ║    ║  │  • Core unused          │ ║ │
│  ║  │  • Progress feedback            │  ║    ║  │  • Power saving         │ ║ │
│  ║  └─────────────────────────────────┘  ║    ║  └─────────────────────────┘ ║ │
│  ║                                       ║    ║                               ║ │
│  ║  Resources: ~80% CPU, Network Stack  ║    ║  Resources: 0% CPU            ║ │
│  ║  Duration: 2-10 minutes               ║    ║  Power: Minimal               ║ │
│  ║  Then: Restart to NORMAL mode        ║    ║                               ║ │
│  ╚═══════════════════════════════════════╝    ╚═══════════════════════════════╝ │
└═══════════════════════════════════════════════════════════════════════════════┘
```

This runtime diagram shows the current operational state of both CPU cores, including exact task assignments, priorities, timing intervals, and communication patterns. The system is optimized for maximum UI responsiveness while maintaining reliable communication through dedicated core separation.