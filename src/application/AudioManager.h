#pragma once

#include "AudioData.h"
#include <functional>
#include <vector>

namespace Application {
namespace Audio {

/**
 * Main audio system manager
 * Consolidates all audio business logic, state management, and external interfaces
 * Single entry point for all audio operations
 */
class AudioManager {
   public:
    // Event callback type
    using StateChangeCallback = std::function<void(const AudioStateChangeEvent&)>;

    // Singleton access
    static AudioManager& getInstance();

    // === LIFECYCLE ===
    bool init();
    void deinit();
    bool isInitialized() const { return initialized; }

    // === STATE ACCESS ===
    const AudioAppState& getState() const { return state; }

    // Quick accessors
    Events::UI::TabState getCurrentTab() const { return state.currentTab; }
    String getCurrentDevice() const { return state.getCurrentSelectedDeviceName(); }
    int getCurrentVolume() const { return state.getCurrentSelectedVolume(); }
    bool isCurrentDeviceMuted() const { return state.isCurrentDeviceMuted(); }
    bool hasDevices() const { return state.hasDevices(); }

    // Device queries
    std::vector<AudioLevel> getAllDevices() const { return state.currentStatus.getAudioLevels(); }
    AudioLevel* getDevice(const String& processName);
    const AudioLevel* getDevice(const String& processName) const;

    // === EXTERNAL DATA INPUT ===
    void onAudioStatusReceived(const AudioStatus& status);

    // === USER ACTIONS ===

    // Device selection
    void selectDevice(const String& deviceName);
    void selectDevice(AudioLevel* device);
    void selectBalanceDevices(const String& device1, const String& device2);

    // Volume control
    void setVolumeForCurrentDevice(int volume);
    void setDeviceVolume(const String& deviceName, int volume);

    // Mute control
    void muteCurrentDevice();
    void unmuteCurrentDevice();
    void muteDevice(const String& deviceName);
    void unmuteDevice(const String& deviceName);

    // Tab management
    void setCurrentTab(Events::UI::TabState tab);

    // === UI STATE CONTROL ===

    // === EVENT SUBSCRIPTION ===
    void subscribeToStateChanges(StateChangeCallback callback);

    // === EXTERNAL COMMUNICATION ===
    void publishStatusUpdate();
    void publishStatusRequest(bool delayed = false);

    // === UTILITY ===
    const char* getTabName(Events::UI::TabState tab) const;
    bool hasValidSelection() const { return state.hasValidSelection(); }

    // === SMART BEHAVIOR ===
    void performSmartAutoSelection();  // Proactive auto-selection for current context

   private:
    AudioManager() = default;
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Internal state
    AudioAppState state;
    bool initialized = false;
    std::vector<StateChangeCallback> callbacks;

    // Internal operations
    void notifyStateChange(const AudioStateChangeEvent& event);
    void autoSelectDeviceIfNeeded();
    void markDevicesAsStale();
    void updateDeviceFromStatus(const AudioLevel& device);
    String findBestDeviceToSelect() const;
    void updateTimestamp();

    // Device management helpers
    void ensureValidSelections();
    void refreshDevicePointers(const String& primaryDeviceName, const String& singleDeviceName, const String& device1Name, const String& device2Name);
    void refreshDevicePointersIfNeeded(const String& deviceName);
};

}  // namespace Audio
}  // namespace Application
