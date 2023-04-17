
#ifndef ENUMS_H
#define ENUMS_H

enum ScreenContext {
  NewSession = 0,
  Settings,
  Log,
  Menu,
  Summary,
  InitSession
};

enum ChallengeLevel { Beginner = 1, Intermediate, Advanced, Master };
enum BatteryLevel { Empty = 0, Low, Half, High, Full };
enum HeartRateSensor { Disconnected = 0, Connected };
enum CoherenceState { None, LowCoherence, MediumCoherence, HighCoherence };
enum SessionSimulation { PoorCoherence, GoodCoherence };

#endif // ENUMS_H
